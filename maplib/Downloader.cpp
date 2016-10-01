/*
 * Copyright (C) 2010 by H.Schurig, Germany, Wöllstadt (DH3HS)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 ************************************************************************/

#include "../global.h"

#define DEBUGLVL 0
#include "mydebug.h"

/*!
 * \file Downloader.cpp
 *
 * Helper to download data.
 *
 * \ingroup Network
 */


#include "Downloader.h"

#include <QCoreApplication>
#include <QNetworkDiskCache>
#include <QProcessEnvironment>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QNetworkRequest>
#ifdef QT_GUI_LIB
#include <QDesktopServices>
#else
#include <QDir>
#endif


/*!
 * \class Downloader
 *
 * \ingroup Storage Network
 *
 * File downloader using http, https and ftp.
 *
 *
 * This is a general-purpose downloader that can use a cache and can
 * associate a key to the downloaded URLs.
 *
 * It honors "http_proxy" and "ftp_proxy" to specify a proxy.
 *
 * It also modifies the "User-Agent"-string in the request according to
 * QApplication::applicationName/Version.
 */


/*!
 * Holds a pointer to a Downloader instance.
 * Initialized as needed by download(const QString &path, const QString &key, QObject *parent).
 */
static Downloader *downloader = 0;


/*!
 * Download a file.
 *
 * \param path Complete URL of the file to be downloaded.
 *
 * \param key Some key that should be associated with the download.
 *
 * \param parent If no \a downloader instance exists, a new one will be
 * created with \a parent as it's parent.
 *
 * \returns newly created QNetworkRequest
 *
 * \sa Downloader::download()
 */
QNetworkReply *download(const QString &cacheFilePath, const QString &path, const QString &key, const QString &userAgent, const QString &referer, QObject *parent)
{
    if (!downloader)
    {
        downloader = new Downloader(cacheFilePath, parent);
    }
    return downloader->download(path, key, userAgent, referer);
}
QNetworkReply *downloadSync(QImage *image, const QString &cacheFilePath, const QString &path, const QString &key, const QString &userAgent, const QString &referer, QObject *parent)
{
    if (!downloader)
    {
        //downloader = new Downloader(".cache/Myapp/", parent);
        downloader = new Downloader(cacheFilePath, parent);
    }
    return downloader->downloadSync(image, path, key, userAgent, referer);
}



/*!
 * Constructor.
 *
 * Configures the \a manager to use hard-disk caching and proxies. For the
 * latter you have to define the standard environment variables "http_proxy",
 * and / or "ftp_proxy".
 *
 * \param parent Pointer to a QObject, which will own the downloader class.
 */
Downloader::Downloader(QString cachePath, QObject* parent)
    : QObject(parent)
{
    MYTRACE("Downloader::Downloader");

    cacheFullPath = cachePath;

    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotFinished(QNetworkReply*)));

    // Cache downloaded files in ~/.cache/http
    QNetworkDiskCache *netCache = new QNetworkDiskCache;
#ifdef QT_GUI_LIB
    netCache->setCacheDirectory(QDesktopServices::storageLocation(QDesktopServices::CacheLocation));
#else



    QDir dir(QDir::home());

    dir.mkpath(cacheFullPath);
    dir.cd(cacheFullPath);


    netCache->setCacheDirectory(dir.absolutePath());

#endif
    manager.setCache(netCache);

    // honor "http_proxy" environment
    QString proxy;
    proxy = QProcessEnvironment::systemEnvironment().value("http_proxy");
    if (!proxy.isEmpty())
    {
        QUrl url(proxy, QUrl::TolerantMode);
        MYVERBOSE("proxy '%s' %d", qPrintable(url.host()), url.port());
        manager.setProxy(QNetworkProxy(QNetworkProxy::HttpProxy,
                                       url.host(),
                                       url.port() ));
    }
    proxy = QProcessEnvironment::systemEnvironment().value("ftp_proxy");
    if (!proxy.isEmpty())
    {
        QUrl url(proxy, QUrl::TolerantMode);
        MYVERBOSE("proxy '%s' %d", qPrintable(url.host()), url.port());
        manager.setProxy(QNetworkProxy(QNetworkProxy::FtpCachingProxy,
                                       url.host(),
                                       url.port() ));
    }
}


/*!
 * Download a file.
 *
 * The \a key will be assigned to the QNetWorkRequest as the first user
 * attribute. If the key was empty, then the path will be used as key.
 *
 * As function returns the QNetworkReply, you can assign a signal there,
 * handle the finished download. Because the QNetworkReply points to the
 * QNetworkRequest (which has the key assigned), you can access this key in
 * the slot, too:
 *
 * \code
 *  QString key = reply->request().attribute(QNetworkRequest::User).toString();
 * \endcode
 *
 *
 * \param path Complete URL of the file to be downloaded.
 *
 * \param key Some key that should be associated with the download.
 *
 * \returns newly created QNetworkReply
 *
 * \sa ::download()
 */
QNetworkReply *Downloader::download(const QString &path, QString key, const QString &userAgent, const QString &referer)
{
    MYTRACE("Downloader::download(%s)", qPrintable(path));

    if (path == "") return NULL;


    if (key.isEmpty())
        key = path;

    // Make sure we don't download the same file concurrently
    if (downloading.contains(key))
    {
        MYVERBOSE("  already downloading");
        return 0;
    }

    QNetworkRequest req(path);
    req.setAttribute(QNetworkRequest::User, QVariant(key));

    // Be nice to webservers and tell them we're some kind of application
    if ( userAgent != "")
    {
        QByteArray MyArray (userAgent.toStdString().c_str());
        req.setRawHeader("User-Agent", MyArray );

    }
    if ( referer != "")
    {
        QByteArray MyArray (referer.toStdString().c_str());
        req.setRawHeader("Referer", MyArray);

    }

    MYVERBOSE("  queued '%s'", qPrintable(path));
    QNetworkReply *reply = manager.get(req);
    downloading.insert(key);

    // return reply so that you can connect to it's progress signal
    return reply;
}


QNetworkReply *Downloader::downloadSync(QImage *image, const QString &path, QString key, const QString &userAgent, const QString &referer)
{
    MYTRACE("Downloader::downloadSync(%s)", qPrintable(path));

    if (path == "") return NULL;


    if (key.isEmpty())
        key = path;

    // Make sure we don't download the same file concurrently
    if (downloading.contains(key))
    {
        MYVERBOSE("  already downloading");
        return 0;
    }

    QNetworkRequest req(path);
    req.setAttribute(QNetworkRequest::User, QVariant(key));

    if ( userAgent != "")
    {
        QByteArray MyArray (userAgent.toStdString().c_str());
        req.setRawHeader("User-Agent", MyArray );

    }
    if ( referer != "")
    {
        QByteArray MyArray (referer.toStdString().c_str());
        req.setRawHeader("Referer", MyArray);

    }

    MYVERBOSE("  queued '%s'", qPrintable(path));
    QNetworkReply *reply = manager.get(req);
    downloading.insert(key);

    QEventLoop eventLoop;
    QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    // --------------------

    image->loadFromData(reply->readAll());
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(statusCode != 200)
    {
        debugp( "Http rrror code = " + QString::number( statusCode ));
    }

    if (reply->error() != QNetworkReply::NoError)
    {
        QString data = QString(reply->errorString ());
        debugp( "data = " +  data);
    }

    if(image->isNull())
    {
        QString strErr(reply->readAll());
        debugp( "image oops : " + QString::number( reply->error() )+ " - Err= " + strErr );
    }


    // return reply so that you can connect to it's progress signal
    return reply;
}


/*!
 * Handler for finished download.
 *
 * This handler simply removes the request from the \a downloading list and
 * calls \a deleteLater() on the reply.
 *
 * \param reply Pointer to the QNetworkReply, that just got finished
 *
 */
void Downloader::slotFinished(QNetworkReply *reply)
{
    MYTRACE("Downloader::slotFinished(%p)", reply);
    QString key = reply->request().attribute(QNetworkRequest::User).toString();
    MYVERBOSE("  key '%s'", qPrintable(key));
    downloading.remove(key);
    MYVERBOSE("  %d file(s) to get", downloading.count());


    reply->deleteLater();
}


/*!
 * \var Downloader::downloading
 *
 * QSet with all current download requests. The set is indexed with the key.
 * This approach makes sure that the same fill will not be downloaded twice.
 *
 *
 *
 * \var Downloader::manager
 *
 * The QNetworkAccessManager used to download stuff.
 */

#include "Downloader.moc"

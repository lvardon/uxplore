#ifndef DOWNLOADER_H
#define DOWNLOADER_H

/*!
 * \file Downloader.h
 *
 * Helper to download data.
 *
 * \ingroup Network
 */


#include <QNetworkAccessManager>
#include <QSet>
#include <QImage>

class QNetworkReply;


QNetworkReply *download(const QString &cacheFilePath, const QString &path, const QString &key=QString(), const QString &userAgent = NULL, const QString &referer = NULL, QObject *parent=0);
QNetworkReply *downloadSync(QImage *image, const QString &cacheFilePath, const QString &path, const QString &key=QString(), const QString &userAgent = NULL, const QString &referer = NULL, QObject *parent=0);


class Downloader : public QObject
{
	Q_OBJECT
public:
	Downloader(QString cachePath, QObject *parent=0);
	QNetworkReply *download(const QString &path, QString key, const QString &userAgent, const QString &referer);
	QNetworkReply *downloadSync(QImage *image, const QString &path, QString key, const QString &userAgent, const QString &referer);
	QString cacheFullPath;
private:
	QNetworkAccessManager manager;
	QSet<QString> downloading;
private slots:
	void slotFinished(QNetworkReply *reply);
};


#endif

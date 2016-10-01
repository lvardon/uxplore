#define DEBUGLVL 3
#include "mydebug.h"

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
 ************************************************************************
 *
 * This can download a SRTM .zip file and extract the .tif file with the
 * DEM data that is inside it.
 *
 * It's a QObject, so that the download can happen in the background.
 */

#include "DemDownloader.h"
#include "Downloader.h"
#include "3rdparty/qzip/qzipreader.h"
#include "SRTMv4.h"


#include <QNetworkReply>
#include <QBuffer>
#include <QFileInfo>
#include <QImageReader>


DemDownloader *demDownloader = 0;

DemDownloader::DemDownloader(const QString &datapath, QObject *parent)
	: QObject(parent)
{
	MYTRACE("DemDownloader::DemDownloader");

	dataPath = datapath;
	// make sure that a non-empty datapath ends with an /
	if (!dataPath.isEmpty())
		if (dataPath.at(dataPath.count()-1) != '/')
			dataPath.append('/');
	MYVERBOSE("   dataPath: '%s'", qPrintable(dataPath));

	demDownloader = this;
}


QString DemDownloader::tileFilename(const QPointF &lonLat)
{
	MYTRACE("DemDownloader::tileFilename(%f, %f)", lonLat.x(), lonLat.y());

	QString srtmBase = SRTMv4::baseFileName(lonLat);
	if (srtmBase.isEmpty())
		return QString::null;

	// test if .tif exists
	QFile f(dataPath + srtmBase + ".tif");
	if (f.exists()) {
		MYDEBUG("  found '%s'", qPrintable(f.fileName()));
		emit useSRTM(f.fileName());
		return f.fileName();
	}

	// test if .zip exists
	f.setFileName(dataPath + srtmBase + ".zip");
	if (f.exists()) {
		MYVERBOSE("  using existing zip file");
		if (!f.open(QIODevice::ReadOnly)) {
			err("Could not open .zip file");
			return QString::null;
		}
		QString fn = srtmBase + ".tif";
		bool ok = extractFromZip(&f, fn);
		return ok ? fn : QString::null;

	}
	MYDEBUG("  not found '%s'", qPrintable(f.fileName()));

	// QString path = QString("ftp://xftp.jrc.it/pub/srtmV4/tiff/%1.zip").arg(srtmBase);
	//QString path = QString("http://droppr.org/srtm/v4.1/6_5x5_TIFs/%1.zip").arg(srtmBase);
	QString path="";

	MYVERBOSE("  get %s", qPrintable(path));
	QNetworkReply *reply = download(path , "", "", "");
	connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
	        this, SLOT(slotDownloadProgress(qint64, qint64)) );
	connect(reply, SIGNAL(finished()),
	        this, SLOT(slotDownloadFinished()) );

	// return false, because the tile hasn't been downloaded yet
	return QString::null;
}


void DemDownloader::slotDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	MYTRACE("DemDownloader::slotDownloadProgress(%lld, %lld)", bytesReceived, bytesTotal);
}


void DemDownloader::slotDownloadFinished()
{
	MYTRACE("DemDownloader::slotDownloadFinished");

	QNetworkReply *reply = dynamic_cast<QNetworkReply *>(sender());
	MYVERBOSE("   error %d", reply->error());
	QString path = reply->request().attribute(QNetworkRequest::User).toString();
	MYVERBOSE("   path '%s'", qPrintable(path));
	QFileInfo fi(path);
	MYVERBOSE("   fname '%s'", qPrintable(fi.fileName()));
	QBuffer buf;
	buf.setData(reply->readAll());

	reply->deleteLater();

	extractFromZip(&buf, fi.baseName() + ".tif");
}


bool DemDownloader::extractFromZip(QIODevice *io, const QString &name)
{
	MYTRACE("handleZip(%p, %s)", io, qPrintable(name));

	QZipReader zip(io);
	MYVERBOSE("  %d files", zip.count());
	for(int i=0; i<zip.count(); i++) {
		QZipReader::FileInfo fi = zip.entryInfoAt(i);
		MYVERBOSE("  entry %d: %s", i, qPrintable(fi.filePath) );
	}

	// TODO: qzip.cpp isn't exactly the best solution, because it always
	// reads in ALL of the compressed bytes and returns ALL of the
	// compressed bytes. That can be a mightty waste of memory space ...

	QByteArray data = zip.fileData(name);
	if (data.isEmpty())
		return err("No data in extracted file");
	QFile out(dataPath + name);
	if (!out.open(QIODevice::WriteOnly))
		return err("Could not open zip stream");
	if (out.write(data) != data.count())
		return err("Could not write extracted data");

	MYVERBOSE("  wrote %s", qPrintable(out.fileName()) );
	emit newSRTM(name);
	emit useSRTM(name);

	return true;
}

#include "DemDownloader.moc"

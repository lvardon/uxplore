#ifndef DEM_ACCESS_H
#define DEM_ACCESS_H

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
 */

#include <QObject>
#include <QPointF>


class QNetworkReply;
class QIODevice;


class DemDownloader : public QObject
{
	Q_OBJECT
public:
	DemDownloader(const QString &dataPath=QString::null, QObject *parent=0);
	//~DemDownloader();
	bool extractFromZip(QIODevice *, const QString &name);
	QString tileFilename(const QPointF &lonLat);

protected:
	QString dataPath;

signals:
	void useSRTM(const QString &name);
	void newSRTM(const QString &name);

private slots:
	void slotDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
	void slotDownloadFinished();
};


extern DemDownloader *demDownloader;


#endif

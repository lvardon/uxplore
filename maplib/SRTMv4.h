#ifndef PARSE_SRTM_H
#define PARSE_SRTM_H

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

/*!
 * \file SRTMv4.h
 *
 * Quick access to SRTM V.4 data files.
 *
 * \ingroup Storage
 */


#include <QObject>
#include <QRectF>
#include <QPointer>
#include <QVector>
#include <QFile>


/*!
 * Pixels per degree.
 *
 * This can only be a const because SRTM::parse() checks
 * that the loaded SRTM file has the needed resolution.
 */
const double ppd = 6000.0 / 5.0;



class SRTMv4 : public QObject {
public:
	SRTMv4(const QString &dataPath=QString::null, QObject *parent=0);
	bool parse(const QString &path);
	bool isOk() const { return ok; };
	QRectF area() const;
	qint16 get(quint32 x, quint32 y);
	qint16 get(const QPointF &lonLat);

	static QString baseFileName(const QPointF &lonLat);

private:
	bool ok;
	QString dataPath;
	QFile f;
	quint32 dataofs;
	quint32 length;
	quint32 width;
	uchar *map;

	// store some GeoTIFF related data
	double modelTiepoint[6];
	QVector<double> geoDoubleParams;
};


#endif

#define DEBUGLVL 0
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
 * Just some geodetic / functions that I took from splat.cpp, converted
 * to Qt/C++.
 */


#include <QCoreApplication>
#include <QPointF>

#include "GeoUtils.h"


/*
 * Two Airports, the coordinates are from http://en.wikipedia.org/wiki/Great_circle_distance
 */
const QPointF BNA(-86.67, 36.12);
const QPointF LAX(-118.4, 33.94);

const QPointF DH3HS(8.77485, 50.28425);
const QPointF DB0FT(8.45741, 50.23153);

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	double d = Great_Circle_Distance(BNA, LAX);
	qDebug("Great circle distance Nashville -> Los Angeles: %.2f km, should be 2886.45 km\n", d/1e3);

	d = Great_Circle_Distance(DB0FT, DH3HS);
	qDebug("Distance DB0FT - DH3HS: %.2f km\n", d/1e3); // 23.32 km

	double a = AzimuthR(DB0FT, DH3HS);
	qDebug("Azimuth DB0FT - DH3HS: %.2f degrees\n", a / DEG2RAD); // 255.56 or 75.32 degrees


	qDebug("Path DB0FT - DH3HS");
	GeoData data;
	MakePath(DH3HS, DB0FT, data);
	qDebug("%d data points\n", data.posCount());


	qDebug("QTH Locator");
	QPointF lonLat;
	bool ok = fromMaidenhead("JO40JG", lonLat);
	qDebug("ok %d", ok);
	if (ok)
		qDebug("  %f,%f", lonLat.x(), lonLat.y());

	QString s = toMaidenhead(lonLat);
	qDebug("locator: %s\n", qPrintable(s));

	return 0;
}

#ifndef GEO_UTILS_H
#define GEO_UTILS_H

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
 * \file GeoUtils.h
 *
 * Various Geodaetic functions.
 *
 * \ingroup Geodetic
 */


#include <QPointF>
#include <QVector>
#include <QString>

#include <math.h>

#ifdef _WIN32
#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2 (1.57079632679489661923)
#endif
#else
#endif



/*! 2 * Pi constant.
 * Formula: \f$2 * \pi\f$
 */
const double TWOPI = M_PI * 2;

/*! Conversion constant to convert from degrees to radians.
 * Formula: \f$2 * \pi / 360\f$
 */
const double DEG2RAD = TWOPI / 360.0;

//! Average earth radius in meter.
const double AVG_EARTH_RADIUS = 6371.01e3;

//! Value if no elevation data is available
const double nodata = -32768;
//! Value is from an invalid area
const double invdata = -32767;


class GeoData
{
public:
	QVector<double> lon;
	QVector<double> lat;
	QVector<double> d;
	QVector<double> e;
	int posCount() const { return lon.count(); };
	void clear();
};


double Great_Circle_Distance(const QPointF &source, const QPointF &dest);
double AzimuthR(const QPointF &source, const QPointF &dest);
void MakePath(const QPointF &source, const QPointF &dest,
              GeoData &data,
              double d=0);
QString toMaidenhead(QPointF lonLat);
bool fromMaidenhead(QString locator, QPointF &lonLat);
QString toQuad ( int x, int y, int zoom);


#endif

#include "../global.h"

#define DEBUGLVL 0
#include "mydebug.h"

#include <QDebug>


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

/*!
 * \file GeoUtils.cpp
 *
 * Various Geodaetic functions.
 *
 * \ingroup Geodetic
 */

#include "GeoUtils.h"


/*!
 * \class GeoData
 *
 * \ingroup Geodetic
 *
 * Simple class to storage geographical data (positions, elevations etc).
 *
 * \fn int GeoData::posCount() const
 *
 * Number of positional data entries. It's a short-cut to call GeoData.lon.count().
 *
 * \return Entries in GeoData::lon
 *
 *
 * \var GeoData::lon
 *
 * Position data, longitude part. Units are degrees, north and east should be positive.
 *
 * \var GeoData::lat
 *
 * Position data, latitude part. Units are degrees, north and east should be positive.
 *
 * \var GeoData::d
 *
 * Distance points. Unit is in meters.
 *
 * \var GeoData::e
 *
 * Elevation (height) data. Unit is in meters.
 */





/*!
 * Adds a new coordinate to a path list.
 *
 * \param data Reference to a \ref GeoData storage class
 *
 * \param lonLat Geographical coordinates in degrees.
 *
 * \param distance Distance (in meters) from the path origin.
 */
static
void addToPath(GeoData &data, const QPointF &lonLat, double distance)
{
    MYTRACE("addToPath(%f,%f, %f)", lonLat.x(), lonLat.y(), distance);
    data.lon.append(lonLat.x());
    data.lat.append(lonLat.y());
    data.d.append(distance);
}




/*!
 * Calculate great circle distance between two locations.
 *
 * You can multiply this by the earth radius to get the mesurement in meters
 * (or miles, inches, yards, or whatever ugly non-SI measurement you're used
 * to).
 *
 * \param lat1 location 1, latitude (in radians)
 * \param lon1 location 1, longitude (in radians)
 * \param lat2 location 2, latitude (in radians)
 * \param lon2 location 2, longitude (in radians)
 * \return \f$\Delta\widehat{\sigma}\f$ from http://en.wikipedia.org/wiki/Great_circle_distance
 */
static
double Delta_sigma(double lat1, double lon1, double lat2, double lon2)
{
    // This function takes radians
    double ds = acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos((lon1) - (lon2)));
    MYVERBOSE("  Delta sigma %f", ds);
    return ds;
}
/*!
 * Calculate great circle distance between two locations.
 *
 * You can multiply this by the earth radius to get the mesurement in meters
 * (or miles, inches, yards, or whatever ugly non-SI measurement you're used
 * to).
 *
 * \param pos1 location 1 (in degrees)
 * \param pos2 location 2 (in degrees)
 * \return \f$\Delta\widehat{\sigma}\f$ from http://en.wikipedia.org/wiki/Great_circle_distance
 */
static
double Delta_sigma(const QPointF &pos1, const QPointF &pos2)
{
    // This function takes degrees
    double lat1 = pos1.y() * DEG2RAD;
    double lon1 = pos1.x() * DEG2RAD;
    double lat2 = pos2.y() * DEG2RAD;
    double lon2 = pos2.x() * DEG2RAD;

    return Delta_sigma(lat1, lon1, lat2, lon2);
}


/*!
 * Calculate great circle distance between two locations.
 *
 * \param lat1 location 1, latitude (in radians)
 * \param lon1 location 1, longitude (in radians)
 * \param lat2 location 2, latitude (in radians)
 * \param lon2 location 2, longitude (in radians)
 * \return Distance in meters.
 * \sa AVG_EARTH_RADIUS
 */
static
double Great_Circle_Distance(double lat1, double lon1, double lat2, double lon2)
{
    // This function takes radians
    double d = Delta_sigma(lat1, lon1, lat2, lon2) * AVG_EARTH_RADIUS;
    MYVERBOSE("  d %.2f m", d);
    return d;

}
/*!
 * Calculate great circle distance between two locations.
 *
 * \param pos1 location 1 (in degrees)
 * \param pos2 location 2 (in degrees)
 * \return Distance in meters.
 * \sa AVG_EARTH_RADIUS
 */
double Great_Circle_Distance(const QPointF &pos1, const QPointF &pos2)
{
    // This function takes degrees
    double d = Delta_sigma(pos1, pos2) * AVG_EARTH_RADIUS;
    MYVERBOSE("  d %.2f m", d);
    return d;
}


/*!
 * Azimuth (in radians) from position 1 to position 2
 *
 * \param lat1 location 1, latitude (in radians)
 * \param lon1 location 1, longitude (in radians)
 * \param lat2 location 2, latitude (in radians)
 * \param lon2 location 2, longitude (in radians)
 * \return Azumuth from location 1 to location 2, in radians
 */
static
double AzimuthR(double lat1, double lon1, double lat2, double lon2)
{
    // This function takes radians

    /* Calculate Surface Distance */
    double delta_sigma = Delta_sigma(lat1, lon1, lat2, lon2);

    /* Calculate Azimuth, in german also called "Kurswinkel" */
    /* http://de.wikipedia.org/wiki/Kurswinkel */
    double num = sin(lat2) - (sin(lat1) * cos(delta_sigma));
    double den = cos(lat1) * sin(delta_sigma);
    double fraction = num / den;  // cos omega

    /* Trap potential problems in acos() due to rounding */
    if (fraction >= 1.0)
        fraction = 1.0;
    else if (fraction <= -1.0)
        fraction = -1.0;

    /* Calculate azimuth */
    double azimuth = acos(fraction);

    /* Reference it to True North */
    double diff = lon1 - lon2;
    if (diff <= -M_PI)
        diff += TWOPI;
    else if (diff >= M_PI)
        diff -= TWOPI;

    if (diff > 0.0)
        azimuth = TWOPI - azimuth;

    MYVERBOSE("  %f radians", azimuth);
    return azimuth;
}
/*!
 * Azimuth (in radians) from position 1 to position 2
 *
 * \param pos1 location 1 (in degrees)
 * \param pos2 location 2 (in degrees)
 * \return Azumuth from location 1 to location 2, in radians
 */
double AzimuthR(const QPointF &pos1, const QPointF &pos2)
{
    // This function takes degrees

    double lat1 = pos1.y() * DEG2RAD;
    double lon1 = pos1.x() * DEG2RAD;
    double lat2 = pos2.y() * DEG2RAD;
    double lon2 = pos2.x() * DEG2RAD;
    return AzimuthR(lat1, lon1, lat2, lon2);
}


/*!
 *
 * Implements the arc cosine function.
 *
 * \param num numerator
 * \param denum denumerator
 *
 * \return a value between 0 and TWOPI.
 */
static
double arccos(double num, double denum)
{
    double result = 0.0;

    if (denum > 0.0)
        result = acos(num / denum);
    else if (denum < 0.0)
        result = M_PI + acos(num / denum);

    return result;
}


/*!
 * Generates sequence of positions between two locations along a great circle path.
 *
 * \param source Origin of path, in degrees.
 *
 * \param dest Destination of path, in degrees.
 *
 * \param data Reference to a \ref GeoData storage class.
 *
 * \param d    Distance between the steps of the generated path. If specified
 *             as zero, then the step size will be calculated so that a step
 *             equals roughtly the pixel size in an \ref SRTMv4 file.
 *
 *             If you're interested in the actual value, you can retrieve it
 *             as GetData.d.at(1)
 */
void MakePath(const QPointF &source, const QPointF &dest, GeoData &data, double d)
{
    MYTRACE("MakePath(%f,%f,  %f,%f)",
            source.x(), source.y(),
            dest.x(), dest.y());

    double lat1 = source.y() * DEG2RAD;
    double lon1 = source.x() * DEG2RAD;
    double lat2 = dest.y()   * DEG2RAD;
    double lon2 = dest.x()   * DEG2RAD;

    double azimuth_r = AzimuthR(lat1, lon1, lat2, lon2);
    MYVERBOSE("  azimuth_r: %.2f radians", azimuth_r);

    double total_distance = Great_Circle_Distance(lat1, lon1, lat2, lon2);
    MYVERBOSE("  total_distance: %.2f km", total_distance);

    // about 0.5 pixel distance if ppd = 1200
    if (total_distance < 0.025)
    {
        // TODO: it would probably be better to just give up?
        addToPath(data, source, 0);
        qWarning("MakePath: total distance < 25m");
        return;
    }

    if (d==0)
    {
        /*
         * If no distance has been given, when we try to calculate one entry
         * for each pixel of an SRTMv4 file.
         *
         * TODO: as soon as I support more than just SRTMv4, the ppd constant
         * has to go.
         */
        const double ppd = 1200;

        double samples_per_radian = ppd / DEG2RAD;
        MYVERBOSE("  sa	mples_per_radian: %f", samples_per_radian);

        double dx = samples_per_radian * acos(cos(lon1 - lon2));
        double dy = samples_per_radian * acos(cos(lat1 - lat2));
        MYVERBOSE("  dx,dy: %f m, %f m", dx, dy);

        double path_length = sqrt((dx * dx) + (dy * dy));		/* Total number of samples */
        MYVERBOSE("  path_length: %f entries", path_length);

        d = total_distance / path_length;
        MYVERBOSE(" 	 d: %f m", d);
    }

    double distance = 0.0;
    while (total_distance != 0.0 && distance <= total_distance)
    {
        double beta = distance / AVG_EARTH_RADIUS;
        lat2 = asin(sin(lat1) * cos(beta) + cos(azimuth_r) * sin(beta) * cos(lat1));
        double num = cos(beta) - (sin(lat1) * sin(lat2));
        double den = cos(lat1) * cos(lat2);

        if (azimuth_r == 0.0 && (beta > M_PI_2 - lat1))
        {
            lon2 = lon1 + M_PI;
            qFatal("TODO check if we should do + M_PI or - M_PI");
        }
        else if (azimuth_r == M_PI_2 && (beta > M_PI_2 + lat1))
        {
            lon2 = lon1 + M_PI;
            qFatal("TODO check if we should do + M_PI or - M_PI");
        }
        else if (fabs(num / den) > 1.0)
            lon2 = lon1;
        else
        {
            if ((M_PI - azimuth_r) >= 0.0)
                lon2 = lon1 + arccos(num, den);
            else
                lon2 = lon1 - arccos(num, den);
        }

        lon2 /= DEG2RAD;
        lat2 /= DEG2RAD;

        // Fix longitude between -180..180
        while (lon2 > 180)
            lon2 -= 360;
        while (lon2 <= -180)
            lon2 += 360;

        addToPath(data, QPointF(lon2, lat2), distance);
        distance += d;
    }

    addToPath(data, dest, total_distance);
    MYVERBOSE("  %d entries", data.posCount());
}




/*!
 * Calculate Maidenhead (QTH) Locator from coordinate
 *
 * \param lonLat Geographical coordinate (in degrees)
 *
 * \result QString containing the locator, e.g. "JO40JG".
 *
 * \sa ftp://ftp.iasi.roedu.net/mirrors/ftp.funet.fi/pub/ham/arrl/contests/ln9404.pdf
 */
QString toMaidenhead(QPointF lonLat)
{
    QString qth;
    double lat = lonLat.y() + 90;
    double lon = lonLat.x() + 180;

    int v = int(lon / 20);
    lon -= v * 20;
    qth.append('A' + v);

    v = int(lat / 10);
    lat -= v * 10;
    qth.append('A' + v);

    v = int(lon/2);
    lon -= v * 2;
    qth.append('0' + v);

    v = int(lat);
    lat -= v;
    qth.append('0' + v);

    v = int(12 * lon);
    qth.append('A' + v);

    v = int(24 * lat);
    qth.append('A' + v);

    return qth;
}


/*!
 * Convert QTH Locator (Maidenhead) into geographical position
 *
 * \param locator QString with the locator, e.g. "JO40JG"
 *
 * \param lonLat The result will be stored into this reference.
 *
 * \return \a true if the locator was valid, \a false otherwise. In the
 * latter case \a lonLat will be kept unchanged.
 */
bool fromMaidenhead(QString locator, QPointF &lonLat)
{
    MYTRACE("fromMaidenhead(%s)", qPrintable(locator));

    if (locator.length() != 6)
        return err("Invalid locator length");

    char c0 = locator.at(0).toLower().toLatin1();
    if (c0 < 'a' || c0 > 'r')
        return err("Locator char 1 invalid");

    char c1 = locator.at(1).toLower().toLatin1();
    if (c1 < 'a' || c1 > 'r')
        return err("Locator char 2 invalid");

    char c2 = locator.at(2).toLower().toLatin1();
    if (c2 < '0' || c2 > '9')
        return err("Locator char 3 invalid");

    char c3 = locator.at(3).toLower().toLatin1();
    if (c3 < '0' || c3 > '9')
        return err("Locator char 4 invalid");

    char c4 = locator.at(4).toLower().toLatin1();
    if (c4 < 'a' || c4 > 'x')
        return err("Locator char 5 invalid");

    char c5 = locator.at(5).toLower().toLatin1();
    if (c5 < 'a' || c5 > 'x')
        return err("Locator char 6 invalid");

    // http://beta.unclassified.de/code/dotnet/maidenheadlocator/MaidenheadLocator.cs
    // is slightly different, this is marked with three Xs

    double longitude = 20 * (c0 - 'a') - 180;
    longitude += 2 * (c2 - '0');
    longitude += (c4 - 'a') / 12.0 + 1/24.0;  // XXX
    lonLat.setX(longitude);

    double latitude = 10 * (c1 - 'a') - 90;
    latitude += c3 - '0';
    latitude += (c5 - 'a') / 24.0 + 1/48.0; // XXX
    lonLat.setY(latitude);

    MYVERBOSE("  %f, %f", longitude, latitude);
    return true;
}



/*!
 * Clears all data (\ref lon, \ref lat, \ref d and \ref e).
 */
void GeoData::clear()
{
    lon.clear();
    lat.clear();
    d.clear();
    e.clear();
}

// Converts TMS tile coordinates to Microsoft QuadTree
QString toQuad ( int x, int y, int zoomLvl)
{
    QString quad = "";

    for (int i = zoomLvl; i > 0; i--)
    {
        int  mask = 1 << (i - 1);
        int cell = 0;
        if ((x & mask) != 0) cell++;
        if ((y & mask) != 0) cell += 2;
        quad = QString("%1%2").arg(quad).arg(cell);
    }

    return quad;

};

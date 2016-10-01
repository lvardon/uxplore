#ifndef LOCATIONS_H
#define LOCATIONS_H


/*!
 * \file locations.h
 * This file has been auto-generated from \a locations.yaml.
 * \ingroup Storage
 */


#include <QString>
#include <QList>


/*!
 * Stores a QTH location.
 */
class Location {
public:
	//! Internal number, corresponds to the position in the file. Mostly used for sorting in the LocationsView.
	quint32 number;
	//! Name of this location.
	QString name;
	//! Longitude (in degrees) of this position. East is positive, west is negative. This uses the WGS 84 datum --- the same as GPS.
	double longitude;
	//! Longitude (in degrees) of this position. North is positive, south is negative. This uses the WGS 84 datum --- the same as GPS.
	double latitude;
};


/*!
 * A container for QTH records.
 *
 * \sa saveLocations()
 * \sa loadLocations()
 * \sa LocationsView
 */
extern QList<Location> locations;

bool saveLocations(const QString &fname);
bool loadLocations(const QString &fname);


#endif

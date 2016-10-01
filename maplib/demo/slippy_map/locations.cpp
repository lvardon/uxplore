/*!
 * \file locations.cpp
 * This file has been auto-generated from \a locations.yaml.
 * \ingroup Storage
 */

#include "locations.h"


#include <QFile>
#include <QTextStream>
#include "ParseCSV.h"


QList<Location> locations;


/*!
 * Stores ::locations into a file as comma-seperated values.
 *
 * \param fname Name of file to write
 *
 * \return \a true if file was created, \a false otherwise.
 */
bool saveLocations(const QString &fname)
{
	QFile file(fname);
	if (! file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;
	QTextStream f(&file);
	foreach (Location m, locations) {
		f << '"'<< m.name.replace('"',"\\\"") << '"'
		  << ", " << m.longitude
		  << ", " << m.latitude << "\n";
	}
	file.close();
	return true;
}


/*!
 * A parser to parse CSV-files (comma seperated values) with Location data.
 * \note This class has been auto-generated from \a locations.yaml.
 * \sa loadLocations()
 */
class LocationsParser : public ParseCSV
{
public:
	/*!
	 * Constructor
	 * \param fname Name of file to parse.
	 */
	LocationsParser(const QString &fname) : ParseCSV(fname) {};
	virtual void setData(int field, const QString &item);
	virtual void saveRecord();

/*!
 * Record of type Location.
 *
 * This is used as an intermediate storage place while parse() parses a
 * line from the CSV file.
 */
	Location m;
};

/*!
 * Called by parse() for every data item read from the CSV file.
 *
 * This implementation stores every item into the proper \ref m field.
 *
 * \param field Indicator of the field inside \ref m.
 *
 * \param item Data \a item to be stored. Will be automatically converted to the
 * target type in Location.
 */
void LocationsParser::setData(int field, const QString &item)
{
	switch (field) {
	case 0: m.name = item; break;
	case 1: m.longitude = item.toDouble(); break;
	case 2: m.latitude = item.toDouble(); break;
	}
}

/*!
 * Called by parse() for every finshed line.
 *
 * This implementation appends \ref m to ::locations.
 */
void LocationsParser::saveRecord()
{
	// START custom code
	m.number = locations.count()+1;
	// END custom code
	locations.append(m);
}


/*!
 * Parses the specified filed and stores it's contents in ::locations.
 *
 * \param fname Name of file to parse.
 * \return The result of LocationsParser::parse(), that is \a true if
 * the file was read successfully, \a false otherwise.
 */
bool loadLocations(const QString &fname)
{
	LocationsParser parser(fname);
	return parser.parse();
}


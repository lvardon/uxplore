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

/*!
 * \file ParseCSV.cpp
 *
 * Code to load locations from a data file.
 *
 * \ingroup Storage
 */


#include "ParseCSV.h"

#include <QFile>


/*!
 * \class ParseCSV
 * \ingroup Storage
 * A pure virtual class for parsing CVS files.
 */



/*!
 * Open, read and parse the CSV file.
 *
 * \return \a false if the internal state machine went into an unknown state
 * or when the file was not readable, \a true otherwise.
 */
bool ParseCSV::parse()
{
	QFile f(fname);
	if (! f.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;
	QByteArray data;
	const int maxSize = 4096;
	enum {
		stBegin,
		stInString,
		stInEscape,
		stInNum,
	} state;
	state = stBegin;
	QString item;
	int field = 0;
	int recNo = 0;
	while (1) {
		data = f.read(maxSize);
		if (data.isEmpty())
			break;

		foreach(char c, data) {
			if (state == stBegin) {
				if (c == '"') {
					// Start of string
					state = stInString;
					continue;
				} else
				if (c == ',') {
					// Field delimter
					field++;
					continue;
				} else
				if ((c >= '0' && c <= '9') || c == '.' || c == '-') {
					// Start of Number
					item = c;
					state = stInNum;
					continue;
				}
				if (c == ' ' || c == '\t') {
					continue;
				}
			} else
			if (state == stInString) {
				if (c == '"') {
					// End of string
					setData(field, item);
					item.clear();
					state = stBegin;
					continue;
				} else
				if (c == '\\') {
					// Escaped character inside string
					state = stInEscape;
					continue;
				} else {
					item.append(c);
					continue;
				}
			} else
			if (state == stInEscape) {
				// Character after escape character
				item.append(c);
				state = stInString;
				continue;
			}
			if (state == stInNum) {
				if ((c >= '0' && c <= '9') || c == '.' || c == '+' || c == '-') {
					// digit for number
					item.append(c);
					continue;
				} else
				// if (c == ' ' || c == '\t') {
				// 	continue;
				// } else
				if (c == ',' || c == '\r' || c == '\n') {
					// end of number
					setData(field, item);
					item.clear();
					field++;
					state = stBegin;
					if (c != ',') {
						// end of line
						saveRecord();
						field = 0;
						recNo++;
					}
					continue;
				}
			}
			qWarning("unhandled: state %d, char '%c'", state, c);
			return false;
		}
	}
	f.close();
	return true;
}



/*!
 * \fn ParseCSV::ParseCSV(const QString &name)
 * Constructor.
 * \param name Name of file to parse.
 *
 * \fn virtual void ParseCSV::setData(int field, const QString &item)
 * \param field Field number to set.
 * \param item New data item.
 * Called by parse() for every data item read from the CSV file.
 *
 * \fn virtual void ParseCSV::saveRecord()
 * Called by parse() for every finshed line.
 *
 * \var ParseCSV::fname
 * File name of file to parse
 */

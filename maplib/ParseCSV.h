#ifndef PARSE_CSV_H
#define PARSE_CSV_H

/*!
 * \file ParseCSV.h
 *
 * Code to load locations from a data file.
 *
 * \ingroup Storage
 */


#include <QString>




class ParseCSV {
public:
	ParseCSV(const QString &name) : fname(name) {};
	bool parse();
protected:
	virtual void setData(int field, const QString &item) = 0;
	virtual void saveRecord() = 0;

	QString fname;
};

#endif

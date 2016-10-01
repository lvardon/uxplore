#ifndef LOCATION_MAP_DIALOG_H
#define LOCATION_MAP_DIALOG_H

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
 * \file LocationMapDialog.h
 *
 * Dialog to enter/edit locations.
 *
 * \ingroup Widgets
 */


#include "locations.h"


class SlippyMapWidget;
class QLineEdit;
class QRegExpValdidator;


class LocationMapDialog : public LocationDialog
{
	Q_OBJECT
public:
	LocationMapDialog(Location *record, QWidget *parent, Qt::WindowFlags f=0);
private:
	SlippyMapWidget *slippy;
	QLineEdit *editQth;
private slots:
	void slotPosition(const QPointF &lonLat);
	void slotLonLatEnter();
	void slotQthEnter();
};


#endif

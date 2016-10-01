#ifndef SLIPPY_LOCATIONS_H
#define SLIPPY_LOCATIONS_H

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
 */


/*!
 * \file SlippyLocations.h
 *
 * Implements a SlippyMapPart that draws \ref Location "Locations".
 *
 * \ingroup Widgets
 */


#include "SlippyMapWidget.h"


class SlippyLocations : public SlippyMapPart
{
public:
	SlippyLocations(SlippyMapWidget *parent) : SlippyMapPart(parent) {};
	void draw(QPainter *painter, QPaintEvent *ev);
	bool mousePressEvent(QMouseEvent *ev);
};


#endif

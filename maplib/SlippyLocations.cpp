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
 ************************************************************************/

#include "SlippyLocations.h"
#include "locations.h"

#include <QPainter>


/*!
 * \class SlippyLocations
 * \ingroup Widgets
 *
 * This class draws the locations (globally defined in from locations.cpp)
 * into a SlippyMapWidget.
 *
 * \fn SlippyLocations::SlippyLocations(SlippyMapWidget *parent)
 *
 * Constructor.
 *
 * \param parent Pointer to the SlippyMapWidget where this SlippyMapPart
 * should render their contents into.
 */

/*!
 * Draws locations into the SlippyMapWidget.
 *
 * \param painter Pointer to a QPainter
 *
 * \param ev Pointer to a QPaintEvent
 */
void SlippyLocations::draw(QPainter *painter, QPaintEvent *ev)
{
	MYTRACE("SlippyLocations::draw");
	Q_UNUSED(ev);

	static const QColor lightRed(255,0,0, 150);

	QRectF area = widget->getArea();
	MYVERBOSE("area: %f,%f, %f,%f", area.left(), area.top(), area.right(), area.bottom());
	for(int i=0; i<locations.count(); i++) {
		Location m = locations.at(i);
		if (!area.contains(m.longitude, m.latitude))
			continue;
		double x = (m.longitude-area.left()) / area.width()  * widget->width();
		double y = (m.latitude -area.top())  / area.height() * widget->height();
		painter->setPen(QColor(0,0,0, 150));
		painter->setBrush(lightRed);
		painter->drawEllipse(QPointF(x, y), 6, 6);
		painter->setPen(Qt::black);
		painter->drawText(x+10, y+5, m.name);
	}
}


/*!
 * No-Op.
 *
 * This widget doesn't react on mouse events.
 *
 * \param ev Pointer to a QMouseEvent
 *
 * \return true, if the mouse press has been handled
 */
bool SlippyLocations::mousePressEvent(QMouseEvent *ev)
{
	Q_UNUSED(ev);
	return false;
}

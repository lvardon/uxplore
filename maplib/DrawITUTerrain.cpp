#define DEBUGLVL 1
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
 */

#include "DrawITUTerrain.h"
#include "ITUTerrain.h"

#include <QPainter>



DrawITUTerrain::DrawITUTerrain(ITUTerrain *t, ProfileWidget *parent)
	: ProfileDrawPart(parent)
	, t(t)
{
	widget->margin_left  = qMax(10, widget->margin_left);
	widget->margin_right = qMax(10, widget->margin_right);
	widget->margin_top   = qMax(10, widget->heightToPixel(qMax(t->htg, t->hrg)));
}


void DrawITUTerrain::drawText(QPainter *painter, double x, double y, const QString &s)
{
	MYTRACE("drawText(%.1f, %.1f, %s)", x, y, qPrintable(s));

	// For drawing text, we need to reset the coordinate system back into pixels,
	// overwise painter->drawText() wreacks havoc.

	QTransform trans = painter->transform();
	QPoint pix = trans.map(QPoint(x,y));
	painter->resetTransform();

	QRect r = painter->boundingRect(pix.x(), pix.y(),
	                                0, 0,
	                                Qt::AlignHCenter | Qt::AlignCenter, s);
	QRect t = r;          // Bounding box of text
	r.adjust(-2,-2,2,0);  // Bounding box of rectange
	//MYVERBOSE("x: %d, w: %d, y: %d, h: %d, %d", t.x(), t.width(), t.y());
	if (r.right() > widget->width()) {
		int dx = r.right() - widget->width();
		MYVERBOSE("enlarging right margin by %d pixels", dx);
		widget->margin_right += dx;
		widget->update();
		return;
	}

	painter->setBrush(Qt::white);
	painter->setPen(Qt::NoPen);
	painter->drawRect(r);
	painter->setPen(Qt::SolidLine);
	painter->drawText(t, s);

	painter->setTransform(trans);
}


void DrawITUTerrain::draw(QPainter *painter, QPaintEvent *ev)
{
	Q_UNUSED(ev);

	if (!t)
		return;

	// draw horizon
	if (t->path_trans_horizon) {
		QVector<QPointF> poly;
		poly.append( QPointF(widget->data->d.at(t->index_t),
		                     0) );
		for (int i = t->index_t ; i <= t->index_r; i++) {
			poly.append( QPointF(widget->data->d.at(i),
			                     widget->data->e.at(i)) );
		}
		poly.append( QPointF( widget->data->d.at(t->index_r),
		                      0) );
		painter->setPen(QColor(100,45,0));
		painter->setBrush(QColor(100,45,0));
		painter->drawPolygon(poly.data(), poly.count());
	}

	// Line of sight. DrawLoS would draw it from hill to hill, but this
	// draws it from antenna to antenna.
	painter->setPen(Qt::SolidLine);
	painter->drawLine(0, t->hts, t->dtot, t->hrs);

	// Draw distance
	QString s;
	s = s.sprintf("d: %.0f km", t->dtot / 1000.0);
	drawText(painter,
	         t->dtot/2,
	         (t->hrs+t->hts)/2,
	         s);

	// Draw transmitter antenna
	painter->setPen(Qt::blue);
	// Because the coordinate system of painter is now in meters, we must
	// know how many meters are 3 pixels.
	double dx = widget->pixelWidthToMeter(3);
	// Lower bottom of antenne. The antenna goes from ground level to antenna
	// height ... or, if that is very small, it is at least 10 Pixels height.
	int y = qMin(widget->data->e.first(), t->hts - widget->pixelHeightToMeter(10));
	painter->drawLine(0, t->hts, dx, y);
	painter->drawLine(0, t->hts, -dx, y);
	painter->drawLine(dx, y, -dx, y);

	// Draw transmitter antenna
	y = qMin(widget->data->e.last(), t->hrs - widget->pixelHeightToMeter(10));
	painter->drawLine(t->dtot, t->hrs, t->dtot+dx, y);
	painter->drawLine(t->dtot, t->hrs, t->dtot-dx, y);
	painter->drawLine(t->dtot+dx, y, t->dtot-dx, y);

	// Draw smooth earth equivalent
	painter->setPen(Qt::darkGray);
	painter->drawLine(0, t->hst, t->dtot, t->hsr);

	// Draw terrain roughtness
	s = s.sprintf("hm: %.0f m", t->hm);
	painter->setPen(Qt::blue);
	painter->drawLine(widget->data->d.at(t->index_hm),
	                  widget->data->e.at(t->index_hm),
	                  widget->data->d.at(t->index_hm),
	                  widget->data->e.at(t->index_hm) - t->hm);
	drawText(painter,
	         widget->data->d.at(t->index_hm),
	         widget->data->e.at(t->index_hm) - t->hm / 2,
	         s);

	// TODO: draw elevation angle theta_r, theta_t
}

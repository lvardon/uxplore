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
 */


#include "ProfileWidget.h"
#include "GeoUtils.h"

#include <QPainter>


/*!
  * \file ProfileWidget.cpp
  *
  * Widget to display elevation profiles.
  *
  * \ingroup Widgets
  *
  *
  * \class ProfileWidget
  *
  * A simple widget that can graphically display elevation data stored in
  * a GeoData.
  *
  * This widget only draws the profile, nothing more. You can however
  * attach subclasses of ProfileDrawPart to the parts member, which
  * could draw Line-of-Sight, Smooth-Earth-Equivalent, and so on.
  *
  * \ingroup Widgets
  */

/*!
 * Constructor
 *
 * \param data   Pointer to a GeoData instance. It uses the GeoData::d
 *               (distance) and GeoData::e (elvation) elements from this.
 * \param parent Pointer to the parent QWidget
 */

ProfileWidget::ProfileWidget(QWidget *parent, GeoData *data)
	: QWidget(parent)
	, data(0)
	, margin_left(0)
	, margin_right(0)
	, margin_top(0)
	, margin_bottom(0)
{
	MYTRACE("ProfileWidget::ProfileWidget");
	setData(data);
}


/*!
 * Update GeoData
 *
 * Use this to set a new GeoData dataset. The widget will re-calculate
 * max_elevation and update() itself.
 *
 * \param d   Pointer to a GeoData instance.
 */
void ProfileWidget::setData(GeoData *d)
{
	data = d;
	if (!d)
		return;
	max_elevation = 0;
	for (int i =0; i < data->e.count(); i++) {
		if (data->e.at(i) > max_elevation)
			max_elevation = data->e.at(i);
	}
	update();
}


/*!
 * Convert elevation height into pixels.
 *
 * The coordiate system of the Widget's painter is scaled to the maximum
 * distance / maximum elevation of the supplied GeoData dataset. Therefore
 * some conversion routines are provided, which can be used in
 * ProfileDrawPart.
 *
 * \param h  Heigh (in meter)
 * \returns  Number of pixels
 *
 * \sa pixelWidthToMeter
 * \sa pixelHeightToMeter
 */
int ProfileWidget::heightToPixel(double h) const
{
	return (height() - margin_bottom - margin_top) * h / max_elevation + margin_bottom;
}


/*!
 * Convert horizontal pixel count into distance
 *
 * The coordiate system of the Widget's painter is scaled to the maximum
 * distance / maximum elevation of the supplied GeoData dataset. Therefore
 * some conversion routines are provided, which can be used in
 * ProfileDrawPart.
 *
 * \param w  Horizontal pixel count.
 * \returns  Distance (in meter)
 *
 * \sa heightToPixel
 * \sa pixelHeightToMeter
 */
double ProfileWidget::pixelWidthToMeter(int w) const
{
	return data->d.last() * w / (width() - margin_left - margin_right);
}


/*!
 * Convert vertical pixel count into elevation height
 *
 * The coordiate system of the Widget's painter is scaled to the maximum
 * distance / maximum elevation of the supplied GeoData dataset. Therefore
 * some conversion routines are provided, which can be used in
 * ProfileDrawPart.
 *
 * \param h  Vertical pixel count
 * \returns  Elevation height (in meter)
 *
 * \sa heightToPixel
 * \sa pixelWidthToMeter
 */
double ProfileWidget::pixelHeightToMeter(int h) const
{
	return max_elevation * h / (height() - margin_top - margin_left);
}


/*!
 * React to a paint event.
 *
 * Set local coordiante system, draw elevation profile and call
 * all attached ProfileDrawParts.
 *
 * \param event   Pointer to a QPaintEvent
 *
 * \sa parts
 */
void ProfileWidget::paintEvent(QPaintEvent *event)
{
	MYTRACE("ProfileWidget::paintEvent");
	Q_UNUSED(event);

	// Minor optimization: call this at resize time
	// recalcMarginFactors();

	QPainter painter(this);
	painter.fillRect(QRect(0, 0, width(), height()), Qt::white);

	if (!data || data->e.isEmpty())
		return;

	painter.translate(QPointF(margin_left, height()-margin_bottom));
	painter.scale((width()-margin_left-margin_right) / data->d.last(),
	              (height()-margin_bottom-margin_top) / -max_elevation);

	if (!data || data->d.count() < 2)
		return;

	// Elevation profile
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QColor(143,65,0));
	painter.setBrush(QColor(143,65,0));
	QVector<QPointF> poly;
	poly.append( QPointF(data->d.first(), 0) );
	for (int i=0 ; i < data->e.count(); i++) {
		poly.append( QPointF(data->d.at(i),
		                     data->e.at(i)) );
	}
	poly.append( QPointF( data->d.last(), 0) );
 	painter.drawPolygon(poly.data(), poly.count());

	// Allow attached drawparts to do their job
	foreach(ProfileDrawPart *part, parts)
		part->draw(&painter, event);
}

/*!
 * \var ProfileWidget::parts
 *
 * QList of all attached \ref ProfileDrawPart "ProfileDrawParts".
 *
 *
 * \var ProfileWidget::data
 *
 * Pointer to an GeoData instance.
 * \sa ProfileWidget::setData()
 *
 *
 * \var ProfileWidget::margin_left
 *
 * Empty space kept left of the profile. Normally 0, but a ProfileDrawPart
 * may increase this to get headroom for it's own drawings.
 *
 *
 * \var ProfileWidget::margin_right
 *
 * Empty space kept right of the profile. Normally 0, but a ProfileDrawPart
 * may increase this to get headroom for it's own drawings.
 *
 *
 * \var ProfileWidget::margin_top
 *
 * Empty space kept atop of the profile. Normally 0, but a ProfileDrawPart
 * may increase this to get headroom for it's own drawings.
 *
 *
 * \var ProfileWidget::margin_bottom
 *
 * Empty space kept below of the profile. Normally 0, but a ProfileDrawPart
 * may increase this to get headroom for it's own drawings.
 *
 *
 * \var ProfileWidget::max_elevation
 *
 * Maximum elevation (in meter) of the attached GeoData instance. Set by
 * ProfileWidget::setData().
 *
 */



/*!
 * \class ProfileDrawPart
 *
 * Pure virtual base class for painters that can draw into a ProfileWidget.
 *
 * \ingroup Widgets
 */

/*!
 * \fn ProfileDrawPart::ProfileDrawPart(ProfileWidget *parent)
 *
 * Constructor
 *
 * Simply stores parent into widget.
 *
 * \param parent  Pointer to a ProfileWidget
 *
 * \sa widget
 */

/*!
 * \fn ProfileDrawPart::draw(QPainter *painter, QPaintEvent *event)
 *
 * Draw something into a ProfileWidget.
 *
 * This is a pure virtual method which needs to be implemented
 * in any descendant of ProfileDrawPart.
 *
 * \param painter  Pointer to a QPainter, provided by ProfileWidget::paintEvent()
 * \param event    Pointer to a QPaintEvent
 */

/*!
 * \fn ProfileDrawPart::mousePressEvent(QMouseEvent *event)
 *
 * React to a mouse-event.
 *
 * This is a virtual method which may be implemented in descendant of
 * ProfileDrawPart.
 *
 * \note This isn't not yet implement, but easy TODO: just make something
 * like SlippyMapWidget::mousePressEvent()
 *
 * \param event    Pointer to a QMouseEvent
 * \return true, if the mouse press has been handled
 */


/*!
 * \var ProfileDrawPart::widget
 *
 * Pointer to the ProfileWidget that this ProfileDrawPart should draw into.
 *
 * Use this to access the widget's parameters, e.g. height, width etc.
 */



/*!
 *
 * \class DrawLoS
 *
 * Draw a Line-of-Sight (LoS) into a ProfileWidget.
 *
 * \sa ProfileWidget
 * \ingroup Widgets
 */

/*!
 * \fn DrawLoS::DrawLoS(ProfileWidget *parent)
 *
 * Constructor
 *
 * \param parent   Pointer to a ProfileWidget
 */

/*!
 * Paints a Line-of-Sight into a ProfileWidget
 *
 * The Line-of-Sight is simply a line from the Transceiver to
 * the Receiver.
 *
 * As this widget doesn't have any idea about antenna heights, it
 * will draw a line from ground to ground.
 *
 * \param painter  Pointer to a QPainter, provided by ProfileWidget::paintEvent()
 * \param event    Pointer to a QPaintEvent
 */
void DrawLoS::draw(QPainter *painter, QPaintEvent *event)
{
	MYTRACE("DrawLoS::draw");
	Q_UNUSED(event);

	painter->setPen(Qt::red);
	painter->drawLine(widget->margin_left,
	                  widget->data->e.first(),
	                  widget->width() - widget->margin_right,
	                  widget->data->e.last() );
}

#ifndef PROFILE_WIDGET_H
#define PROFILE_WIDGET_H

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
  * \file ProfileWidget.h
  *
  * Widget to display elevation profiles.
  *
  * \ingroup Widgets
  */

#include <QWidget>


class GeoData;
class ProfileDrawPart;


class ProfileWidget : public QWidget
{
public:
	ProfileWidget(QWidget *parent=0, GeoData *data=0);
	void setData(GeoData *data);
	virtual void paintEvent(QPaintEvent * event);

	int heightToPixel(double h) const;
	double pixelWidthToMeter(int w) const;
	double pixelHeightToMeter(int w) const;


	QList<ProfileDrawPart *> parts;
	GeoData *data;
	double max_elevation;

	int margin_left;
	int margin_right;
	int margin_top;
	int margin_bottom;
};


class ProfileDrawPart : public QObject
{
public:
	ProfileDrawPart(ProfileWidget *parent) : QObject(parent), widget(parent) {};
	virtual void draw(QPainter *painter, QPaintEvent *ev) = 0;
	virtual bool mousePressEvent(QMouseEvent *) { return false; };

protected:
	ProfileWidget *widget;
};


class DrawLoS : public ProfileDrawPart
{
public:
	DrawLoS(ProfileWidget *parent) : ProfileDrawPart(parent) {};
	virtual void draw(QPainter *painter, QPaintEvent *ev);
};



#endif

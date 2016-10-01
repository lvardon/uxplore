#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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


#include <QMainWindow>

#include "ui_mainwindow.h"
#include "GeoUtils.h"
#include "ITUTerrain.h"


class SRTMv4;
class SlippyPath;
class DrawITUTerrain;


class MainWindow : public QMainWindow, Ui::MainWindow
{
	Q_OBJECT
public:
	MainWindow();
private slots:
	void slotPosition(const QPointF &pos);
	void slotTxPressed();
	void slotRxPressed();
private:
	void log(const QString &s);
	QPointF tx;
	QPointF rx;
	GeoData data;
	SRTMv4 *srtm;
	SlippyPath *path;
	DrawITUTerrain *terrain;
	ITUTerrain t;
};


#endif

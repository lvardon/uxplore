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
 */

#include "mainwindow.h"
#include "SlippyLocations.h"
#include "GeoUtils.h"
#include "SRTMv4.h"
#include "DrawITUTerrain.h"


MainWindow::MainWindow()
{
	MYTRACE("MainWindow::MainWindow");
	setupUi(this);
	setWindowTitle("Profile Evaluation Demo");

	// Add locations to map
	SlippyLocations *loc = new SlippyLocations(map);
	map->appendPart(loc);

	// Connect slippy map mouse click to us
	connect(map, SIGNAL(position(const QPointF &)),
	        this, SLOT(slotPosition(const QPointF &)) );

	// Connect MapChoser to SlippyMap
	connect(mapChooser, SIGNAL(setMap(const QString &, const QString &, int, int)),
	        map, SLOT(setMap(const QString &, const QString &, int, int)) );

	connect(txButton, SIGNAL(clicked(bool)), this, SLOT(slotTxPressed()) );
	connect(rxButton, SIGNAL(clicked(bool)), this, SLOT(slotRxPressed()) );

	srtm = new SRTMv4("../../data", this);
	path = new SlippyPath(0, map);
	map->appendPart(path);

	terrain = new DrawITUTerrain(&t, profile);
	profile->parts.append(terrain);

	log(tr("Select transmitter position"));
}


void MainWindow::slotPosition(const QPointF &pos)
{
	MYTRACE("MainWindow::slotPosition(%f, %f)", pos.x(), pos.y());

	if (txButton->isChecked()) {
		tx = pos;
		log(QString("  Transmitter: %1,%2").arg(pos.x()).arg(pos.y()));
		txButton->setChecked(false);
		rxButton->setChecked(true);
		log(tr("Select receiver position"));
	} else

	if (rxButton->isChecked()) {
		rx = pos;
		log(QString("  Receiver: %1,%2").arg(pos.x()).arg(pos.y()));
	} else

	if (!txButton->isChecked() && !rxButton->isChecked())
		return;

	if (tx.isNull() || rx.isNull())
		return;

	data.clear();
	MakePath(tx, rx, data);

	log(QString("Points on great-circle path: %1").arg(data.posCount()));

	int valid = 0;
	for (int i=0; i<data.posCount(); i++) {
		double elev = srtm->get( QPointF(data.lon.at(i), data.lat.at(i)) );
		//MYVERBOSE("elev %d: %f", i, elev);
		if (elev != nodata)
			valid++;
		data.e.append(elev);
	}
	path->setPath(&data);

	// Report invalid points?
	if (valid != data.posCount())
		log(QString("Points without valid heights: %1").arg(data.posCount() - valid ));

	if (valid >= 2) {
		profile->setData(&data);
		t.pathProfileAnalysis(144, 45, 10, 10, data);

		log("\nGreat-Circle distances:");
		log(tr("%1 km\tBetween Tx and Rx").arg(t.dtot / 1000, 0, 'g', 3));
		log(tr("%1 km\tTx horizon").arg(t.dlt / 1000, 0, 'g', 3));
		log(tr("%1 km\tRx horizon").arg(t.dlr / 1000, 0, 'g', 3));

		log("\nHeights above ground:");
		log(tr("%1 m\tTx antenna").arg(t.htg));
		log(tr("%1 m\tRx antenna").arg(t.hrg));

		log("\nHeights above mean sea level:");
		log(tr("%1 m\tTx base").arg(t.hgt));
		log(tr("%1 m\tTx antenna").arg(t.hts));
		log(tr("%1 m\tRx base").arg(t.hgr));
		log(tr("%1 m\tRx antenna").arg(t.hrs));

		log("\nSmooth earth approximation:");
		log(tr("%1 m\tTx smooth earth surface").arg(t.hst));
		log(tr("%1 m\tRx smooth earth surface").arg(t.hsr));
		log(tr("%1 m\tEffective Tx antenna height").arg(t.hte));
		log(tr("%1 m\tEffective Rx antenna height").arg(t.hre));

		log("\nMiscelleanous:");
		log(tr("Propagation\t%1").arg(t.path_trans_horizon ? tr("Over-the-Horizon") : tr("Line-of-Sight")));
		log(tr("%1 m\tTerrain roughness parameter").arg(t.hm));
		log(tr("%1\tMedian effective earth radius factor").arg(t.k50));
		log(tr("%1 km\tMedian value of effective Earth radius").arg(t.ae));
		// log(tr("%1: Horizontal elevation angle (above local horizont) at Tx ant OR the elevation angle to the other terminal [mrad]").arg(t.theta_t));
		// log(tr("%1: Horizontal elevation angle (above local horizont) at Tx ant OR the elevation angle to the other terminal [mrad]").arg(t.theta_r));
		// log(tr("%1: Earth centered angular distance between Tx & Rx antennas [mrad]").arg(t.theta));
		log("");

	} else {
		profile->setData(NULL);
	}

	map->update();
}

void MainWindow::log(const QString &s)
{
	textEdit->append(s);
}


void MainWindow::slotTxPressed()
{
	MYTRACE("MainWindow::slotTxPressed");
	textEdit->clear();
	log(tr("Select transmitter position"));
	rx.setX(0);
	rx.setY(0);
	path->setPath(NULL);
	map->update();
	rxButton->setChecked(false);
}


void MainWindow::slotRxPressed()
{
	MYTRACE("MainWindow::slotTxPressed");
	txButton->setChecked(false);
}

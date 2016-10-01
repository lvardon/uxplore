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

#include "LocationMapDialog.h"

#include <QFormLayout>
#include <QLineEdit>

#include "SlippyMapWidget.h"
#include "SlippyLocations.h"
#include "GeoUtils.h"


/*!
 * \file LocationMapDialog.cpp
 *
 * Dialog to enter/edit locations.
 *
 * \ingroup Widgets
 *
 *
 * \class LocationMapDialog
 * \ingroup Widgets
 *
 * Subclass of LocationDialog.
 *
 * This class enhances the automatically generated LocationDialog with a
 * \ref SlippyMapWidget "slippy map", and a Maidenhead locator field.
 */

/*!
 * Constructor.
 *
 * \param record Pointer to the Location record to edit.
 *
 * \param parent Pointer to a QWidget parent
 *
 * \param f Window flags, if needed
 */
LocationMapDialog::LocationMapDialog(Location *record, QWidget *parent, Qt::WindowFlags f)
		: LocationDialog(record, parent, f)
{
	MYTRACE("LocationMapDialog::LocationMapDialog");

	setMinimumSize(QSize(width(), height()*2));

	slippy = new SlippyMapWidget(this);
	formLayout->insertRow(1, trUtf8("Map"), slippy);
	if (record->longitude && record->latitude)
		slippy->setLonLat(QPointF(record->longitude, record->latitude), 13);
	SlippyCenterMark *mark = new SlippyCenterMark(slippy);
	slippy->appendPart(mark);

	SlippyLocations *loc = new SlippyLocations(slippy);
	slippy->appendPart(loc);

	connect(slippy, SIGNAL(position(const QPointF &)),
	        this, SLOT(slotPosition(const QPointF &)) );
	connect(editLongitude, SIGNAL(textEdited(const QString &)),
	        this, SLOT(slotLonLatEnter()) );
	connect(editLatitude, SIGNAL(textEdited(const QString &)),
	        this, SLOT(slotLonLatEnter()) );

	editQth = new QLineEdit(this);
	formLayout->insertRow(2, trUtf8("&QTH"), editQth);
	editQth->setText( toMaidenhead(QPointF(m->longitude, m->latitude)) );

	connect(editQth, SIGNAL(textEdited(const QString &)),
	        this, SLOT(slotQthEnter()) );
}


/*!
 * Update position from \ref slippy.
 *
 * This updates the QLineEdit widgets \ref editLongitude,
 * \ref editLatitude and \ref editQth whenever the user clicks into
 * the slippy map.
 *
 * \param lonLat Geographical coordinate in degrees
 */
void LocationMapDialog::slotPosition(const QPointF &lonLat)
{
	MYTRACE("LocationMapDialog::slotPosition");

	editLongitude->setText(QString::number(lonLat.x()));
	editLatitude->setText(QString::number(lonLat.y()));
	editQth->setText( toMaidenhead(lonLat) );
}


/*!
 * Update position from edit widgets
 *
 * This is connected to \ref editLongitude and \ref editLatitude \a
 * textEdited() signal. If those widgets contain valid data, convert them to
 * a double and if that succeeds, then the \ref slippy and \ref editQth will
 * be updated.
 */
void LocationMapDialog::slotLonLatEnter()
{
	MYTRACE("LocationMapDialog::slotLonLatEnter");
	bool ok;

	if (!editLongitude->hasAcceptableInput() ||
	    !editLatitude->hasAcceptableInput()) return;

	double lon = editLongitude->text().toDouble(&ok);
	if (!ok)
		return;
	double lat = editLatitude->text().toDouble(&ok);
	if (!ok)
		return;
	QPointF lonLat(lon, lat);
	slippy->setLonLat(lonLat);
	editQth->setText( toMaidenhead(lonLat) );
}


/*!
 * Update position from QTH (Maidenhead) edit widget
 *
 * This converts the Maidenhead locator from \ref editQth using
 * ::fromMaidenhead() into latitudes. Then it updates
 * \ref slippy, \ref editLongitude and \ref editLatitude accordingly.
 *
 * \note Entering a Maidenhead locator changes the zoom of the
 * map to zoom level 12, which is rather coarse. This ensures that
 * the real position of the QTH should now be visible.
 */
void LocationMapDialog::slotQthEnter()
{
	MYTRACE("LocationMapDialog::slotQthEnter");

	QPointF lonLat;
	bool ok = fromMaidenhead(editQth->text(), lonLat);
	if (ok) {
		slippy->setLonLat(lonLat, 12);
		editLongitude->setText( QString::number(lonLat.y()) );
		editLatitude->setText( QString::number(lonLat.x()) );
	}
}




/*!

\var LocationMapDialog::slippy

This is a sliding map, implemented via SlippyMapWidget.


\var LocationMapDialog::editQth

This is a QLineEdit widget that you can use to enter a Maidenhead locator.

*/

#include "LocationMapDialog.moc"
#include "locations.moc"

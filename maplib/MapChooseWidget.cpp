#include "../global.h"

#define DEBUGLVL 0
#include "mydebug.h"

#include <QDebug>

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


#include "MapChooseWidget.h"


/*!
 * \class MapChooseWidget
 *
 * A QComboBox-derived widget to change the displayed map in a SlippyMapWidget.
 *
 * Each "map" has the properties Description (e.g. copyright notice), Location (URL),
 * minimum and maximum zoom level.
 */

/*!
 * Constructor
 *
 * \param parent  Pointer to a QWidget
 */
MapChooseWidget::MapChooseWidget(QWidget *parent)
    : QComboBox(parent)
{
    MYTRACE("MapChooseWidget::MapChooseWidget");


    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(mapChanged(int)) );


    setFocusPolicy( Qt::NoFocus );
}

void MapChooseWidget::clearAll( )
{


    paths.clear();     //!< URLs to the tiles
    licenses.clear();  //!< License text to be display in the lower right corner
    minZoom.clear();    //!< Minumum supported zoom level
    maxZoom.clear();    //!< Maximum supported zoom level
    userAgents.clear(); //!< user agent to the tiles
    referers.clear();
    keyShortCuts.clear();
    keyZooms.clear();

    this->clear();
}

int MapChooseWidget::getMapIndexBykeyShortCut( int p_keyShortCut )
{

    int indexMap=0;
    debugp("search map index for key :  " +  QString::number( p_keyShortCut )) ;

    for(QList<int>::const_iterator it(keyShortCuts.begin()); it!=keyShortCuts.end(); ++it)
    {
        const int KeyParamValue(*it);

        if (KeyParamValue == p_keyShortCut ) {
            debugp("map index for F key is (zero based) found :  " +  QString::number( indexMap )) ;
            return indexMap;
        }
        indexMap++;
    }

    debugp("not found : map index for key :  " +  QString::number( p_keyShortCut )) ;
    return -1;
}
QString MapChooseWidget::getMapNameBykeyShortCut( int p_keyShortCut )
{
    QString retval;
    retval = "";
    int indexMap=0;
    debugp("search map index for key :  " +  QString::number( p_keyShortCut )) ;

    for(QList<int>::const_iterator it(keyShortCuts.begin()); it!=keyShortCuts.end(); ++it)
    {
        const int KeyParamValue(*it);

        if (KeyParamValue == p_keyShortCut ) {
            debugp("map index for F key is (zero based) found :  " +  QString::number( indexMap )) ;
            retval = itemText(indexMap);
            return retval;
        }
        indexMap++;
    }

    debugp("not found : map index for key :  " +  QString::number( p_keyShortCut )) ;
    return retval;
}
int MapChooseWidget::getMapZoomBykeyShortCuts( int p_keyShortCut )
{

    int indexMap=0;

    for(QList<int>::const_iterator it(keyShortCuts.begin()); it!=keyShortCuts.end(); ++it)
    {
        const int KeyParamValue(*it);

        if (KeyParamValue == p_keyShortCut ) {
            debugp("map Zoom index for F key is (0 based) found :  " +  QString::number( indexMap )) ;
            return keyZooms[indexMap];
        }
        indexMap++;
    }

    debugp("not found : map Zoom for key :  " +  QString::number( p_keyShortCut )) ;
    return -1;
}

/*!
 * Add a new map tile source.
 *
 * \param desc Text to be displayed in the QComboBox
 *
 * \param path Path or URL to the map data, e.g.
 *             "http://tile.openstreetmap.org/%1/%2/%3.png". The %%1 ... %%3
 *             place-holders get replaced by current zoom Level (curZoom), x
 *             and y position (in that order).
 *
 * \param min Minimum zoom level.
 *
 * \param max Maximum zoom level.
 *
 * \param license License text to be show in the lower-right of the displayed map.
 *
 */

void MapChooseWidget::add(const QString &desc,
                          const QString &path,
                          int zmin,
                          int zmax,
                          const QString &license,
                          const QString &userAgent,
                          const QString &referer,
                          int keyShortCut,
                          int keyZoom
                         )
{
    MYVERBOSE("MapChooseWidget::add");

    addItem(desc);
    paths.append(path);
    minZoom.append(zmin);
    maxZoom.append(zmax);
    licenses.append(license);
    userAgents.append(userAgent);
    referers.append(referer);
    keyShortCuts.append(keyShortCut);
    keyZooms.append(keyZoom);

}


void MapChooseWidget::mapChoose( int mapIndex)
{

    emit setMap(paths.at(mapIndex),
                licenses.at(mapIndex),
                minZoom.at(mapIndex),
                maxZoom.at(mapIndex),
                userAgents.at(mapIndex),
                referers.at(mapIndex),
                keyShortCuts.at(mapIndex),
                keyZooms.at(mapIndex)
               );

}
/*!
 * Private signal when the user selects a new entry.
 *
 * This is connected automatically to QComboBox::currentIndexChanged(int)
 *
 * \param i   index of selected entry
 */
void MapChooseWidget::mapChanged(int i)
{
    debugp("Map changed i= " + QString::number(i));

    MYTRACE("MapChooseWidget::mapChanged(%d)", i);

    if (!paths.count())
    {
        debugp("paths.count == 0 : return;");
        return;
    }

    emit setMap(paths.at(i),
                licenses.at(i),
                minZoom.at(i),
                maxZoom.at(i),
                userAgents.at(i),
                referers.at(i),
                keyShortCuts.at(i),
                keyZooms.at(i)
               );
}



/*!
 * \fn MapChooseWidget::setMap(const QString &path, const QString &license, int min, int max)
 *
 * Signal with new map data. Should be connected to SlippyMapWidget::setMap().
 *
 * \param path Path or URL to the map data, e.g.
 *             "http://tile.openstreetmap.org/%1/%2/%3.png". The %%1 ... %%3
 *             place-holders get replaced by current zoom Level (curZoom), x
 *    	       and y position (in that order).
 *
 * \param license License text to be show in the lower-right of the displayed map.
 *
 * \param min Minimum zoom level.
 *
 * \param max Maximum zoom level.
 */

#include "MapChooseWidget.moc"

#ifndef MAP_CHOOSER_H
#define MAP_CHOOSER_H

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
 * \file MapChooseWidget.h
 *
 * A QComboBox-derived widget to change the displayed map in a SlippyMapWidget.
 */


#include <QComboBox>
#include <QList>


class SlippyMapWidget;
class QVBoxLayout;


class MapChooseWidget : public QComboBox
{
    Q_OBJECT
public:
    MapChooseWidget(QWidget *parent=0);

    void add(const QString &desc,
             const QString &path,
             int min,
             int max,
             const QString &license,
             const QString &userAgent,
             const QString &referer,
             int keyShortCut,
             int ketyZoom

             );

    QStringList paths;     //!< URLs to the tiles
    QStringList licenses;  //!< License text to be display in the lower right corner
    QList<int> minZoom;    //!< Minumum supported zoom level
    QList<int> maxZoom;    //!< Maximum supported zoom level
    QStringList userAgents; //!< user agent to the tiles
    QStringList referers;   //!< referer URLs to the tiles
    QList<int> keyShortCuts;
    QList<int> keyZooms;

    void mapChoose( int mapIndex) ;

    void clearAll(void);

    int getMapIndexBykeyShortCut( int keyShortCuts );
    int getMapZoomBykeyShortCuts( int keyShortCuts );
    QString getMapNameBykeyShortCut( int p_keyShortCut );

private slots:
    void mapChanged(int);
signals:
    void setMap(const QString &path,
                const QString &license,
                int min,
                int max,
                const QString &userAgent,
                const QString &referer,
                int keyShortCut,
                int keyZoom
                );
};

#if 0
class Chooser : public QWidget
{
public:
    Chooser(QWidget *parent=0);
private:
    QComboBox *c;
    SlippyMapWidget *map;
    QVBoxLayout *layout;
};
#endif



#endif

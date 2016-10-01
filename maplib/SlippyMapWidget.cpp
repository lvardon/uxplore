#define DEBUGLVL 0
#include "mydebug.h"

#include "../mainWindow.h"
#include "../projectData.h"

#include "GeoUtils.h"

#include <QFont>
#include "../dialogWptProp.h"

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


/*!
 * \file SlippyMapWidget.cpp
 *
 * Widget that implements a slippy map.
 *
 * \ingroup Widgets
 */

#include <QDebug>
#include <QMenu>
#include <QFile>
#include <QPainter>
#include <QPixmap>
#include <QWheelEvent>
#include <QNetworkReply>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <math.h>

#include "SlippyMapWidget.h"
#include "Downloader.h"


/*!
 * Size (in pixels) of a tile.
 */
const int tileSize = 256;


/*!
 * Convert coordinates from the Google-Maps API coordinates to Longitude (in
 * degrees).
 *
 * \param x  x-position of tile.
 * \param z  Zoom level of tile.
 * \return Longitude in degrees.
 *
 * See http://wiki.openstreetmap.org/index.php/Slippy_map_tilenames#C.2FC.2B.2B
 * for reference.
 *
 * this returns the NW-corner of the square. Use the function with xtile+1 and/or ytile+1 to get the other corners.
 * With xtile+0.5 & ytile+0.5 it will return the center of the tile.
 */
static double tile2lon(double x, int z)
{
    return x / pow(2.0, z) * 360.0 - 180;
}

/*!
 * Convert coordinates from the Google-Maps API coordinates to Latitude (in
 * degrees).
 *
 * \param y  y-position of tile.
 * \param z  Zoom level of tile.
 * \return Latitude in degrees.
 *
 * See http://wiki.openstreetmap.org/index.php/Slippy_map_tilenames#C.2FC.2B.2B
 * for reference.
 *
 * this returns the NW-corner of the square. Use the function with xtile+1 and/or ytile+1 to get the other corners.
 * With xtile+0.5 & ytile+0.5 it will return the center of the tile.
 */
static double tile2lat(double y, int z)
{
    double n = M_PI - 2.0 * M_PI * y / pow(2.0, z);
    return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}


/*!
 * \class SlippyMapWidget
 * \ingroup Widgets
 *
 * Sliding map widget that uses one or more \ref SlippyMapPart
 * "SlippyMapParts" for map drawing.
 *
 * Can be used to browse online, tile-based maps like OpenStreetMap or
 * Google-Maps. It's veeeery loosely based on the %SlippyMapWidget from
 * Merkaartor and the lightmaps demo from Qt.
 *
 * Compared with Ligthmaps (which is a demo), there's no useless lens, but
 * zoom support (calulate the tile of a higher zoom level based an already
 * loaded lower zoom tile). It also sports some logic to keep the number of
 * downloaded tiles minimal.
 *
 * The make-up-a-tile-by-zooming feature is present in Merkaartor's
 * %SlippyMapWidget. However, Merkaartor uses deprecated interfaces (QHttp
 * instead of QNetworkAccessManager) and has more dependencies.
 *
 * This widget delegates drawing to SlippyMapPart derived classes. The can
 * draw the map itself, some overlays, markers, flags, whatever.
 */

/*!
 * Constructor
 *
 * This constructor will already create a SlippyMapTiled instance
 * and stores this in #map and adds it to #parts.
 *
 * \param parent Pointer to QWidget
 */
SlippyMapWidget::SlippyMapWidget(QWidget* p_parent)
    : QWidget(p_parent)
    , map(0)
{

    parent = p_parent;

    MYTRACE("SlippyMapWidget::SlippyMapWidget(%p)", p_parent);

    // Without this sizepolicy the widget won't show up inside a
    // QFormLayout. Especially the stretch is imported.
    QSizePolicy pol(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pol.setVerticalStretch(1);
    setSizePolicy(pol);

    map = new SlippyMapTiled(this);
    parts.append(map);
    setLonLat(QPointF(10.3, 51.25), 6); // Germany

    mapZoomToUpper = 15;
    mapZoomToLower = 5;

    curentWptId = -1;
    pastedWptId = -1;

    contextMenuCreated = false;
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),this, SLOT(ShowContextMenu(const QPoint &)));

    setFocusPolicy( Qt::ClickFocus );
    installEventFilter(this);
}

bool SlippyMapWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        debugp("map key " + QString::number( keyEvent->key() ) );
    }
    return QObject::eventFilter(obj, event);
}


void SlippyMapWidget::ShowContextMenu(const QPoint &pos)
{

    debugp("Menu display");

    if (contextMenuCreated  == false)
    {
        debugp ("Menu display false");

        contextMenuCreated  = true;

        mapActionWpt = new QAction( QString::fromUtf8("Ajouter une balise ici") + QString::number(clicLon) + ", " + QString::number(clicLat), this );
        mapActionWpt ->setIcon(QIcon( iconDir + "tool_ctx_addpoi.png"));
        contextMenu.addAction( mapActionWpt );
        connect( mapActionWpt, SIGNAL( triggered() ), this, SLOT( onMapNewWaypoint() ) );

        mapActionZoomInHere = new QAction ( QString::fromUtf8("Zoommer"), this );
        contextMenu.addAction( mapActionZoomInHere );
        mapActionZoomInHere ->setIcon(QIcon(iconDir + "tool_ctx_zoomplus.png"));
        connect(mapActionZoomInHere , SIGNAL(triggered()), this, SLOT(onMapActionZoomInHere()));

        mapActionZoomOutHere = new QAction ( QString::fromUtf8("Dé-zoommer"), this );
        contextMenu.addAction( mapActionZoomOutHere );
        mapActionZoomOutHere ->setIcon(QIcon(iconDir + "tool_ctx_zoomminus.png"));
        connect(mapActionZoomOutHere , SIGNAL(triggered()), this, SLOT(onMapActionZoomOutHere()));

        mapActionZoomInUpper = new QAction ( QString::fromUtf8("Zoom +"), this );
        contextMenu.addAction( mapActionZoomInUpper );
        connect(mapActionZoomInUpper, SIGNAL(triggered()), this, SLOT(onMapActionZoomInUpper()));

        mapActionZoomInLower = new QAction ( QString::fromUtf8("Zoom -"), this );
        contextMenu.addAction( mapActionZoomInLower );
        connect(mapActionZoomInLower, SIGNAL(triggered()), this, SLOT(onMapActionZoomInLower()));

        mapActionCenterHere = new QAction ( QString::fromUtf8("Centrer"), this );
        contextMenu.addAction( mapActionCenterHere );
        mapActionCenterHere->setIcon(QIcon( iconDir + "tool_wptcenter.png"));
        connect(mapActionCenterHere , SIGNAL(triggered()), this, SLOT(onMapActionCenterHere()));

        contextMenu.addSeparator();
        mapActionCopyWpt= new QAction ( trUtf8("Déplacer cette balise","wpt copy"), this );
        contextMenu.addAction( mapActionCopyWpt );
        mapActionCopyWpt->setIcon(QIcon( iconDir + "tool_wptmove.png"));
        connect(mapActionCopyWpt , SIGNAL(triggered()), this, SLOT(onMapActionCopyWpt()));

        mapActionPasteWptHere = new QAction ( trUtf8("Placer la balise ici", "wpt - paste here"), this );
        contextMenu.addAction( mapActionPasteWptHere );
        mapActionPasteWptHere->setIcon(QIcon( iconDir + "tool_wptmovehere.png"));
        connect(mapActionPasteWptHere , SIGNAL(triggered()), this, SLOT(onMapActionPasteWptHere()));
        contextMenu.addSeparator();

        mapActionCopyWptCoord= new QAction ( trUtf8("Copier les coordonnées","wpt copy coord"), this );
        mapActionCopyWptCoord->setIcon(QIcon( iconDir + "tool_ctx_copycoord.png"));
        contextMenu.addAction( mapActionCopyWptCoord );
        connect(mapActionCopyWptCoord , SIGNAL(triggered()), this, SLOT(onMapActionCopyWptCoord()));

        //QString libSearchWeb;
        QString url = searchInternetMapUrl;
        url.replace("%1", QString::number(clicLat));
        url.replace("%2", QString::number(clicLon));
        //libSearchWeb = "Ouvrir le lien"; // + searchInternetMapUrl;
        mapActionSearchInternet = new QAction (url + "..." , this );
        mapActionSearchInternet->setIcon(QIcon( iconDir + "tool_ctx_web.png"));
        contextMenu.addAction( mapActionSearchInternet );
        connect(mapActionSearchInternet, SIGNAL(triggered()), this, SLOT(onMapActionSearchInternet()));

        connect(&contextMenu, SIGNAL(aboutToHide()), this, SLOT(contextMenuEventHide()));

    }
    else
    {
        debugp("Menu display true");
        mapActionCopyWpt->setEnabled(false);
    }

    qlonglong markerId = getSelectedIdMarker( pos );

    debugp ("markerSelected id ="  + QString::number(  markerId ) )  ;

    QAction *action = contextMenu.actions().at(0); // access just the first QAction
    if (markerId > 0)
    {
        tableWpt->curentWptId = markerId;
        curentWptId = markerId;

        action->setText(QString::fromUtf8("Propriétés : ") + markersPtr->getMarkerLabelById(markerId) + " (#" + QString::number(markerId) + ")" );
        action->setIcon(QIcon(iconDir + "tool_properties.png"));
        disconnect(mapActionWpt, 0, 0, 0);
        connect( mapActionWpt, SIGNAL( triggered() ), this, SLOT( onMapEditWaypoint() ) );

        debugp ("Before color " + QString::number( curentWptId) ) ;
        mainWindow *p_mainWindow = getMainwindow();
        if (p_mainWindow  != 0 )
            tableWpt->hilightWpt( curentWptId-1, p_mainWindow->lineSelectColor, Qt::white);

        debugp ("After color " + QString::number( curentWptId ));

        mapActionCopyWpt->setEnabled(true);
        mapActionPasteWptHere->setEnabled(false);
    }
    else
    {
        action->setText(QString::fromUtf8("Ajouter une balise ici : ") + QString::number(clicLon) + ", " + QString::number(clicLat));
        action->setIcon(QIcon( iconDir + "tool_ctx_addpoi.png"));
        disconnect(mapActionWpt, 0, 0, 0);
        connect( mapActionWpt, SIGNAL( triggered() ), this, SLOT( onMapNewWaypoint() ) );

        mapActionCopyWpt->setEnabled(false);

        if ( pastedWptId == -1)
            mapActionPasteWptHere->setEnabled(false);
        else
            mapActionPasteWptHere->setEnabled(true);

    }

    contextMenu.exec(mapToGlobal(pos));
}

void SlippyMapWidget::contextMenuEventHide()
{
    debugp("Hide menu curentWptId=" + QString::number( curentWptId) ) ;
    if (curentWptId != -1 && curentWptId < tableWpt->rowCount() )
    {
        tableWpt->hilightWpt( curentWptId-1, Qt::white, Qt::black );
    }
    debugp("end HIDE menu");
}

void SlippyMapWidget::onContextMenuClose( void )
{

}

qlonglong SlippyMapWidget::getSelectedIdMarker( const QPoint &pos )
{

    int markerId =-1;
    QRectF r = getArea();
    QPointF lonLat = getLonLat(pos);

    QPointF srcPoint;
    srcPoint =  QPointF( r.left(), lonLat.y());
    QPointF dstPoint;
    dstPoint =  QPointF( r.right(), lonLat.y());
    double distPoints  = Great_Circle_Distance( srcPoint, dstPoint ); // Metres

    QString strDistPointsKm = QString::number(distPoints / 1000, 'g', 10);
    QString strDistPointsM = QString::number(distPoints, 'g', 10);
    debugp( "Echelle, largeur totale carte  = " +  strDistPointsKm + " km, ( " + strDistPointsM + " m.)" ) ;

    strDistPointsKm = QString::number( ((distPoints/width())*100) / 1000, 'g', 10);
    strDistPointsM = QString::number( ((distPoints/width())*100), 'g', 10);
    debugp( "Echelle, 100 px = " + strDistPointsKm  + " km, ( " + strDistPointsM  +  " m.)" );

    QString strDistPointsPixel = QString::number( (width() / (distPoints)), 'g', 10);
    debugp( "Echelle, 1 m = " + strDistPointsPixel  + " pixels") ;
    debugp( "markersCurrent displayed count ->" + QString::number( markersPtr->countMarkers()) );

    qlonglong nbMarkers = markersPtr->countMarkers();
    qlonglong idxCloseMarker = -1;
    double distanceCloseMarker = -1;
    double distMapPixel = -1;

    for (qlonglong imk = 0; imk < nbMarkers; imk++)
    {
        QPointF markerPos = markersPtr->getMarkerPosByIdx(imk);
        double currentDistMarker;
        currentDistMarker = Great_Circle_Distance( lonLat, markerPos);
        debugp("markersCurrent label #id=" + QString::number( markersPtr->getMarkerIdByIdx(imk) )+ "  idx =" + QString::number( imk ) + ":" + markersPtr->getMarkerLabelByIdx(imk) + " pos=" + markerPos.rx() + "," + markerPos.ry() + "Distance (km) = " + (currentDistMarker/1000) );
        if (imk == 0)
        {
            distanceCloseMarker = currentDistMarker;
            idxCloseMarker = 0;
        }
        else
        {
            if ( currentDistMarker < distanceCloseMarker )
            {
                distanceCloseMarker = currentDistMarker;
                idxCloseMarker = imk;
            }
        }

        // pixels
        QPointF srcPoint;
        srcPoint =  QPointF( r.left(), lonLat.y());
        QPointF dstPoint;
        dstPoint =  QPointF( r.right(), lonLat.y());
        distMapPixel = (distPoints/width())*1;

        if (idxCloseMarker != -1 )
        {
            QPointF markerPos = markersPtr->getMarkerPosByIdx(idxCloseMarker);
            debugp( "Le plus proche markersCurrent label #id=" + QString::number( markersPtr->getMarkerIdByIdx(idxCloseMarker)) + " idx =" + QString::number( idxCloseMarker) + ":" + markersPtr->getMarkerLabelByIdx(idxCloseMarker)  + " pos=" + QString::number( markerPos.rx()) + "," + QString::number( markerPos.ry()) + " distance = " + QString::number( distanceCloseMarker) + " soit : " + QString::number( distanceCloseMarker / distMapPixel ) + " pixels" );

            markerId = markersPtr->getMarkerIdByIdx(idxCloseMarker);

            if (( distanceCloseMarker / distMapPixel )< (32/16) || ( distanceCloseMarker / distMapPixel ) < (37/2))
            {
                debugp("HIT !!!");

            }
            else markerId = -1;

        }
    }

    return markerId;
}

void SlippyMapWidget::onMapActionSearchInternet(void)
{

    debugp( "onMapActionSearchInternet : Current point : " +  QString::number( clicLat ) + ", " +QString::number( clicLon ));
    QString url = searchInternetMapUrl;
    url.replace("%1", QString::number(clicLat));
    url.replace("%2", QString::number(clicLon));
    debugp( "url=" + url );
    QDesktopServices::openUrl(QUrl(url));
}


void SlippyMapWidget::onMapActionCopyWpt()
{
    debugp("copy wpt , curentWptId=" + QString::number( curentWptId) ) ;
    pastedWptId = curentWptId;

}
void SlippyMapWidget::onMapActionPasteWptHere()
{

    debugp("paste wpt ici : " + QString::number( clicLat)  + "," + QString::number( clicLon) + " pastedWptId=" + QString::number( pastedWptId) ) ;

    if (pastedWptId >= 0)
    {
        debugp("markersCurrent label #id=" + QString::number( pastedWptId)  +  ":"  +   markersPtr->getMarkerLabelById(pastedWptId)) ;
        markersPtr->setMarkerPosById( pastedWptId , QPointF (clicLat, clicLon) );
        tableWpt->wptSetPos( pastedWptId , QPointF (clicLat, clicLon) );
        projectDatas::wptType *wptCur = m_projectDatas->getWptFromId2( pastedWptId );

        if (wptCur==NULL)
        {
            debugp("wptCur is NULL");
            return;
        }
        tableWpt->refreshMarkerByWpt( wptCur );
        update();
    }

}

void SlippyMapWidget::onMapActionCopyWptCoord()
{
    QClipboard *myClip = QApplication::clipboard();
    myClip->setText(QString ( QString::number(clicLat) + "," + QString::number(clicLon)) );
}

// Map context menu action
void SlippyMapWidget::onMapActionCenterHere(void)
{
    debugp ( "Map center = " + QString::number( clicLon)  + "," + QString::number( clicLat) ) ;

    QPointF lonlat = QPointF( clicLon, clicLat);
    setLonLat( lonlat, curZoom );
}
void SlippyMapWidget::onMapActionZoomInHere(void)
{

    debugp ( "Map center = " + QString::number( clicLon)  + "," + QString::number( clicLat) ) ;

    QPointF lonlat = QPointF( clicLon, clicLat);

    if (curZoom<= 19)
        curZoom++;

    setLonLat( lonlat, curZoom );

}
void SlippyMapWidget::onMapActionZoomOutHere(void)
{

    debugp ( "Map center = " + QString::number( clicLon)  + "," + QString::number( clicLat) ) ;

    QPointF lonlat = QPointF( clicLon, clicLat);
    if (curZoom >1)
        curZoom--;
    setLonLat( lonlat, curZoom );

}

void SlippyMapWidget::onMapNewWaypoint(void)
{
    debugp ( "Ajouter ici = " + QString::number( clicLon)  + "," + QString::number( clicLat) ) ;

    QPointF lonlat = QPointF( clicLat, clicLon);
    tableWpt->addNewWpt( lonlat );

    debugp ( "Ajouté ici = " + QString::number( clicLon)  + "," + QString::number( clicLat) ) ;

}

void SlippyMapWidget::onMapEditWaypoint(void)
{
    tableWpt->sheetEditProp();
}

void  SlippyMapWidget::onMapActionZoomInLower(void)
{

    QPointF lonlat = QPointF( clicLon, clicLat);
    curZoom = mapZoomToLower;
    setLonLat( lonlat, curZoom );

}

void  SlippyMapWidget::onMapActionZoomInUpper(void)
{

    QPointF lonlat = QPointF( clicLon, clicLat);
    curZoom = mapZoomToUpper;
    setLonLat( lonlat, curZoom );

}
mainWindow  * SlippyMapWidget::getMainwindow(void)
{
    mainWindow  *p_mainWindow = dynamic_cast<mainWindow*>(QApplication::activeWindow());
    if (p_mainWindow == 0)
    {
        debugp ("Parent is not MainWindow !");
        return 0;
    }
    return p_mainWindow;
}


/*!
 * Append another SlippyMapPart to this widget.
 *
 * \param part Pointer to a SlippyMapPart
 */
void SlippyMapWidget::appendPart(SlippyMapPart *part)
{
    MYTRACE("SlippyMapWidget::appendPart(%p)", part);

    parts.append(part);
    QPointF lonLat = getLonLat();
    part->setLonLat(lonLat, curZoom);
}

/*!
 * Remove specified SlippyMapPart from this widget.
 *
 * The part is not free()'d, that's your job.
 *
 * \param part Pointer to a SlippyMapPart
 */
void SlippyMapWidget::removePart(SlippyMapPart *part)
{
    MYTRACE("SlippyMapWidget::removePart(%p)", part);

    parts.removeOne(part);
}


/*!
 * Remove all SlippyMapPart from this widget.
 */
void SlippyMapWidget::clearParts()
{
    MYTRACE("SlippyMapWidget::clearParts()");

    parts.clear();
    // don't get rid of the map itself, at least for now :-)
    parts.append(map);
}

SlippyMapWidget *SlippyMapPart::getSlippyMapWidget(void)
{
    return widget;
}


/*!
 * Set map source (URL)
 *
 * A call to this method will be simply relayed to SplippyTilesMap::setMap().
 *
 * \param path Path or URL to the map data, e.g.
 *             "http://tile.openstreetmap.org/%1/%2/%3.png". The %%1 ... %%3
 *             place-holders get replaced by current zoom Level (curZoom), x
 *             and y position (in that order).
 *
 * \param copyright License text to be show in the lower-right of the displayed map.
 *
 * \param min Minimum zoom level.
 *
 * \param max Maximum zoom level.
 */
void SlippyMapWidget::setMap(const QString &path,
                             const QString &copyright,
                             int min,
                             int max,
                             const QString &userAgent,
                             const QString &referer,
                             int keyShortCut,
                             int keyZoom
                            )
{
    MYTRACE("SlippyMapWidget::setMap(%s, %s, %d, %d)",
            qPrintable(path),
            qPrintable(copyright),
            min,
            max);


    if (getZoom() > max )
    {
        QPointF lonlat = getLonLat();
        setLonLat( lonlat, max);
    }
    if (getZoom() < min )
    {
        QPointF lonlat = getLonLat();
        setLonLat( lonlat, min);
    }


    //if (map)
    {

        debugp ("SlippyMapWidget : set map fired." );
        map->setMap(path, copyright, min, max, userAgent, referer, keyShortCut, keyZoom);
    }

};


/*!
 * Zoom / unzoom relative to a cursor position
 *
 * \param xy  x/y position into the widget.
 *
 * \param newZoom New zoom level.
 */
void SlippyMapWidget::zoomTo(const QPoint &xy, int newZoom)
{
    MYTRACE("SlippyMapWidget::zoomTo(%d,%d, %d)", xy.x(), xy.y(), newZoom);

    int minZoomLevel = map->getMinZoomLevel();
    int maxZoomLevel = map->getMaxZoomLevel();

    debugp(" zoom min,max = " +  QString::number(minZoomLevel) + "," +  QString::number(maxZoomLevel) );

    if (newZoom > maxZoomLevel )
    {
        newZoom = maxZoomLevel;
        return;
    }
    if (newZoom <= 0  )
    {
        newZoom = 1;
        return;
    }
    if (newZoom >= 21   )
    {
        newZoom = 21;
        return;
    }

    if (newZoom < minZoomLevel  )
    {
        newZoom = minZoomLevel;
        return;
    }

    if (map)
        setLonLat(getLonLat(xy), newZoom);
}


/*!
 * React to a mouse-wheel event with proper zooming / unzooming.
 *
 * \param ev Pointer to a QWheelEvent
 *
 * \sa zommTo(const QPoint &xy, int newZoom)
 */
void SlippyMapWidget::wheelEvent(QWheelEvent *ev)
{
    // TODO: this event should probably propagate into the parts
    // and let the parts do the work

    MYTRACE("SlippyMapWidget::wheelEvent, delta %d", ev->delta());
    int newZoom = ev->delta()/120 + curZoom;
    zoomTo(ev->pos(), newZoom);
    // When we call this via a timer, we don't download intermediate zoom
    // level pictures if the user scrolls very fast.
    QTimer::singleShot(300, this, SLOT(update()));
}


/*!
 * React to a mouse-press event.
 *
 * First the event will be sent to SlippyMapPart::mousePressEvent()
 * of attached parts. The first one that returns true processed
 * the event, so the function exits.
 *
 * Otherwise, the function checks if the left button has been pressed
 * and emits a signal in this case.
 *
 * \param ev Pointer to a QMouseEvent
 *
 * \sa SlippyMapPart::mousePressEvent()
 * \sa position()
 */
void SlippyMapWidget::mousePressEvent(QMouseEvent* ev)
{
    // TODO: this event should probably propagate into the parts
    // and let the parts do the work

    debugp("clic detect");

    foreach(SlippyMapPart *part, parts)
    {
        if (part->mousePressEvent(ev))
            return;
    }

    if (ev->button() == Qt::MidButton)
    {
        zoomTo(ev->pos(), curZoom + 1);
        update();
    }
    else if (ev->button() == Qt::LeftButton)
    {
        // Just report position
        if (map)
        {
            QPointF lonLat = getLonLat(ev->pos());
            MYDEBUG("%f, %f, zoom %d", lonLat.x(), lonLat.y(), curZoom);

            emit position(lonLat);
        }

        previousDrag = ev->pos();
    }
    else if (ev->button() == Qt::RightButton)
    {
        // Remember for dragging
        previousDrag = ev->pos();

        QRectF r = getArea();
        QPointF lonLat = getLonLat(ev->pos());

        debugp ( "clic @lon, lat = : " +  QString::number(lonLat.x()) + "," + QString::number( lonLat.y() ) );
        clicLon = lonLat.x();
        clicLat = lonLat.y();

        debugp( "clic widget w,h = " + QString::number( width()) + "," + QString::number( height() ));
        debugp( "clic widget top left,w  / bottom right,h = " +  QString::number(r.left()) + "," +  QString::number(r.width()) + " / " +  QString::number(r.right()) + "," +  QString::number(r.height()) );

        debugp( "clic map curX, curY = " +  QString::number(curX) + ", " +  QString::number(curY));
        int minZoomLevel = map->getMinZoomLevel();
        int maxZoomLevel = map->getMaxZoomLevel();
        debugp( "Zoom min/max = " +  QString::number(minZoomLevel) + "," +  QString::number(maxZoomLevel) + " / " + " Zoom current  = " +  QString::number(curZoom));

    } // end right button
}


/*!
 * React to a mouse-move event.
 *
 * First this relays the move to the map.
 * The returned geographical coordinate is then send to every
 * attached SlippyMapPart.
 *
 * \param ev Pointer to a QMouseEvent
 *
 * \sa SlippyMapTiled::move()
 * \sa SlippyMapPart::setLonLat()
 */
void SlippyMapWidget::mouseMoveEvent(QMouseEvent* ev)
{
    // TODO: this event should probably propagate into the parts
    // and let the parts do the work

    QPoint delta = ev->pos()-previousDrag;
    if (!delta.isNull())
    {
        if (map)
        {
            QPointF lonLat = move(delta);
            foreach(SlippyMapPart *part, parts)
            if (part != map)
                part->setLonLat(lonLat, curZoom);
        }
        previousDrag = ev->pos();
        update();
    }
}


/*!
 * React to a paint event.
 *
 * Creates a painter with white background and call all attached
 * SlippyMapPart instances to paint into this.
 *
 * \param ev Pointer to a QPaintEvent
 *
 * \sa SlippyMapPart::draw()
 * \sa SlippyMapPart::drawTile()
 */
void SlippyMapWidget::paintEvent(QPaintEvent *ev)
{
    MYTRACE("SlippyMapWidget::paintEvent, w %d", width());

    int wPrint = width() ;
    int hPrint = height() ;

    QPainter painter(this);
    painter.fillRect(QRect(0, 0, width(), height()), Qt::white);

    int latRect = int(floor(curX));
    int latPixel = int((latRect - curX) * tileSize + width()/2);
    int lonRect = int(floor(curY));
    int lonPixel = int((lonRect - curY) * tileSize + height()/2);
    while (latPixel > 0)
        latPixel -= tileSize, --latRect;
    while (lonPixel > 0)
        lonPixel -= tileSize, --lonRect;
    for (int x=latPixel; x < wPrint; x += tileSize)
    {
        for (int y=lonPixel; y < hPrint; y+= tileSize)
        {
            foreach(SlippyMapPart *part, parts)
            {
                debugp("Painting part : " + QString::number(part->getId()) );
                if (part->getId()==MAPPART_ZOOM)
                {
                    debugp("Painting part zoom #1"  );
                }
                if (part->getId()==MAPPART_BOX)
                {
                    debugp("Painting part box #5" );
                }
                if (part->getId()==MAPPART_CENTER)
                {
                    debugp("Painting part center #0" );
                }
                if (part->getId()==MAPPART_POI)
                {
                    debugp("Painting part poi #4" );
                }
                if (part->getId()==MAPPART_SCALE)
                {
                    debugp("Painting part scale #3" );
                }
                if (part->getId()==MAPPART_ZOOMVAL)
                {
                    debugp("Painting part zoom val #2");
                }
                if (part->getId()==6)
                {
                    debugp("Painting part #6 : ");
                }
                part->drawTile(&painter,
                               QPoint(x, y),
                               latRect + (x-latPixel)/tileSize,
                               lonRect + (y-lonPixel)/tileSize,
                               curZoom);

            }
        }
    }

    int i=0;
    foreach(SlippyMapPart *part, parts)
    {
        i++;
        part->draw(&painter, ev);
    }

}

/* Return coordinate of top left tile printed on page
    X, Y : tile refs
*/
QPointF SlippyMapWidget::getPrintMapTile( int X, int Y, int Z)
{
    QPointF retPos;

    retPos.setX(0);
    retPos.setY(0);

    for( int i=0; i<printMapTiles.count(); ++i )
    {
        if (printMapTiles[i].X == X && printMapTiles[i].Y == Y)
        {
            retPos.setX(printMapTiles[i].posX);
            retPos.setY(printMapTiles[i].posY);
            break;
        }
    }
    return retPos;

}
QPointF SlippyMapWidget::getPrintMapTileXY( int X, int Y, int Z)
{
    QPointF retPos;

    retPos.setX(0);
    retPos.setY(0);

    for( int i=0; i<printMapTiles.count(); ++i )
    {

        if (printMapTiles[i].X == X && printMapTiles[i].Y == Y)
        {

            retPos.setX(printMapTiles[i].x);
            retPos.setY(printMapTiles[i].y);
            break;
        }
    }
    return retPos;

}

void SlippyMapWidget::render(QPainter *painter, qreal marginLeft, qreal marginTop, qreal marginRight, qreal marginBottom, int printerResolution, int scaleFactor, double scaleLen, double mapZoom, bool drawTileBorder, int pageWpixel, int pageHpixel, QRectF mapBorder, bool modeDebug)
{
    MYTRACE("SlippyMapWidget::paintEvent, w %d", width());

    debugp("Enter map render");

    int wPrint = width() ;
    int hPrint = height() ;

    float tileSizeMm = (scaleFactor * float( float(tileSize) / float(printerResolution) ) ) * 25.4;

    int maxTileW = ceil(  float(pageW / tileSizeMm)) +1;
    int maxTileH = ceil(  float(pageH / tileSizeMm)) +1;

    painter->fillRect(QRect(0, 0, wPrint, hPrint), Qt::white);

    int latRect = int(floor(curX));
    int latPixel = int((latRect - curX) * tileSize + width()/2);
    int lonRect = int(floor(curY));
    int lonPixel = int((lonRect - curY) * tileSize + height()/2);
    while (latPixel > 0 )
        latPixel -= tileSize, --latRect;
    while (lonPixel > 0 )
        lonPixel -= tileSize, --lonRect;

    int xx;
    int yy;

    SlippyMapTiled *firstPart;
    bool firstPartGet = false;
    QPixmap *pm;

    foreach(SlippyMapPart *part, parts)
    {
        if(firstPartGet == false)
        {

            firstPartGet=true;
            firstPart = qobject_cast<SlippyMapTiled*>(part);
            break;
        }
    }

    if (!firstPart) return;

    QFont fontLegend ;
    fontLegend = QFont( "Arial", 20);
    fontLegend.setPixelSize( 20 );
    painter->setFont( fontLegend );
    QString lineToPrint;

    int cptTileW;
    int cptTileH;

    QPointF printMapCoord;
    QPoint xy;
    QRectF printMapZone = getArea();

    int idX;
    int idY;


    xx=latPixel ;

    printMapTilesStruct currentTile;
    printMapTiles.clear();

    bool lastTileFound = false;
    QPointF bottomCoord;

    for (cptTileW=1; cptTileW < maxTileW ; cptTileW +=1) // W
    {
        yy=lonPixel;

        double tileLon = tile2lon(latRect + (xx-latPixel)/tileSize, curZoom);

        for (cptTileH=1; cptTileH < maxTileH ; cptTileH +=1) // H
        {
            pm = firstPart->getTileSync( latRect + (xx-latPixel)/tileSize , lonRect + (yy-lonPixel)/tileSize, curZoom );
            if (pm)
            {

                double tileLat = tile2lat(lonRect + (yy-lonPixel)/tileSize, curZoom);
                firstPart->drawTile(painter,
                                    QPoint( xx+ (marginTop / scaleFactor), yy+ (marginLeft / scaleFactor) ),  // x,y print @
                                    latRect + (xx-latPixel)/tileSize,   // Tile ref
                                    lonRect + (yy-lonPixel)/tileSize,   // Tile ref
                                    curZoom);

                idX = latRect + (xx-latPixel)/tileSize;
                idY = lonRect + (yy-lonPixel)/tileSize;

                int currentCol = xx+ (marginLeft / scaleFactor);
                int currentLig = yy+ (marginTop / scaleFactor) ;
                int decalLig = painter->fontMetrics().lineSpacing();

                painter->setPen(Qt::black);

                lineToPrint.clear();
                lineToPrint = QString ("H,W=%1,%2").arg( QString::number(cptTileH)  , QString::number(cptTileW)  );
                if (modeDebug == true)painter->drawText( QPoint(currentCol,  currentLig + decalLig *1 ) , lineToPrint );

                lineToPrint.clear();
                lineToPrint = QString ("@x,y=%1,%2").arg( QString::number( xx+ (marginTop / scaleFactor) , 'f', 0),  QString::number( yy+ (marginLeft / scaleFactor) , 'f', 0)  );
                if (modeDebug == true)painter->drawText( QPoint( currentCol,  currentLig + decalLig*2 ) , lineToPrint );

                lineToPrint.clear();
                lineToPrint = QString ("Top=%1,%2").arg( QString::number( tileLon , 'f', 3),  QString::number( tileLat, 'f', 3)  );
                if (modeDebug == true)painter->drawText( QPoint(currentCol,  currentLig + decalLig*3 ) , lineToPrint );

                if (lastTileFound  ==false && xx + tileSize+ (marginTop / scaleFactor) >= mapBorder.bottomRight().rx() && yy + tileSize + (marginLeft / scaleFactor)>= mapBorder.bottomRight().ry() )
                {
                    lineToPrint.clear();
                    lineToPrint = QString ("Bt @+x,y=%1,%2").arg( QString::number( mapBorder.bottomRight().rx() - ( xx + (marginTop / scaleFactor) ) , 'f', 0)  , QString::number( mapBorder.bottomRight().ry() - (yy + (marginLeft / scaleFactor)) , 'f', 0)  );
                    if (modeDebug == true)painter->drawText( QPoint(currentCol,  currentLig + decalLig*4 ) , lineToPrint );

                    double BottomLon = tile2lon( floor(latRect + (xx-latPixel)/tileSize) + ( (double)(mapBorder.bottomRight().rx() - ( xx + (marginTop / scaleFactor) ))/(double)tileSize ) , curZoom);
                    double BottomLat = tile2lat( floor(lonRect + (yy-lonPixel)/tileSize) + ( (double)(mapBorder.bottomRight().ry() - (yy + (marginLeft / scaleFactor)))/(double)tileSize ) , curZoom);

                    lineToPrint.clear();
                    lineToPrint = QString ("Bt lo/la=%1,%2").arg( QString::number(  BottomLon , 'f', 3),  QString::number( BottomLat, 'f', 3)  );
                    printedBottomRight = QPointF(BottomLon, BottomLat);
                    if (modeDebug == true)painter->drawText( QPoint(currentCol,  currentLig + decalLig*5 ) , lineToPrint );

                    lastTileFound = true;

                }
                lineToPrint.clear();
                lineToPrint = QString ("Tile x,y,z=%1,%2,%3").arg( QString::number(idX )  , QString::number(idY)  , QString::number(curZoom, 'f', 0) );
                if (modeDebug == true)painter->drawText( QPoint(currentCol,  currentLig + decalLig*6 ) , lineToPrint );

                lineToPrint.clear();
                lineToPrint = QString ("@x,y bottom =%1,%2").arg( QString::number(xx + tileSize+ (marginTop / scaleFactor), 'f', 0)  , QString::number(yy + tileSize + (marginLeft / scaleFactor), 'f', 0)  );
                if (modeDebug == true)painter->drawText( QPoint(currentCol,  currentLig + decalLig*8 ) , lineToPrint );

                if (modeDebug == true)
                {
                    painter->setBrush(Qt::NoBrush);
                    painter->setPen(Qt::red);
                    painter->drawRect(xx +(marginLeft / scaleFactor), yy+ (marginTop / scaleFactor) ,tileSize ,tileSize);
                }

                // Keep track of printed tile
                currentTile.lig = cptTileH;
                currentTile.col = cptTileW;
                currentTile.X = idX;
                currentTile.Y = idY;
                currentTile.Z = curZoom;
                currentTile.posX =  xx+ (marginTop / scaleFactor);
                currentTile.posY = yy+ (marginLeft / scaleFactor);
                currentTile.x =  xx;
                currentTile.y = yy;
                currentTile.lon = tileLon;
                currentTile.lat = tileLat;
                printMapTiles.append( currentTile );

            }
            else debugp("Error print tile w,h : "  +QString::number( cptTileW)  + ","  +  QString::number(cptTileH ) +  " : not found" );

            yy += tileSize;
        }

        xx += tileSize;
    }

    xy= QPoint(0,0);
    printedTopLeft = QPointF(printMapZone.left()   + printMapZone.width()  * xy.x() / width(), printMapZone.bottom() + fabs(printMapZone.height()) * (height()-xy.y()) / height());
    int i=0;

    foreach(SlippyMapPart *part, parts)
    {
        i++;


        if (part->getId()==MAPPART_ZOOM && printZoom)
        {
            part->render(painter, marginLeft, marginTop, marginRight, marginBottom, printerResolution, scaleFactor,  scaleLen, mapZoom, pageWpixel, pageHpixel, mapBorder, modeDebug);

        }
        if (part->getId()==MAPPART_ZOOMVAL && printZoomVal) part->render(painter, marginLeft, marginTop, marginRight, marginBottom, printerResolution, scaleFactor,  scaleLen, mapZoom, pageWpixel, pageHpixel, mapBorder, modeDebug);
        if (part->getId()==MAPPART_SCALE && printScale) part->render(painter, marginLeft, marginTop, marginRight, marginBottom, printerResolution, scaleFactor, scaleLen, mapZoom, pageWpixel, pageHpixel, mapBorder, modeDebug);
        if (part->getId()==MAPPART_POI && printPoi) part->render(painter, marginLeft, marginTop, marginRight, marginBottom, printerResolution, scaleFactor, scaleLen, mapZoom, pageWpixel, pageHpixel, mapBorder, modeDebug);
        if (part->getId()==MAPPART_CENTER && printCenter) part->render(painter, marginLeft, marginTop, marginRight, marginBottom, printerResolution, scaleFactor, scaleLen, mapZoom, pageWpixel, pageHpixel, mapBorder, modeDebug);
        if (part->getId()==MAPPART_TILE && printTile) part->render(painter, marginLeft, marginTop, marginRight, marginBottom, printerResolution, scaleFactor, scaleLen, mapZoom, pageWpixel, pageHpixel, mapBorder, modeDebug);
        if (part->getId()==MAPPART_BOX && printBox) part->render(painter, marginLeft, marginTop, marginRight, marginBottom, printerResolution, scaleFactor, scaleLen, mapZoom, pageWpixel, pageHpixel, mapBorder, modeDebug);
    }

}

double SlippyMapWidget::getZoomForScale( qlonglong scaleValue )
{

    QRectF r = getArea();
    QPointF srcPoint;
    srcPoint =  QPointF( r.left(), r.bottom());
    QPointF dstPoint;
    dstPoint =  QPointF( r.right(), r.bottom());

    double mapDistPoints  = Great_Circle_Distance( srcPoint, dstPoint ); // Metres
    return mapDistPoints  ;

}

/*!
 * Set map position.
 *
 * \param lonLat new geographical coordinates in degrees. QPointF.x() has the
 * longitude, y() has the latitude (LV).
 *
 * \param newZoom new zoom level. If unspecified or -1, then the current
 * zoom level doesn't change.
 *
 * \sa getLonLat()
 */
void SlippyMapWidget::setLonLat(QPointF lonLat, int newZoom)
{
    MYTRACE("SlippyMapWidget::setLonLat(%f,%f, %d)",
            lonLat.x(), lonLat.y(), newZoom);


#ifdef TODO
    if (newZoom != -1)
    {
        if (newZoom < minZoomLevel)
            newZoom = minZoomLevel;
        if (newZoom > maxZoomLevel)
            newZoom = maxZoomLevel;
        curZoom = newZoom;
    }
#else
    curZoom = newZoom;
#endif
    if (newZoom <=0 )
    {
        newZoom = 1;
    }
    if (newZoom > 20 )
    {
        newZoom = 20;
    }

    curX = ((lonLat.x() + 180.0) / 360.0 * pow(2.0, curZoom));
    curY = (1.0 - log( tan(lonLat.y() * M_PI/180.0) + 1.0 / cos(lonLat.y() * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, curZoom);
    MYVERBOSE("  cur %f, %f", curX, curY);
    debugp("Move Map to lon/lat = " +  QString::number(lonLat.x()) + "/" + QString::number(lonLat.y()) +", curX/curY: " + QString::number( curX) + "/" +  QString::number(curY) +  ", zoom=" +  QString::number(newZoom));
    //QDebug() << lonLat.x();

    update();

}

/*!
 * Get geographic coordiates from a given screen position.
 *
 * \param xy x/y position into the widget
 *
 * \return current geographical coordinates in degrees.
 */
QPointF SlippyMapWidget::getLonLat(const QPoint &xy) const
{
    MYTRACE("SlippyMapWidget::getLonLat(%d, %d)", xy.x(), xy.y());

    QRectF r = getArea();
    return QPointF(r.left()   + r.width()  * xy.x() / width(),
                   r.bottom() + fabs(r.height()) * (height()-xy.y()) / height());
}


/*!
 * \fn SlippyMapWidget::getLonLat() const
 *
 * Get geographic coordiates from the middle of the widget.
 *
 * \return Current geographical coordinates in degrees.
 */


/*!
 * Get currently visible geographic area
 *
 * \return Current geographical area in degrees.
 *
 * http://wiki.openstreetmap.org/wiki/Slippy_map_tilenames#C.2FC.2B.2B
 */
QRectF SlippyMapWidget::getArea() const
{
    MYTRACE("SlippyMapWidget::getArea");

    double wh = width()  / 2.0;
    double hh = height() / 2.0;
    double x1 = curX - wh / tileSize;
    double x2 = curX + wh / tileSize;
    double y1 = curY - hh / tileSize;
    double y2 = curY + hh / tileSize;


    double lon1 = tile2lon(x1, curZoom);
    double lon2 = tile2lon(x2, curZoom);
    double lat1 = tile2lat(y1, curZoom);
    double lat2 = tile2lat(y2, curZoom);
    return QRectF(QPointF(lon1, lat1), QPointF(lon2, lat2));
}

/*!
 * Move center position of map.
 *
 * \param delta x/y increment.
 *
 * \return New geographical coordiates in degrees.
 */
QPointF SlippyMapWidget::move(const QPoint &delta)
{
    MYTRACE("SlippyMapWidget::move(%d, %d)", delta.x(), delta.y());
    curX -= delta.x() / (tileSize*1.);
    curY -= delta.y() / (tileSize*1.);
    MYVERBOSE("  cur %f, %f", curX, curY);
    return getLonLat();
}
QPointF SlippyMapWidget::move(const QPointF &delta)
{
    curX -= delta.x() / (tileSize*1.);
    curY -= delta.y() / (tileSize*1.);
    return getLonLat();
}
QPointF SlippyMapWidget::move(QPointF &delta)
{

    curX -= delta.x() / (tileSize*1.);
    curY -= delta.y() / (tileSize*1.);
    return getLonLat();
}


/*!
 * \fn void SlippyMapWidget::position(const QPointF &lonLat)
 *
 * Signal will be emitted with the map position whenever the user clicks into the map.
 *
 * \param lonLat geographical coordinates in degrees. lonLat.x() has the
 * longitude, lonlat.y() has the latitude.
 *
 *
 *
 * \fn int SlippyMapWidget::getZoom() const
 * \returns The current zoom.
 *
 *
 * \var SlippyMapWidget::map
 * Points to the main, SlippyMapTiled derived, map.
 *
 *
 * \var SlippyMapWidget::parts
 * Contains a list of SlippyMapParts which will get paint and mouse events
 * and can draw their part of the map.
 *
 *
 * \var SlippyMapWidget::previousDrag
 * Helper variable to detect panning.
 *
 *
 * \var SlippyMapWidget::curZoom.
 *
 * Current zoom level.
 *
 * \sa SlippyMapWidget::getZoom()
 *
 *
 * \var SlippyMapWidget::curZoom
 * Current zoom level.
 *
 * \var SlippyMapWidget::curX
 * Current X position, in Google-API coordinates
 *
 *
 * \var SlippyMapWidget::curY
 * Current Y position, in Google-API coordinates
 */












/*!
 * \class SlippyMapPart
 * \ingroup Widgets
 *
 * Base class for all painters that will paint into the map.
 */


/*!
 * Constructor.
 *
 * \param parent Pointer to the SlippyMapWidget where subclasses of this
 *               virtual class should render their contents into.
 */
SlippyMapPart::SlippyMapPart(SlippyMapWidget *parent)
    : QObject(parent)
    , widget(parent)
{
    MYTRACE("SlippyMapPart::SlippyMapPart");
}

/*!
 * \fn SlippyMapPart::draw(QPainter *painter, QPaintEvent *ev)
 *
 * \param painter Pointer to a QPainter
 *
 * \param ev Pointer to a QPaintEvent
 *
 * virtual function which is responsible for drawing into
 * map area of the parent SlippyMapWidget.
 *
 * \note you must either implement this function or drawTile() in subclasses.
 *
 *
 * \fn SlippyMapPart::drawTile(QPainter *painter, const QPoint &pos, int tileX, int tileY, int tileZoom)
 *
 * \param painter Pointer to a QPainter
 *
 * \param pos position (in pixels) inside the drawing area
 *
 * \param tileX  x-number of tile to draw
 *
 * \param tileY  y-number of tile to draw
 *
 * \param tileZoom  zoom-level of tile to draw
 *
 * Virtual function which is responsible for drawing tiles into the map area
 * of the parent SlippyMapWidget. The tile numbers follow the naming scheme
 * of OpenStreetMap, which is identical to Google-Streetmaps.
 *
 * \note you must either implement this function or drawTile() in subclasses.
 *
 *
 *
 * \fn virtual void SlippyMapPart::setLonLat(QPointF lonLat, int newZoom)
 *
 * Change position of the map. The default implementation does nothing, but
 * subclasses can re-implement this.
 *
 * \param lonLat new geographical coordinates in degrees. QPointF.x() has the
 * longitude, y() has the longitude.
 *
 * \param newZoom  new zoom level. If unspecified or -1, then the current
 * zoom level doesn't change.
 *
 *
 *
 * \fn virtual bool SlippyMapPart::mousePressEvent (QMouseEvent *ev)
 *
 * React to a mouse event. The default implementation does nothing, but
 * subclasses can re-implement this.
 *
 * \param ev Pointer to a QMouseEvent
 *
 * \return true, if the mouse press has been handled
 *
 *
 *
 * \var SlippyMapPart::widget
 *
 * Pointer to the SlippyMapWidget that this part should draw into.
 */







/*!
 * \class SlippyMapTiled
 * \ingroup Widgets
 *
 * Tile-based Map painter.
 *
 * This class can download tiles from a google-api-like tile server (e.g. OpenStreetMap,
 * Yahoo, Google) and paint it into a QPainter.
 *
 *
 * \section work Work flow
 *
 * Program flow is driven via the paintEvent(). This function calls getTile()
 * for every tile it wants to show. getTile() has now two options:
 *
 * 1. the tile is in the tileCache: return it
 * 2. the tile is not in the tileCache: recursive calls itself for a
 *    lower zoom level (coarser display) of the tile. Scale that accordingly
 *    and return it. Because this tile is now coarse, ask for via download()
 *    to the real tile.
 *
 * Now, what would happen if the tile at zoom level 4 wasn't there, but also
 * the upper tile at zoom level 3? Then the tile from zoom level 2 will be
 * used. That's fine. Unfortunately, step 2. from above would now also download
 * all of the other zoomlevels, despite the fact that we only need to view
 * the tile at the current zoom. So we call getTile() for the upper tile with
 * get==false.
 *
 */

/*!
 * Constructor.
 * \param parent Pointer to the SlippyMapWidget where this class should render
 *               the page into.
 */
SlippyMapTiled::SlippyMapTiled(SlippyMapWidget *p_parent)
    : SlippyMapPart(p_parent)

{

    _id = MAPPART_TILE;

    MYTRACE("SlippyMapTiled::SlippyMapTiled");

    // Load the tiny world picture into tileCache
    /*    preload("0/0/0", ":/SlippyMap/000.png");

        // Load four partial picture of the world
        preload("0/0/1", ":/SlippyMap/100.png");
        preload("0/1/1", ":/SlippyMap/101.png");
        preload("1/0/1", ":/SlippyMap/110.png");
        preload("1/1/1", ":/SlippyMap/111.png");
    */

}


/*!
 * Set map source (URL).
 *
 * \param path Path or URL to the map data, e.g.
 *             "http://tile.openstreetmap.org/%1/%2/%3.png". The %%1 ... %%3
 *             place-holders get replaced by current zoom Level (curZoom), x
 *             and y position (in that order).
 *
 * \param license License text to be show in the lower-right of the displayed map.
 *
 * \param min Minimum zoom level.
 *
 * \param max Maximum zoom level.
*/
void SlippyMapTiled::setMap(const QString &path,
                            const QString &license,
                            int min,
                            int max,
                            const QString &p_userAgent,
                            const QString &p_referer,
                            int p_keyShortCut,
                            int p_keyZoom
                           )
{
    MYTRACE("SlippyMapTiled::setMap");

    tilePath = path;
    copyright = license;
    minZoomLevel = min;
    maxZoomLevel = max;
    tileCache.clear();
    userAgent = p_userAgent;
    referer = p_referer;
    keyShortCut = p_keyShortCut;
    keyZoom = p_keyZoom;

    // TODO adjusts zoom level accordingly
    // widget->setLonLat(widget->getLonLat(), widget->curZoom);
    widget->update();

}

int SlippyMapTiled::getMaxZoomLevel(void)
{
    return maxZoomLevel;
}
int SlippyMapTiled::getMinZoomLevel(void)
{
    return minZoomLevel;
}


/*!
 * Pre-load images into the tile cache.
 *
 * \param key Key is used to reference the image in the tileCache
 *
 * \param path Path for this image. Often this will be a path into q Qt resource.
*/
void SlippyMapTiled::preload(const QString &key, const QString &path) /* private */
{
    MYTRACE("SlippyMapTiled::preload(%s, %s)",
            qPrintable(key),
            qPrintable(path));


    QFile f(path);
    f.open(QIODevice::ReadOnly);
    QByteArray buf(f.readAll());
    if (!buf.isEmpty())
    {
        QPixmap *pm = new QPixmap();
        pm->loadFromData(buf);
        tileCache.insert(key, pm);
    }
}


/*!
 * Get image tile for a given position.
 *
 * \param x x-position in Google-API coordinates.
 *
 * \param y y-position in Google-API coordinates.
 *
 * \param zoomLvl zoom level of requested tile.
 *
 * \param get Defaults to true and specifies if the tile should be
 * downloaded. A tile could also be made up from tiles with another zoom
 * level by proper image scaling.
 *
 * \return Pointer to a QPixmap.
 *
 * \sa download()
 */
QPixmap * SlippyMapTiled::getTile(int x, int y, int zoomLvl, bool get)
{

    int max = 1 << zoomLvl;
    if (x < 0 || x >= max || y < 0 || y >= max)
    {
        //MYVERBOSE("  out of bounds, return");
        return 0;
    }

    MYTRACE("SlippyMapTiled::getTile(%d/%d/%d %d)", x, y, zoomLvl, get);

    debugp("key : " + QString::number(x) + "," + QString::number(y)  +"," + QString::number(zoomLvl) );

    QString key("%1/%2/%3");
    key = key.arg(x).arg(y).arg(zoomLvl);
    debugp("key fmt=" + key);

    QPixmap *pm = tileCache.object(key);
    if (get && zoom.contains(key))
    {

        download(x, y, zoomLvl);
    }

    // Fix
    if (!pm)
    {
        download(x, y, zoomLvl);
    }
    //end fix

    if (!pm)
    {
        MYVERBOSE("  %s: not found", qPrintable(key));
        QPixmap *upper = getTile(x/2, y/2, zoomLvl-1, false);
        if (upper)
        {
            MYVERBOSE("  %s: no upper", qPrintable(key));
            pm = new QPixmap(upper->copy((x % 2) * tileSize / 2,
                                         (y % 2) * tileSize / 2,
                                         tileSize / 2,
                                         tileSize / 2).scaled(tileSize, tileSize));
            zoom.insert(key);
            tileCache.insert(key, pm);
        }
        if (get)
        {
            MYVERBOSE("  %s: download", qPrintable(key));
            download(x, y, zoomLvl);
        }
    }
    MYVERBOSE("  %s: return", qPrintable(key));
    debugp("end function");
    return pm;
}


QPixmap * SlippyMapTiled::getTileSync(int x, int y, int zoomLvl)
{
    int max = 1 << zoomLvl;
    if (x < 0 || x >= max || y < 0 || y >= max)
    {
        //MYVERBOSE("  out of bounds, return");
        return 0;
    }

    QString key("%1/%2/%3");
    key = key.arg(x).arg(y).arg(zoomLvl);
    QPixmap *pm = 0;
    pm = tileCache.object(key);
    // Fix
    if (!pm)
    {

        downloadSync(x, y, zoomLvl);
        pm = tileCache.object(key);
        if (!pm)
        {
            debugp("Err pm is null");
        }
    }
    return pm;
}


/*!
 * Download an image tile from a tile server.
 *
 * This requests a download via the Downloader class. Once the download is
 * finsihed, slotFinished() get's called.
 *
 * \param x x-position in Google-API coordinates
 *
 * \param y y-position in Google-API coordinates
 *
 * \param zoomLvl zoom level of requested tile
 *
 * \sa http://wiki.openstreetmap.org/wiki/List_of_OSM_based_Services
 * \sa getTile()
 * \sa slotFinished()
 */
void SlippyMapTiled::download(int x, int y, int zoomLvl)
{
    MYTRACE("SlippyMapTiled::download(%d/%d/%d)", x, y, zoomLvl);


    if (tilePath=="") return;

    QString key("%1/%2/%3");
    key = key.arg(x).arg(y).arg(zoomLvl);
    debugp("key=" + key);

    QString path;
    debugp("tilePath=" + tilePath);

    if (tilePath.contains("%4") )
    {
        QString quadkey =  toQuad (  x,  y, zoomLvl );
        path = tilePath.arg(quadkey);
    }
    else
    {
        path = tilePath.arg(zoomLvl).arg(x).arg(y);
    }

    debugp("get : " +  path);

    QNetworkReply *reply = ::download(cacheFilePath, path, key, userAgent, referer);
    if (reply)
    {
        connect(reply, SIGNAL(finished()), this, SLOT(slotFinished()) );
    }
}

void SlippyMapTiled::downloadSync(int x, int y, int zoomLvl)
{
    MYTRACE("SlippyMapTiled::download(%d/%d/%d)", x, y, zoomLvl);

    QString key("%1/%2/%3");
    key = key.arg(x).arg(y).arg(zoomLvl);

    QString path;
    if (tilePath.contains("%4") )
    {
        QString quadkey =  toQuad (  x,  y, zoomLvl );

        path = tilePath.arg(quadkey);
    }
    else
    {
        path = tilePath.arg(zoomLvl).arg(x).arg(y);
    }

    MYVERBOSE("  get %s", qPrintable(path));

    QImage image;
    QNetworkReply *reply = ::downloadSync(&image, cacheFilePath, path, key, userAgent, referer );
    if (reply)
    {
        QPixmap pm = QPixmap::fromImage(image);
        tileCache.insert(key, new QPixmap(pm));
    }

    return;
}

/*!
 * Handler for finished download.
 *
 * \sa download()
 */
void SlippyMapTiled::slotFinished()
{
    MYTRACE("SlippyMapTiled::slotFinished");

    QNetworkReply *reply = dynamic_cast<QNetworkReply *>(sender());
    MYVERBOSE("  reply %p", reply);

    QString key = reply->request().attribute(QNetworkRequest::User).toString();
    MYVERBOSE("  key '%s'", qPrintable(key));


    if (!reply->error())
    {
        QImage img;
        if (img.loadFromData(reply->readAll(), 0))
        {
            QPixmap pm = QPixmap::fromImage(img);
            tileCache.insert(key, new QPixmap(pm));
            zoom.remove(key);
        }
        else
        {

            debugp( "warning geting tile : key (x/y/zoom) =" + key );
            debugp( "warning tilePath=" + tilePath );
        }
    }
    else
    {
        //FIX TEMP LVN
        debugp( "error geting tile : " +  QString::number((reply->error()))  + " key (x/y/zoom) =" + key );
        debugp( "error tilePath=" + tilePath );
    }
    reply->deleteLater();

    widget->update();
}


void SlippyMapTiled::drawTile(QPainter *painter,
                              const QPoint &pos,
                              int tileX,
                              int tileY,
                              int tileZoom)
{
    //painter->setOpacity(0.6);
    debugp("call getTile : " + QString::number(tileX) + "," + QString::number(tileY)  +"," + QString::number(tileZoom) );
    QPixmap *pm = getTile(tileX, tileY, tileZoom);
    if (pm)
        painter->drawPixmap(pos, *pm);
    //painter->setOpacity(1);
}

/*!
 * \var SlippyMapTiled::tileCache
 * Cache that saves us from downloading already downloaded tiles.
 *
 * \var SlippyMapTiled::tilePath
 * Path (URL) to a tileserver.
 * \sa setMap()
 *
 * \var SlippyMapTiled::copyright
 * License text to be display in the lower-left edge of the map.
 * \sa setMap()
 *
 * \var SlippyMapTiled::minZoomLevel
 * Minimum zoom level, most often 1.
 * \sa setMap()
 *
 * \var SlippyMapTiled::maxZoomLevel
 * Maximum zoom level, most often 17.
 * \sa setMap()
 *
 * \var SlippyMapTiled::zoom
 * Container where the class remembers which tiles have been made up from
 * other zoom levels.
 * \sa getTile()
 */



void SlippyBoxPrint::draw(QPainter *painter, QPaintEvent *ev)
{
    Q_UNUSED(ev);

    painter->setPen(Qt::red);

    painter->fillRect(x,y,width, height, QBrush(QColor(128, 128, 255, 128)));
}

void SlippyBoxPrint::setBox( int p_x, int p_y, int p_width, int p_height)
{
    x= p_x;
    y = p_y;
    width = p_width;
    height = p_height;
}



/*!
 * \class SlippyCenterMark
 * \ingroup Widgets
 *
 * Draws a cyan cross-hair into the middle of the map.
 *
 *
 *
 * \fn SlippyCenterMark::SlippyCenterMark(SlippyMapWidget *parent)
 *
 * Constructor.
 *
 * \param parent Pointer to the SlippyMapWidget where this SlippyMapPart
 * should render their contents into.
 */


/*!
 * Draws a cross-hair into the middle of the map.
 *
 * \param painter Pointer to a QPainter
 *
 * \param ev Pointer to a QPaintEvent
 */
void SlippyCenterMark::draw(QPainter *painter, QPaintEvent *ev)
{
    MYTRACE("SlippyMapMark::draw");
    Q_UNUSED(ev);

    if (this->display == false) return;

    // LV FONTCOLOR
    //painter->setPen(Qt::red);
    painter->setPen(this->fontColor);

#if 0
    // Draw a cyan colored rectangle into the middle of the widget
    painter->drawRect(widget->width()/2-5,
                      widget->height()/2+5,
                      11,11);

#else
    // Draw a cross
    int w = widget->width()/2;
    int h = widget->height()/2;

    int baseSizeCross = 4;

    painter->drawLine(w, h-(baseSizeCross*this->sizeCross), w, h-1);
    painter->drawLine(w, h+(baseSizeCross*this->sizeCross), w, h+1);
    painter->drawLine(w-1, h, w-(baseSizeCross*this->sizeCross), h);
    painter->drawLine(w+1, h, w+(baseSizeCross*this->sizeCross), h);

    QRect corderCoo = QRect( QPoint(w-(baseSizeCross*this->sizeCross), h-(baseSizeCross*this->sizeCross)) , QPoint(w+(baseSizeCross*this->sizeCross), h+(baseSizeCross*this->sizeCross)) );
    QBrush rectBrush = QBrush ( this->bgColor );


    //painter->fillRect( corderCoo , rectBrush);
    painter->setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(corderCoo, 2, 2 );
    painter->setPen(QPen( this->fontColor));
    painter->fillPath(path, this->bgColor );
    painter->drawPath(path);



#endif

}

void SlippyCenterMark::render(QPainter *painter,  qreal marginLeft, qreal marginTop, qreal marginRight, qreal marginBottom, int printerResolution, int scaleFactor, double scaleLen, double mapZoom, int pageWpixel, int pageHpixel, QRectF mapBorder, bool modeDebug)
{
    MYTRACE("SlippyMapMark::draw");

    // LV FONTCOLOR - printer
    painter->setPen(Qt::red);

#if 0
    // Draw a cyan colored rectangle into the middle of the widget
    painter->drawRect(widget->width()/2-5,
                      widget->height()/2+5,
                      11,11);

#else
    // Draw a cross
    int w = ((widget->width()/2 ) *scaleFactor);
    int h = ((widget->height()/2) * scaleFactor);

    w += marginLeft / scaleFactor;
    h += marginTop / scaleFactor;

    painter->drawLine(w, h-20, w, h-5);
    painter->drawLine(w, h+20, w, h+5);
    painter->drawLine(w-5, h, w-20, h);
    painter->drawLine(w+5, h, w+20, h);
#endif

}



/*!
 * \class SlippyZoomButtons
 * \ingroup Widgets
 *
 * Draws zoom buttons at the left side of the map and reacts to mouse
 * events accordingly.
 *
 *
 *
 * \fn SlippyZoomButtons::SlippyZoomButtons(SlippyMapWidget *parent)
 *
 * Constructor.
 *
 * \param parent Pointer to the SlippyMapWidget where this SlippyMapPart
 * should render their contents into.
 */

// Position for the zoom symbols
const int symPlusY = 0;    //!< Y-position of the plus button from SlippyZoomButtons
const int symMinusY = 25;  //!< Y-position of the minus button from SlippyZoomButtons
const int symWidth = 20;   //!< Width of the buttons from SlippyZoomButtons
const int symHeight = 20;  //!< Height of the buttons from SlippyZoomButtons


/*!
 * Draws zoom buttons into the map area of the parent SlippyMapWidget.
 *
 * \param painter Pointer to a QPainter
 *
 * \param ev Pointer to a QPaintEvent
 */
void SlippyZoomButtons::draw(QPainter *painter, QPaintEvent *ev)
{
    MYTRACE("SlippyZoomButtons::draw");
    Q_UNUSED(ev);

    if (this->display == false ) return;

    QString lineToPrint;
    QRect r;
    int txtH;
    QPainterPath path;

    QFont* fontControl = new QFont("FreeMono");
    fontControl->setPointSize( this->font.pointSize() );
    painter->setFont( *fontControl );
    QFontMetrics fm = painter->fontMetrics();

    lineToPrint  = QString ("%1").arg( "+");
    r = fm.boundingRect(lineToPrint);
    r.moveTo( 0, 0  );
    txtH = fm.height();

    painter->setRenderHint(QPainter::Antialiasing);

    path.addRoundedRect(QRectF(0, 0 , r.width(), fm.height()), 2, 2 );
    painter->setPen(QPen( this->fontColor));
    //painter->fillPath(path, this->bgColor );
    //painter->drawPath(path);

    painter->drawText( r, Qt::AlignVCenter| Qt::AlignLeft, lineToPrint );
    dynamic_cast< SlippyMapWidget * >(parent())->zoomControlPlus.setTopLeft( QPoint(0,0) );
    dynamic_cast< SlippyMapWidget * >(parent())->zoomControlPlus.setBottomRight( QPoint( r.width(), fm.height() ) );

    lineToPrint  = QString ("%1").arg( "-");
    r = fm.boundingRect(lineToPrint);
    r.moveTo( 0, txtH);

    painter->setRenderHint(QPainter::Antialiasing);

    path.addRoundedRect(QRectF(0, txtH , r.width(), fm.height()), 2, 2 );
    painter->setPen(QPen( this->fontColor));
    painter->fillPath(path, this->bgColor );
    painter->drawPath(path);

    painter->drawText( r, Qt::AlignVCenter| Qt::AlignLeft, lineToPrint );

    this->txtHeight = txtH + fm.height();

    dynamic_cast< SlippyMapWidget * >(parent())->zoomControlHeight = this->txtHeight;
    dynamic_cast< SlippyMapWidget * >(parent())->zoomControlMinus.setTopLeft( QPoint(0,txtH) );
    dynamic_cast< SlippyMapWidget * >(parent())->zoomControlMinus.setBottomRight( QPoint( r.width(), txtH + fm.height() ) );

}
void SlippyZoomButtons::render(QPainter *painter, qreal marginLeft, qreal marginTop, qreal marginRight, qreal marginBottom, int printerResolution, int scaleFactor,double scaleLen, double mapZoom, int pageWpixel, int pageHpixel, QRectF mapBorder, bool modeDebug)
{
    MYTRACE("SlippyZoomButtons::draw");

    return;

}


/*!
 * Handle zooming when the user presses on the zoom buttons.
 *
 * \param ev Pointer to a QMouseEvent
 *
 * \return true, if the mouse press has been handled
 */
bool SlippyZoomButtons::mousePressEvent(QMouseEvent *ev)
{

    debugp("clic event zoom button");

    int x1,x2,x3,x4;
    int y1,y2,y3,y4;

    dynamic_cast< SlippyMapWidget * >(parent())->zoomControlPlus.getCoords( &x1, &y1, &x2, &y2);
    dynamic_cast< SlippyMapWidget * >(parent())->zoomControlMinus.getCoords( &x3, &y3, &x4, &y4);

    if (ev->button() == Qt::LeftButton)
    {
        // Plus
        if ( ev->pos().x() < x2  && ev->pos().y() < y2 )
        {
            widget->setLonLat(widget->getLonLat(), widget->getZoom()+1);
            widget->update();
            return true;
        }

        // Minus
        if ( ev->pos().x() > x3  && ev->pos().x() < x4  && ev->pos().y() < y4 )
        {
            widget->setLonLat(widget->getLonLat(), widget->getZoom()-1);
            widget->update();
            return true;
        }
    }

    return false;



}


// Zoom value
void SlippyZoomVal::draw(QPainter *painter, QPaintEvent *ev)
{
    MYTRACE("SlippyZoomVal::draw");
    Q_UNUSED(ev);

    if (this->zoomDisplayVal == false) return;

    painter->setFont( this->font );
    QFontMetrics fm = painter->fontMetrics();

    QString lineToPrint  = QString ("x %1").arg( QString::number( getSlippyMapWidget()->curZoom, 'f', 0)) ;
    QRect r = fm.boundingRect(lineToPrint);

    painter->setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(QRectF(0, dynamic_cast< SlippyMapWidget * >(parent())->zoomControlHeight , r.width(), fm.height()), 2, 2 );
    painter->setPen(QPen( this->fontColor));
    painter->fillPath(path, this->bgColor );
    painter->drawPath(path);

    r.moveTo( 0, dynamic_cast< SlippyMapWidget * >(parent())->zoomControlHeight   );
    painter->drawText( r, Qt::AlignVCenter| Qt::AlignLeft, lineToPrint );
}

void SlippyZoomVal::render(QPainter *painter, qreal marginLeft, qreal marginTop, qreal marginRight, qreal marginBottom, int printerResolution, int scaleFactor, double scaleLen, double mapZoom, int pageWpixel, int pageHpixel, QRectF mapBorder, bool modeDebug)
{

    return;

}

/*!
 * \class SlippyPath
 * \ingroup Widgets
 *
 * This class draws the a path (stored in a GeoData instance) into
 * a SlippyMapWidget.
 *
 *
 * \var SlippyPath::data
 *
 * Pointer to an GeoPath instance from which GeoPath::lon and GeoPath::lat
 * will be used to draw the path.
 *
 * \sa SlippyMapWidget::getZoom()

 */

/*!
 *
 * Constructor.
 *
 * \param d      Pointer to the path to draw.
 *
 * \param parent Pointer to the SlippyMapWidget where this SlippyMapPart
 * should render their contents into.
 */
SlippyPath::SlippyPath(GeoData *d, SlippyMapWidget *parent)
    : SlippyMapPart(parent)
    , data(d)
{
    MYTRACE("SlippyPath::SlippyPath");
}


/*!
 * Set new path.
 *
 * Use this function if you want the SlippyMapWidget show another path.
 *
 * \param d      Pointer to the path to draw.
 */
void SlippyPath::setPath(GeoData *d)
{
    MYTRACE("SlippyPath::setPath");

    data = d;
    widget->update();

}


/*!
 * Actually drawing function.
 *
 * \param painter Pointer to a QPainter
 *
 * \param ev Pointer to a QPaintEvent
 */
void SlippyPath::draw(QPainter *painter, QPaintEvent *ev)
{
    MYTRACE("SlippyPath::draw");
    Q_UNUSED(ev);

    if (!data)
    {

        return;
    }

    QRectF area = widget->getArea();

    painter->setPen(QColor(0,0,0, 150));
    int xp = - 1;
    int yp;

    for (int i=0; i<data->posCount(); i++)
    {
        QPointF m(data->lon.at(i), data->lat.at(i));
        if (!area.contains(m))
        {
            xp = -1;
            continue;
        }
        double x = (m.x()-area.left()) / area.width()  * widget->width();
        double y = (m.y()-area.top())  / area.height() * widget->height();
        if (i && xp != -1)
            painter->drawLine(xp, yp, x, y);
        xp = x;
        yp = y;
    }

}


/*!
 *
 * Constructor.

 */
SlippyScale::SlippyScale(SlippyMapWidget *parent)
    : SlippyMapPart(parent)

{
    MYTRACE("SlippyPath::SlippyPath");
    _id = MAPPART_SCALE;

    scale1fontColor = QColor(10,10,10);
    scale1bgColor = QColor(200,200,200,50);
    scale1fonfFamilly = QFont("Arial");
    display1 = true;
    display2 = true;

    scale2Precision=0;
    scale1Precision=0;
    scale1PixelLength=81;
    scale2MetersLength = 1000;
    scale1MarginX=10;
    scale1MarginY=30;
    scale2MarginX=200;
    scale2MarginY=30;

}


/*!
 * Set new path.
 */
void SlippyScale::setScale()
{
    widget->update();

}


/*!
 * Actually drawing function.
 * \param ev Pointer to a QPaintEvent
 */
void SlippyScale::draw(QPainter *painter, QPaintEvent *ev)
{
    Q_UNUSED(ev);

    if ( display1 == true )
    {
        double lenScalePix = scale1PixelLength; // Set scale pixels length
        int xMargin = scale1MarginX ; // Set scale offset pixels from bottom left
        int yMargin = scale1MarginY ; // Set scale offset pixels from bottom left

        QRect br ;
        painter->setFont( scale1fonfFamilly  );

        // ...................
        // Part1 : scale lines
        // ...................

        // Scale lines coord.
        int xp = 0 + xMargin;
        int yp = widget->height() - yMargin;
        int x = xp + lenScalePix;
        int y = yp;

        // Background fill
        QPainterPath path;

        // Scale lines background margins
        int debord=5;

        path.addRoundedRect( xp-debord, yp-scaleLimitHeight-debord, x-xp+(2*debord), (2*scaleLimitHeight)+(2*debord) , 2, 2 );
        painter->setPen(QPen( this->scale1fontColor));
        painter->fillPath(path, this->scale1bgColor );
        //path.addRoundedRect( xp, yp-scaleLimitHeight, x-xp, y-scaleLimitHeight-50 , 2, 2 ); //  Border line 1/2
        //painter->drawPath(path); // Border line 2/2

        // Draw scale lines
        painter->drawLine(xp, yp, x, y);
        painter->drawLine(xp, yp+scaleLimitHeight, xp, y-scaleLimitHeight);
        painter->drawLine(x, yp+scaleLimitHeight, x, y-scaleLimitHeight);

        // ..................
        // Part 2 : scale lib
        // ..................

        // Define scale lib  : x meters or kilometers
        QRectF r = getSlippyMapWidget()->getArea();
        QPointF srcPoint;
        srcPoint =  QPointF( r.left(), r.bottom());
        QPointF dstPoint;
        dstPoint =  QPointF( r.right(), r.bottom());
        double distPoints  = Great_Circle_Distance( srcPoint, dstPoint ); // Metres

        double mapScaleValue =  (distPoints/getSlippyMapWidget()->width())*lenScalePix ;

        QString mapScaleUnit;

        if (mapScaleValue >= 1000)
        {
            mapScaleUnit = "%1 km";
            mapScaleValue = mapScaleValue / 1000;
        }
        else
        {
            mapScaleUnit = "%1 m";
        }

        QString libScale = QString (mapScaleUnit).arg( QString::number( mapScaleValue , 'f', scale1Precision)) ;

        // Center scale lib
        QFontMetrics fm = painter->fontMetrics();
        br = fm.boundingRect(libScale);
        qreal decalLib;
        decalLib = ( lenScalePix - br.width() ) /2 ;

        // Draw scale lib
        painter->drawText( QPoint(xp  +  decalLib , yp + (br.height()*0.9) ) , libScale );

    }

    if ( display2 == true )
    {
        int xMargin = scale2MarginX  ; // Set scale offset pixels from bottom left
        int yMargin = scale2MarginY ; // Set scale offset pixels from bottom left

        QRect br ;
        painter->setFont( scale2fonfFamilly  );

        // ..................
        // Part 2 : scale lib
        // ..................

        // Define scale lib  : x meters or kilometers
        QRectF r = getSlippyMapWidget()->getArea();
        QPointF srcPoint;
        srcPoint =  QPointF( r.left(), r.bottom());
        QPointF dstPoint;
        dstPoint =  QPointF( r.right(), r.bottom());
        double distPoints  = Great_Circle_Distance( srcPoint, dstPoint ); // Metres

        QString mapScaleUnit;

        int ratio ;
        if (scale2MetersLength >= 1000)
        {
            mapScaleUnit = "%1 km";
            ratio = 1000;
        }
        else
        {
            mapScaleUnit = "%1 m";
            ratio =1;
        }

        QString libScale = QString (mapScaleUnit).arg( QString::number( scale2MetersLength  / ratio , 'f', scale2Precision )) ;

        double newLenScalePix = scale2MetersLength  /  (distPoints/getSlippyMapWidget()->width())   ;


        // Center scale lib
        QFontMetrics fm = painter->fontMetrics();
        br = fm.boundingRect(libScale);
        qreal decalLib;
        decalLib = ( newLenScalePix - br.width() ) /2 ;

        // ...................
        // Part1 : scale lines
        // ...................

        // Scale lines coord.
        int xp = 0 + xMargin;
        int yp = widget->height() - yMargin;
        int x = xp + newLenScalePix;
        int y = yp;

        // Background fill
        QPainterPath path;

        // Scale lines background margins
        int debord=5;

        path.addRoundedRect( xp-debord, yp-scaleLimitHeight-debord, x-xp+(2*debord), (2*scaleLimitHeight)+(2*debord) , 2, 2 );
        painter->setPen(QPen( this->scale2fontColor));
        painter->fillPath(path, this->scale2bgColor );

        // Draw scale lines
        painter->drawLine(xp, yp, x, y);
        painter->drawLine(xp, yp+scaleLimitHeight, xp, y-scaleLimitHeight);
        painter->drawLine(x, yp+scaleLimitHeight, x, y-scaleLimitHeight);

        // Draw scale lib
        painter->drawText( QPoint(xp  +  decalLib , yp + (br.height()*0.9) ) , libScale );

    }

}

void SlippyScale::render(QPainter *painter, qreal marginLeft, qreal marginTop, qreal marginRight, qreal marginBottom, int printerResolution, int scaleFactor, qreal scaleLen, double mapZoom, int pageWpixel, int pageHpixel, QRectF mapBorder, bool modeDebug)
{
    return;
}


SlippyMarkers::SlippyMarkers(SlippyMapWidget *parent) : SlippyMapPart(parent)
{
    _id = MAPPART_POI;
}

void SlippyMarkers::draw(QPainter *painter, QPaintEvent *ev)
{

    Q_UNUSED(ev);

    QRectF area = widget->getArea();

    for( int i=0; i<posList.count(); ++i )
    {
        QPointF m(posList[i].rx(), posList[i].ry());
        if (!area.contains(m))
        {
            continue;
        }
        double x = ( posList[i].rx() - area.left()) / area.width()  * widget->width();
        double y = ( posList[i].ry() - area.top())  / area.height() * widget->height();

        QPixmap pixmapPng(iconDir + "/" + markerIconFileList[i] + ".png");
        painter->drawPixmap(x + offsetPixmap.rx() , y + offsetPixmap.ry(), pixmapPng);

        if (markerDisplayName == true)
        {

            painter->setFont(QFont( markerFonfFamilly));

            QFontMetrics fm(markerFonfFamilly);
            int pixelsWide = fm.width(getMarkerLabelByIdx(i));
            int pixelsHigh = fm.height();

            painter->fillRect(x + offsetText.rx(), y + offsetText.rx() + painter->fontMetrics().height() -pixelsHigh+5, pixelsWide, pixelsHigh, this->markerBgColor);

            QPen penHText( this->markerFgColor );
            painter->setPen(penHText);
            painter->drawText( QPoint(x + offsetText.rx(), y + offsetText.rx() + painter->fontMetrics().height() ) , getMarkerLabelByIdx(i) );

        }
    }
}

void SlippyMarkers::render(QPainter *painter, qreal marginLeft, qreal marginTop, qreal marginRight, qreal marginBottom, int printerResolution, int scaleFactor, double scaleLen, double mapZoom, int pageWpixel, int pageHpixel, QRectF mapBorder, bool modeDebug)
{
    return;
}


void SlippyMarkers::addMarker( const qlonglong p_id, const QPointF &p_pos, const QString p_label, const QString p_markerIconFile )
{
    if (getMarkerPosById(p_id) == -1 )
    {
        idList.append(p_id);
        posList.append(p_pos);
        labelList.append(p_label);
        markerIconFileList.append(p_markerIconFile);
    }

}
void SlippyMarkers::setLabelMarkerById( qlonglong curentWptId , QString newLabel)
{

    for( qlonglong i=0; i<idList.count(); ++i )
    {
        if ( idList[i] == curentWptId)
        {
            labelList.value(curentWptId, newLabel);

        }
    }

}
void SlippyMarkers::setSymbolMarkerById( qlonglong curentWptId , QString newSymbol)
{

    for( qlonglong i=0; i<idList.count(); ++i )
    {
        if ( idList[i] == curentWptId)
        {
            markerIconFileList.value(curentWptId, newSymbol);

        }
    }

}

void SlippyMarkers::removeById( qlonglong p_id )
{
    qlonglong markerPos;
    markerPos = getMarkerPosById(p_id);
    if ( markerPos != -1 )
    {
        idList.removeAt(markerPos);
        posList.removeAt(markerPos);
        labelList.removeAt(markerPos);
        markerIconFileList.removeAt(markerPos);

    }
}
void SlippyMarkers::clearAllMarkers( void )
{
    idList.clear();
    posList.clear();
    labelList.clear();
    markerIconFileList.clear();
}
void SlippyMarkers::removeMarkerById( qlonglong curentWptId  )
{
    for( qlonglong i=0; i<idList.count(); ++i )
    {
        if ( idList[i] == curentWptId)
        {
            debugp("removed marker @" +  QString::number(i) + " id=" + QString::number( idList[i]) + " : " + labelList[i] );
            idList.removeAt(i);
            posList.removeAt(i);
            labelList.removeAt(i);
            markerIconFileList.removeAt(i);

            break;
        }
    }

    for( qlonglong i=0; i<idList.count(); ++i )
    {
        if ( idList[i] > curentWptId)
        {
            idList[i] --;
            debugp("Reinddex marker @" +  QString::number(i) + " id=" +  QString::number(idList[i]) + " : "  +labelList[i] + " -" +  QString::number(posList[i].rx()) + "," +  QString::number(posList[i].ry()) + " " + markerIconFileList[i]  );
        }

    }


}
void SlippyMarkers::getMarkerList()
{
    for( qlonglong i=0; i<idList.count(); ++i )
    {
        debugp(  QString::number(i) + " #" +  QString::number(idList[i]) + " : " + labelList[i] + " -" +  QString::number(posList[i].rx()) + "," +  QString::number(posList[i].ry()) + " " + markerIconFileList[i]);
    }

}

qlonglong SlippyMarkers::getMarkerPosById( qlonglong p_id )
{
    for( qlonglong i=0; i<idList.count(); ++i )
    {
        if (p_id == idList[i]) return i;
    }
    return -1;
}
qlonglong SlippyMarkers::getMarkerIdByIdx( qlonglong p_idx )
{

    for( qlonglong i=0; i<idList.count(); ++i )
    {
        if (i == p_idx) return idList[p_idx];
    }

    return -1;
}


qlonglong SlippyMarkers::countMarkers()
{
    return  idList.count();
}

QString SlippyMarkers::getMarkerLabelById( qlonglong p_id )
{

    for( qlonglong i=0; i<idList.count(); ++i )
    {
        if (p_id == idList[i]) return labelList[i];
    }
    return "";

}
QString SlippyMarkers::getMarkerLabelByIdx( qlonglong idxMarker )
{
    return labelList[idxMarker];

}
QPointF SlippyMarkers::getMarkerPosByIdx( qlonglong idxMarker )
{
    return posList[idxMarker];
}

void SlippyMarkers::setMarkerPosById( qlonglong p_id , QPointF newPos)
{

    for( qlonglong i=0; i<idList.count(); ++i )
    {
        if (p_id == idList[i])
        {
            posList[i] = newPos;
            break;
        }
    }
}

QPoint SlippyMarkers::setIconDir( QString p_iconDir )
{
    iconDir = p_iconDir;

    QString strKey("offsetScreen/");
    QString iniFileName;

    iniFileName = iconDir + "/symbol.ini";
    if (!QFile(iniFileName).exists())
        debugp( "File does not exist : " + iniFileName);

    QSettings * settings = 0;
    settings = new QSettings( iniFileName, QSettings::IniFormat );
    int X = settings->value( strKey + "icoX", 0).toInt();
    int Y = settings->value( strKey + "icoY", 0).toInt();
    debugp("Text offset X,Y = " + iniFileName + " :"  + QString::number( X ) + "," +QString::number( Y ) ) ;
    offsetPixmap = QPoint(X,Y);

    X = settings->value( strKey + "txtX", 0).toInt();
    Y = settings->value( strKey + "txtY", 0).toInt();
    debugp( "Icon offset X,Y = " + iniFileName + " : " + QString::number(X) + "," + QString::number(Y));
    offsetText = QPoint(X,Y);

    debugp("return");
    return offsetPixmap;
}

bool SlippyMarkers::mousePressEvent(QMouseEvent *mouseEvent)
{
    if(mouseEvent->button() == Qt::RightButton)
    {
        debugp("clic pixmap marker right  x,y = " + QString::number(mouseEvent->pos().x()) + "," + QString::number(mouseEvent->pos().y()) );

        return false;
    }
    if(mouseEvent->button() == Qt::LeftButton)
    {
        debugp( "clic pixmap marker left x,y = " + QString::number(mouseEvent->pos().x()) + "," + QString::number(mouseEvent->pos().y() ) );
        return false;
    }


    return false;
}


#include "SlippyMapWidget.moc"



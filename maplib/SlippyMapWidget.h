#ifndef SLIPPY_MAP_WIDGET_H
#define SLIPPY_MAP_WIDGET_H

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
 * \file SlippyMapWidget.h
 *
 * Widget that implements a slippy map.
 *
 * \ingroup Widgets
 */


#include <QWidget>
#include <QCache>
#include <QSet>
#include <QList>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QMenu>

#include "GeoUtils.h"
#include "../projectData.h"

//#include "../mainWindow.h"
class mainWindow;

class QNetworkReply;
class QPainter;
class QPaintEvent;
class SlippyMapPart;
class SlippyMapTiled;
class SlippyMarkers;
class SlippyScale;
class wptEdit;
//class dialogWptProp;


/*
 * Sample usage:
 *
 * slippy = new SlippyMapWidget(this);
 * slippy->setLonLat(10.3, 51.25, 6);
 * connect(slippy, SIGNAL(position(const QPointF &)),
 *         this, SLOT(setPosition(const QPointF &)) );
 */


class SlippyMapWidget :	public QWidget
{
    Q_OBJECT

public:
    SlippyMapWidget(QWidget *parent=0);
    QWidget *parent;
    //mainWindow *p_mainWindow;

    QString iconDir;

    void appendPart(SlippyMapPart *part);
    void removePart(SlippyMapPart *part);
    void clearParts();
    void setLonLat(QPointF lonLat, int zoom=-1);
    int getZoom() const
    {
        return curZoom;
    };

    mainWindow  * getMainwindow(void);

    // get Longitude/Latitude at specified position inside view
    QPointF getLonLat(const QPoint &xy) const;
    inline QPointF getLonLat() const
    {
        return getLonLat( QPoint(width()/2, height()/2) );
    }
    inline QPointF getMinLonLat() const
    {
        return getLonLat( QPoint(0, 0) );
    }
    inline QPointF getMaxLonLat() const
    {
        return getLonLat( QPoint(width(), height()) );
    }
    // get bounding of visible map
    QRectF getArea() const;

    // relative move by delta pixels, return new lonLat
    QPointF move(const QPoint &delta);
    QPointF move(const QPointF &delta);
    QPointF move(QPointF &delta);

    float clicLon;
    float clicLat;

    int mapZoomToUpper;
    int mapZoomToLower;
    QString searchInternetMapUrl;

    SlippyMarkers *markersPtr;

    wptEdit *tableWpt;

    QString m_iconSymbolRootDir;
    qlonglong curentWptId;
    qlonglong pastedWptId;
    projectDatas *m_projectDatas;

    int pageW; // mm
    int pageH; // mm
    int printerPageW; // pixels
    int printerPageH; // pixels

    bool printCenter;
    bool printZoom;
    bool printZoomVal;
    bool printScale;
    bool printPoi;
    bool printBox;
    bool printTile;

    QPointF printedTopLeft;
    QPointF printedBottomRight;
    struct printMapTilesStruct
    {
        int lig;
        int col;
        int X;
        int Y;
        int Z;
        int posX;
        int posY;
        int x;
        int y;
        double lon;
        double lat;
    };
    QList< printMapTilesStruct > printMapTiles;
    QPointF getPrintMapTile( int X, int Y, int Z);
    QPointF getPrintMapTileXY( int X, int Y, int Z);

    double getZoomForScale( qlonglong scaleValue );

    bool PrintingMap = false;

    int zoomControlHeight;
    QRect zoomControlPlus;
    QRect zoomControlMinus;

public slots:

    void setMap(const QString &path,
                const QString &copyright,
                int minZoom,
                int maxZoom,
                const QString &userAgent,
                const QString &referer,
                int keyShortCut,
                int keyZoom
               );

    void render( QPainter *painter, qreal marginLeft, qreal marginTop, qreal marginRight, qreal marginBottom, int printerResolution, int scaleFactor, double scaleLen, double mapZoom, bool drawTileBorder, int pageWpixel, int pageHpixel, QRectF mapBorder, bool modeDebug );

private slots:

    void onMapActionCenterHere(void);
    void onMapActionZoomOutHere(void);
    void onMapActionZoomInHere(void);
    void onMapActionZoomInLower(void);
    void onMapActionZoomInUpper(void);
    void onMapNewWaypoint(void);
    void onMapEditWaypoint(void);
    void onMapActionCopyWpt();
    void onMapActionPasteWptHere();
    void onContextMenuClose( void );
    void onMapActionCopyWptCoord();
    void onMapActionSearchInternet(void);
    void ShowContextMenu(const QPoint &pos);
    void contextMenuEventHide( );
    bool eventFilter(QObject *obj, QEvent *event);


signals:
    void position(const QPointF &lonLat);

protected:
    virtual void paintEvent(QPaintEvent *ev);
    virtual void wheelEvent (QWheelEvent *ev );
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseMoveEvent(QMouseEvent *ev);

private:
    QList<SlippyMapPart *> parts;
    QMenu contextMenu;
    bool contextMenuCreated;
    QAction *mapActionWpt;
    QAction *mapActionZoomInHere;
    QAction *mapActionZoomOutHere;
    QAction *mapActionZoomInUpper;
    QAction *mapActionZoomInLower;
    QAction *mapActionCenterHere;
    QAction *mapActionCopyWpt;
    QAction *mapActionPasteWptHere;
    QAction *mapActionCopyWptCoord;
    QAction *mapActionSearchInternet;

    qlonglong getSelectedIdMarker( const QPoint &pos  );

    // Called when a user zooms in or out of the widget
    void zoomTo(const QPoint &newCenter, int newZoom);
    // used when dragging with mouse
    QPoint previousDrag;



public:
    int curZoom;          // current zoom level
    double curX, curY;    // current x/y tiles, can have fractionals
    SlippyMapTiled *map;



};


#define MAPPART_CENTER  0
#define MAPPART_ZOOM    1
#define MAPPART_ZOOMVAL 2
#define MAPPART_SCALE   3
#define MAPPART_POI     4
#define MAPPART_BOX     5
class SlippyMapPart : public QObject
#define MAPPART_TILE    6

{
    Q_OBJECT
public:
    SlippyMapPart(SlippyMapWidget *parent);

    int _id ;
    int  getId( void )
    {
        return _id;
    } ;

    virtual void draw(QPainter *painter, QPaintEvent *ev)
    {
        Q_UNUSED(painter);
        Q_UNUSED(ev);
    };
    virtual void render(QPainter *painter, qreal marginLeft, qreal marginTop, qreal marginRight, qreal marginBottom, int printerResolution, int scaleFactor, double scaleLen, double mapZoom, int pageWpixel, int pageHpixel, QRectF mapBorder, bool modeDebug)
    {
        Q_UNUSED(painter);
        Q_UNUSED(marginLeft);
        Q_UNUSED(marginTop);
        Q_UNUSED(marginRight);
        Q_UNUSED(marginBottom);
        Q_UNUSED(printerResolution);
        Q_UNUSED(scaleFactor);
        Q_UNUSED(scaleLen);
        Q_UNUSED(mapZoom);
        Q_UNUSED(pageHpixel);
        Q_UNUSED(pageWpixel);
        Q_UNUSED(modeDebug);

    };
    virtual void drawTile(QPainter *painter,
                          const QPoint &pos,
                          int tileX,
                          int tileY,
                          int tileZoom)
    {
        Q_UNUSED(painter);
        Q_UNUSED(pos);
        Q_UNUSED(tileX);
        Q_UNUSED(tileY);
        Q_UNUSED(tileZoom);
    };

    virtual void setLonLat(QPointF lonLat, int newZoom)
    {
        Q_UNUSED(lonLat);
        Q_UNUSED(newZoom);
    };

    virtual bool mousePressEvent(QMouseEvent *)
    {
        return false;
    };

    SlippyMapWidget *getSlippyMapWidget(void);



protected:
    SlippyMapWidget *widget;
};


class SlippyCenterMark : public SlippyMapPart
{
public:

    SlippyCenterMark(SlippyMapWidget *parent) : SlippyMapPart(parent)
    {
        _id = MAPPART_CENTER;
        sizeCross = 5;
        fontColor = QColor(QColor (255,0,0, 150));
        bgColor = QColor(QColor (255,255,0, 150));
        bgColor.setAlphaF(0.5);
        display = true;
    };
    void draw(QPainter *painter, QPaintEvent *ev);
    void render(QPainter *painter, qreal marginLeft, qreal marginTop, qreal marginRight, qreal marginBottom, int printerResolution, int scaleFactor, double scaleLen, double mapZoom, int pageWpixel, int pageHpixel, QRectF mapBorder, bool modeDebug);
    QFont font;
    QColor fontColor;
    QColor bgColor;
    bool display;
    int sizeCross;
};


class SlippyZoomButtons : public SlippyMapPart
{
public:
    SlippyZoomButtons(SlippyMapWidget *parent) : SlippyMapPart(parent)
    {
        _id = MAPPART_ZOOM;
        txtHeight = 0;
    };
    void draw(QPainter *painter, QPaintEvent *ev);
    void render(QPainter *painter, qreal marginLeft, qreal marginTop, qreal marginRight, qreal marginBottom, int printerResolution, int scaleFactor, double scaleLen, double mapZoom, int pageWpixel, int pageHpixel, QRectF mapBorder, bool modeDebug);
    bool mousePressEvent(QMouseEvent *ev);
    QFont font;
    QColor fontColor;
    QColor bgColor;
    bool display;
    int txtHeight;

};

class SlippyZoomVal : public SlippyMapPart
{
public:
    //QString scaleFontColor;
    //QString scaleFontBgcolor;
    //int scaleFontSize;
    //QString scaleFontFamily;
    int scaleLegendX; // 10
    int scaleLegendY; // 15

    // LV param
    QFont font;
    QColor fontColor;
    QColor bgColor;
    bool zoomDisplayVal;

    SlippyZoomVal(SlippyMapWidget *parent) : SlippyMapPart(parent)
    {
        _id = MAPPART_ZOOMVAL;

    };
    void draw(QPainter *painter, QPaintEvent *ev);
    void render(QPainter *painter, qreal marginLeft, qreal marginTop, qreal marginRight, qreal marginBottom, int printerResolution, int scaleFactor, double scaleLen, double mapZoom, int pageWpixel, int pageHpixel, QRectF mapBorder, bool modeDebug);
};

class SlippyMarkers : public SlippyMapPart//, public QGraphicsScene, public QGraphicsPixmapItem
{
public:
    SlippyMarkers(SlippyMapWidget *parent)  ;
    QPoint setIconDir( QString p_iconDir );
    void draw(QPainter *painter, QPaintEvent *ev);
    void render(QPainter *painter, qreal marginLeft, qreal marginTop, qreal marginRight, qreal marginBottom, int printerResolution, int scaleFactor, double scaleLen, double mapZoom, int pageWpixel, int pageHpixel, QRectF mapBorder, bool modeDebug);
    void addMarker( const qlonglong p_id, const QPointF &p_pos, const QString p_label, const QString p_markerIconFile );
    void clearAllMarkers(void);
    void removeById( qlonglong p_index );
    void setLabelMarkerById( qlonglong curentWptId , QString newLabel);
    void setSymbolMarkerById( qlonglong p_id , QString newSymbol);

    qlonglong countMarkers();
    QString getMarkerLabelByIdx( qlonglong idxMarker );
    QPointF getMarkerPosByIdx( qlonglong idxMarker );
    QString getMarkerLabelById( qlonglong p_id );
    qlonglong getMarkerPosById( qlonglong p_id );
    qlonglong getMarkerIdByIdx( qlonglong p_idx );
    void getMarkerList();
    void removeMarkerById( qlonglong curentWptId );
    void setMarkerPosById( qlonglong p_id , QPointF newPos);

    QColor markerFgColor;
    QColor markerBgColor;
    QFont markerFonfFamilly;

    bool markerDisplayName;



protected:
    //void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    bool mousePressEvent(QMouseEvent *mouseEvent);
    //void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
private:
    QPoint offsetPixmap;
    QPoint offsetText;
    QString iconDir;
    QList<qlonglong> idList;
    QList<QPointF> posList;
    QList<QString> labelList;
    QList<QString> markerIconFileList;

};


class SlippyScale : public SlippyMapPart
{
public:
    SlippyScale( SlippyMapWidget *parent);
    void setScale();
    void draw(QPainter *painter, QPaintEvent *ev);
    void render(QPainter *painter, qreal marginLeft, qreal marginTop, qreal marginRight, qreal marginBottom, int printerResolution, int scaleFactor, qreal scaleLen, double mapZoom, int pageWpixel, int pageHpixel, QRectF mapBorder, bool modeDebug);

    int scaleLimitHeight; // 10

    bool display1;
    QColor scale1fontColor;
    QColor scale1bgColor;
    QFont scale1fonfFamilly;
    int scale1MarginX;    // 10
    int scale1MarginY; // 20
    int scale1Precision;
    double scale1PixelLength;

    bool display2;
    QColor scale2fontColor;
    QColor scale2bgColor;
    QFont scale2fonfFamilly;
    int scale2Precision;
    int scale2MarginX;    // 10
    int scale2MarginY; // 20
    double scale2MetersLength; // variable Scale len (meters)


private:

};

class SlippyBoxPrint : public SlippyMapPart
{
public:

    SlippyBoxPrint(SlippyMapWidget *parent) : SlippyMapPart(parent)
    {
        _id = MAPPART_BOX;
    };
    void draw(QPainter *painter, QPaintEvent *ev);
    void setBox( int x, int y, int width, int height);
private :
    int x;
    int y;
    int width;
    int height;
};


class SlippyPath : public SlippyMapPart
{
public:
    SlippyPath(GeoData *data, SlippyMapWidget *parent);
    void setPath(GeoData *data);
    void draw(QPainter *painter, QPaintEvent *ev);
private:
    GeoData *data;
};



class SlippyMapTiled : public SlippyMapPart
{
    Q_OBJECT
public:
    SlippyMapTiled(SlippyMapWidget *parent);

    // draw map tile
    void drawTile(QPainter *painter,
                  const QPoint &pos,
                  int tileX,
                  int tileY,
                  int tileZoom);

    void setMap(const QString &path,
                const QString &copyright,
                int minZoom,
                int maxZoom,
                const QString &userAgent,
                const QString &referer,
                int keyShortCut,
                int keyZoom
               );

    QString cacheFilePath;


    int getMaxZoomLevel(void);
    int getMinZoomLevel(void);

    // Retrieve a tile. The tile comes either from the tileCache, will be
    // made up on-the-fly from a lower resolution tile and/or it will be
    // downloaded from the web.
    QPixmap *getTile(int x, int y, int zoomLvl, bool get=true);
    QPixmap *getTileSync(int x, int y, int zoomLvl);
    void download(int x, int y, int zoom); // download a tile
    void downloadSync(int x, int y, int zoom); // download a tile


private:
    // Preload images (e.g. from a Qt resource) into the cache. Use this to
    // be able to display some picture even without network connection.
    void preload(const QString &key, const QString &path);

    QCache<QString, QPixmap> tileCache;


    QString tilePath;
    QString copyright;

    int minZoomLevel;
    int maxZoomLevel;

    QString userAgent;
    QString referer;

    QSet<QString> zoom;                    // list of all made-up tiles

    int keyShortCut;

    int keyZoom;

private slots:
    void slotFinished();
};


#endif

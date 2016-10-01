#ifndef DIALOGAPPREF_H
#define DIALOGAPPREF_H

#include "global.h"
#include "mainWindow.h"

#include <QDialog>
#include <QCheckBox>
#include <QTableWidget>


class dialogAppPref : public QDialog
{
    Q_OBJECT

public:
    mainWindow *m_parent;
    dialogAppPref( mainWindow *parent= 0 );
    qlonglong sizeOfdir(QString path);
    int CountFiles(QString path);
    colsDisposition::colBaseType localColBase[COL_WPT_COUNT];

public slots:
    void okClicked();
    void cancelClicked();
    void displayWptCol();
    void ctxMenuColDispo(const QPoint &);
    void ctxMenuMapDispo(const QPoint &);
    void wptDispoColUp();
    void wptDispoColDown();
    void mapDispoColInsert();
    void mapDispoColSupp();
    void mapDispoColUp();
    void mapDispoColDown();
    void mapDispoColTop( );
    void mapDispoColBottom();

    void onCacheDirSelect();
    void onMarkerDirSelect();

    void onMarkerFontColor();
    void onMarkerBgColor();
    void onMarkerFontSelect();
    void onMarkerTransparency();

    void onZoomFontColor();
    void onZoomBgColor();
    void onZoomFontSelect();
    void onZoomTransparency();

    void onCrossFontColor();
    void onCrossBgColor();
    void onCrossTransparency();

    void onScaleBigFontColor();
    void onScaleBigBgColor();
    void onScaleBigFontSelect();
    void onScaleBigTransparency();

    void onScaleVarFontColor();
    void onScaleVarBgColor();
    void onScaleVarFontSelect();
    void onScaleVarTransparency();

    void onFoundBgColor();
    void onLineEditBgColor();
    void onLineSelectBgColor();

    void onMapPoiFontColor( );
    void onMapPoiBgColor( );
    void onMapPoiFontSelect( );
    void onMapPoiTransparency( );

    void onListPoiFontColor( );
    void onListPoiBgColor( );
    void onListPoiFontSelect( );
    void onListPoiTransparency( );

    void onMapLegendFontColor( );
    void onMapLegendBgColor( );
    void onMapLegendFontSelect( );
    void onMapLegendTransparency( );


private :
    QCheckBox *debugModeRunCheck;
    QCheckBox *confirmQuitCheck;
    QCheckBox *razRecentCheck;

    //QCheckBox * writeSettingChk;

    QTableWidget *tableWidgetAppPref;
    QTableWidget *tableWidgetMapDef;
    QTableWidget *tableWidgetMapSrc;
    QTableWidget *tableWidgetMapCache;
    QTableWidget *tableWidgetColDispo;
    QTableWidget *tableWidgetKbShort;
    QTableWidget *tableWidgetWebSearch;
    QTableWidget *tableWidgetMarker;
    QTableWidget *tableWidgetMapLegend;
    QTableWidget *tableWidgetLegend;
    QTableWidget *tableWidgetPrinter;

    int wptColLig;
    QColor selectColorBox( QColor currentColor );

    bool selectStatut;

    QFont markerFonfFamilly;
    QColor markerBgColor;
    QColor markerFgColor;

    QFont zoomFonfFamilly;
    QColor zoomBgColor;
    QColor zoomFgColor;
    QCheckBox *mapLegendDisplayZoomCheck;
    QCheckBox *mapLegendDisplayZoomPlusMoinsCheck;

    QFont crossFonfFamilly;
    QColor crossBgColor;
    QColor crossFgColor;

    QFont scale1FonfFamilly;
    QColor scale1BgColor;
    QColor scale1FgColor;

    QFont scale2FonfFamilly;
    QColor scale2BgColor;
    QColor scale2FgColor;

    QColor foundColor;
    QColor lineSelectColor;
    QColor lineEditColor;


    QColor mapPoiFontColorFg;
    QColor mapPoiFontColorBg;
    QFont mapPoiFontFamilly;

    QColor listPoiFontColorFg;
    QColor listPoiFontColorBg;
    QFont listPoiFontFamilly;

    QColor legendPoiFontColorFg;
    QColor legendPoiFontColorBg;
    QFont legendPoiFontFamilly;

    void onFontPreview( QTableWidget *arrayWidget, int lig, int col, QColor txtColor, QColor bgColor , QFont markerFont );


};

#endif // DIALOGAPPREF_H



#ifndef HEADER_mainWindow
#define HEADER_mainWindow

#include <QtGui>

#include "global.h"
#include "projectData.h"
#include "wptEdit.h"
#include "dialogFind.h"
#include "dialogReplace.h"
#include "dialogSearchDup.h"
#include "dialogAbout.h"
#include "maplib/SlippyMapWidget.h"
#include "maplib/MapChooseWidget.h"

class appEventFilter: public QObject
{
public:
    appEventFilter():QObject()
    {};
    ~appEventFilter() {};
    MapChooseWidget * mapChooseWidget;

    bool eventFilter(QObject* object,QEvent* event)
    {
        if(event->type() == QEvent::KeyPress)
        {
            debugp( "Filtered KeyPress Event...");
            QKeyEvent *KeyEvent = (QKeyEvent*)event;

            switch(KeyEvent->key())
            {
            case Qt::Key_F1:
                debugp( "F1 press detected" );
                return true;
            default:
                break;
            }

            return QObject::eventFilter(object,event);
        }
        else
        {
            return QObject::eventFilter(object,event);
        }
    };
};


class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    mainWindow(QWidget *parent = 0);

    void firstDisplay( void );
    void show( void );
    void init(void);

    int recentFilesMax;
    QStringList recentFiles;
    QList <QAction> *recentFilesActions;
    QMenu *recentFilesMenu ;

    // Menus
    QAction *actionNew ;
    QAction *actionSave ;
    QAction *actionImport;
    QAction *actionExport;
    QAction *actionProjProperties;
    QAction *actionMapPrint;

    QAction *actionMapMoveDefaultPos;
    QAction *actionMapSetDefault;
    QAction *actionPrefDef;
    QAction *actionMapProperties;
    QAction *actionMapCenterLatLon;

    QAction *actionEditSelectAll;
    QAction *actionEditUnselectAll;
    QAction *actionEditInvertSelect;
    QAction *actionEditSelectByMap;
    QAction *actionEditUnSelectByMap;
    QAction *actionEditshowMapBySelect;

    QAction *actionWptSearchDuplicate;
    QAction *actionWptRemoveSelected;
    QAction *actionWptDisplayLabel;
    QAction *actionWptSort;
    QAction *actionWptSearch;
    QAction *actionWptReplace;

    QAction* actionMap1;
    QAction* actionMap2;
    QAction* actionMap3;
    QAction* actionMap4;
    QAction* actionMap5;

    QStatusBar *barreEtat;

    // App pref
    QSettings *appSettings;
    bool appSetingsWriteAtExit;
    bool confirmQuit;
    void readSettings( QString iniFileName, QString cacheDirName);
    void writeSettings();
    void defaultSettings();
    void getDefMapByIndexSettings();
    QString iconDir;
    QString iconSymbolRootDir;
    QString iconSymbolName;


    // Project datas
    QString currentProjectFileName;
    QString bckCurrentProjectFileName;
    QString filters;
    QString defaultFilter;
    QString defaultSaveFileName;
    QString defaultOpenFileName;
    QString importDefaultFileName;
    QString importFilters;
    QString importDefaultFilter;
    QString exportDefaultFileName;
    QString exportFilters;
    QString exportDefaultFilter;

    QString defaultOpenPath;
    QString defaultImportPath;
    QString defaultExportPath;

    QString dateTimeFormat;
    QString mapGotoFromClipboard;
    QString clipSep;

    // Project map datas
    SlippyMapWidget slippyMap;
    //SlippyMapWidget *slippyMap;
    MapChooseWidget mapChooseWidget;
    QString slippyMapCacheFilePath;
    GeoData mapData;
    int slippyMapIndex;
    int slippyMapZoom ;
    float slippyMapLat;
    float slippyMapLon;
    float slippyMapCurX;
    float slippyMapCurY;
    int mapDefIndex;
    int mapDefZoom ;
    float mapDefLat;
    float mapDefLon;
    float mapDefCurX;
    float mapDefCurY;
    void setDefMapByIndexSettings( MapChooseWidget *mapChooseWidget, QString mapName, int mapMinZoom, int mapMaxZoom, QString mapLicence, QString mapUSerAgent, QString mapReferer, QString mapPath );

    int mapZoomToUpper;
    int mapZoomToLower;


    QString markerFontColor;
    QString markerFontBgcolor;

    QColor markerFgColor;
    QColor markerBgColor;
    QFont markerFonfFamilly;

    QString scaleFontColor;
    QString scaleFontBgcolor;
    int scaleFontSize;
    QString scaleFontFamily;
    bool zoomDisplayVal;

    int scalePixelLength;
    int scaleMarginX;
    int scaleMarginY;
    int scaleLimitHeight;
    int scaleLegendX;
    int scaleLegendY;
    int scaleLocation;
    bool scaleDisplayName;

    SlippyZoomVal *mapZoomVal;  // current zoom value (displayed hover map)
    SlippyZoomButtons *mapZoomControl;  // current zoom + / - (displayed hover map)
    SlippyCenterMark *centerMark ;      // Center cross
    SlippyScale *mapScale ;

    QString searchInternetUrl;
    QString searchInternetMapUrl;

    projectDatas projectData;

    wptEdit *tableWpt;

    bool dialogFindDisplayed;
    bool dialogSearchDupDisplayed;
    bool dialogReplaceDisplayed;
    bool dialogAboutDisplayed;
    bool dialogHelpDisplayed;

    void OnProjectDataSaved(void);
    void OnProjectDataUnSaved(void );
    void projectDataStatus(void);
    void setWptDisplayLabel(bool displayMode );
    void setMarkerFont();
    void openMap( int mapIndex);

    SlippyMarkers *markers ;


    QColor foundColor;
    QColor lineSelectColor;
    QColor lineEditColor;


    // Printer
    int printMarginTop;
    int printMarginRight;
    int printMarginBottom;
    int printMarginLeft;
    bool printDebugMode;
    bool printMapPoi;
    bool printListPoi;
    int printResolution;
    bool printMapBorder;

    QColor printMapPoiColorFg;
    QColor printMapLegendColorFg;
    QColor printListPoiColorFg;
    QColor printMapPoiColorBg;
    QColor printMapLegendColorBg;
    QColor printListPoiColorBg;
    QFont printMapPoiFont;
    QFont printMapLegendFont;
    QFont printListPoiFont;

    bool doOpenProject( QString fileOpenName);
    void mapMoveTo( int slippyMapIndex, int slippyMapZoom, float slippyMapLon, float slippyMapLat );

    bool doimportImportfile( QString fileToImport );

    void recentFileAdd( QString filePath);
    void recentFileClear();
private :

    void closeEvent ( QCloseEvent * event );
    void wptViewAsTable(void);
    void menuCheckEnableAction(void);

    QSplitter* mainSpliterWidget ;


    bool eventFilter(QObject *obj, QEvent *event);
    dialogFind *dlgFind;
    dialogReplace*dlgReplace;
    dialogSearchDup *dlgSearchDup;
    dialogAbout*dlgAbout;

private slots:

    // Menu actions
    void OnAppQuit();
    void OnAbout();
    void OnHelp();
    void OnFileNew();
    void OnFileSave();
    void OnFileSaveAs();
    void OnFileOpen();
    void OnFileImport();
    void OnFileExport();
    void OnPref();
    void OnPrefDef();
    void OnProjProperty();
    void OnEditSelectInvert();
    void OnEditUnSelectAll();
    void OnEditSelectAll();
    void OnEditSelectByMap();
    void OnEditUnSelectByMap();
    void OnEditshowMapBySelect();
    void OnWptRemoveSelected();
    void OnWptDisplayLabel(void);

    void OnSearchDuplicate(void);
    void OnWptSearch(void);
    void OnWptReplace(void);
    void OnMapMoveToDefault(void);
    void OnMapSetDefault(void);
    void OnMapPrint(void);
    void OnMapCenterLatLon(void);

    void OnOpenMap_1(void);
    void OnOpenMap_2(void);
    void OnOpenMap_3(void);
    void OnOpenMap_4(void);
    void OnOpenMap_5(void);

    void onRecentFile( QAction *action );
    void recentFileCreateMenu(void);

};



#endif

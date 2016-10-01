#include "global.h"
#include "mainWindow.h"
#include "dialogAppPref.h"
#include "dialogProjProperty.h"
#include "dialogFind.h"
#include "dialogSearchDup.h"
#include "dialogPrint.h"
#include "dialogAbout.h"
#include "dialogCsv.h"

#include <QFrame>
#include <QSplitter>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QScrollArea>
#include <QTableView>
#include <QStandardItemModel>
#include <QPoint>
#include <QDate>
#include <QtGui>


// --------- Map
#include <QComboBox>

#include "maplib/SlippyMapWidget.h"
#include "maplib/GeoUtils.h"
#include "maplib/MapChooseWidget.h"

#include "maplib/SlippyLocations.h"
#include "maplib/GeoUtils.h"
#include "maplib/SRTMv4.h"
#include "maplib/DrawITUTerrain.h"

#include <stdio.h>


QMainWindow* getMainWindow()
{
    QWidgetList widgets = qApp->topLevelWidgets();
    for (QWidgetList::iterator i = widgets.begin(); i != widgets.end(); ++i)
        if ((*i)->objectName() == "MainWindow")
            return (QMainWindow*) (*i);
    return NULL;
}

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
{
}


void mainWindow::init(void)
{
    appEventFilter globEventFilter;
    qApp->installEventFilter( &globEventFilter );
    qApp->installEventFilter(this);

    appSetingsWriteAtExit = true;

    iconDir = qApp->applicationDirPath() + "/icon/";
    iconSymbolRootDir = qApp->applicationDirPath() + "/icon/symbols/";
    iconSymbolName = "number";
    slippyMap.iconDir =  iconDir;

    // -----------------------------------------------------
    // Menus

    QMenu *menuFichier = menuBar()->addMenu( trUtf8("&Fichier", "File menu")) ;

    menuFichier->addSeparator();

    QAction *actionNew = new QAction( trUtf8("Nouveau","File menu - New") , this);
    menuFichier->addAction(actionNew);
    actionNew->setIcon(QIcon(iconDir + "tool_new.png"));
    connect(actionNew, SIGNAL(triggered()), this,  SLOT(OnFileNew( ) ) );
    actionNew->setStatusTip( trUtf8( "Nouveau projet","File new action")) ;
    menuFichier->addSeparator();

    QAction *actionOpen = new QAction( trUtf8("&Ouvrir","File menu - Load") , this);
    menuFichier->addAction(actionOpen);
    actionOpen->setShortcut(QKeySequence("Ctrl+O"));
    actionOpen->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    actionOpen->setIcon(QIcon(iconDir + "tool_open.png"));
    connect(actionOpen, SIGNAL(triggered()), this,  SLOT(OnFileOpen( ) ) );
    actionOpen->setStatusTip( trUtf8( "Ouvrir un projet","File open action")) ;

    menuFichier->addSeparator();
    actionSave = new QAction( trUtf8("&Sauver","File menu - Save") , this);
    menuFichier->addAction(actionSave);
    actionSave->setShortcut(QKeySequence("Ctrl+S"));
    actionSave->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    actionSave->setIcon(QIcon(iconDir + "tool_save.png"));
    connect(actionSave, SIGNAL(triggered()), this,  SLOT(OnFileSave( ) ) );
    actionSave->setStatusTip( trUtf8("Sauver ce projet","File save action")) ;

    QAction *actionSaveAs = new QAction( trUtf8("Sauver Sous ...","File menu - Save as") , this);
    menuFichier->addAction(actionSaveAs);
    actionSaveAs->setIcon(QIcon(iconDir + "tool_saveas.png"));
    connect(actionSaveAs, SIGNAL(triggered()), this,  SLOT(OnFileSaveAs( ) ) );
    actionSaveAs->setStatusTip( trUtf8("Sauver ce projet sous un nouveau nom","File save as action")) ;

    recentFilesMenu = menuFichier->addMenu(tr("Fichiers récents..."));
    recentFilesMenu->setIcon(QIcon(iconDir + "tool_recentfiles.png"));  // FIX

    menuFichier->addSeparator();
    actionImport = new QAction( trUtf8("&Importer","File menu - Import") , this);
    menuFichier->addAction(actionImport);
    actionImport->setIcon(QIcon(iconDir + "tool_import.png"));
    connect(actionImport, SIGNAL(triggered()), this,  SLOT(OnFileImport( ) ) );
    actionImport->setStatusTip( trUtf8("Importer un fichier de données","File import action")) ;

    actionExport = new QAction( trUtf8("&Exporter","File menu - Export") , this);
    menuFichier->addAction(actionExport);
    actionExport->setIcon(QIcon(iconDir + "tool_export.png"));
    connect(actionExport, SIGNAL(triggered()), this,  SLOT(OnFileExport( ) ) );
    actionExport->setStatusTip( trUtf8("Exporter un fichier de données","File export action")) ;

    menuFichier->addSeparator();
    QAction *actionPref = new QAction( QString::fromUtf8("&Préferences") , this);
    menuFichier->addAction(actionPref);

    connect(actionPref, SIGNAL(triggered()), this, SLOT(OnPref()));
    actionPref->setShortcut(QKeySequence("Ctrl+P"));
    actionPref->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
    actionPref->setIcon(QIcon( iconDir + "tool_pref.png"));
    actionPref->setStatusTip( trUtf8("Préférences du programme" ,"Program prefs action")) ;

    /*
                            For test dev only :

    actionPrefDef = new QAction( trUtf8("DUMMY ENTRY","Wpt menu - default pref.") , this);
    menuFichier->addAction(actionPrefDef);
    actionPrefDef->setIcon(QIcon( iconDir + "tool_defaultpref.png"));
    connect(actionPrefDef, SIGNAL(triggered()), this,  SLOT(OnPrefDef( ) ) );
    actionPrefDef->setStatusTip( trUtf8("Charger les préférences par défaut","default pref.")) ;
    */

    actionProjProperties = new QAction( trUtf8("Propriétés du projet","Wpt menu - project properties") , this);
    menuFichier->addAction(actionProjProperties);
    actionProjProperties->setIcon(QIcon( iconDir + "tool_projproperties.png"));
    connect(actionProjProperties, SIGNAL(triggered()), this,  SLOT(OnProjProperty( ) ) );
    actionProjProperties->setStatusTip( trUtf8("Propriétés du projet","Wpt properties")) ;

    actionMapPrint = new QAction( trUtf8("Imprimer","Wpt print map") , this);
    menuFichier->addAction(actionMapPrint);
    connect(actionMapPrint, SIGNAL(triggered()), this,  SLOT(OnMapPrint( ) ) );
    actionMapPrint->setIcon(QIcon(iconDir + "tool_print.png"));
    actionMapPrint->setStatusTip( trUtf8("Imprimer la carte et les balises","Wpt print map ans waypoints")) ;

    menuFichier->addSeparator();
    QAction *actionQuitter = new QAction( trUtf8("&Quitter","File menu - Quit option") , this);
    menuFichier->addAction(actionQuitter);
    connect(actionQuitter, SIGNAL(triggered()), this, SLOT(OnAppQuit()));
    actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));
    actionQuitter->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    actionQuitter->setIcon(QIcon(iconDir + "tool_quit.png"));
    actionQuitter->setStatusTip( trUtf8("Quitter le programme","Program exit")) ;

    // --------------------------------------------- Menu vue
    QMenu *menuEdit = menuBar()->addMenu("Vue");
    actionEditSelectAll = new QAction( trUtf8("Tout selectionner","Edit - select all") , this);
    menuEdit->addAction(actionEditSelectAll );
    actionEditSelectAll->setIcon(QIcon(iconDir + "tool_select.png"));
    connect(actionEditSelectAll, SIGNAL(triggered()), this, SLOT(OnEditSelectAll()));
    actionEditSelectAll->setStatusTip( trUtf8("Selectionner toutes les lignes","Select all")) ;

    actionEditUnselectAll = new QAction( trUtf8("Tout de-selectionner","Edit - Unselect all") , this);
    menuEdit->addAction(actionEditUnselectAll );
    actionEditUnselectAll ->setIcon(QIcon(iconDir + "tool_unselect.png"));
    connect(actionEditUnselectAll, SIGNAL(triggered()), this, SLOT(OnEditUnSelectAll()));
    actionEditUnselectAll->setStatusTip( trUtf8("Dé-selectionner toutes les lignes","Unselect all")) ;

    actionEditInvertSelect = new QAction( trUtf8("Inverser la selection","Edit - invert selection") , this);
    menuEdit->addAction(actionEditInvertSelect );
    actionEditInvertSelect ->setIcon(QIcon(iconDir + "tool_selectinvert.png"));
    connect(actionEditInvertSelect, SIGNAL(triggered()), this, SLOT(OnEditSelectInvert()));
    actionEditInvertSelect->setStatusTip( trUtf8("Inverser la selection des lignes","Invert selection")) ;

    actionEditSelectByMap = new QAction( trUtf8("Selection selon la carte affichée","Edit - by map select") , this);
    menuEdit->addAction(actionEditSelectByMap );
    actionEditSelectByMap ->setIcon(QIcon(iconDir + "tool_selectbymap.png"));
    connect(actionEditSelectByMap, SIGNAL(triggered()), this, SLOT(OnEditSelectByMap()));
    actionEditSelectByMap->setStatusTip( trUtf8("Selection des waypoints selon la carte affichée","Select waypoints upon displayed map")) ;

    actionEditUnSelectByMap = new QAction( trUtf8("Dé-selection selon la carte affichée","Edit - by map unselect") , this);
    menuEdit->addAction(actionEditUnSelectByMap );
    actionEditUnSelectByMap->setIcon(QIcon(iconDir + "tool_unselectbymap.png"));
    connect(actionEditUnSelectByMap, SIGNAL(triggered()), this, SLOT(OnEditUnSelectByMap()));
    actionEditUnSelectByMap->setStatusTip( trUtf8("Dé-selection des balises selon la carte affichée","Unselect waypoints upon displayed map")) ;

    actionEditshowMapBySelect= new QAction( trUtf8("Ajuster la carte","Edit - Adjust map") , this);
    menuEdit->addAction(actionEditshowMapBySelect );
    actionEditshowMapBySelect->setIcon(QIcon(iconDir + "tool_adjustmap.png"));
    connect(actionEditshowMapBySelect, SIGNAL(triggered()), this, SLOT(OnEditshowMapBySelect()));
    actionEditshowMapBySelect->setStatusTip( trUtf8("Ajuster la position et taille de la carte selon les balises selectionnées","Show map upon selected waypoints")) ;

    // --------------------------------------------- Menu Balises
    QMenu *menuTool = menuBar()->addMenu("Balises");

    actionWptSearch= new QAction( trUtf8("Rechercher","Wpt - search") , this);
    menuTool->addAction(actionWptSearch);
    actionWptSearch->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
    actionWptSearch->setIcon(QIcon(iconDir + "tool_find.png"));
    connect(actionWptSearch, SIGNAL(triggered()), this, SLOT(OnWptSearch()));
    actionWptSearch->setStatusTip( trUtf8("Rechercher des balises","Search waypoints")) ;

    actionWptReplace= new QAction( trUtf8("Remplacer","Wpt - replace") , this);
    menuTool->addAction(actionWptReplace);
    actionWptReplace->setIcon(QIcon(iconDir + "tool_replace.png"));

    connect(actionWptReplace, SIGNAL(triggered()), this, SLOT(OnWptReplace()));
    actionWptReplace->setStatusTip( trUtf8("Remplacer du texte dans les balises","Replace in waypoints")) ;

    actionWptSearchDuplicate = new QAction( trUtf8("&Rechercher les doublons","Wpt - search duplicate") , this);
    menuTool->addAction(actionWptSearchDuplicate);
    actionWptSearchDuplicate ->setIcon(QIcon(iconDir + "tool_duplicate.png"));
    connect(actionWptSearchDuplicate, SIGNAL(triggered()), this, SLOT(OnSearchDuplicate()));
    actionWptSearchDuplicate->setStatusTip( trUtf8("Rechercher les balise en double","Search duplicate waypoints")) ;

    actionWptRemoveSelected = new QAction( trUtf8("Supprimer la selection","Wpt - delete selected") , this);
    menuTool->addAction(actionWptRemoveSelected);
    actionWptRemoveSelected ->setIcon(QIcon(iconDir + "tool_delselection.png"));
    connect(actionWptRemoveSelected, SIGNAL(triggered()), this, SLOT(OnWptRemoveSelected()));
    actionWptRemoveSelected->setStatusTip( trUtf8("Supprimer les balses selectionnées","Remove selected waypoints")) ;

    menuTool->addSeparator();

    actionWptDisplayLabel= new QAction( trUtf8("Afficher les libellés","Wpt - hide/display wpt label") , this);
    actionWptDisplayLabel->setCheckable(true);
    //if (markers->markerDisplayName==true) actionWptDisplayLabel->setChecked(true);
    menuTool->addAction(actionWptDisplayLabel);
    connect(actionWptDisplayLabel, SIGNAL(triggered()), this, SLOT(OnWptDisplayLabel()));
    actionWptDisplayLabel->setStatusTip( trUtf8("Afficher (ou cacher) les libellés des balises sur la carte","Display (or hide) waypoints descriptions")) ;

    // --------------------------------------------- Menu cartes
    QMenu *menuMap = menuBar()->addMenu("Cartes");
    actionMapMoveDefaultPos = new QAction( trUtf8("Aller a la carte par defaut","Map - move to def pos") , this);
    menuMap->addAction(actionMapMoveDefaultPos);
    actionMapMoveDefaultPos ->setIcon(QIcon( iconDir + "tool_mapdefgo.png"));
    connect(actionMapMoveDefaultPos , SIGNAL(triggered()), this, SLOT(OnMapMoveToDefault()));
    actionMapMoveDefaultPos->setStatusTip( trUtf8("Initialiser l'affichage de la carte","Map - reset map position")) ;

    actionMapSetDefault = new QAction( trUtf8("Fixer la carte par defaut","Map - move to def pos") , this);
    menuMap->addAction(actionMapSetDefault);
    actionMapSetDefault ->setIcon(QIcon( iconDir + "tool_mapdefset.png"));
    connect(actionMapSetDefault , SIGNAL(triggered()), this, SLOT(OnMapSetDefault()));
    actionMapSetDefault->setStatusTip( trUtf8("Faire de cette carte, la carte par défaut","Map - set default map")) ;

    actionMapCenterLatLon = new QAction( trUtf8("Centrer aux coordonnées ...","Go to coord ...") , this);
    menuMap->addAction(actionMapCenterLatLon);
    actionMapCenterLatLon->setIcon(QIcon( iconDir + "tool_ctx_copycoord.png"));
    connect(actionMapCenterLatLon , SIGNAL(triggered()), this, SLOT(OnMapCenterLatLon()));
    actionMapCenterLatLon->setStatusTip( trUtf8("Positionner la cartes aux coordonnées ...","Go to coord ...")) ;


    QMenu* subMenuMaps = menuMap->addMenu( "Cartes..." );
    actionMap1 = subMenuMaps->addAction( "Carte #1" );
    actionMap1->setShortcut(QKeySequence(Qt::Key_F1));
    actionMap1->setIcon(QIcon(iconDir + "tool_map_f1.png"));
    connect(actionMap1, SIGNAL(triggered()), this,  SLOT(OnOpenMap_1( ) ) );

    actionMap2 = subMenuMaps->addAction( "Carte #2" );
    actionMap2->setShortcut(QKeySequence(Qt::Key_F2));
    actionMap2->setIcon(QIcon(iconDir + "tool_map_f2.png"));
    connect(actionMap2, SIGNAL(triggered()), this,  SLOT(OnOpenMap_2( ) ) );

    actionMap3 = subMenuMaps->addAction( "Carte #3" );
    actionMap3->setShortcut(QKeySequence(Qt::Key_F3));
    actionMap3->setIcon(QIcon(iconDir + "tool_map_f3.png"));
    connect(actionMap3, SIGNAL(triggered()), this,  SLOT(OnOpenMap_3( ) ) );

    actionMap4 = subMenuMaps->addAction( "Carte #4" );
    actionMap4->setShortcut(QKeySequence(Qt::Key_F4));
    actionMap4->setIcon(QIcon(iconDir + "tool_map_f4.png"));
    connect(actionMap4, SIGNAL(triggered()), this,  SLOT(OnOpenMap_4( ) ) );

    actionMap5 = subMenuMaps->addAction( "Carte #5" );
    actionMap5->setShortcut(QKeySequence(Qt::Key_F5));
    connect(actionMap5, SIGNAL(triggered()), this,  SLOT(OnOpenMap_5( ) ) );
    actionMap5->setIcon(QIcon(iconDir + "tool_map_f5.png"));



    QMenu *menuHelp = menuBar()->addMenu("Aide");
    QAction *actionAbout = new QAction( QIcon(iconDir +  "icon_logoapp.png"), "&A propos", this);
    menuHelp->addAction(actionAbout);
    connect(actionAbout, SIGNAL(triggered()), this,  SLOT(OnAbout() ) );
    actionAbout->setStatusTip( trUtf8("A propos de ce programme","Program about action")) ;

    QAction *actionHelp = new QAction( QIcon(iconDir +  "icon_logoapp.png"), "&Aide", this);
    menuHelp->addAction(actionHelp);
    connect(actionHelp, SIGNAL(triggered()), this,  SLOT(OnHelp() ) );
    actionHelp->setStatusTip( trUtf8("Aide : lire l'aide locale (html)","Program help")) ;

    // -----------------------------------------------------
    // Toolbars
    QToolBar *toolBarFichier = addToolBar("Fichier");
    toolBarFichier->addAction(actionQuitter);
    toolBarFichier->addAction(actionPref);
    toolBarFichier->addAction(actionSave);
    toolBarFichier->addAction(actionProjProperties);

    QToolBar *toolBarMaps = addToolBar("Cartes");
    toolBarMaps->addAction(actionMap1);
    toolBarMaps->addAction(actionMap2);
    toolBarMaps->addAction(actionMap3);
    toolBarMaps->addAction(actionMap4);
    toolBarMaps->addAction(actionMap5);

    // -----

    // -----------------------------------------------------
    // Statut bar
    barreEtat = statusBar();
    // -----

    // -----------------------------------------------------
    // Layout
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mainSpliterWidget = new QSplitter();
    mainLayout->addWidget(mainSpliterWidget);

    QTabWidget *ongletsWidget = new QTabWidget(this);

    QWidget *pageWpt = new QWidget;
    QWidget *pageTrk = new QWidget;
    QWidget *pageRte = new QWidget;

    ongletsWidget->addTab(pageWpt, QString::fromUtf8("Balises"));
    ongletsWidget->addTab(pageTrk, QString::fromUtf8("Traces"));
    ongletsWidget->addTab(pageRte, QString::fromUtf8("Itinéraires"));

    ongletsWidget->setTabIcon( 0, QIcon( iconDir + "tab_balises.png")) ;
    ongletsWidget->setTabIcon( 1, QIcon( iconDir + "tab_traces.png")) ;
    ongletsWidget->setTabIcon( 2, QIcon( iconDir + "tab_itis.png")) ;

    ongletsWidget->setFocusPolicy( Qt::ClickFocus );

    mainSpliterWidget->addWidget(ongletsWidget);
    mainSpliterWidget->setFocusPolicy( Qt::ClickFocus );

    QHBoxLayout *tableLayout = new QHBoxLayout();
    // -----

    // -----------------------------------------------------
    // Wpt edit pane
    tableWpt = new wptEdit(this);
    tableLayout->addWidget(tableWpt);
    // -----

    QWidget *dummywidget = new QWidget();
    dummywidget->setLayout(tableLayout);

    pageWpt->setLayout(tableLayout);

    setCentralWidget(mainSpliterWidget);
    // ----------------------------------------------------

    // -----------------------------------------------------
    // Map widget
    QWidget *mapWidget = new QWidget();

    QVBoxLayout *mapPaneLayout = new QVBoxLayout(mapWidget);

    mapPaneLayout->setSpacing(0);
    mapPaneLayout->setContentsMargins(0, 0, 0, 0);

    mapPaneLayout->addWidget(&mapChooseWidget);
    mapPaneLayout->addWidget(&slippyMap);

    centerMark = new SlippyCenterMark(&slippyMap);
    slippyMap.appendPart(centerMark);

    mapZoomControl = new SlippyZoomButtons(&slippyMap);
    slippyMap.appendPart(mapZoomControl);

    mapZoomVal = new SlippyZoomVal(&slippyMap);

    // LV Param
    mapZoomVal->font = QFont( "Arial", 20);
    mapZoomVal->zoomDisplayVal = true;
    mapZoomVal->bgColor = QColor(255, 10, 100, 100);
    mapZoomVal->fontColor = QColor(2, 1, 1, 50);

    slippyMap.appendPart(mapZoomVal);

    markers = new SlippyMarkers(&slippyMap);
    markers->setIconDir( iconSymbolRootDir + iconSymbolName);
    slippyMap.appendPart(markers);
    slippyMap.markersPtr = markers;
    tableWpt->setMarkers(markers);
    tableWpt->setMap(&slippyMap);

    mapScale = new SlippyScale(&slippyMap);
    slippyMap.appendPart(mapScale);


    slippyMap.tableWpt = tableWpt;

    // Map contetx menu
    mapWidget->connect(&mapChooseWidget, SIGNAL(setMap(const QString &, const QString &, int, int, const QString, const QString, int, int  )), &slippyMap, SLOT(setMap(const QString &, const QString &, int, int, const QString &, const QString, int, int)) );
    mainSpliterWidget->addWidget( mapWidget );
    // -----

    // ----------------------------------------
    // Various init
    filters = QString("Projet gpp (*.gpp);;Tous (*.*)");
    defaultFilter = QString("Project gpp (*.gpp)");

    defaultOpenFileName = defaultOpenPath + "/" + "myproject.gpp";
    defaultSaveFileName = defaultOpenPath + "/" + "myproject.gpp";

    currentProjectFileName = "";

    importFilters = QString::fromUtf8("Fichier Gpx (*.gpx);;Fichier Csv (*.csv);;Fichier Kml (*.kml);;Fichier Kmz (*.kmz);;Tous (*.*)");
    importDefaultFilter = QString::fromUtf8("Fichier Gpx (*.gpx)");
    exportFilters = QString::fromUtf8("Fichier Gpx (*.gpx);;Fichier Csv (*.csv);;Tous (*.*)");
    exportDefaultFilter = QString::fromUtf8("Fichier Gpx (*.gpx)");

    projectData.initStruct();

    projectData.m_iconSymbolRootDir = iconSymbolRootDir + iconSymbolName;

    // -----------------------------------------------------
    // Message status
    barreEtat->showMessage( QString::fromUtf8("Prêt"));
    setWindowTitle( currentProjectFileName);
    setWindowIcon(QIcon(iconDir +  "icon_logoapp.png"));
    // -----

    // installEventFilter(this);

    dialogFindDisplayed = false;

    projectData.dataSaved = true;
    projectDataStatus();

}

void mainWindow::openMap( int mapIndex)
{

    int mapIndexValue;
    int keyMapZoom;

    QPointF lonLatCenter;
    QPoint centerCoord;

    centerCoord.setX(slippyMap.width() / 2);
    centerCoord.setY(slippyMap.height() / 2);
    lonLatCenter= slippyMap.getLonLat(centerCoord);

    // index de la carte
    mapIndexValue = mapChooseWidget.getMapIndexBykeyShortCut( mapIndex );
    if (mapIndexValue>=0)
    {
        debugp("set list index to " + QString::number( mapIndexValue  ) );
        mapChooseWidget.setCurrentIndex( mapIndexValue  );
        keyMapZoom = mapChooseWidget.getMapZoomBykeyShortCuts( mapIndex );
        if (keyMapZoom >0)
        {
            slippyMap.setLonLat( lonLatCenter , keyMapZoom);
            slippyMap.update();
        }
    }

}


void mainWindow::OnOpenMap_1(void)
{
    openMap (1);
}
void mainWindow::OnOpenMap_2(void)
{
    openMap (2);
}
void mainWindow::OnOpenMap_3(void)
{
    openMap (3);
}
void mainWindow::OnOpenMap_4(void)
{
    openMap (4);
}
void mainWindow::OnOpenMap_5(void)
{
    openMap (5);
}

bool mainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        debugp( "key " + QString::number(keyEvent->key()) );

    }
    if (event->type() == QEvent::MouseMove)
    {
        //QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        //statusBar()->showMessage(QString("Mouse move (%1,%2)").arg(mouseEvent->pos().x()).arg(mouseEvent->pos().y()));
    }
    return false;

    //  return QObject::eventFilter(obj, event);
}

void mainWindow::firstDisplay( void )
{
    debugp("init display ...");
    tableWpt->initDisplay();
    if (markers->markerDisplayName==true) actionWptDisplayLabel->setChecked(true);
    mapChooseWidget.mapChoose(slippyMapIndex);
    recentFileCreateMenu();
    actionMap1->setToolTip( trUtf8(mapChooseWidget.getMapNameBykeyShortCut(1).toStdString().c_str(),"map1")) ;
    actionMap2->setToolTip( trUtf8(mapChooseWidget.getMapNameBykeyShortCut(2).toStdString().c_str(),"map2")) ;
    actionMap3->setToolTip( trUtf8(mapChooseWidget.getMapNameBykeyShortCut(3).toStdString().c_str(),"map3")) ;
    actionMap4->setToolTip( trUtf8(mapChooseWidget.getMapNameBykeyShortCut(4).toStdString().c_str(),"map3")) ;
    actionMap5->setToolTip( trUtf8(mapChooseWidget.getMapNameBykeyShortCut(5).toStdString().c_str(),"map5")) ;

}

void mainWindow::show( void )
{
    QMainWindow::show();
    QApplication::processEvents();

    slippyMap.map->cacheFilePath= slippyMapCacheFilePath;

    mapMoveTo( slippyMapIndex, slippyMapZoom, slippyMapLon, slippyMapLat );

    tableWpt->mapZoomToUpper = mapZoomToUpper;
    tableWpt->mapZoomToLower = mapZoomToLower;
    tableWpt->m_iconSymbolRootDir = iconSymbolRootDir + iconSymbolName ;
    tableWpt->searchInternetUrl = searchInternetUrl;
    tableWpt->iconDir = iconDir;

    slippyMap.mapZoomToUpper = mapZoomToUpper;
    slippyMap.mapZoomToLower = mapZoomToLower;
    slippyMap.searchInternetMapUrl = searchInternetMapUrl;

    mapZoomVal->zoomDisplayVal = zoomDisplayVal;

    mapScale->scaleLimitHeight = scaleLimitHeight; // 10

    slippyMap.update();

    setWindowTitle( currentProjectFileName );

    OnFileNew();

    setWptDisplayLabel(markers->markerDisplayName);

    projectDataStatus();

}

/*
On Unix systems, if the file format is NativeFormat , the following files are used by default:

    $HOME/.config/MySoft/Star Runner.conf (Qt for Embedded Linux: $HOME/Settings/MySoft/Star Runner.conf)
    $HOME/.config/MySoft.conf (Qt for Embedded Linux: $HOME/Settings/MySoft.conf)
    /etc/xdg/MySoft/Star Runner.conf
    /etc/xdg/MySoft.conf

*/
void mainWindow::writeSettings()
{

    appSettings->setValue("debugModeRun", debugModeRun);
    appSettings->clear();

    // Center of the map = QPointF maplonlat = slippyMap.getLonLat();
    QPointF mapPos = slippyMap.getLonLat();

    appSettings->setValue("appSetingsWriteAtExit", appSetingsWriteAtExit);
    appSettings->setValue("confirmQuit", confirmQuit);

    appSettings->setValue("dateTimeFormat", dateTimeFormat);

    appSettings->setValue("pos", pos());
    appSettings->setValue("size", size());
    appSettings->setValue("splitterState", mainSpliterWidget->saveState());
    appSettings->setValue("importDefaultFileName", importDefaultFileName );
    appSettings->setValue("slippyMapIndex", mapChooseWidget.currentIndex() );
    appSettings->setValue("slippyMapZoom",  slippyMap.curZoom );
    appSettings->setValue("slippyMapLon", mapPos.x() );
    appSettings->setValue("slippyMapLat", mapPos.y() );
    appSettings->setValue("slippyMapCurX", slippyMap.curX  );
    appSettings->setValue("slippyMapCurY", slippyMap.curY  );

    debugp( "mapPos.x / y = " + QString::number(mapPos.x()) + " / " + QString::number(mapPos.y()));

    appSettings->setValue("defaultOpenPath", defaultOpenPath  );
    appSettings->setValue("defaultImportPath", defaultImportPath );
    appSettings->setValue("defaultExportPath", defaultExportPath );

    //appSettings->setValue("mapCacheFilePath", slippyMapCacheFilePath);
    appSettings->setValue("iconSymbolName", iconSymbolName);

    appSettings->setValue("mapDefIndex", mapDefIndex);
    appSettings->setValue("mapDefZoom", mapDefZoom);
    appSettings->setValue("mapDefLon", mapDefLon);
    appSettings->setValue("mapDefLat", mapDefLat);
    appSettings->setValue("mapDefCurX", mapDefCurX);
    appSettings->setValue("mapDefCurY", mapDefCurY);

    appSettings->setValue("mapZoomToUpper", mapZoomToUpper);
    appSettings->setValue("mapZoomToLower", mapZoomToLower);

    appSettings->setValue("mapSrcCount", mapChooseWidget.count());
    int i;
    for (i = 0; i < mapChooseWidget.count(); i++)
    {
        QString currentItemText = mapChooseWidget.itemText(i);
        currentItemText = currentItemText.trimmed();
        debugp("map add #"+ QString(i) + " = " + currentItemText);

        appSettings->setValue( QString("mapName_%1").arg(i) , currentItemText );
        appSettings->setValue( QString("mapMinZoom_%1").arg(i) , mapChooseWidget.minZoom[i] );
        appSettings->setValue( QString("mapMaxZoom_%1").arg(i) , mapChooseWidget.maxZoom[i] );
        appSettings->setValue( QString("mapLicence_%1").arg(i) , mapChooseWidget.licenses[i] );
        appSettings->setValue( QString("mapUSerAgent_%1").arg(i) , mapChooseWidget.userAgents[i] );
        appSettings->setValue( QString("mapReferer_%1").arg(i) , mapChooseWidget.referers[i] );
        appSettings->setValue( QString("mapPath_%1").arg(i) , mapChooseWidget.paths[i] );
        appSettings->setValue( QString("keyShortCut_%1").arg(i) , mapChooseWidget.keyShortCuts[i] );
        appSettings->setValue( QString("keykeyZoom_%1").arg(i) , mapChooseWidget.keyZooms[i] );
    }

    appSettings->setValue("markerFgColor", markers->markerFgColor);
    appSettings->setValue("markerBgColor", markers->markerBgColor);
    appSettings->setValue("markerFonfFamilly", markers->markerFonfFamilly);

    appSettings->setValue("markerDisplayName", markers->markerDisplayName);

    appSettings->setValue("scaleFontColor", scaleFontColor);
    appSettings->setValue("scaleFontBgcolor", scaleFontBgcolor);
    appSettings->setValue("scaleFontSize", scaleFontSize);
    appSettings->setValue("scaleFontFamily", scaleFontFamily);
    appSettings->setValue("scalePixelLength", scalePixelLength);
    appSettings->setValue("scaleMarginX", scaleMarginX);
    appSettings->setValue("scaleMarginY", scaleMarginY);
    appSettings->setValue("scaleLimitHeight", scaleLimitHeight);
    appSettings->setValue("scaleLegendX", scaleLegendX);
    appSettings->setValue("scaleLegendY", scaleLegendY);
    appSettings->setValue("scaleLocation", scaleLocation);
    appSettings->setValue("scaleDisplayName", scaleDisplayName);

    appSettings->setValue("zoomDisplayVal", zoomDisplayVal);

    appSettings->setValue("searchInternetUrl", searchInternetUrl );
    appSettings->setValue("searchInternetMapUrl", searchInternetMapUrl );


    appSettings->setValue("colMetaCount", COL_WPT_COUNT );

    for (i = 0; i < COL_WPT_COUNT; i++)
    {
        debugp ("Col meta #" + QString::number( i ) + "  id= "+  QString::number( tableWpt->m_colsDisposition.getWptColId(i)) + " -colname=" + tableWpt->m_colsDisposition.getWptColName(i) );
        appSettings->setValue( QString("colMeta_%1").arg(i) , tableWpt->m_colsDisposition.getWptColId(i) );
        appSettings->setValue( QString("colName_%1").arg(i) , tableWpt->m_colsDisposition.getWptColName(i) );
        appSettings->setValue( QString("colTooltip_%1").arg(i) , tableWpt->m_colsDisposition.getWptColTooltip(i) );
        appSettings->setValue( QString("colLen_%1").arg(i) , tableWpt->m_colsDisposition.getWptColLen(i) );
        appSettings->setValue( QString("colVisible_%1").arg(i) , tableWpt->m_colsDisposition.getWptColVisibility(i)  );
    }

    // Map
    appSettings->setValue( QString("mapZoomVal_font") , mapZoomVal->font );
    appSettings->setValue( QString("mapZoomVal_zoomDisplayVal"), mapZoomVal->zoomDisplayVal );
    appSettings->setValue( QString("mapZoomVal_bgColor") , mapZoomVal->bgColor );
    appSettings->setValue( QString("mapZoomVal_fontColor"), mapZoomVal->fontColor );

    // Cross
    appSettings->setValue( QString("centerMark_bgColor"), centerMark->bgColor );
    appSettings->setValue( QString("centerMark_fontColor") , centerMark->fontColor );
    appSettings->setValue( QString("centerMark_display"), centerMark->display );
    appSettings->setValue( QString("centerMark_font") , centerMark->font );

    // Scale
    appSettings->setValue( QString("mapScale1_display") , mapScale->display1 );
    appSettings->setValue( QString("mapScale1_bgColor"), mapScale->scale1bgColor );
    appSettings->setValue( QString("mapScale1_fontColor") , mapScale->scale1fontColor );
    appSettings->setValue( QString("mapScale1_fontfamilly") , mapScale->scale1fonfFamilly );
    appSettings->setValue( QString("mapScale1_marginx") , mapScale->scale1MarginX );
    appSettings->setValue( QString("mapScale1_marginy") , mapScale->scale1MarginY );
    appSettings->setValue( QString("mapScale1_precision") , mapScale->scale1Precision );
    appSettings->setValue( QString("mapScale1_len") , mapScale->scale1PixelLength );

    appSettings->setValue( QString("mapScale2_display") , mapScale->display2 );
    appSettings->setValue( QString("mapScale2_bgColor") , mapScale->scale2bgColor );
    appSettings->setValue( QString("mapScale2_fontColor") , mapScale->scale2fontColor );
    appSettings->setValue( QString("mapScale2_fontfamilly") , mapScale->scale2fonfFamilly );
    appSettings->setValue( QString("mapScale2_marginx") , mapScale->scale2MarginX );
    appSettings->setValue( QString("mapScale2_marginy") , mapScale->scale2MarginY );
    appSettings->setValue( QString("mapScale2_precision") , mapScale->scale2Precision );
    appSettings->setValue( QString("mapScale2_len") , mapScale->scale2MetersLength );

    appSettings->setValue( QString("foundColor") , foundColor );
    appSettings->setValue( QString("lineSelectColor") , lineSelectColor );
    appSettings->setValue( QString("lineEditColor") , lineEditColor );

    // Printer
    appSettings->setValue( QString("printMarginTop") , printMarginTop );
    appSettings->setValue( QString("printMarginRight") , printMarginRight );
    appSettings->setValue( QString("printMarginBottom") , printMarginBottom );
    appSettings->setValue( QString("printMarginLeft") , printMarginLeft );
    appSettings->setValue( QString("printMapPoiColorFg") , printMapPoiColorFg );
    appSettings->setValue( QString("printMapLegendColorFg") , printMapLegendColorFg );
    appSettings->setValue( QString("printListPoiColorFg") , printListPoiColorFg );
    appSettings->setValue( QString("printMapPoiColorBg") , printMapPoiColorBg );
    appSettings->setValue( QString("printMapLegendColorBg") , printMapLegendColorBg );
    appSettings->setValue( QString("printListPoiColorBg") , printListPoiColorBg );
    appSettings->setValue( QString("printMapPoiFont") , printMapPoiFont );
    appSettings->setValue( QString("printMapLegendFont") , printMapLegendFont );
    appSettings->setValue( QString("printListPoiFont") , printListPoiFont );
    appSettings->setValue( QString("printDebugMode") , printDebugMode );
    appSettings->setValue( QString("printMapPoi") , printMapPoi );
    appSettings->setValue( QString("printListPoi") , printListPoi );
    appSettings->setValue( QString("printResolution") , printResolution );
    appSettings->setValue( QString("printMapBorder") , printMapBorder );

    appSettings->setValue( QString("mapGotoFromClipboard") , mapGotoFromClipboard );

    appSettings->setValue( QString("clipSep") , clipSep );

    appSettings->setValue( QString("recentFiles") , recentFiles );
    appSettings->setValue( QString("recentFilesMax") , recentFilesMax );


}
/*void mainWindow::defaultSettings()
{
    mapDefIndex = 0;
    mapDefZoom = 6;
    mapDefLon = 3.35;
    mapDefLat = 46.88;
    mapDefCurX = 32.59;
    mapDefCurY = 22.45;

    // Default map first display
    slippyMapIndex = mapDefIndex;
    slippyMapZoom = mapDefZoom;
    slippyMapLon = mapDefLon;
    slippyMapLat = mapDefLat;
    slippyMapCurX = mapDefCurX;
    slippyMapCurY = mapDefCurY;

    mapZoomToUpper = 15;
    mapZoomToLower = 5;

    markers->markerDisplayName = false;

    scaleFontColor = "#001122";
    scaleFontBgcolor = "#FF2255";
    scaleFontSize = 12;
    scaleFontFamily = "Arial";
    zoomDisplayVal = 1;
    scalePixelLength=100;
    scaleMarginX=10;
    scaleMarginY=20;
    scaleLimitHeight=10;
    scaleLegendX=10;
    scaleLegendY=15;
    scaleLocation=4;

    searchInternetUrl = "https://duckduckgo.com/?q=";
    searchInternetMapUrl = "http://easy-street-view.com/streetview/";

    tableWpt->setRowCount(0);

    tableWpt->m_colsDisposition.setColBase(0, COL_WPT_SELECTED    , QString::fromUtf8("Selection"), QString::fromUtf8("Point séléctionné ou non") , true, 50 );
    tableWpt->m_colsDisposition.setColBase(1, COL_WPT_ID          , QString::fromUtf8("Identifiant"), QString::fromUtf8("Identifiant interne") , false, 20 );
    tableWpt->m_colsDisposition.setColBase(2, COL_WPT_NOTE        , QString::fromUtf8("Note"), QString::fromUtf8("Note/remarques utilisateur") , true, 80 );
    tableWpt->m_colsDisposition.setColBase(3, COL_WPT_GROUP       , QString::fromUtf8("Groupe"), QString::fromUtf8("Valeur du groupe") , false, 80 );
    tableWpt->m_colsDisposition.setColBase(4, COL_WPT_LAT         , QString::fromUtf8("Latitude"), QString::fromUtf8("Latitude") , true, 80 );
    tableWpt->m_colsDisposition.setColBase(5, COL_WPT_LON         , QString::fromUtf8("Longitude"), QString::fromUtf8("Longitude") , true, 80 );
    tableWpt->m_colsDisposition.setColBase(6, COL_WPT_NAME        , QString::fromUtf8("Nom"), QString::fromUtf8("Libellé de la balise") , true, 100 );
    tableWpt->m_colsDisposition.setColBase(7, COL_WPT_ELE         , QString::fromUtf8("Altitude"), QString::fromUtf8("Altitude en mètres") , false, 20 );
    tableWpt->m_colsDisposition.setColBase(8, COL_WPT_TIME        , QString::fromUtf8("Date de modification"), QString::fromUtf8("Date de creation/modification de la balise. (Univeral Coordinated Time : UTC)") , false, 20 );
    tableWpt->m_colsDisposition.setColBase(9, COL_WPT_MAGVAR      , QString::fromUtf8("Orientation Gps"), QString::fromUtf8("L'orientation magnétique du récepteur GPS en degrés") , false, 20 );
    tableWpt->m_colsDisposition.setColBase(10, COL_WPT_GEOIDHEIGHT , QString::fromUtf8("Hauteur Géoïde"), QString::fromUtf8("La hauteur du géoïde (niveau de la mer) au-dessus de l'ellipsoïde WGS 84, tel que décrit dans la spécification des messages NMEA GGA") , false, 20 );
    tableWpt->m_colsDisposition.setColBase(11, COL_WPT_CMT         , QString::fromUtf8("Commentaire"), QString::fromUtf8("Commentaire") , false, 20 );
    tableWpt->m_colsDisposition.setColBase(12, COL_WPT_DESC        , QString::fromUtf8("Description"), QString::fromUtf8("Description") , true, 20 );
    tableWpt->m_colsDisposition.setColBase(13, COL_WPT_SRC         , QString::fromUtf8("Modèle"), QString::fromUtf8("Le modèle et le fabricant de l'appareil à l'origine de la géolocalisation du point") , false, 20 );
    tableWpt->m_colsDisposition.setColBase(14, COL_WPT_FIX         , QString::fromUtf8("Type"), QString::fromUtf8("Le type de GPS ayant permis l'obtention du point (fix)  : none'|'2d'|'3d'|'dgps'|'pps") , false, 20 );
    tableWpt->m_colsDisposition.setColBase(15, COL_WPT_SAT         , QString::fromUtf8("Satellites"), QString::fromUtf8("Le nombre de satellites acquis lors du calcul des coordonnées") , false, 20 );
    tableWpt->m_colsDisposition.setColBase(16, COL_WPT_DGPSID      , QString::fromUtf8("Dgps"), QString::fromUtf8("Des informations sur les mesures DGPS comme la durée depuis la dernière mise à jour (<ageofdgpsdata>) ou le l'identifiant de la station") , false, 20 );
    tableWpt->m_colsDisposition.setColBase(17, COL_WPT_LINKS       , QString::fromUtf8("www"), QString::fromUtf8("Liens internet d'informations sur le way point") , false, 20 );
    tableWpt->m_colsDisposition.setColBase(18, COL_WPT_SYM         , QString::fromUtf8("Icône"), QString::fromUtf8("Le nom exact du symbole illustrant le point sur l'appareil GPS (fichier icone)") , false, 20 );
    tableWpt->m_colsDisposition.setColBase(19, COL_WPT_TYPE        , QString::fromUtf8("Catégorie"), QString::fromUtf8("Le type de point (catégorie)") , false, 20 );
    tableWpt->m_colsDisposition.setColBase(20, COL_WPT_HDOP        , QString::fromUtf8("Hdop"), QString::fromUtf8("Précision (<hdop>, <vdhop>, <pdop> voir Geometric_dilution_of_precision)") , false, 20 );
    tableWpt->m_colsDisposition.setColBase(21, COL_WPT_VDOP        , QString::fromUtf8("Vdop"), QString::fromUtf8("Précision (<hdop>, <vdhop>, <pdop> voir Geometric_dilution_of_precision)") , false, 20 );
    tableWpt->m_colsDisposition.setColBase(22, COL_WPT_PDOP        , QString::fromUtf8("Pdop"), QString::fromUtf8("Précision (<hdop>, <vdhop>, <pdop> voir Geometric_dilution_of_precision)") , false, 20 );
    tableWpt->m_colsDisposition.setColBase(23, COL_WPT_EXTENTIONS  , QString::fromUtf8("Extentions"), QString::fromUtf8("Optionnellement : des extensions d'un autre espace de noms") , false, 20 );
    tableWpt->showWpt();

}
*/
void mainWindow::readSettings( QString iniFileName, QString cacheDirName )
{
    //QString appExeName = QFileInfo( QCoreApplication::applicationFilePath() ).fileName();
    //appSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope ,appExeName, "pref");

    appSettings = new QSettings( iniFileName, QSettings::IniFormat);
    debugp( "appSettings filename = " + appSettings->fileName() );

    QFile file( appSettings->fileName() );

    //debugModeRun = appSettings->value("debugModeRun").toBool();

    dateTimeFormat = appSettings->value("dateTimeFormat").toString();

    confirmQuit = appSettings->value("confirmQuit").toBool();

    QPoint pos = appSettings->value("pos", QPoint(200, 200)).toPoint();
    QSize size = appSettings->value("size", QSize(980, 600)).toSize();

    mainSpliterWidget->restoreState(appSettings->value("splitterState").toByteArray());
    importDefaultFileName = appSettings->value("importDefaultFileName").toString();

    // map cache
    slippyMapCacheFilePath = cacheDirName ; //appSettings->value("mapCacheFilePath").toString();

    iconSymbolName = appSettings->value("iconSymbolName", "number").toString();
    projectData.m_iconSymbolRootDir = iconSymbolRootDir + iconSymbolName;
    markers->setIconDir( iconSymbolRootDir + iconSymbolName);

    slippyMapIndex = appSettings->value("slippyMapIndex" ).toInt();
    slippyMapZoom = appSettings->value("slippyMapZoom" ).toInt();
    slippyMapLon = appSettings->value("slippyMapLon" ).toDouble();
    slippyMapLat = appSettings->value("slippyMapLat" ).toDouble();
    slippyMapCurX = appSettings->value("slippyMapcurX" ).toDouble();
    slippyMapCurY = appSettings->value("slippyMapcurY" ).toDouble();
    defaultOpenPath = appSettings->value("defaultOpenPath").toString();
    defaultImportPath = appSettings->value("defaultImportPath").toString();
    defaultExportPath = appSettings->value("defaultExportPath").toString();

    mapDefIndex = appSettings->value("mapDefIndex" ).toInt();
    mapDefZoom = appSettings->value("mapDefZoom").toInt();
    mapDefLon = appSettings->value("mapDefLon").toDouble();
    mapDefLat = appSettings->value("mapDefLat").toDouble();
    mapDefCurX = appSettings->value("mapDefCurX").toDouble();
    mapDefCurY = appSettings->value("mapDefCurY").toDouble();

    mapZoomToUpper = appSettings->value("mapZoomToUpper").toInt();
    mapZoomToLower = appSettings->value("mapZoomToLower").toInt();

    int mapSrcCount = appSettings->value("mapSrcCount").toInt();
    int i;
    QString mapName;
    int mapMinZoom;
    int mapMaxZoom;
    QString mapLicence;
    QString mapUSerAgent;
    QString mapReferer;
    QString mapPath;
    int keyShortCut;
    int keyZoom;

    for (i = 0; i < mapSrcCount; i++)
    {
        mapName = appSettings->value(QString("mapName_%1").arg(i)).toString();
        mapMinZoom = appSettings->value(QString("mapMinZoom_%1").arg(i)).toInt();
        mapMaxZoom = appSettings->value(QString("mapMaxZoom_%1").arg(i)).toInt();
        mapLicence = appSettings->value(QString("mapLicence_%1").arg(i)).toString();
        mapUSerAgent = appSettings->value(QString("mapUSerAgent_%1").arg(i)).toString();
        mapReferer = appSettings->value(QString("mapReferer_%1").arg(i)).toString();
        mapPath = appSettings->value(QString("mapPath_%1").arg(i)).toString();
        keyShortCut = appSettings->value( QString("keyShortCut_%1").arg(i)).toInt();
        keyZoom = appSettings->value( QString("keykeyZoom_%1").arg(i)).toInt();
        mapChooseWidget.add( mapName, mapPath, mapMinZoom, mapMaxZoom, mapLicence, mapUSerAgent, mapReferer, keyShortCut, keyZoom);
        debugp("Adding map #: " + QString::number(i) + "," + mapName + " kb=" + QString::number( keyShortCut) ) ;
    }

    markers->markerFgColor = appSettings->value( QString("markerFgColor")).value<QColor>();
    markers->markerBgColor = appSettings->value( QString("markerBgColor")).value<QColor>();
    markers->markerFonfFamilly = appSettings->value( QString("markerFonfFamilly")).value<QFont>();

    markers->markerDisplayName = appSettings->value("markerDisplayName").toBool();

    scaleFontColor = appSettings->value("scaleFontColor").toString();
    scaleFontBgcolor = appSettings->value("scaleFontBgcolor").toString();
    scaleFontSize = appSettings->value("scaleFontSize").toInt();
    scaleFontFamily = appSettings->value("scaleFontFamily").toString();
    scalePixelLength  = appSettings->value("scalePixelLength").toInt();
    scaleDisplayName = appSettings->value("scaleDisplayName").toBool();

    zoomDisplayVal = appSettings->value("zoomDisplayVal").toBool();

    scaleMarginX = appSettings->value("scaleMarginX").toInt();
    scaleMarginY = appSettings->value("scaleMarginY").toInt();
    scaleLimitHeight = appSettings->value("scaleLimitHeight").toInt();
    scaleLegendX = appSettings->value("scaleLegendX").toInt();
    scaleLegendY= appSettings->value("scaleLegendY").toInt();
    scaleLocation= appSettings->value("scaleLocation").toInt();

    searchInternetUrl = appSettings->value("searchInternetUrl").toString();
    searchInternetMapUrl = appSettings->value("searchInternetMapUrl").toString();

    for (i = 0; i <   COL_WPT_COUNT ; i++)
    {
        tableWpt->m_colsDisposition.setColBase(i,
                                               appSettings->value(QString("colMeta_%1").arg(i)).toInt(),
                                               appSettings->value(QString("colName_%1").arg(i)).toString(),
                                               appSettings->value(QString("colTooltip_%1").arg(i)).toString(),
                                               appSettings->value(QString("colVisible_%1").arg(i)).toBool(),
                                               appSettings->value(QString("colLen_%1").arg(i)).toInt()
                                              );
    }

    resize(size);
    move(pos);

    if (slippyMapCacheFilePath=="")
    {
        QString appExeName = QFileInfo( QCoreApplication::applicationFilePath() ).fileName();
        slippyMapCacheFilePath = "~/.cache/" + appExeName + "/";
    }

    QDir homeDir(QDir::home());
    slippyMapCacheFilePath.replace("~", homeDir.absolutePath());
    QDir dir(slippyMapCacheFilePath);
    slippyMapCacheFilePath = dir.absolutePath();

    // Map
    mapZoomVal->font = appSettings->value("mapZoomVal_font", QFont("Arial",14)).value<QFont>(); // appSettings->value( QString("mapZoomVal_font")  );
    mapZoomVal->zoomDisplayVal = appSettings->value( QString("mapZoomVal_zoomDisplayVal")).toBool();
    mapZoomVal->bgColor = appSettings->value( QString("mapZoomVal_bgColor")  ).value<QColor>() ;
    mapZoomVal->fontColor = appSettings->value( QString("mapZoomVal_fontColor")).value<QColor>() ;

    mapZoomControl->font = mapZoomVal->font ;
    mapZoomControl->bgColor = mapZoomVal->bgColor ;
    mapZoomControl->fontColor = mapZoomVal->fontColor ;
    mapZoomControl->display = true;

    centerMark->font = appSettings->value("centerMark_font", QFont("Arial",14)).value<QFont>();
    centerMark->display = appSettings->value( QString("centerMark_display")).toBool();
    centerMark->bgColor  = appSettings->value( QString("centerMark_bgColor")  ).value<QColor>() ;
    centerMark->fontColor  = appSettings->value( QString("centerMark_fontColor")).value<QColor>() ;

    // map scale
    mapScale->display1 = appSettings->value( QString("mapScale1_display")).toBool();
    mapScale->scale1bgColor  = appSettings->value( QString("mapScale1_bgColor")  ).value<QColor>() ;
    mapScale->scale1fontColor = appSettings->value( QString("mapScale1_fontColor")).value<QColor>() ;
    mapScale->scale1fonfFamilly = appSettings->value("mapScale1_fontfamilly", QFont("Arial",14)).value<QFont>();
    mapScale->scale1MarginX = appSettings->value( QString("mapScale1_marginx") ).toInt();
    mapScale->scale1MarginY = appSettings->value( QString("mapScale1_marginy") ).toInt();
    mapScale->scale1Precision = appSettings->value( QString("mapScale1_precision") ).toInt();
    mapScale->scale1PixelLength  = appSettings->value( QString("mapScale1_len")  ).toDouble();

    mapScale->display2 = appSettings->value( QString("mapScale2_display")).toBool();
    mapScale->scale2bgColor  = appSettings->value( QString("mapScale2_bgColor")  ).value<QColor>() ;
    mapScale->scale2fontColor = appSettings->value( QString("mapScale2_fontColor")).value<QColor>() ;
    mapScale->scale2fonfFamilly = appSettings->value("mapScale2_fontfamilly", QFont("Arial",14)).value<QFont>();
    mapScale->scale2MarginX = appSettings->value( QString("mapScale2_marginx") ).toInt();
    mapScale->scale2MarginY = appSettings->value( QString("mapScale2_marginy") ).toInt();
    mapScale->scale2Precision = appSettings->value( QString("mapScale2_precision") ).toInt();
    mapScale->scale2MetersLength = appSettings->value( QString("mapScale2_len") ).toDouble();

    foundColor = appSettings->value( QString("foundColor")  ).value<QColor>() ;
    lineSelectColor= appSettings->value( QString("lineSelectColor")  ).value<QColor>() ;
    lineEditColor= appSettings->value( QString("lineEditColor")  ).value<QColor>() ;

    printMarginTop = appSettings->value( QString("printMarginTop")).toInt();
    printMarginRight = appSettings->value( QString("printMarginRight")).toInt();
    printMarginBottom = appSettings->value( QString("printMarginBottom")).toInt();
    printMarginLeft = appSettings->value( QString("printMarginLeft")).toInt();
    printMapPoiColorFg = appSettings->value( QString("printMapPoiColorFg")).value<QColor>() ;
    printMapLegendColorFg = appSettings->value( QString("printMapLegendColorFg")).value<QColor>() ;
    printListPoiColorFg = appSettings->value( QString("printListPoiColorFg")).value<QColor>() ;
    printMapPoiColorBg = appSettings->value( QString("printMapPoiColorBg")).value<QColor>() ;
    printMapLegendColorBg = appSettings->value( QString("printMapLegendColorBg")).value<QColor>() ;
    printMapLegendColorBg = QColor(Qt::white);
    printListPoiColorBg = appSettings->value( QString("printListPoiColorBg")).value<QColor>() ;
    printListPoiColorBg = QColor(Qt::white);
    printMapPoiFont = appSettings->value( QString("printMapPoiFont")).value<QFont>() ;
    printMapLegendFont = appSettings->value( QString("printMapLegendFont")).value<QFont>() ;
    printListPoiFont = appSettings->value( QString("printListPoiFont")).value<QFont>() ;
    printDebugMode = appSettings->value( QString("printDebugMode")).toBool();
    printMapPoi = appSettings->value( QString("printMapPoi")).toBool();
    printListPoi = appSettings->value( QString("printListPoi")).toBool();
    printResolution = appSettings->value( QString("printResolution") ).toInt();
    printMapBorder = appSettings->value( QString("printMapBorder") ).toBool();

    mapGotoFromClipboard = appSettings->value( QString("mapGotoFromClipboard") ).toString();
    clipSep = appSettings->value( QString("clipSep") ).toString();

    recentFiles = appSettings->value( QString("recentFiles")).value<QStringList>() ;
    recentFilesMax = appSettings->value( QString("recentFilesMax") ).toInt();

    debugp("End read setting.");
}


void mainWindow::OnMapPrint(void)
{
    QString pdfName;
    dialogPrint customPrintDialog(this);

    customPrintDialog.slippyMap = &slippyMap;
    customPrintDialog.projectData = &projectData;

    customPrintDialog.marginBottom->setText( QString::number(printMarginBottom));
    customPrintDialog.marginLeft->setText( QString::number(printMarginLeft));
    customPrintDialog.marginTop->setText( QString::number(printMarginTop));
    customPrintDialog.marginRight->setText( QString::number(printMarginRight));

    customPrintDialog.printerDpi->setText( QString::number(printResolution) );
    customPrintDialog.cbDebugMode->setChecked( printDebugMode );

    customPrintDialog.cbPagePoi->setChecked(printMapPoi);
    customPrintDialog.cbMapPoiPrint->setChecked(printListPoi);
    customPrintDialog.cbMapBorderPrint->setChecked(printMapBorder);


    customPrintDialog.mapColor = printMapPoiColorFg;
    customPrintDialog.mapBgColor = printMapPoiColorBg;
    customPrintDialog.poiMapFontTransparency->setText( QString::number(printMapPoiColorBg.alphaF()) );
    customPrintDialog.mapFonfFamilly = printMapPoiFont;

    customPrintDialog.listColor = printListPoiColorFg;
    customPrintDialog.listBgColor = printListPoiColorBg;
    customPrintDialog.listFonfFamilly = printListPoiFont;

    customPrintDialog.legendColor = printMapLegendColorFg;
    customPrintDialog.legendBgColor = printMapLegendColorBg;
    customPrintDialog.legendFonfFamilly = printMapLegendFont;

    customPrintDialog.iconFullPath = projectData.m_iconSymbolRootDir;

    debugp("----- ON MAP PRINT -----");
    debugp("Symbols dir = " + customPrintDialog.iconFullPath );

    if (currentProjectFileName == "")
    {
        pdfName = "map";
        customPrintDialog.pdfName->setText(qApp->applicationDirPath() + "/" + pdfName + ".pdf");
    }
    else
    {
        QFileInfo fi(currentProjectFileName);
        pdfName = fi.completeBaseName();
        customPrintDialog.pdfName->setText(qApp->applicationDirPath() + "/" + pdfName + ".pdf");
    }

    customPrintDialog.setScalePrintByZoom( slippyMap.getZoom());
    customPrintDialog.mapLibName = mapChooseWidget.currentText();

    if (customPrintDialog.exec() == true)
    {
        customPrintDialog.doPrint();

        if (customPrintDialog.cbOpenPdf->isChecked() && customPrintDialog.comboBoxPrinterList->currentIndex()==0)
        {
            QDesktopServices::openUrl(QUrl("file:///"+customPrintDialog.pdfName->text(), QUrl::TolerantMode));
        }

    }
}

void mainWindow::OnMapSetDefault(void)
{

    QMessageBox Msgbox(this);
    Msgbox.setText( QString::fromUtf8("Changer la carte par défaut ?"));
    Msgbox.setInformativeText( QString::fromUtf8("Confirmez pour faire de cette carte, la carte par défaut"));
    Msgbox.setStandardButtons( QMessageBox::Cancel | QMessageBox::Yes );
    Msgbox.setIconPixmap(QPixmap(iconDir + "icon_info.png"));
    Msgbox.setWindowTitle("");
    Msgbox.setDefaultButton(QMessageBox::Cancel);

    switch( Msgbox.exec() )
    {
    case QMessageBox::Yes:

        QPointF maplonlat = slippyMap.getLonLat(  );
        mapDefIndex = mapChooseWidget.currentIndex();
        mapDefZoom = slippyMap.curZoom;
        mapDefLon = maplonlat.x();
        mapDefLat = maplonlat.y();
        mapDefCurX = slippyMap.curX;
        mapDefCurY = slippyMap.curY;
    }
}


void mainWindow::OnMapCenterLatLon(void)
{
    bool ok;
    QInputDialog* inputDialog = new QInputDialog();
    inputDialog->setOptions(QInputDialog::NoButtons);

    QClipboard *clipboard = QApplication::clipboard();
    QString clipText = clipboard->text();

    QString v= clipText;
    QRegExp rx( mapGotoFromClipboard );
    debugp("regexp = " + mapGotoFromClipboard);
    QStringList coordList;
    int pos = 0;
    while ((pos = rx.indexIn(v, pos)) != -1)
    {
        coordList << rx.cap(0);
        pos += rx.matchedLength();
    }

    clipText= "";
    QString sepCoord ="";
    foreach (const QString &str, coordList)
    {
        debugp(QString(" coord = [%1] ").arg(str) );
        clipText += sepCoord + str;
        sepCoord = ", " ;
    }

    debugp("clipText=" + clipText);

    clipText = clipText.replace ("N","");
    clipText = clipText.replace ("W","-");
    clipText = clipText.replace ("E","");
    clipText = clipText.replace ("S","-");
    clipText = clipText.replace (" ","");

    QString latLonUser =  inputDialog->getText(NULL , trUtf8("Centrer la carte à ces coordonnées") ,"Longitude, Latitude", QLineEdit::Normal, clipText , &ok);

    if ( ok && !latLonUser.isEmpty())
    {
        QRegExp rx("(\\,|\\:|\\t)"); //RegEx for ' ' or ',' or '.' or ':' or '\t'
        QStringList latLonElem = latLonUser.split(rx);

        if (latLonElem.size() < 2 )
        {
            QMessageBox Msgbox;
            QString libValueScalePrint = QString ( trUtf8("Info : impossible d'extraire les coordonnées de : \"") + clipText+"\"" )  ;
            Msgbox.setText( libValueScalePrint );
            Msgbox.exec();
            return;
        }

        slippyMapLat = latLonElem[0].toDouble();
        slippyMapLon = latLonElem[1].toDouble();

        debugp( "center map to " + latLonElem[0]  + " --- " +  latLonElem[1] );

        slippyMapZoom = slippyMap.curZoom;

        QPointF lonlat = QPointF( latLonElem[1].toDouble(), latLonElem[0].toDouble() );
        slippyMap.setLonLat( lonlat , slippyMapZoom);

        QPointF maplonlat = slippyMap.getLonLat();
        debugp("After move, center is = " + QString::number( maplonlat.x())  + "," + QString::number( maplonlat.y() ));

    }
}


void mainWindow::mapMoveTo( int slippyMapIndex, int slippyMapZoom, float slippyMapLon, float slippyMapLat )
{

    debugp(" index=" + QString::number(slippyMapIndex) + ", Zoom=" + QString::number(slippyMapZoom) + ", Lon/lat=" + QString::number(slippyMapLon) + " / " + QString::number(slippyMapLat) );

    mapChooseWidget.setCurrentIndex(slippyMapIndex);

    QPointF lonlat = QPointF( slippyMapLon, slippyMapLat );

    slippyMap.setLonLat( lonlat, slippyMapZoom);
}


void mainWindow::OnMapMoveToDefault(void)
{


    debugp(  "slippyMap.curX,Y = " + QString::number( slippyMap.curX) + "," + QString::number( slippyMap.curY) );
    debugp(  "slippyMap.curZoom = " + QString::number( slippyMap.curZoom ));
    debugp(  "dmapChooseWidget.currentIndex  = " + QString::number( mapChooseWidget.currentIndex() ));
    QPointF maplonlat = slippyMap.getLonLat(  );
    debugp(  "maplonlat.x , y = " + QString::number( maplonlat.x() )+ "," + QString::number( maplonlat.y() ));


    mapMoveTo(mapDefIndex, mapDefZoom, mapDefLon, mapDefLat);
    return;

}

void mainWindow::OnHelp()
{

    QString docFile = qApp->applicationDirPath() + "/doc/index.html";
    QDesktopServices::openUrl(QUrl::fromLocalFile(docFile));
    return;
}

void mainWindow::OnAbout()
{
    if (dialogAboutDisplayed == false)
    {
        dlgAbout = new dialogAbout(this);
        dlgAbout->show();
        dialogAboutDisplayed= true;
    }
    else
        dialogAboutDisplayed = true;

    return;
}


void mainWindow::OnProjectDataSaved()
{

    debugp("Datasaved");
    projectDataStatus();

}
void mainWindow::OnProjectDataUnSaved()
{

    debugp("Data unsaved");
    if (projectData.m_wptList.size() > 0)
        projectData.dataSaved = false;
    else
        projectData.dataSaved = true;

    projectDataStatus();

}

void mainWindow::projectDataStatus()
{
    if (projectData.dataSaved == false )
    {

        setWindowTitle( QString::fromUtf8("uXplore : ")  + currentProjectFileName + QString::fromUtf8(" [ modifié ]")  + " " + QString::number(projectData.m_wptList.count()) + " balises.") ;
    }
    else
    {
        QString sepa;
        if (currentProjectFileName != "") sepa=", ";
        else sepa = "";
        setWindowTitle( QString::fromUtf8("uXplore : ")  +currentProjectFileName + sepa + QString::number(projectData.m_wptList.count()) + " balises.") ;
    }

    menuCheckEnableAction();

}


void mainWindow::OnAppQuit()
{
    close(); // Send closeEvent() signal
}


void mainWindow::closeEvent ( QCloseEvent * event )
{
    event->ignore();

    if ( projectData.dataSaved  == false)
    {

        QMessageBox Msgbox(this);
        Msgbox.setText("Vous etes sur le point de fermer l'application :");
        Msgbox.setInformativeText( QString::fromUtf8("Enregistrer le projet avant de quitter ?"));
        Msgbox.setStandardButtons( QMessageBox::No | QMessageBox::Yes | QMessageBox::Cancel);
        Msgbox.setIconPixmap(QPixmap(iconDir + "icon_saveproj.png"));
        Msgbox.setWindowTitle("");
        Msgbox.setDefaultButton(QMessageBox::Cancel);
        switch( Msgbox.exec() )
        {
        case QMessageBox::Yes:
            mainWindow::OnFileSave();
            break;

        case QMessageBox::No:
            break;

        case QMessageBox::Cancel:
            return;
            break;
        }

    }

    if (confirmQuit == true )
    {
        QMessageBox Msgbox(this);
        Msgbox.setText("Confirmez pour quitter l'application");
        Msgbox.setInformativeText( QString::fromUtf8("Quitter ?"));
        Msgbox.setStandardButtons( QMessageBox::Cancel | QMessageBox::Yes );
        Msgbox.setIconPixmap(QPixmap(iconDir + "icon_info.png"));
        Msgbox.setWindowTitle("");
        Msgbox.setDefaultButton(QMessageBox::Cancel);

        switch( Msgbox.exec() )
        {
        case QMessageBox::Yes:
            writeSettings();
            event->accept();
            break;

        case QMessageBox::Cancel:
            break;
        }

    }
    else
    {
        debugp("Exit, no confirmation");
        writeSettings();
        event->accept();
    }


};

void mainWindow::OnFileNew()
{

    if (projectData.dataSaved == false )
    {
        QMessageBox Msgbox;
        Msgbox.setText( QString::fromUtf8("Le projet actuel n'a pas été sauvegardé :" ) );
        Msgbox.setInformativeText( QString::fromUtf8("Sauvegarder maintenant ?"));
        Msgbox.setStandardButtons(QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);
        Msgbox.setButtonText(QMessageBox::Yes, trUtf8("Sauvegarder"));
        Msgbox.setButtonText(QMessageBox::No, trUtf8("Ne pas sauvegarder"));
        Msgbox.setButtonText(QMessageBox::Cancel, trUtf8("Annuler"));
        Msgbox.setDefaultButton(QMessageBox::Cancel);
        Msgbox.setIconPixmap(QPixmap(iconDir + "icon_saveproj.png"));
        Msgbox.setWindowTitle("");

        switch( Msgbox.exec() )
        {
        case QMessageBox::Yes:
            OnFileSave();
            break;

        case QMessageBox::No:
            break;
        case QMessageBox::Cancel:
            return;
            break;
        }
    }

    currentProjectFileName = "";
    bckCurrentProjectFileName  = currentProjectFileName;
    barreEtat->showMessage( QString::fromUtf8("Nouveau projet") + currentProjectFileName);
    setWindowTitle( currentProjectFileName  );

    tableWpt->hideAllMarkersOnMap();
    tableWpt->curentWptId = -1;

    slippyMap.curentWptId = -1;

    projectData.deleteAll();

    projectData.m_currentColSort = -1;
    projectData.m_currentColSortOrder = -1;

    slippyMap.update();

    wptViewAsTable();

    projectData.dataSaved = true;
    projectDataStatus();

}

void mainWindow::OnFileSave()
{
    QString fileSaveName  = currentProjectFileName;

    if (currentProjectFileName == "")
    {
        currentProjectFileName = defaultOpenPath + "/" + defaultSaveFileName;
        fileSaveName = QFileDialog::getSaveFileName(this, QString::fromUtf8("Sauver le projet"), currentProjectFileName,filters, &defaultFilter);

        if (fileSaveName == "")
        {
            currentProjectFileName = bckCurrentProjectFileName;
            return;
        }
    }

    defaultOpenPath = QFileInfo(fileSaveName).path();

    QFile::remove(fileSaveName);
    QSettings fichier_w(fileSaveName, QSettings::IniFormat);
    fichier_w.beginGroup("gppdatas");
    fichier_w.setValue("gpxdata", qVariantFromValue(projectData));
    fichier_w.endGroup();
    fichier_w.sync();

    currentProjectFileName = fileSaveName;
    barreEtat->showMessage( QString::fromUtf8("Projet sauvé : ") + currentProjectFileName);
    setWindowTitle( currentProjectFileName  );

    projectData.dataSaved = true;
    OnProjectDataSaved();

    recentFileAdd( currentProjectFileName );

    debugp( "write project done" );
}

void mainWindow::OnFileSaveAs()
{
    bckCurrentProjectFileName  = currentProjectFileName;
    currentProjectFileName = "";
    mainWindow::OnFileSave();

    projectData.dataSaved = true;
    OnProjectDataSaved();

    recentFileAdd( currentProjectFileName );

}

void mainWindow::OnFileOpen()
{

    if (projectData.dataSaved == false )
    {
        QMessageBox Msgbox;
        Msgbox.setText("Sauvegarder ?");
        Msgbox.setInformativeText( QString::fromUtf8("Le projet actuel n'a pas été sauvegardé.\n\nSauvegarder maintenant ?"));
        Msgbox.setStandardButtons( QMessageBox::No | QMessageBox::Yes );
        Msgbox.setIconPixmap(QPixmap(iconDir + "icon_saveproj.png"));
        Msgbox.setWindowTitle("");
        Msgbox.setDefaultButton(QMessageBox::Cancel);

        switch( Msgbox.exec() )
        {
        case QMessageBox::Yes:
            OnFileSave();
            break;

        case QMessageBox::No:
            break;
        }
    }

    QString fileOpenName = QFileDialog::getOpenFileName(this, QString::fromUtf8("Charger un projet"), defaultOpenPath, filters, &defaultFilter);
    if (fileOpenName == "")
    {
        return;
    }


    doOpenProject( fileOpenName);

}
bool mainWindow::doOpenProject( QString fileOpenName)
{

    defaultOpenPath = QFileInfo(fileOpenName).path();

    QFile f(fileOpenName);
    if (!f.open(QFile::ReadOnly | QFile::Text))
    {
        debugp("Err : file do not exists! : " + fileOpenName);
        QMessageBox::critical(this, "Fichier",  QString::fromUtf8("Load : Erreur , le fichier n'existe pas : " ) +fileOpenName );
        return false;
    }
    f.close();

    this->setCursor(Qt::WaitCursor);

    QSettings fichier_r(fileOpenName, QSettings::IniFormat);
    fichier_r.beginGroup("gppdatas");

    QVariant variantVal = qVariantFromValue( fichier_r.value("gpxdata") ) ;

    if (variantVal.canConvert<projectDatas>() == false )
    {
        debugp( "Err : CANT CONVERT" );
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::critical(this, "Fichier",  QString::fromUtf8("Load : Erreur de format  (canConvert() false) : " ) +fileOpenName );
        return false;
    }
    if (variantVal.isValid() == false )
    {
        debugp ("Err : NOT VALID");
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::critical(this, "Fichier",  QString::fromUtf8("Load : Erreur de format (isValid() false) : " ) +fileOpenName);
        return false;
    }

    projectData = variantVal.value<projectDatas>();

    fichier_r.endGroup();

    projectData.m_iconSymbolRootDir = iconSymbolRootDir + iconSymbolName;

    markers->clearAllMarkers();
    wptViewAsTable();

    currentProjectFileName = fileOpenName;
    bckCurrentProjectFileName  = currentProjectFileName;
    barreEtat->showMessage( QString::fromUtf8("Projet chargé : ") + currentProjectFileName);

    projectData.dataSaved = true;

    projectDataStatus();

    this->setCursor(Qt::ArrowCursor);

    recentFileAdd( currentProjectFileName );

    return true;

}
void mainWindow::recentFileAdd( QString filePath)
{
    if (filePath=="") return;

    // Clear menu
    recentFileClear();

    // Déjà dans la liste ?
    int indexRecent = 0;
    for (QStringList::iterator it = recentFiles.begin();
            it != recentFiles.end(); ++it)
    {
        QString current = *it;

        if( current == filePath)
        {
            recentFiles.removeAt(indexRecent);
            debugp("retirer de liste @" + QString::number(indexRecent));
            break;
        }
        indexRecent++;
    }

    recentFiles.insert(0, filePath);

    // Overflow ?
    if (recentFiles.count() > recentFilesMax )
    {
        recentFiles.removeLast();

    }

    recentFileCreateMenu();
}
void mainWindow::recentFileClear()
{

    foreach (QAction *action, recentFilesMenu->actions())
    {
        if (action->isSeparator())
        {
            debugp("this action is a separator");
        }
        else if (action->menu())
        {
            debugp("remove action: " + action->text());
            recentFilesMenu->removeAction(action);
        }
        else
        {
            //debugp("action: "  +  qUtf8Printable(action->text()));
            debugp("remove action: "  +  action->text());
            recentFilesMenu->removeAction(action);
        }
    }


    //recentFileCreateMenu();
}
void mainWindow::recentFileCreateMenu(void)
{
    // Re-create menu
    QActionGroup* recentActionGroup = new QActionGroup(recentFilesMenu);
    recentActionGroup->setExclusive(false);

    for (QStringList::iterator it = recentFiles.begin();
            it != recentFiles.end(); ++it)
    {
        QString current = *it;
        debugp( current );
        /*
        currentAction->setIcon(QIcon(iconDir + "tool_saveas.png"));
        */
        QAction *actionMenuItem  = recentFilesMenu->addAction( current  );
        actionMenuItem->setData( current );
        recentActionGroup->addAction( actionMenuItem );
    }
    connect(recentActionGroup, SIGNAL(triggered(QAction*)), SLOT(onRecentFile(QAction*)));
}

void mainWindow::onRecentFile(QAction *action)
{
    bool fileOpened;
    QString menuData = action->data().toString();
    debugp( "action data = " + menuData );

    if (projectData.dataSaved == false )
    {
        QMessageBox Msgbox;
        Msgbox.setText( QString::fromUtf8("Le projet actuel n'a pas été sauvegardé :" ) );
        Msgbox.setInformativeText( QString::fromUtf8("Sauvegarder maintenant ?"));
        Msgbox.setStandardButtons(QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);
        Msgbox.setButtonText(QMessageBox::Yes, trUtf8("Sauvegarder"));
        Msgbox.setButtonText(QMessageBox::No, trUtf8("Ne pas sauvegarder"));
        Msgbox.setButtonText(QMessageBox::Cancel, trUtf8("Annuler"));
        Msgbox.setDefaultButton(QMessageBox::Cancel);
        Msgbox.setIconPixmap(QPixmap(iconDir + "icon_saveproj.png"));
        Msgbox.setWindowTitle("");

        switch( Msgbox.exec() )
        {
        case QMessageBox::Yes:
            OnFileSave();
            debugp( "on file save done");
            if (projectData.dataSaved == true) fileOpened = doimportImportfile( menuData);
            break;

        case QMessageBox::No:
            debugp( "do import");
            fileOpened = doimportImportfile( menuData);
            break;
        case QMessageBox::Cancel:
            return;
            break;
        }
    }
    else
    {
        fileOpened = doimportImportfile( menuData);
    }

    if (fileOpened == false )
    {
        debugp("ERR open file");
        QMessageBox::critical(this, "Erreur d'ouverture du fichier",  QString::fromUtf8("Erreur à l'ouverture de : \n" ) +menuData );
    }


}

void mainWindow::OnFileImport()
{
    QString defImportFileName;
    bool appendData;

    appendData = true;

    QString importFileName = QFileDialog::getOpenFileName(this, QString::fromUtf8("Importer les données"), defaultImportPath, importFilters, &importDefaultFilter);
    if ( importFileName  == "")
    {
        return;
    }

    defaultImportPath = QFileInfo(importFileName).path();
    QFileInfo fi(importFileName);
    importDefaultFileName =  fi.baseName();

    this->setCursor(Qt::WaitCursor);

    if (appendData == false)
    {
        tableWpt->hideAllMarkersOnMap();
        projectData.deleteAll();
        projectData.m_lastWptId = 0;
    }
    else
    {
        projectData.m_lastWptId = projectData.getMaxId() ;
    }

    doimportImportfile(importFileName);


}
bool mainWindow::doimportImportfile( QString importFileName )
{
    int fileFormat;
    bool isImported ;

    isImported= false;
    fileFormat = -2;
    projectData.importedCount = 0;

    this->setCursor(Qt::WaitCursor);

    debugp("importDefaultFilter=" + importDefaultFilter);


    fileFormat = projectData.guessFileFormat( importFileName );

    if (fileFormat == FILEFORMAT_GPP ) importDefaultFilter = QString::fromUtf8("Project gpp (*.gpp)");
    if (fileFormat == FILEFORMAT_GPX ) importDefaultFilter = QString::fromUtf8("Fichier Gpx (*.gpx)");
    if (fileFormat == FILEFORMAT_CSV ) importDefaultFilter = QString::fromUtf8("Fichier Csv (*.csv)");
    if (fileFormat == FILEFORMAT_KML ) importDefaultFilter = QString::fromUtf8("Fichier Kml (*.kml)");
    if (fileFormat == FILEFORMAT_KMZ ) importDefaultFilter = QString::fromUtf8("Fichier Kmz (*.kmz)");

    if (importDefaultFilter == QString::fromUtf8("Project gpp (*.gpp)") )
    {

        bool projectIsOpened = doOpenProject(importFileName);
        this->setCursor(Qt::ArrowCursor);
        return projectIsOpened;
    }

    if (importDefaultFilter == QString::fromUtf8("Fichier Gpx (*.gpx)") )
    {
        isImported = projectData.importFromFile( IMPORT_FORMAT_GPX, importFileName );
    }
    else if (importDefaultFilter == QString::fromUtf8("Fichier Kml (*.kml)") )
    {
        isImported = projectData.importFromFile( IMPORT_FORMAT_KLM, importFileName );
    }
    else if (importDefaultFilter == QString::fromUtf8("Fichier Kmz (*.kmz)") )
    {
        isImported = projectData.importFromFile( IMPORT_FORMAT_KMZ, importFileName );
    }
    else if (importDefaultFilter == QString::fromUtf8("Fichier Csv (*.csv)") )
    {
        csvParam csvParamImportImport;

        csvParamImportImport.fistLineIsHeader = false;
        csvParamImportImport.csvSepFieldValue = ";|,|\\t";
        csvParamImportImport.csvSepTextValue= "\"";
        for (int j= 0 ; j < COL_WPT_COUNT ; j++ )
        {
            csvParamImportImport.columns.m_colBase[j].id =  tableWpt->m_colsDisposition.getWptColId(j);
            csvParamImportImport.columns.m_colBase[j].name =  tableWpt->m_colsDisposition.getWptColName(j);
            csvParamImportImport.columns.m_colBase[j].format =  "%1";
            if (csvParamImportImport.columns.m_colBase[j].id == COL_WPT_TIME)
                csvParamImportImport.columns.m_colBase[j].format =  dateTimeFormat;

            csvParamImportImport.columns.m_colBase[j].len =  tableWpt->m_colsDisposition.getWptColLen(j);
            csvParamImportImport.columns.m_colBase[j].tooltip =  tableWpt->m_colsDisposition.getWptColTooltip(j);
            csvParamImportImport.columns.m_colBase[j].colvisible =  tableWpt->m_colsDisposition.getWptColVisibility(j);
        }

        bool result;

        dialogCsvImport dlgCsvImport(this, CSV_MODE_IMPORT);
        dlgCsvImport.maxSampleDataRow = -1;
        dlgCsvImport.fileName=importFileName;
        dlgCsvImport.initCsvParam (&csvParamImportImport);

        debugp("about to import : " + importFileName);
        result = dlgCsvImport.exec();
        if(result == true)
        {
            isImported = projectData.importFromFile( IMPORT_FORMAT_CSV, importFileName , &dlgCsvImport.contextMenuLibListe,  csvParamImportImport.fistLineIsHeader,csvParamImportImport.csvSepFieldValue, csvParamImportImport.csvSepTextValue );
        }

    }
    else
    {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::information(this, "Importation",  QString::fromUtf8("Erreur : Format de fichier non reconnu. file format=: " ) + QString::number(fileFormat));
    }

    if (isImported == true)
    {
        this->setCursor(Qt::ArrowCursor);
        projectData.m_iconSymbolRootDir = iconSymbolRootDir + iconSymbolName;
        wptViewAsTable();
        OnProjectDataUnSaved();
        barreEtat->showMessage( QString::fromUtf8("Données importées : ") + importFileName + QString::fromUtf8(". Nombre de points importés : ") + QString::number(projectData.importedCount));
        QMessageBox::information(this, "Importation",  QString::fromUtf8("Données importées de : \n" ) +importFileName + "\n\n" + QString::fromUtf8("Nombre de points importés : ") + QString::number(projectData.importedCount));
    }
    else
    {
        this->setCursor(Qt::ArrowCursor);
        if (projectData.lastErrorText != "")
        {
            barreEtat->showMessage( QString::fromUtf8("Erreur : données non importées : ") + importFileName + " , " + projectData.lastErrorText + QString::fromUtf8(". Nombre de points importés : ") + QString::number(projectData.importedCount)) ;
            QMessageBox::critical(this, "Importation",  QString::fromUtf8("Erreur dans l'importation de : \n" ) +importFileName + "\n\n" + projectData.lastErrorText + "\n\n" + QString::fromUtf8("Nombre de points importés : ") + QString::number(projectData.importedCount));
        }
    }

    recentFileAdd( importFileName );

    return isImported;

}
void mainWindow::OnFileExport()
{
    QString defExportFileName;

    QFileDialog exportDialog(this);

    debugp("defaultExportPath= " + defaultExportPath)

    QFileInfo fileName(defaultExportPath);
    debugp("fileName.baseName= " + fileName.baseName());

    if (fileName.isFile()==false)
    {
        if (currentProjectFileName == "")
            defaultExportPath = defaultExportPath + "/export.gpx";
        else
        {

            QFileInfo fileNameExport(currentProjectFileName);
            defaultExportPath = defaultExportPath + "/" +  fileNameExport.baseName() + ".gpx";
        }

    }

    QString exportFileName = exportDialog.getSaveFileName(this, QString::fromUtf8("Exporter les données"), defaultExportPath, exportFilters, &exportDefaultFilter);
    if ( exportFileName  == "")
    {
        return;
    }

    defaultExportPath = QFileInfo(exportFileName).path();
    QFileInfo fi(exportFileName);
    exportDefaultFileName =  fi.baseName();

    debugp( "fi.suffix() =" + fi.suffix() );
    debugp( "exportDefaultFilter = " + exportDefaultFilter );

    bool isExported ;
    this->setCursor(Qt::WaitCursor);

    isExported = false;
    if (exportDefaultFilter == QString::fromUtf8("Données Gpx (*.gpx)") )
    {
        debugp( "Exportation en cours : GPX vers : " + exportFileName );
        projectData.exportToFile(EXPORT_FORMAT_GPX, exportFileName );
        isExported = true;

    }
    if (exportDefaultFilter ==  "Fichier Csv (*.csv)" || exportDefaultFilter == "Tous (*.*)")
    {
        debugp( "Exportation en cours : CSV vers : " + exportFileName );

        csvParam csvParamImportExport;  // Csv parmeters
        dialogCsvExport dlgCsvExport(this, CSV_MODE_EXPORT); // Csv dialog

        csvParamImportExport.fistLineIsHeader = true;
        csvParamImportExport.csvSepFieldValue = ";|,|\\t";
        csvParamImportExport.csvSepTextValue= "\"";
        for (int j= 0 ; j < COL_WPT_COUNT ; j++ )
        {
            csvParamImportExport.columns.m_colBase[j].id =  tableWpt->m_colsDisposition.getWptColId(j);
            csvParamImportExport.columns.m_colBase[j].name =  tableWpt->m_colsDisposition.getWptColName(j);
            csvParamImportExport.columns.m_colBase[j].format =  "%1";
            if (csvParamImportExport.columns.m_colBase[j].id == COL_WPT_TIME)
                csvParamImportExport.columns.m_colBase[j].format =  dateTimeFormat;

            csvParamImportExport.columns.m_colBase[j].len =  tableWpt->m_colsDisposition.getWptColLen(j);
            csvParamImportExport.columns.m_colBase[j].tooltip =  tableWpt->m_colsDisposition.getWptColTooltip(j);
            csvParamImportExport.columns.m_colBase[j].colvisible =  tableWpt->m_colsDisposition.getWptColVisibility(j);

        }

        dlgCsvExport.maxSampleDataRow = -1;
        csvParamImportExport.csvSepFieldValue = ";";
        dlgCsvExport.fileName = exportFileName;
        dlgCsvExport.initCsvParam (&csvParamImportExport);

        bool result = dlgCsvExport.exec();
        if(result == true)
        {
            isExported = projectData.exportToFile(EXPORT_FORMAT_CSV, exportFileName , &dlgCsvExport.contextMenuLibListe,  csvParamImportExport.fistLineIsHeader,csvParamImportExport.csvSepFieldValue, csvParamImportExport.csvSepTextValue );
        }

    }

    this->setCursor(Qt::ArrowCursor);
    if (isExported == true)
    {
        barreEtat->showMessage( QString::fromUtf8("Données exportées : ") + exportFileName);
        QMessageBox::information(this, "Exportation",  QString::fromUtf8("Données exportées vers : " ) +exportFileName );
    }
    else
    {
        if (projectData.lastErrorText != "")
        {
            barreEtat->showMessage( QString::fromUtf8("Erreur : données non exportées : ") + exportFileName);
            QMessageBox::critical(this, "Exportation",  QString::fromUtf8("Erreur dans l'exportation vers : " ) +exportFileName );
        }
    }

    recentFileAdd( exportFileName );

}
void mainWindow::OnPref()
{
    dialogAppPref dlg(this);
    dlg.exec();

}

void mainWindow::OnPrefDef()
{

    return;





// DEBUG
    projectData.afficher_metadata();
    return;


    QString diffe;
    for (int j= 0 ; j < COL_WPT_COUNT ; j++ )
    {
        if (tableWpt->horizontalHeaderItem(j)->data(Qt::DisplayRole).toString() != tableWpt->m_colsDisposition.getWptColName(j) )
            diffe="X";
        else diffe=" ";

        debugp( diffe + "--onSectionMoved # " + QString::number( j ) + " :" + tableWpt->horizontalHeaderItem(j)->data(Qt::DisplayRole).toString() );

        QTableWidgetItem* headerItem = tableWpt->horizontalHeaderItem(j);
        if (headerItem)
        {
            debugp( "#" + QString::number( j ) + "  - " + headerItem->text() );

        }
    }

}

void mainWindow::OnProjProperty()
{
    dialogProjProperty dlg;
    dlg.mainParentForm = this;
    dlg.exec();

}

// Enable / disable menu actions
void mainWindow::menuCheckEnableAction(void)
{
    if (projectData.dataSaved == true )
    {
        actionSave->setEnabled(false);

    }
    else
    {
        actionSave->setEnabled(true);
    }
}

void mainWindow::OnEditSelectInvert(void)
{
    tableWpt->inverseAllMarkersOnMap();
}

void mainWindow::OnEditUnSelectAll(void)
{
    this->setCursor(Qt::WaitCursor);
    tableWpt->hideAllMarkersOnMap();
    this->setCursor(Qt::ArrowCursor);

}

void mainWindow::OnEditSelectAll(void)
{
    this->setCursor(Qt::WaitCursor);
    tableWpt->showAllMarkersOnMap();
    this->setCursor(Qt::ArrowCursor);
}
void mainWindow::OnEditSelectByMap( void )
{
    this->setCursor(Qt::WaitCursor);
    tableWpt->showAllMarkersByMap( true );
    this->setCursor(Qt::ArrowCursor);
}
void mainWindow::OnEditUnSelectByMap( void )
{
    this->setCursor(Qt::WaitCursor);
    tableWpt->showAllMarkersByMap( false );
    this->setCursor(Qt::ArrowCursor);

}
void mainWindow::OnEditshowMapBySelect(void)
{

    if (tableWpt->getSelectedCount())
    {

        QPointF lonlat;
        QPointF maplonlat;
        QPointF mapMinlonlat;
        QPointF mapMaxlonlat;

        double areaLeft;
        double areaRight;
        double areaTop;
        double areaBottom;

        this->setCursor(Qt::WaitCursor);

        tableWpt->computeMinMaxCoo( );
        debugp( "0 Selected poi's min lon/lat x,y= " + QString::number(tableWpt->minCoo.x()) + "/" + QString::number(tableWpt->minCoo.y()) );
        debugp( "0 Selected poi's max lon/lat x,y = " + QString::number(tableWpt->maxCoo.x()) + "/" + QString::number(tableWpt->maxCoo.y()) );

        areaLeft = tableWpt->minCoo.x();
        areaTop = tableWpt->minCoo.y();
        areaRight = tableWpt->maxCoo.x();
        areaBottom = tableWpt->maxCoo.y();

        int currentZoom = slippyMap.curZoom;
        int testZoom=19;

        // 1/2 center mapo
        lonlat = QPointF( (areaLeft+areaRight ) /2 , (areaTop+areaBottom ) /2  );
        slippyMap.setLonLat( lonlat , currentZoom );

        // 2/2 : zoom
        while (testZoom > 1)
        {
            slippyMap.setLonLat( lonlat , testZoom);
            maplonlat = slippyMap.getLonLat();
            mapMinlonlat = slippyMap.getMinLonLat();
            mapMaxlonlat = slippyMap.getMaxLonLat();
            debugp( "--- test Zoom = " + QString::number(slippyMap.curZoom ));

            QRectF mapArea = slippyMap.getArea();
            debugp( "map min lon/lat x,y= " + QString::number(mapArea.left()) + "/" + QString::number(mapArea.top()) );
            debugp( "map max lon/lat x,y = " + QString::number(mapArea.right()) + "/" + QString::number(mapArea.bottom()) );

            debugp( "Map min lon, lat x,y = " + QString::number( mapMinlonlat.x())  + "," + QString::number( mapMinlonlat.y() )  );
            debugp( "Map max lon, lat x,y = " + QString::number( mapMaxlonlat.x())  + "," + QString::number( mapMaxlonlat.y() )  );

            debugp ( QString::number( mapMinlonlat.x()) + " < " + QString::number( areaLeft ) ) ;
            debugp ( QString::number( mapMinlonlat.y()) + " > " + QString::number( areaTop ) ) ;
            debugp ( QString::number( mapMaxlonlat.x()) + " > " + QString::number( areaRight ) ) ;
            debugp ( QString::number( mapMaxlonlat.y()) + " < " + QString::number( areaBottom) ) ;

            if (mapMinlonlat.x() < areaLeft)
            {
                debugp ("1");
            }
            if (mapMinlonlat.y() > areaTop )
            {
                debugp ("2");
            }
            if (mapMaxlonlat.x() > areaRight )
            {
                debugp ("3");
            }
            if (mapMaxlonlat.y() < areaBottom )
            {
                debugp ("4");
            }

            if (
                mapMinlonlat.x() < areaLeft &&
                mapMinlonlat.y() > areaTop &&
                mapMaxlonlat.x() > areaRight &&
                mapMaxlonlat.y() < areaBottom
            )
            {
                //if (testZoom>0) testZoom--;
                debugp("Found zoom : " + QString::number( testZoom ));
                break;
            }

            testZoom--;
        }

        debugp("set zoom to = " + QString::number(testZoom));
        slippyMap.setLonLat( lonlat , testZoom);
        slippyMap.update();

        maplonlat = slippyMap.getLonLat();
        mapMinlonlat = slippyMap.getMinLonLat();
        mapMaxlonlat = slippyMap.getMaxLonLat();
        debugp( "After map move, center is = " + QString::number( maplonlat.x())  + "," + QString::number( maplonlat.y() ));
        debugp( "After map move, min = " + QString::number( mapMinlonlat.x())  + "," + QString::number( mapMinlonlat.y() ));
        debugp( "After map move, max = " + QString::number( mapMaxlonlat.x())  + "," + QString::number( mapMaxlonlat.y() ));
        debugp( "Zoom = " + QString::number(slippyMap.curZoom ));

        this->setCursor(Qt::ArrowCursor);
    }
    else
    {
        debugp("Aucune balise selectionnée.");

        QMessageBox::information( this, "Information",  QString("Le nombre de balises selectionnées est invalide : %1\n\nSelectionnez au moins une balise.").arg(0) , QMessageBox::Ok, 0 );
    }

}


void mainWindow::wptViewAsTable()
{

    tableWpt->setFormatGpx( &projectData );
    tableWpt->showWpt();
    tableWpt->refreshMarkers();
    slippyMap.m_projectDatas = &projectData;
}

void mainWindow::OnWptRemoveSelected(void)
{

    QMessageBox Msgbox(this);
    Msgbox.setText( QString::fromUtf8("Supprimer  ?"));
    Msgbox.setInformativeText( QString::fromUtf8("Confirmez pour Supprimer les points sélectionnés"));
    Msgbox.setStandardButtons( QMessageBox::Cancel | QMessageBox::Yes );
    Msgbox.setIconPixmap(QPixmap(iconDir + "icon_info.png"));
    Msgbox.setWindowTitle("");
    Msgbox.setDefaultButton(QMessageBox::Cancel);

    switch( Msgbox.exec() )
    {
    case QMessageBox::Yes:

        this->setCursor(Qt::WaitCursor);

        tableWpt->removeSelected();
        tableWpt->showWpt();

        //projectData.dataSaved = false;
        //projectDataStatus();
        OnProjectDataUnSaved();

        this->setCursor(Qt::ArrowCursor);
    }
}

void mainWindow::OnWptDisplayLabel(void)
{

    if (markers->markerDisplayName == true)
    {
        markers->markerDisplayName = false;

    }
    else
    {
        markers->markerDisplayName = true;

    }

    tableWpt->refreshMarkers();

}
void mainWindow::setWptDisplayLabel(bool displayMode )
{
    actionWptDisplayLabel->setChecked(displayMode);
    markers->markerDisplayName = displayMode;
    //markers->markerDisplayName = displayMode;
    tableWpt->refreshMarkers();

}
void mainWindow::setMarkerFont()
{

    tableWpt->refreshMarkers();

}
void mainWindow::OnSearchDuplicate(void)
{

    if (dialogSearchDupDisplayed == false)
    {
        dlgSearchDup = new dialogSearchDup(this);
        dlgSearchDup->setFormatGpx( &projectData);
        dlgSearchDup->setWptEdit( tableWpt );
        dlgSearchDup->show();
        dialogSearchDupDisplayed = true;
    }
    else {
        dialogSearchDupDisplayed = true;
        debugp( "dialogSearchDupDisplayed false ");
    }
}


void mainWindow::OnWptSearch(void)
{

    if (dialogFindDisplayed == false)
    {
        dlgFind = new dialogFind(this);
        dlgFind->setFormatGpx( &projectData );
        dlgFind->setWptEdit( tableWpt );
        dlgFind->show();
        dialogFindDisplayed = true;
    }
    else
        dialogFindDisplayed = true;

}
void mainWindow::OnWptReplace(void)
{

    if (dialogReplaceDisplayed == false)
    {
        dlgReplace = new dialogReplace(this);
        dlgReplace->setFormatGpx( &projectData );
        dlgReplace->setWptEdit( tableWpt );
        dlgReplace->show();
        dialogReplaceDisplayed = true;
    }
    else
        dialogReplaceDisplayed = true;

}

//-----------------------------------------------------------

#include "mainWindow.moc"





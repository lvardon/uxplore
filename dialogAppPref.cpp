#include "global.h"
#include "dialogAppPref.h"
#include "mainWindow.h"

#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QTableWidget>


// Lignes du tableau , onglet legende carte (offset, ordre)
#define MAPLEGEND_ZOOMCOLOR         0
#define MAPLEGEND_ZOOMDISPLAY       1
#define MAPLEGEND_ZOOMCTRLDISPLAY   2
#define MAPLEGEND_CROSSCOLOR        3
#define MAPLEGEND_CROSSSIZE         4
#define MAPLEGEND_CROSSDISPLAY      5
#define MAPLEGEND_SCALEFIXCOLOR     6
#define MAPLEGEND_SCALEFIXDISPLAY   7
#define MAPLEGEND_SCALEFIXLEN       8
#define MAPLEGEND_SCALEFIXPREC      9
#define MAPLEGEND_SCALEFIXOFFSETX   10
#define MAPLEGEND_SCALEFIXOFFSETY   11
#define MAPLEGEND_SCALEVARCOLOR     12
#define MAPLEGEND_SCALEVARDISPLAY   13
#define MAPLEGEND_SCALEVARLEN       14
#define MAPLEGEND_SCALEVARPREC      15
#define MAPLEGEND_SCALEVAROFFSETX   16
#define MAPLEGEND_SCALEVAROFFSETY   17

// Lignes du tableau imprimante
#define PRINTER_MAPPOICOLOR   0
#define PRINTER_LISTPOICOLOR   1
#define PRINTER_MAPLEGENDCOLOR   2
#define PRINTER_RESOLUTION  3
#define PRINTER_MODEDEBUG  4
#define PRINTER_MARGIN_TOP  5
#define PRINTER_MARGIN_RIGHT  6
#define PRINTER_MARGIN_BOTTOM  7
#define PRINTER_MARGIN_LEFT  8
#define PRINTER_BORDER  9
#define PRINTER_POIMAP  10
#define PRINTER_POILIST  11


dialogAppPref::dialogAppPref(mainWindow *parent) : QDialog(parent)
{
    resize(1150, 600); // W x H

    m_parent = parent;

    QVBoxLayout *layout = new QVBoxLayout();

    QPushButton *button;
    QDialogButtonBox *box = new QDialogButtonBox( Qt::Horizontal );

    button = new QPushButton( "Ok" );
    connect( button, SIGNAL(clicked()), this, SLOT(okClicked()) );
    box->addButton( button, QDialogButtonBox::AcceptRole );

    button = new QPushButton( "Annuler" );
    connect( button, SIGNAL(clicked()), this, SLOT(cancelClicked()) );
    box->addButton( button, QDialogButtonBox::RejectRole );

    QTabWidget *ongletsWidget = new QTabWidget(this);

    QWidget *pageGeneral = new QWidget;
    QWidget *pageMapDef = new QWidget;
    QWidget *pageMapSrc = new QWidget;
    QWidget *pageMapCache = new QWidget;
    QWidget *pageWptColdispo = new QWidget;
    QWidget *pageWebSearch = new QWidget;
    QWidget *pageMarker = new QWidget;
    QWidget *pageMapLegend = new QWidget;
    QWidget *pageLegend = new QWidget;

    ongletsWidget->addTab(pageMapDef, QString::fromUtf8("Carte"));
    ongletsWidget->addTab(pageGeneral, QString::fromUtf8("Options"));
    ongletsWidget->addTab(pageMapSrc, QString::fromUtf8("Source des cartes"));
    ongletsWidget->addTab(pageMapCache, QString::fromUtf8("Cache des cartes"));
    ongletsWidget->addTab(pageWptColdispo, QString::fromUtf8("Onglet balises"));
    ongletsWidget->addTab(pageWebSearch, QString::fromUtf8("Liens Web"));
    ongletsWidget->addTab(pageMarker, QString::fromUtf8("Balises"));
    ongletsWidget->addTab(pageMapLegend, QString::fromUtf8("Légende"));
    ongletsWidget->addTab(pageLegend, QString::fromUtf8("Imprimante"));

    layout->addWidget(ongletsWidget);

    // Tab app pref -----------------------------------------------------------------------------------------
    tableWidgetAppPref = new QTableWidget(0, 0, this);
    //tableWidgetAppPref->setEditTriggers(QAbstractItemView::AllEditTriggers);
    tableWidgetAppPref->horizontalHeader()->setStretchLastSection(true);
    tableWidgetAppPref->setAlternatingRowColors(true);

    tableWidgetAppPref->setColumnCount(2);
    tableWidgetAppPref->setRowCount(12);

    QWidget *cellWidgetfoundcolor = new QWidget;
    QHBoxLayout *cellLayoutFoundColor = new QHBoxLayout();
    QLabel *BtChooseFoundLib = new QLabel(QString::fromUtf8("Couleur de recherche"));
    QPushButton *BtChooseFoundBgColor = new QPushButton("Fond");
    cellLayoutFoundColor->addWidget(BtChooseFoundLib);
    cellLayoutFoundColor->addWidget(BtChooseFoundBgColor);
    cellLayoutFoundColor->setContentsMargins(0,0,0,0);
    cellLayoutFoundColor->setSpacing(1);
    cellWidgetfoundcolor->setLayout(cellLayoutFoundColor);
    tableWidgetAppPref->setCellWidget(0, 0, cellWidgetfoundcolor);
    connect(BtChooseFoundBgColor, SIGNAL(clicked()), this, SLOT(onFoundBgColor()));
    this->foundColor = m_parent->foundColor;
    this->onFontPreview( this->tableWidgetAppPref, 0,1, QColor(Qt::black), this->foundColor , QFont("arial") );


    QWidget *cellWidgetlineEditcolor = new QWidget;
    QHBoxLayout *cellLayoutLineEditColor = new QHBoxLayout();
    QLabel *BtChooseLineEditLib = new QLabel(QString::fromUtf8("Couleur ligne édition"));
    QPushButton *BtChooseLineEditBgColor = new QPushButton("Fond");
    cellLayoutLineEditColor->addWidget(BtChooseLineEditLib);
    cellLayoutLineEditColor->addWidget(BtChooseLineEditBgColor);
    cellLayoutLineEditColor->setContentsMargins(0,0,0,0);
    cellLayoutLineEditColor->setSpacing(1);
    cellWidgetlineEditcolor->setLayout(cellLayoutLineEditColor);
    tableWidgetAppPref->setCellWidget(1, 0, cellWidgetlineEditcolor);
    connect(BtChooseLineEditBgColor, SIGNAL(clicked()), this, SLOT(onLineEditBgColor()));
    this->lineEditColor = m_parent->lineEditColor;
    this->onFontPreview( this->tableWidgetAppPref, 1,1, QColor(Qt::black), this->lineEditColor , QFont("arial") );


    QWidget *cellWidgetlineSelectColor = new QWidget;
    QHBoxLayout *cellLayoutLineSelectColor = new QHBoxLayout();
    QLabel *BtChooseLineSelectLib = new QLabel(QString::fromUtf8("Couleur ligne selection"));
    QPushButton *BtChooseLineSelectBgColor = new QPushButton("Fond");
    cellLayoutLineSelectColor->addWidget(BtChooseLineSelectLib);
    cellLayoutLineSelectColor->addWidget(BtChooseLineSelectBgColor);
    cellLayoutLineSelectColor->setContentsMargins(0,0,0,0);
    cellLayoutLineSelectColor->setSpacing(1);
    cellWidgetlineSelectColor->setLayout(cellLayoutLineSelectColor);
    tableWidgetAppPref->setCellWidget(2, 0, cellWidgetlineSelectColor);
    connect(BtChooseLineSelectBgColor, SIGNAL(clicked()), this, SLOT(onLineSelectBgColor()));
    this->lineSelectColor = m_parent->lineSelectColor;
    this->onFontPreview( this->tableWidgetAppPref, 2,1, QColor(Qt::black), this->lineSelectColor , QFont("arial") );


    tableWidgetAppPref->setItem(3,0,new QTableWidgetItem("Zoom max"));
    tableWidgetAppPref->setItem(3,1,new QTableWidgetItem( QString("%1").arg(parent->mapZoomToUpper )  ) );

    tableWidgetAppPref->setItem(4,0,new QTableWidgetItem("Zoom min"));
    tableWidgetAppPref->setItem(4,1,new QTableWidgetItem( QString("%1").arg(parent->mapZoomToLower )  ) );

    tableWidgetAppPref->setItem(5,0,new QTableWidgetItem("Format des champs dateTime"));
    tableWidgetAppPref->setItem(5,1,new QTableWidgetItem( QString("%1").arg(parent->dateTimeFormat )  ) );

    tableWidgetAppPref->setItem(6,0,new QTableWidgetItem("Lecture coordonnées presse-papier (regexp)"));
    tableWidgetAppPref->setItem(6,1,new QTableWidgetItem( QString("%1").arg(parent->mapGotoFromClipboard )  ) );

    tableWidgetAppPref->setItem(7,0,new QTableWidgetItem("Séparateur copier/coller presse-papier"));
    tableWidgetAppPref->setItem(7,1,new QTableWidgetItem( QString("%1").arg(parent->clipSep )  ) );

    confirmQuitCheck = new QCheckBox("");
    confirmQuitCheck->setChecked( parent->confirmQuit );
    tableWidgetAppPref->setItem(8,0,new QTableWidgetItem("Confirmer pour quitter"));
    tableWidgetAppPref->setCellWidget(8,1, confirmQuitCheck);

    debugModeRunCheck = new QCheckBox("");
    debugModeRunCheck->setChecked( debugModeRun );
    tableWidgetAppPref->setItem(9,0,new QTableWidgetItem("Mode debug et traces"));
    tableWidgetAppPref->setCellWidget(9,1, debugModeRunCheck);

    tableWidgetAppPref->setItem(10,0,new QTableWidgetItem("Nombre de fichiers récemment ouverts"));
    tableWidgetAppPref->setItem(10,1,new QTableWidgetItem( QString("%1").arg(parent->recentFilesMax )  ) );

    tableWidgetAppPref->setItem(11,0,new QTableWidgetItem("Raz nombre de fichiers récemment ouverts"));
    razRecentCheck = new QCheckBox("");
    tableWidgetAppPref->setCellWidget(11,1, razRecentCheck );

    tableWidgetAppPref->setColumnWidth(0, 200);
    tableWidgetAppPref->setColumnWidth(1, 400);
    tableWidgetAppPref->resizeColumnsToContents();

    tableWidgetAppPref->setHorizontalHeaderLabels(QString("Parametre;Valeur").split(";"));
    tableWidgetAppPref->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Nom du paramètre"));
    tableWidgetAppPref->horizontalHeaderItem(1)->setToolTip(QString::fromUtf8("Valeur actuelle du paramètre"));


    //writeSettingChk = new QCheckBox( QString::fromUtf8("Enregistrer la configuration actuelle à la sortie du programme"));
    //writeSettingChk->setChecked(true);

    QVBoxLayout *page1Layout = new QVBoxLayout();
    page1Layout->addWidget(tableWidgetAppPref);
    //page1Layout->addWidget(writeSettingChk);
    pageGeneral->setLayout(page1Layout);

    // Tab map -----------------------------------------------------------------------------------------
    tableWidgetMapDef = new QTableWidget(0, 0, this);
    //tableWidgetMapDef->setEditTriggers(QAbstractItemView::AllEditTriggers);
    tableWidgetMapDef->horizontalHeader()->setStretchLastSection(true);
    tableWidgetMapDef->setAlternatingRowColors(true);


    tableWidgetMapDef->setColumnCount(2);
    tableWidgetMapDef->setRowCount(10);

    tableWidgetMapDef->setItem(0,0,new QTableWidgetItem("Carte par defaut"));
    QString currentItemTextDefaultMap = parent->mapChooseWidget.itemText( parent->mapDefIndex );
    currentItemTextDefaultMap = currentItemTextDefaultMap.trimmed();
    tableWidgetMapDef->setItem(0,1,new QTableWidgetItem(  QString("%1").arg(currentItemTextDefaultMap ) )) ;
    tableWidgetMapDef->item(0, 1 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    tableWidgetMapDef->item(0, 1 )->setTextColor(QColor(Qt::blue));

    tableWidgetMapDef->setItem(1,0,new QTableWidgetItem("Carte par defaut (index)"));
    tableWidgetMapDef->setItem(1,1,new QTableWidgetItem( QString("%1").arg(parent->mapDefIndex)  ) );
    tableWidgetMapDef->item(1, 1 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    tableWidgetMapDef->item(1, 1 )->setTextColor(QColor(Qt::blue));

    tableWidgetMapDef->setItem(2,0,new QTableWidgetItem("Carte par defaut zoom"));
    tableWidgetMapDef->setItem(2,1,new QTableWidgetItem( QString("%1").arg(parent->mapDefZoom )  ) );
    tableWidgetMapDef->item(2, 1 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    tableWidgetMapDef->item(2, 1 )->setTextColor(QColor(Qt::blue));

    tableWidgetMapDef->setItem(3,0,new QTableWidgetItem("Carte par defaut lon,lat (centre)"));
    tableWidgetMapDef->setItem(3,1,new QTableWidgetItem( QString("%1 , %2").arg(parent->mapDefLon).arg(parent->mapDefLat)  ) );
    tableWidgetMapDef->item(3, 1 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    tableWidgetMapDef->item(3, 1 )->setTextColor(QColor(Qt::blue));

    tableWidgetMapDef->setItem(4,0,new QTableWidgetItem( QString::fromUtf8("Nom du fichier des paramètres")));
    tableWidgetMapDef->setItem(4,1,new QTableWidgetItem( parent->appSettings->fileName() ));
    tableWidgetMapDef->item(4, 1 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    tableWidgetMapDef->item(4, 1 )->setTextColor(QColor(Qt::blue));

    tableWidgetMapDef->setItem(5,0,new QTableWidgetItem("Carte actuelle"));
    tableWidgetMapDef->setItem(5,1,new QTableWidgetItem( parent->mapChooseWidget.currentText() ) );
    tableWidgetMapDef->item(5, 1 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    tableWidgetMapDef->item(5, 1 )->setTextColor(QColor(Qt::blue));

    tableWidgetMapDef->setItem(6,0,new QTableWidgetItem("Carte actuelle (index)"));
    tableWidgetMapDef->setItem(6,1,new QTableWidgetItem( QString("%1").arg(parent->mapChooseWidget.currentIndex())  ) );
    tableWidgetMapDef->item(6, 1 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    tableWidgetMapDef->item(6, 1 )->setTextColor(QColor(Qt::blue));

    tableWidgetMapDef->setItem(7,0,new QTableWidgetItem("Carte actuelle zoom"));
    tableWidgetMapDef->setItem(7,1,new QTableWidgetItem( QString("%1").arg(parent->slippyMap.curZoom )  ) );
    tableWidgetMapDef->item(7, 1 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    tableWidgetMapDef->item(7, 1 )->setTextColor(QColor(Qt::blue));

    QPointF maplonlat = parent->slippyMap.getLonLat();
    tableWidgetMapDef->setItem(8,0,new QTableWidgetItem("Carte actuelle lon,lat (centre)"));
    tableWidgetMapDef->setItem(8,1,new QTableWidgetItem( QString("%1 , %2").arg(maplonlat.x()).arg(maplonlat.y())  ) );
    tableWidgetMapDef->item(8, 1 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    tableWidgetMapDef->item(8, 1 )->setTextColor(QColor(Qt::blue));

    QRectF area = parent->slippyMap.getArea();
    tableWidgetMapDef->setItem(9,0,new QTableWidgetItem("Carte actuelle lon,lat (haut,gauche / droite,bas)"));
    tableWidgetMapDef->setItem(9,1,new QTableWidgetItem( QString("%1 , %2  /  %3 , %4").arg( area.left()).arg( area.top()).arg( area.right()).arg( area.bottom()) ) );
    tableWidgetMapDef->item(9, 1 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    tableWidgetMapDef->item(9, 1 )->setTextColor(QColor(Qt::blue));


    tableWidgetMapDef->setColumnWidth(0, 200);
    tableWidgetMapDef->setColumnWidth(1, 400);
    tableWidgetMapDef->resizeColumnsToContents();

    tableWidgetMapDef->setHorizontalHeaderLabels(QString("Parametre;Valeur").split(";"));
    tableWidgetMapDef->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Nom du paramètre"));
    tableWidgetMapDef->horizontalHeaderItem(1)->setToolTip(QString::fromUtf8("Valeur actuelle du paramètre"));

    QVBoxLayout *page2Layout = new QVBoxLayout();
    page2Layout->addWidget(tableWidgetMapDef);
    pageMapDef->setLayout(page2Layout);

    // Tab  map src -----------------------------------------------------------------------------------------
    tableWidgetMapSrc = new QTableWidget(0, 0, this);
    //tableWidgetMapSrc->setEditTriggers(QAbstractItemView::AllEditTriggers);
    tableWidgetMapSrc->horizontalHeader()->setStretchLastSection(true);
    tableWidgetMapSrc->setAlternatingRowColors(true);


    tableWidgetMapSrc->setColumnCount(9);
    tableWidgetMapSrc->setRowCount(parent->mapChooseWidget.count());

    for (int i = 0; i < parent->mapChooseWidget.count(); i++)
    {
        QString currentItemText = parent->mapChooseWidget.itemText(i);
        currentItemText = currentItemText.trimmed();

        tableWidgetMapSrc->setItem(i,0,new QTableWidgetItem( QString("%1").arg(currentItemText)) );
        tableWidgetMapSrc->setItem(i,1,new QTableWidgetItem( QString("%1").arg(parent->mapChooseWidget.minZoom[i] )));
        tableWidgetMapSrc->setItem(i,2,new QTableWidgetItem( QString("%1").arg(parent->mapChooseWidget.maxZoom[i] )));
        tableWidgetMapSrc->setItem(i,3,new QTableWidgetItem( QString("%1").arg(parent->mapChooseWidget.keyShortCuts[i] )));
        tableWidgetMapSrc->setItem(i,4,new QTableWidgetItem( QString("%1").arg(parent->mapChooseWidget.keyZooms[i] )));
        tableWidgetMapSrc->setItem(i,5,new QTableWidgetItem( QString("%1").arg(parent->mapChooseWidget.licenses[i] )));
        tableWidgetMapSrc->setItem(i,6,new QTableWidgetItem( QString("%1").arg(parent->mapChooseWidget.userAgents[i] )));
        tableWidgetMapSrc->setItem(i,7,new QTableWidgetItem( QString("%1").arg(parent->mapChooseWidget.referers[i] )));
        tableWidgetMapSrc->setItem(i,8,new QTableWidgetItem( QString("%1").arg(parent->mapChooseWidget.paths[i] )));
    }


    tableWidgetMapSrc->setColumnWidth(0, 150);
    tableWidgetMapSrc->setColumnWidth(1, 30);
    tableWidgetMapSrc->setColumnWidth(2, 30);
    tableWidgetMapSrc->setColumnWidth(3, 30);
    tableWidgetMapSrc->setColumnWidth(4, 70);
    tableWidgetMapSrc->setColumnWidth(5, 70);
    tableWidgetMapSrc->setColumnWidth(6, 100);
    tableWidgetMapSrc->setColumnWidth(7, 100);
    tableWidgetMapSrc->setColumnWidth(8, 400);
    //tableWidgetMapSrc->resizeColumnsToContents();

    tableWidgetMapSrc->setHorizontalHeaderLabels(QString("Nom de la carte;Zoom min; Zoom max;Fct;Fct zoom;Licence;User Agent;Referer;Url de la carte;").split(";"));
    tableWidgetMapSrc->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Nom de la carte"));
    tableWidgetMapSrc->horizontalHeaderItem(1)->setToolTip( QString::fromUtf8("Valeur minimum du zoom"));
    tableWidgetMapSrc->horizontalHeaderItem(2)->setToolTip( QString::fromUtf8("Valeur maximum du zoom"));
    tableWidgetMapSrc->horizontalHeaderItem(3)->setToolTip( QString::fromUtf8("Raccourci clavier de la carte"));
    tableWidgetMapSrc->horizontalHeaderItem(4)->setToolTip( QString::fromUtf8("Zoom par défaut de la carte"));
    tableWidgetMapSrc->horizontalHeaderItem(5)->setToolTip( QString::fromUtf8("Texte de licence"));
    tableWidgetMapSrc->horizontalHeaderItem(6)->setToolTip( QString::fromUtf8("User agent"));
    tableWidgetMapSrc->horizontalHeaderItem(7)->setToolTip( QString::fromUtf8("Referer"));
    tableWidgetMapSrc->horizontalHeaderItem(8)->setToolTip( QString::fromUtf8("Url de la carte (%1: zoom, %2: latitude, %3: longitude ou %4: quadkey (exclusif))"));

    tableWidgetMapSrc->setContextMenuPolicy(Qt::CustomContextMenu);
    connect( tableWidgetMapSrc, SIGNAL( customContextMenuRequested(const QPoint &) ), this, SLOT( ctxMenuMapDispo(const QPoint &) ) );

    QVBoxLayout *page3Layout = new QVBoxLayout();
    page3Layout->addWidget(tableWidgetMapSrc);
    pageMapSrc->setLayout(page3Layout);


    // Map cache tab -----------------------------------------------------------------------------------------
    tableWidgetMapCache = new QTableWidget(0, 0, this);
    //tableWidgetMapCache->setEditTriggers(QAbstractItemView::AllEditTriggers);
    tableWidgetMapCache->horizontalHeader()->setStretchLastSection(true);
    tableWidgetMapCache->setAlternatingRowColors(true);

    tableWidgetMapCache->setColumnCount(2);
    tableWidgetMapCache->setRowCount(3);

    //tableWidgetMapCache->setItem(0,0,new QTableWidgetItem( QString::fromUtf8("Nom du dossier de cache")));
    //tableWidgetMapCache->item(0, 0 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    QPushButton *cacheSelectDirButton = new QPushButton("Dossier du cache");
    tableWidgetMapCache->setCellWidget(0,0, cacheSelectDirButton);
    connect(cacheSelectDirButton, SIGNAL(clicked()), this, SLOT(onCacheDirSelect()));
    tableWidgetMapCache->setItem(0,1,new QTableWidgetItem( QString("%1").arg(parent->slippyMapCacheFilePath ) ));
    //tableWidgetMapCache->item(0, 1 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    //tableWidgetMapCache->item(0, 1 )->setTextColor(QColor(Qt::blue));

    QDir cacheDir(parent->slippyMapCacheFilePath);
    tableWidgetMapCache->setItem(1,0,new QTableWidgetItem( QString::fromUtf8("Nombre d'entrées")));
    tableWidgetMapCache->item(1, 0 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    tableWidgetMapCache->setItem(1,1,new QTableWidgetItem( QString("%1").arg( CountFiles(cacheDir.absolutePath()) ) ));
    tableWidgetMapCache->item(1, 1 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    tableWidgetMapCache->item(1, 1 )->setTextColor(QColor(Qt::blue));

    tableWidgetMapCache->setItem(2,0,new QTableWidgetItem( QString::fromUtf8("Taille du cache (Mo) ")));
    tableWidgetMapCache->item(2, 0 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    qlonglong sizeOfCacheLong = sizeOfdir(cacheDir.absolutePath()) ;
    double sizeOfCacheFloat = (double)(sizeOfCacheLong) /1024 / 1024;

    tableWidgetMapCache->setItem(2,1,new QTableWidgetItem( QString("%1").arg( QString::number( sizeOfCacheFloat ,'f', 2)) ));
    tableWidgetMapCache->item(2, 1 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    tableWidgetMapCache->item(2, 1 )->setTextColor(QColor(Qt::blue));



    tableWidgetMapCache->setColumnWidth(0, 200);
    tableWidgetMapCache->setColumnWidth(1, 400);
    tableWidgetMapCache->resizeColumnsToContents();

    tableWidgetMapCache->setHorizontalHeaderLabels(QString("Parametre;Valeur").split(";"));
    tableWidgetMapCache->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Nom du paramètre"));
    tableWidgetMapCache->horizontalHeaderItem(1)->setToolTip(QString::fromUtf8("Valeur actuelle du paramètre"));

    QVBoxLayout *pageMapCacheLayout = new QVBoxLayout();
    pageMapCacheLayout->addWidget(tableWidgetMapCache);
    pageMapCache->setLayout(pageMapCacheLayout);
    // End map cache


    // Tab Wpt Columns Disposition -----------------------------------------------------------------------------------------
    tableWidgetColDispo = new QTableWidget(0, 0, this);
    //tableWidgetColDispo->setEditTriggers(QAbstractItemView::AllEditTriggers);
    tableWidgetColDispo->horizontalHeader()->setStretchLastSection(true);
    tableWidgetColDispo->setAlternatingRowColors(true);

    tableWidgetColDispo->setColumnCount(5);

    // Enable drag and drop to rearrange rows
    /*tableWidgetColDispo->setDragEnabled(true);
    tableWidgetColDispo->setAcceptDrops(true);
    tableWidgetColDispo->setDropIndicatorShown(true);
    tableWidgetColDispo->setDragDropOverwriteMode(false);
    tableWidgetColDispo->setDefaultDropAction( Qt::MoveAction);
    tableWidgetColDispo->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidgetColDispo->setDragDropMode(QAbstractItemView::InternalMove);
    tableWidgetColDispo->setSelectionMode(QAbstractItemView::NoSelection);
    */

    for (int j= 0 ; j < COL_WPT_COUNT ; j++ )
    {
        localColBase[j].id =  parent->tableWpt->m_colsDisposition.getWptColId(j);
        localColBase[j].name =  parent->tableWpt->m_colsDisposition.getWptColName(j);
        localColBase[j].len =  parent->tableWpt->m_colsDisposition.getWptColLen(j);
        localColBase[j].tooltip =  parent->tableWpt->m_colsDisposition.getWptColTooltip(j);
        localColBase[j].colvisible =  parent->tableWpt->m_colsDisposition.getWptColVisibility(j);
        debugp ( "pref # " +  QString::number(j) + " :"  +  parent->tableWpt->m_colsDisposition.getWptColName(j) + " : " + parent->tableWpt->m_colsDisposition.getWptColLen(j) );

    }
    displayWptCol();

    tableWidgetColDispo->hideColumn(2);


    tableWidgetColDispo->setColumnWidth(0, 60);
    tableWidgetColDispo->setColumnWidth(1, 160);
    tableWidgetColDispo->setColumnWidth(2, 60);
    tableWidgetColDispo->setColumnWidth(3, 80);
    tableWidgetColDispo->resizeColumnsToContents();

    tableWidgetColDispo->setHorizontalHeaderLabels(QString::fromUtf8("Visible;Libellé;Code;Largeur;Tooltip").split(";"));
    tableWidgetColDispo->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Colonne visible ou non"));
    tableWidgetColDispo->horizontalHeaderItem(1)->setToolTip( QString::fromUtf8("Libellé du point"));
    tableWidgetColDispo->horizontalHeaderItem(2)->setToolTip( QString::fromUtf8("Identifiant interne"));
    tableWidgetColDispo->horizontalHeaderItem(3)->setToolTip( QString::fromUtf8("Largeur d'affichage"));
    tableWidgetColDispo->horizontalHeaderItem(4)->setToolTip( QString::fromUtf8("Tooltip"));

    tableWidgetColDispo->setContextMenuPolicy(Qt::CustomContextMenu);
    connect( tableWidgetColDispo, SIGNAL( customContextMenuRequested(const QPoint &) ), this, SLOT( ctxMenuColDispo(const QPoint &) ) );

    QVBoxLayout *pageColDispoLayout = new QVBoxLayout();
    pageColDispoLayout->addWidget( tableWidgetColDispo );
    pageWptColdispo->setLayout(pageColDispoLayout);
    // End Wpt Columns Disposition


    // Lien web
    tableWidgetWebSearch = new QTableWidget(0, 0, this);
    //tableWidgetWebSearch->setEditTriggers(QAbstractItemView::AllEditTriggers);
    tableWidgetWebSearch->horizontalHeader()->setStretchLastSection(true);
    tableWidgetWebSearch->setAlternatingRowColors(true);


    tableWidgetWebSearch->setColumnCount(2);
    tableWidgetWebSearch->setRowCount(2);


    tableWidgetWebSearch->setItem(0,0,new QTableWidgetItem("Lien web recherche (liste)"));
    tableWidgetWebSearch->item(0, 0 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    tableWidgetWebSearch->setItem(0,1,new QTableWidgetItem( QString("%1").arg(parent->searchInternetUrl)  ) );

    tableWidgetWebSearch->setItem(1,0,new QTableWidgetItem("Lien web de recherche (plan)"));
    tableWidgetWebSearch->item(1, 0 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    tableWidgetWebSearch->setItem(1,1,new QTableWidgetItem( QString("%1").arg(parent->searchInternetMapUrl)  ) );

    tableWidgetWebSearch->resizeColumnsToContents();

    tableWidgetWebSearch->setColumnWidth(0, 180);
    tableWidgetWebSearch->setColumnWidth(1, 140);
    tableWidgetWebSearch->resizeColumnsToContents();

    tableWidgetWebSearch->setHorizontalHeaderLabels(QString::fromUtf8("Lien Web;Valeur").split(";"));
    tableWidgetWebSearch->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Type de lien"));
    tableWidgetWebSearch->horizontalHeaderItem(1)->setToolTip( QString::fromUtf8("Valeur : %1=Latitude, %2=Longitude, %3=Nom du waypoint, %4=Description, %5=Commentaires, %6=Notes"));

    QVBoxLayout *pageWebSearchLayout = new QVBoxLayout();
    pageWebSearchLayout->addWidget( tableWidgetWebSearch );
    pageWebSearch->setLayout(pageWebSearchLayout);
    // End lien web



    // Tab Markers -----------------------------------------------------------------------------------------
    tableWidgetMarker = new QTableWidget(0, 0, this);
    //tableWidgetMarker->setEditTriggers(QAbstractItemView::AllEditTriggers);
    tableWidgetMarker->horizontalHeader()->setStretchLastSection(true);
    tableWidgetMarker->setAlternatingRowColors(true);

    tableWidgetMarker->setColumnCount(2);
    tableWidgetMarker->setRowCount(3);

    tableWidgetMarker->setColumnWidth(0, 180);
    tableWidgetMarker->setColumnWidth(1, 140);
    tableWidgetMarker->resizeColumnsToContents();

    tableWidgetMarker->setHorizontalHeaderLabels(QString::fromUtf8("Paramètre;Valeur").split(";"));
    tableWidgetMarker->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Nom du paramètre"));
    tableWidgetMarker->horizontalHeaderItem(1)->setToolTip( QString::fromUtf8("Valeur du paramètre"));

    QPushButton *markerSelectDirButton = new QPushButton("Dossier des balises");
    tableWidgetMarker->setCellWidget(0,0, markerSelectDirButton);
    connect(markerSelectDirButton, SIGNAL(clicked()), this, SLOT(onMarkerDirSelect()));
    tableWidgetMarker->setItem(0,1,new QTableWidgetItem( QString("%1").arg(parent->iconSymbolRootDir + parent->iconSymbolName)));


    QWidget *cellWidgetMarker = new QWidget;
    QHBoxLayout *cellLayoutMarkerFont = new QHBoxLayout();
    QLabel *BtChooseMarkerLib = new QLabel("Balises");
    QPushButton *BtChooseMarkerTextColor = new QPushButton("Texte");
    QPushButton *BtChooseMarkerBgColor = new QPushButton("Fond");
    QPushButton *BtChooseMarkerBgTrans = new QPushButton("Transparence");
    QPushButton *BtChooseMarkerFont = new QPushButton("Police");
    cellLayoutMarkerFont->addWidget(BtChooseMarkerLib);
    cellLayoutMarkerFont->addWidget(BtChooseMarkerTextColor);
    cellLayoutMarkerFont->addWidget(BtChooseMarkerBgColor);
    cellLayoutMarkerFont->addWidget(BtChooseMarkerBgTrans);
    cellLayoutMarkerFont->addWidget(BtChooseMarkerFont);
    cellLayoutMarkerFont->setContentsMargins(0,0,0,0);
    cellLayoutMarkerFont->setSpacing(1);
    cellWidgetMarker->setLayout(cellLayoutMarkerFont);
    tableWidgetMarker->setCellWidget(1, 0, cellWidgetMarker);
    connect(BtChooseMarkerTextColor, SIGNAL(clicked()), this, SLOT(onMarkerFontColor()));
    connect(BtChooseMarkerBgColor, SIGNAL(clicked()), this, SLOT(onMarkerBgColor()));
    connect(BtChooseMarkerFont, SIGNAL(clicked()), this, SLOT(onMarkerFontSelect()));
    connect(BtChooseMarkerBgTrans, SIGNAL(clicked()), this, SLOT(onMarkerTransparency()));

    this->markerFgColor = m_parent->markers->markerFgColor ;
    this->markerBgColor = m_parent->markers->markerBgColor;
    this->markerFonfFamilly = m_parent->markers->markerFonfFamilly  ;
    this->onFontPreview( this->tableWidgetMarker, 1,1, this->markerFgColor, this->markerBgColor , this->markerFonfFamilly );

    tableWidgetMarker->resizeColumnsToContents();


    tableWidgetMarker->setItem(2,0,new QTableWidgetItem(QString::fromUtf8("Afficher les libellés")));
    QCheckBox *markerDisplayNameChk  = new QCheckBox("");
    if (parent->markers->markerDisplayName == true ) markerDisplayNameChk->setChecked(true);
    else  markerDisplayNameChk->setChecked(false);
    tableWidgetMarker->setCellWidget(2,1, markerDisplayNameChk);


    QVBoxLayout *pageMarkerLayout = new QVBoxLayout();
    pageMarkerLayout->addWidget( tableWidgetMarker );
    pageMarker->setLayout(pageMarkerLayout);



    // Tab Map legend -----------------------------------------------------------------------------------------

    tableWidgetMapLegend = new QTableWidget(0, 0, this);
    //tableWidgetMapLegend->setEditTriggers(QAbstractItemView::AllEditTriggers);
    tableWidgetMapLegend->horizontalHeader()->setStretchLastSection(true);
    tableWidgetMapLegend->setAlternatingRowColors(true);

    tableWidgetMapLegend->setColumnCount(2);
    tableWidgetMapLegend->setRowCount(18);
    tableWidgetMapLegend->setHorizontalHeaderLabels(QString::fromUtf8("Paramètre;Valeur").split(";"));
    tableWidgetMapLegend->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Nom de paramètre"));
    tableWidgetMapLegend->horizontalHeaderItem(1)->setToolTip( QString::fromUtf8("Valeur du paramètre"));



    QWidget *cellWidgetZoom = new QWidget;
    QHBoxLayout *cellLayoutZoomFont = new QHBoxLayout();
    QLabel *BtChooseZoomLib = new QLabel("Zoom");
    QPushButton *BtChooseZoomTextColor = new QPushButton("Texte");
    QPushButton *BtChooseZoomBgColor = new QPushButton("Fond");
    QPushButton *BtChooseZoomBgTrans = new QPushButton("Transparence");
    QPushButton *BtChooseZoomFont = new QPushButton("Police");
    cellLayoutZoomFont->addWidget(BtChooseZoomLib);
    cellLayoutZoomFont->addWidget(BtChooseZoomTextColor);
    cellLayoutZoomFont->addWidget(BtChooseZoomBgColor);
    cellLayoutZoomFont->addWidget(BtChooseZoomBgTrans);
    cellLayoutZoomFont->addWidget(BtChooseZoomFont);
    cellLayoutZoomFont->setContentsMargins(0,0,0,0);
    cellLayoutZoomFont->setSpacing(1);
    cellWidgetZoom->setLayout(cellLayoutZoomFont);
    tableWidgetMapLegend->setCellWidget(MAPLEGEND_ZOOMCOLOR, 0, cellWidgetZoom);
    connect(BtChooseZoomTextColor, SIGNAL(clicked()), this, SLOT(onZoomFontColor()));
    connect(BtChooseZoomBgColor, SIGNAL(clicked()), this, SLOT(onZoomBgColor()));
    connect(BtChooseZoomFont, SIGNAL(clicked()), this, SLOT(onZoomFontSelect()));
    connect(BtChooseZoomBgTrans, SIGNAL(clicked()), this, SLOT(onZoomTransparency()));

    this->zoomBgColor = parent->mapZoomVal->bgColor;
    this->zoomFgColor = parent->mapZoomVal->fontColor;
    this->zoomFonfFamilly = parent->mapZoomVal->font;
    this->onFontPreview( tableWidgetMapLegend, MAPLEGEND_ZOOMCOLOR, 1, this->zoomFgColor, this->zoomBgColor, this->zoomFonfFamilly);

    mapLegendDisplayZoomCheck = new QCheckBox("");
    mapLegendDisplayZoomCheck->setChecked( parent->mapZoomVal->zoomDisplayVal );
    tableWidgetMapLegend->setItem(MAPLEGEND_ZOOMDISPLAY,0,new QTableWidgetItem("Afficher le zoom"));
    tableWidgetMapLegend->setCellWidget(MAPLEGEND_ZOOMDISPLAY,1, mapLegendDisplayZoomCheck);

    mapLegendDisplayZoomPlusMoinsCheck  = new QCheckBox("");
    mapLegendDisplayZoomPlusMoinsCheck->setChecked( parent->mapZoomControl->display );
    tableWidgetMapLegend->setItem(MAPLEGEND_ZOOMCTRLDISPLAY,0,new QTableWidgetItem("Afficher zoom +/-"));
    tableWidgetMapLegend->setCellWidget(MAPLEGEND_ZOOMCTRLDISPLAY,1, mapLegendDisplayZoomPlusMoinsCheck);

    QWidget *cellWidgetCross = new QWidget;
    QHBoxLayout *cellLayoutCrossFont = new QHBoxLayout();
    QLabel *BtChooseCrossLib = new QLabel("Croix centrale");
    QPushButton *BtChooseCrossTextColor = new QPushButton("Texte");
    QPushButton *BtChooseCrossBgColor = new QPushButton("Fond");
    QPushButton *BtChooseCrossBgTrans = new QPushButton("Transparence");
    //QPushButton *BtChooseCrossFont = new QPushButton("Police");
    cellLayoutCrossFont->addWidget(BtChooseCrossLib);
    cellLayoutCrossFont->addWidget(BtChooseCrossTextColor);
    cellLayoutCrossFont->addWidget(BtChooseCrossBgColor);
    cellLayoutCrossFont->addWidget(BtChooseCrossBgTrans);
    //cellLayoutCrossFont->addWidget(BtChooseCrossFont);
    cellLayoutCrossFont->setContentsMargins(0,0,0,0);
    cellLayoutCrossFont->setSpacing(1);
    cellWidgetCross->setLayout(cellLayoutCrossFont);
    tableWidgetMapLegend->setCellWidget(MAPLEGEND_CROSSCOLOR, 0, cellWidgetCross);
    this->crossBgColor = parent->centerMark->bgColor;
    this->crossFgColor = parent->centerMark->fontColor;
    this->crossFonfFamilly = parent->centerMark->font;
    onFontPreview( tableWidgetMapLegend, MAPLEGEND_CROSSCOLOR,1, this->crossFgColor, this->crossBgColor , this->crossFonfFamilly );
    connect(BtChooseCrossTextColor, SIGNAL(clicked()), this, SLOT(onCrossFontColor()));
    connect(BtChooseCrossBgColor, SIGNAL(clicked()), this, SLOT(onCrossBgColor()));
    connect(BtChooseCrossBgTrans, SIGNAL(clicked()), this, SLOT(onCrossTransparency()));

    tableWidgetMapLegend->setItem(MAPLEGEND_CROSSSIZE,0,new QTableWidgetItem( "Taille de la croix centrale"  ) );
    tableWidgetMapLegend->setItem(MAPLEGEND_CROSSSIZE,1,new QTableWidgetItem( QString("%1").arg( parent->centerMark->sizeCross )  ) );

    QCheckBox *mapLegendDisplayCrossCheck  = new QCheckBox("");
    tableWidgetMapLegend->setItem(MAPLEGEND_CROSSDISPLAY,0,new QTableWidgetItem("Afficher la croix centrale"));
    tableWidgetMapLegend->setCellWidget(MAPLEGEND_CROSSDISPLAY,1, mapLegendDisplayCrossCheck);
    if (parent->centerMark->display == true ) mapLegendDisplayCrossCheck->setChecked(true);
    else  mapLegendDisplayCrossCheck->setChecked(false);


    QWidget *cellWidgetScaleBig = new QWidget;
    QHBoxLayout *cellLayoutScaleBigFont = new QHBoxLayout();
    QLabel *BtChooseScaleBigLib = new QLabel("Echelle fixe");
    QPushButton *BtChooseScaleBigTextColor = new QPushButton("Texte");
    QPushButton *BtChooseScaleBigBgColor = new QPushButton("Fond");
    QPushButton *BtChooseScaleBigBgTrans = new QPushButton("Transparence");
    QPushButton *BtChooseScaleBigFont = new QPushButton("Police");
    cellLayoutScaleBigFont->addWidget(BtChooseScaleBigLib);
    cellLayoutScaleBigFont->addWidget(BtChooseScaleBigTextColor);
    cellLayoutScaleBigFont->addWidget(BtChooseScaleBigBgColor);
    cellLayoutScaleBigFont->addWidget(BtChooseScaleBigBgTrans);
    cellLayoutScaleBigFont->addWidget(BtChooseScaleBigFont);
    cellLayoutScaleBigFont->setContentsMargins(0,0,0,0);
    cellLayoutScaleBigFont->setSpacing(1);
    cellWidgetScaleBig->setLayout(cellLayoutScaleBigFont);
    tableWidgetMapLegend->setCellWidget(MAPLEGEND_SCALEFIXCOLOR, 0, cellWidgetScaleBig);
    connect(BtChooseScaleBigTextColor, SIGNAL(clicked()), this, SLOT(onScaleBigFontColor()));
    connect(BtChooseScaleBigBgColor, SIGNAL(clicked()), this, SLOT(onScaleBigBgColor()));
    connect(BtChooseScaleBigFont, SIGNAL(clicked()), this, SLOT(onScaleBigFontSelect()));
    connect(BtChooseScaleBigBgTrans, SIGNAL(clicked()), this, SLOT(onScaleBigTransparency()));

    QCheckBox *mapLegendDisplayScale1Check  = new QCheckBox("");
    tableWidgetMapLegend->setItem(MAPLEGEND_SCALEFIXDISPLAY,0,new QTableWidgetItem( QString::fromUtf8("Afficher l'échelle fixe")));
    tableWidgetMapLegend->setCellWidget(MAPLEGEND_SCALEFIXDISPLAY,1, mapLegendDisplayScale1Check);

    scale1FonfFamilly = parent->mapScale->scale1fonfFamilly ;
    scale1BgColor = parent->mapScale->scale1bgColor ;
    scale1FgColor = parent->mapScale->scale1fontColor ;

    if (parent->mapScale->display1 == true ) mapLegendDisplayScale1Check->setChecked(true);
    else  mapLegendDisplayScale1Check->setChecked(false);

    this->onFontPreview( tableWidgetMapLegend, MAPLEGEND_SCALEFIXCOLOR,1, this->scale1FgColor, this->scale1BgColor, this->scale1FonfFamilly);

    tableWidgetMapLegend->setItem(MAPLEGEND_SCALEFIXPREC,0,new QTableWidgetItem( QString::fromUtf8("Précision (chiffres après la virgule)")));
    tableWidgetMapLegend->setItem(MAPLEGEND_SCALEFIXPREC,1,new QTableWidgetItem( QString("%1").arg(parent->mapScale->scale1Precision) ) );

    tableWidgetMapLegend->setItem(MAPLEGEND_SCALEFIXLEN,0,new QTableWidgetItem("Longeur échelle (pixels)"));
    tableWidgetMapLegend->setItem(MAPLEGEND_SCALEFIXLEN,1,new QTableWidgetItem( QString("%1").arg(parent->mapScale->scale1PixelLength) ) );

    tableWidgetMapLegend->setItem(MAPLEGEND_SCALEFIXOFFSETX,0,new QTableWidgetItem( QString::fromUtf8("Offset X")));
    tableWidgetMapLegend->setItem(MAPLEGEND_SCALEFIXOFFSETX, 1,new QTableWidgetItem( QString("%1").arg(parent->mapScale->scale1MarginX) ) );

    tableWidgetMapLegend->setItem(MAPLEGEND_SCALEFIXOFFSETY,0,new QTableWidgetItem( QString::fromUtf8("Offset Y")));
    tableWidgetMapLegend->setItem(MAPLEGEND_SCALEFIXOFFSETY,1,new QTableWidgetItem( QString("%1").arg(parent->mapScale->scale1MarginY) ) );


    QWidget *cellWidgetScaleSmall = new QWidget;
    QHBoxLayout *cellLayoutScaleSmallFont = new QHBoxLayout();
    QLabel *BtChooseScaleSmallLib = new QLabel("Echelle variable");
    QPushButton *BtChooseScaleSmallTextColor = new QPushButton("Texte");
    QPushButton *BtChooseScaleSmallBgColor = new QPushButton("Fond");
    QPushButton *BtChooseScaleSmallBgTrans = new QPushButton("Transparence");
    QPushButton *BtChooseScaleSmallFont = new QPushButton("Police");
    cellLayoutScaleSmallFont->addWidget(BtChooseScaleSmallLib);
    cellLayoutScaleSmallFont->addWidget(BtChooseScaleSmallTextColor);
    cellLayoutScaleSmallFont->addWidget(BtChooseScaleSmallBgColor);
    cellLayoutScaleSmallFont->addWidget(BtChooseScaleSmallBgTrans);
    cellLayoutScaleSmallFont->addWidget(BtChooseScaleSmallFont);
    cellLayoutScaleSmallFont->setContentsMargins(0,0,0,0);
    cellLayoutScaleSmallFont->setSpacing(1);
    cellWidgetScaleSmall->setLayout(cellLayoutScaleSmallFont);
    tableWidgetMapLegend->setCellWidget(MAPLEGEND_SCALEVARCOLOR, 0, cellWidgetScaleSmall);
    connect(BtChooseScaleSmallTextColor, SIGNAL(clicked()), this, SLOT(onScaleVarFontColor()));
    connect(BtChooseScaleSmallBgColor, SIGNAL(clicked()), this, SLOT(onScaleVarBgColor()));
    connect(BtChooseScaleSmallFont, SIGNAL(clicked()), this, SLOT(onScaleVarFontSelect()));
    connect(BtChooseScaleSmallBgTrans, SIGNAL(clicked()), this, SLOT(onScaleVarTransparency()));

    QCheckBox *mapLegendDisplayScale2Check  = new QCheckBox("");
    tableWidgetMapLegend->setItem(MAPLEGEND_SCALEVARDISPLAY,0,new QTableWidgetItem(QString::fromUtf8("Afficher échelle variable")));
    tableWidgetMapLegend->setCellWidget(MAPLEGEND_SCALEVARDISPLAY,1, mapLegendDisplayScale2Check);
    if (parent->mapScale->display2 == true ) mapLegendDisplayScale2Check->setChecked(true);
    else  mapLegendDisplayScale2Check->setChecked(false);

    scale2FonfFamilly = parent->mapScale->scale2fonfFamilly ;
    scale2BgColor = parent->mapScale->scale2bgColor ;
    scale2FgColor = parent->mapScale->scale2fontColor ;

    this->onFontPreview( tableWidgetMapLegend, MAPLEGEND_SCALEVARCOLOR,1, this->scale2FgColor, this->scale2BgColor, this->scale2FonfFamilly);

    tableWidgetMapLegend->setItem(MAPLEGEND_SCALEVARPREC,0,new QTableWidgetItem( QString::fromUtf8("Précision (chiffres après la virgule)")));
    tableWidgetMapLegend->setItem(MAPLEGEND_SCALEVARPREC,1,new QTableWidgetItem( QString("%1").arg(parent->mapScale->scale2Precision) ) );

    tableWidgetMapLegend->setItem(MAPLEGEND_SCALEVARLEN,0,new QTableWidgetItem( QString::fromUtf8("Longueur échelle (en metres)")));
    tableWidgetMapLegend->setItem(MAPLEGEND_SCALEVARLEN,1,new QTableWidgetItem( QString("%1").arg(parent->mapScale->scale2MetersLength) ) );

    tableWidgetMapLegend->setItem(MAPLEGEND_SCALEVAROFFSETX,0,new QTableWidgetItem( QString::fromUtf8("Offset X")));
    tableWidgetMapLegend->setItem(MAPLEGEND_SCALEVAROFFSETX,1,new QTableWidgetItem( QString("%1").arg(parent->mapScale->scale2MarginX) ) );

    tableWidgetMapLegend->setItem(MAPLEGEND_SCALEVAROFFSETY,0,new QTableWidgetItem( QString::fromUtf8("Offset Y")));
    tableWidgetMapLegend->setItem(MAPLEGEND_SCALEVAROFFSETY,1,new QTableWidgetItem( QString("%1").arg(parent->mapScale->scale2MarginY) ) );


    tableWidgetMapLegend->resizeColumnsToContents();

    QVBoxLayout *pageMapLegendLayout = new QVBoxLayout();
    pageMapLegendLayout->addWidget( tableWidgetMapLegend );
    pageMapLegend->setLayout(pageMapLegendLayout);



    // Printer
    tableWidgetLegend = new QTableWidget(0, 0, this);
    //tableWidgetLegend->setEditTriggers(QAbstractItemView::AllEditTriggers);
    tableWidgetLegend->horizontalHeader()->setStretchLastSection(true);
    tableWidgetLegend->setAlternatingRowColors(true);

    tableWidgetLegend->setColumnCount(2);
    tableWidgetLegend->setRowCount(12);


    tableWidgetLegend->setHorizontalHeaderLabels(QString::fromUtf8("Paramètre;Valeur").split(";"));
    tableWidgetLegend->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Paramètre"));
    tableWidgetLegend->horizontalHeaderItem(1)->setToolTip( QString::fromUtf8("Valeur"));


    QWidget *cellWidgetMapPoi = new QWidget;
    QHBoxLayout *cellLayoutMapPoiFont = new QHBoxLayout();
    QLabel *BtChooseMapPoiLib = new QLabel("Balises sur la carte");
    QPushButton *BtChooseMapPoiTextColor = new QPushButton("Texte");
    QPushButton *BtChooseMapPoiBgColor = new QPushButton("Fond");
    QPushButton *BtChooseMapPoiBgTrans = new QPushButton("Transparence");
    QPushButton *BtChooseMapPoiFont = new QPushButton("Police");
    cellLayoutMapPoiFont->addWidget(BtChooseMapPoiLib);
    cellLayoutMapPoiFont->addWidget(BtChooseMapPoiTextColor);
    cellLayoutMapPoiFont->addWidget(BtChooseMapPoiBgColor);
    cellLayoutMapPoiFont->addWidget(BtChooseMapPoiBgTrans);
    cellLayoutMapPoiFont->addWidget(BtChooseMapPoiFont);
    cellLayoutMapPoiFont->setContentsMargins(0,0,0,0);
    cellLayoutMapPoiFont->setSpacing(1);
    cellWidgetMapPoi->setLayout(cellLayoutMapPoiFont);
    tableWidgetLegend->setCellWidget(PRINTER_MAPPOICOLOR, 0, cellWidgetMapPoi);
    connect(BtChooseMapPoiTextColor, SIGNAL(clicked()), this, SLOT(onMapPoiFontColor()));
    connect(BtChooseMapPoiBgColor, SIGNAL(clicked()), this, SLOT(onMapPoiBgColor()));
    connect(BtChooseMapPoiFont, SIGNAL(clicked()), this, SLOT(onMapPoiFontSelect()));
    connect(BtChooseMapPoiBgTrans, SIGNAL(clicked()), this, SLOT(onMapPoiTransparency()));
    this->mapPoiFontColorFg = m_parent->printMapPoiColorFg;
    this->mapPoiFontColorBg = m_parent->printMapPoiColorBg;
    this->mapPoiFontFamilly = m_parent->printMapPoiFont;
    this->onFontPreview( tableWidgetLegend, PRINTER_MAPPOICOLOR ,1, this->mapPoiFontColorFg, this->mapPoiFontColorBg, this->mapPoiFontFamilly);

    QWidget *cellWidgetListPoi = new QWidget;
    QHBoxLayout *cellLayoutListPoiFont = new QHBoxLayout();
    QLabel *BtChooseListPoiLib = new QLabel("liste des Balises");
    QPushButton *BtChooseListPoiTextColor = new QPushButton("Texte");
    QPushButton *BtChooseListPoiBgColor = new QPushButton("Fond");
    //QPushButton *BtChooseListPoiBgTrans = new QPushButton("Transparence");
    QPushButton *BtChooseListPoiFont = new QPushButton("Police");
    cellLayoutListPoiFont->addWidget(BtChooseListPoiLib);
    cellLayoutListPoiFont->addWidget(BtChooseListPoiTextColor);
    //cellLayoutListPoiFont->addWidget(BtChooseListPoiBgColor);
    //cellLayoutListPoiFont->addWidget(BtChooseListPoiBgTrans);
    cellLayoutListPoiFont->addWidget(BtChooseListPoiFont);
    cellLayoutListPoiFont->setContentsMargins(0,0,0,0);
    cellLayoutListPoiFont->setSpacing(1);
    cellWidgetListPoi->setLayout(cellLayoutListPoiFont);
    tableWidgetLegend->setCellWidget(PRINTER_LISTPOICOLOR, 0, cellWidgetListPoi);
    connect(BtChooseListPoiTextColor, SIGNAL(clicked()), this, SLOT(onListPoiFontColor()));
    connect(BtChooseListPoiBgColor, SIGNAL(clicked()), this, SLOT(onListPoiBgColor()));
    connect(BtChooseListPoiFont, SIGNAL(clicked()), this, SLOT(onListPoiFontSelect()));
    //connect(BtChooseListPoiBgTrans, SIGNAL(clicked()), this, SLOT(onListPoiTransparency()));
    this->listPoiFontColorFg = m_parent->printListPoiColorFg;
    this->listPoiFontColorBg = m_parent->printListPoiColorBg;
    this->listPoiFontFamilly = m_parent->printListPoiFont;
    this->onFontPreview( tableWidgetLegend, PRINTER_LISTPOICOLOR ,1, this->listPoiFontColorFg, this->listPoiFontColorBg, this->listPoiFontFamilly);


    QWidget *cellWidgetMapLegend = new QWidget;
    QHBoxLayout *cellLayoutMapLegendFont = new QHBoxLayout();
    QLabel *BtChooseMapLegendLib = new QLabel( QString::fromUtf8("Légende"));
    QPushButton *BtChooseMapLegendTextColor = new QPushButton("Texte");
    QPushButton *BtChooseMapLegendBgColor = new QPushButton("Fond");
//    QPushButton *BtChooseMapLegendBgTrans = new QPushButton("Transparence");
    QPushButton *BtChooseMapLegendFont = new QPushButton("Police");
    cellLayoutMapLegendFont->addWidget(BtChooseMapLegendLib);
    cellLayoutMapLegendFont->addWidget(BtChooseMapLegendTextColor);
    //cellLayoutMapLegendFont->addWidget(BtChooseMapLegendBgColor);
    //cellLayoutMapLegendFont->addWidget(BtChooseMapLegendBgTrans);
    cellLayoutMapLegendFont->addWidget(BtChooseMapLegendFont);
    cellLayoutMapLegendFont->setContentsMargins(0,0,0,0);
    cellLayoutMapLegendFont->setSpacing(1);
    cellWidgetMapLegend->setLayout(cellLayoutMapLegendFont);
    tableWidgetLegend->setCellWidget(PRINTER_MAPLEGENDCOLOR, 0, cellWidgetMapLegend);
    connect(BtChooseMapLegendTextColor, SIGNAL(clicked()), this, SLOT(onMapLegendFontColor()));
    connect(BtChooseMapLegendBgColor, SIGNAL(clicked()), this, SLOT(onMapLegendBgColor()));
    connect(BtChooseMapLegendFont, SIGNAL(clicked()), this, SLOT(onMapLegendFontSelect()));
    //connect(BtChooseMapLegendBgTrans, SIGNAL(clicked()), this, SLOT(onMapLegendTransparency()));
    this->legendPoiFontColorFg = m_parent->printMapLegendColorFg;
    this->legendPoiFontColorBg = m_parent->printMapLegendColorBg;
    this->legendPoiFontFamilly = m_parent->printMapLegendFont;
    this->onFontPreview( tableWidgetLegend, PRINTER_MAPLEGENDCOLOR ,1, this->legendPoiFontColorFg, this->legendPoiFontColorBg, this->legendPoiFontFamilly);



    tableWidgetLegend->setItem(PRINTER_RESOLUTION,0,new QTableWidgetItem( QString::fromUtf8("Résolution")));
    tableWidgetLegend->setItem(PRINTER_RESOLUTION,1,new QTableWidgetItem( QString("%1").arg(m_parent->printResolution )   ) );

    QCheckBox *mapPrintModeDebug  = new QCheckBox("");
    tableWidgetLegend->setItem(PRINTER_MODEDEBUG,0,new QTableWidgetItem(QString::fromUtf8("Mode debug")));
    tableWidgetLegend->setCellWidget(PRINTER_MODEDEBUG,1, mapPrintModeDebug);
    mapPrintModeDebug->setChecked( parent->printDebugMode);

    tableWidgetLegend->setItem(PRINTER_MARGIN_LEFT,0,new QTableWidgetItem( QString::fromUtf8("Marge gauche (mm)")));
    tableWidgetLegend->setItem(PRINTER_MARGIN_LEFT,1,new QTableWidgetItem( QString("%1").arg(parent->printMarginLeft )   ));
    tableWidgetLegend->setItem(PRINTER_MARGIN_TOP,0,new QTableWidgetItem( QString::fromUtf8("Marge haut (mm)")));
    tableWidgetLegend->setItem(PRINTER_MARGIN_TOP,1,new QTableWidgetItem( QString("%1").arg(parent->printMarginTop) ));
    tableWidgetLegend->setItem(PRINTER_MARGIN_RIGHT,0,new QTableWidgetItem( QString::fromUtf8("Marge droite (mm)")));
    tableWidgetLegend->setItem(PRINTER_MARGIN_RIGHT,1,new QTableWidgetItem( QString("%1").arg(parent->printMarginRight) ));
    tableWidgetLegend->setItem(PRINTER_MARGIN_BOTTOM,0,new QTableWidgetItem( QString::fromUtf8("Marge bas (mm)")));
    tableWidgetLegend->setItem(PRINTER_MARGIN_BOTTOM,1,new QTableWidgetItem( QString("%1").arg(parent->printMarginBottom) ));

    QCheckBox *mapPrintBorder  = new QCheckBox("");
    tableWidgetLegend->setItem(PRINTER_BORDER,0,new QTableWidgetItem(QString::fromUtf8("Cadre autour de la carte")));
    tableWidgetLegend->setCellWidget(PRINTER_BORDER,1, mapPrintBorder);
    mapPrintBorder->setChecked( parent->printMapBorder);

    QCheckBox *mapPrintPoiMap  = new QCheckBox("");
    tableWidgetLegend->setItem(PRINTER_POIMAP,0,new QTableWidgetItem(QString::fromUtf8("Imprimer les balises sur la carte")));
    tableWidgetLegend->setCellWidget(PRINTER_POIMAP,1, mapPrintPoiMap);
    mapPrintPoiMap->setChecked( parent->printMapPoi);

    QCheckBox *mapPrintPoiList  = new QCheckBox("");
    tableWidgetLegend->setItem(PRINTER_POILIST,0,new QTableWidgetItem(QString::fromUtf8("Imprimer la liste des balises")));
    tableWidgetLegend->setCellWidget(PRINTER_POILIST,1, mapPrintPoiList);
    mapPrintPoiList->setChecked( parent->printListPoi);

    tableWidgetLegend->resizeColumnsToContents();

    QVBoxLayout *pageLegendLayout = new QVBoxLayout();
    pageLegendLayout->addWidget( tableWidgetLegend );
    pageLegend->setLayout(pageLegendLayout);



    // Final layout
    layout->addWidget( box );

    setLayout(layout);

    setWindowTitle( QString::fromUtf8("Préférences"));


}

void dialogAppPref::onCacheDirSelect()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Dossier cache des cartes"),
                  tableWidgetMapCache->item(0, 1)->text(),
                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (dir != "")
    {
        tableWidgetMapCache->item(0, 1)->setText( dir );
    }
}

void dialogAppPref::onMarkerDirSelect()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Dossier des markers"),
                  m_parent->iconSymbolRootDir + tableWidgetMarker->item(0, 1)->text(),
                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (dir != "")
    {
        tableWidgetMarker->item(0, 1)->setText( dir );
    }

}


void dialogAppPref::onLineEditBgColor()
{
    QColor newColor = selectColorBox(this->foundColor);
    if (this->selectStatut == true )
    {
        this->lineEditColor = newColor;
        this->onFontPreview( this->tableWidgetAppPref, 1,1, QColor(Qt::black), this->lineEditColor , QFont("arial") );
    }

}

void dialogAppPref::onLineSelectBgColor()
{
    QColor newColor = selectColorBox(this->foundColor);
    if (this->selectStatut == true )
    {
        this->lineSelectColor = newColor;
        this->onFontPreview( this->tableWidgetAppPref, 2,1, QColor(Qt::black), this->lineSelectColor , QFont("arial") );
    }

}


void dialogAppPref::onFoundBgColor()
{
    QColor newColor = selectColorBox(this->foundColor);
    if (this->selectStatut == true )
    {
        this->foundColor = newColor;
        this->onFontPreview( this->tableWidgetAppPref, 0,1, QColor(Qt::black), this->foundColor , QFont("arial") );
    }

}

void dialogAppPref::onMarkerFontSelect()
{
    bool okButton;
    QFont mapFonfFamilly;

    mapFonfFamilly = QFontDialog::getFont(&okButton, markerFonfFamilly);
    if (okButton)
    {
        markerFonfFamilly = mapFonfFamilly;
        this->onFontPreview( this->tableWidgetMarker, 1,1, this->markerFgColor, this->markerBgColor , this->markerFonfFamilly );

    }
}
void dialogAppPref::onMarkerFontColor()
{
    QColor newColor = selectColorBox( this->markerFgColor );
    if (this->selectStatut == true )
    {
        this->markerFgColor = newColor;
        this->onFontPreview( this->tableWidgetMarker, 1,1, this->markerFgColor, this->markerBgColor , this->markerFonfFamilly );
    }
}
void dialogAppPref::onMarkerBgColor()
{
    QColor newColor = selectColorBox(this->markerBgColor);
    if (this->selectStatut == true )
    {
        qreal alpha = this->markerBgColor.alphaF();
        this->markerBgColor = newColor;
        this->markerBgColor.setAlphaF( alpha );
        this->onFontPreview( this->tableWidgetMarker, 1,1, this->markerFgColor, this->markerBgColor , this->markerFonfFamilly );
    }

}
void dialogAppPref::onMarkerTransparency()
{

    bool ok_select = false;
    float transparency = QInputDialog::getDouble(this,"Transparence", "Entrez la transparence (0...1) :", this->markerBgColor.alphaF(), 0, 1, 2, &ok_select); // label, value, min, max, decimales

    if (ok_select == true )
    {
        this->markerBgColor.setAlphaF(transparency);
        this->onFontPreview( this->tableWidgetMarker, 1,1, this->markerFgColor, this->markerBgColor , this->markerFonfFamilly );
    }
}




void dialogAppPref::onZoomFontSelect()
{
    bool okButton;
    QFont mapFonfFamilly;

    mapFonfFamilly = QFontDialog::getFont(&okButton, zoomFonfFamilly);
    if (okButton)
    {
        zoomFonfFamilly = mapFonfFamilly;
        this->onFontPreview( this->tableWidgetMapLegend, MAPLEGEND_ZOOMCOLOR,1, this->zoomFgColor, this->zoomBgColor , this->zoomFonfFamilly );

    }
}
void dialogAppPref::onZoomFontColor()
{
    QColor newColor = selectColorBox( this->zoomFgColor );
    if (this->selectStatut == true )
    {
        this->zoomFgColor = newColor;
        this->onFontPreview( this->tableWidgetMapLegend, MAPLEGEND_ZOOMCOLOR,1, this->zoomFgColor, this->zoomBgColor , this->zoomFonfFamilly );
    }
}
void dialogAppPref::onZoomBgColor()
{
    QColor newColor = selectColorBox(this->zoomBgColor);
    if (this->selectStatut == true )
    {
        qreal alpha = this->zoomBgColor.alphaF();
        this->zoomBgColor = newColor;
        this->zoomBgColor.setAlphaF( alpha );
        this->onFontPreview( this->tableWidgetMapLegend, MAPLEGEND_ZOOMCOLOR,1, this->zoomFgColor, this->zoomBgColor , this->zoomFonfFamilly );
    }

}
void dialogAppPref::onZoomTransparency()
{

    bool ok_select = false;
    float transparency = QInputDialog::getDouble(this,"Transparence", "Entrez la transparence (0...1) :", this->zoomBgColor.alphaF(), 0, 1, 2, &ok_select); // label, value, min, max, decimales

    if (ok_select == true )
    {
        this->zoomBgColor.setAlphaF(transparency);
        this->onFontPreview( this->tableWidgetMapLegend, MAPLEGEND_ZOOMCOLOR,1, this->zoomFgColor, this->zoomBgColor , this->zoomFonfFamilly );
    }
}




void dialogAppPref::onCrossFontColor()
{
    QColor newColor = selectColorBox( this->crossFgColor );
    if (this->selectStatut == true )
    {
        this->crossFgColor = newColor;
        this->onFontPreview( this->tableWidgetMapLegend, MAPLEGEND_CROSSCOLOR,1, this->crossFgColor, this->crossBgColor , this->crossFonfFamilly );
    }
}
void dialogAppPref::onCrossBgColor()
{
    QColor newColor = selectColorBox(this->crossBgColor);
    if (this->selectStatut == true )
    {
        qreal alpha = this->crossBgColor.alphaF();
        this->crossBgColor = newColor;
        this->crossBgColor.setAlphaF( alpha );
        this->onFontPreview( this->tableWidgetMapLegend, MAPLEGEND_CROSSCOLOR,1, this->crossFgColor, this->crossBgColor , this->crossFonfFamilly );
    }

}
void dialogAppPref::onCrossTransparency()
{

    bool ok_select = false;
    float transparency = QInputDialog::getDouble(this,"Transparence", "Entrez la transparence (0...1) :", this->crossBgColor.alphaF(), 0, 1, 2, &ok_select); // label, value, min, max, decimales

    if (ok_select == true )
    {
        this->crossBgColor.setAlphaF(transparency);
        this->onFontPreview( this->tableWidgetMapLegend, MAPLEGEND_CROSSCOLOR,1, this->crossFgColor, this->crossBgColor , this->crossFonfFamilly );
    }
}





void dialogAppPref::onScaleBigFontSelect()
{
    bool okButton;
    QFont mapFonfFamilly;

    mapFonfFamilly = QFontDialog::getFont(&okButton, scale1FonfFamilly);
    if (okButton)
    {
        scale1FonfFamilly = mapFonfFamilly;
        this->onFontPreview( tableWidgetMapLegend, MAPLEGEND_SCALEFIXCOLOR,1, this->scale1FgColor, this->scale1BgColor, this->scale1FonfFamilly);

    }
}
void dialogAppPref::onScaleBigFontColor()
{
    QColor newColor = selectColorBox( this->scale1FgColor );
    if (this->selectStatut == true )
    {
        this->scale1FgColor = newColor;
        this->onFontPreview( tableWidgetMapLegend, MAPLEGEND_SCALEFIXCOLOR,1, this->scale1FgColor, this->scale1BgColor, this->scale1FonfFamilly);
    }
}
void dialogAppPref::onScaleBigBgColor()
{
    QColor newColor = selectColorBox(this->scale1BgColor);
    if (this->selectStatut == true )
    {

        qreal alpha = this->scale1BgColor.alphaF();
        this->scale1BgColor = newColor;
        this->scale1BgColor.setAlphaF( alpha );

        this->onFontPreview( tableWidgetMapLegend, MAPLEGEND_SCALEFIXCOLOR,1, this->scale1FgColor, this->scale1BgColor, this->scale1FonfFamilly);
    }

}
void dialogAppPref::onScaleBigTransparency()
{

    bool ok_select = false;
    float transparency = QInputDialog::getDouble(this,"Transparence", "Entrez la transparence (0...1) :", this->scale1BgColor.alphaF(), 0, 1, 2, &ok_select); // label, value, min, max, decimales

    if (ok_select == true )
    {
        this->scale1BgColor.setAlphaF(transparency);
        this->onFontPreview( tableWidgetMapLegend, MAPLEGEND_SCALEFIXCOLOR,1, this->scale1FgColor, this->scale1BgColor, this->scale1FonfFamilly);

    }
}


void dialogAppPref::onScaleVarFontSelect()
{
    bool okButton;
    QFont mapFonfFamilly;

    mapFonfFamilly = QFontDialog::getFont(&okButton, scale2FonfFamilly);
    if (okButton)
    {
        scale2FonfFamilly = mapFonfFamilly;
        this->onFontPreview( tableWidgetMapLegend, MAPLEGEND_SCALEVARCOLOR,1, this->scale2FgColor, this->scale2BgColor, this->scale2FonfFamilly);

    }
}
void dialogAppPref::onScaleVarFontColor()
{
    QColor newColor = selectColorBox( this->scale2FgColor );
    if (this->selectStatut == true )
    {
        this->scale2FgColor = newColor;
        this->onFontPreview( tableWidgetMapLegend, MAPLEGEND_SCALEVARCOLOR,1, this->scale2FgColor, this->scale2BgColor, this->scale2FonfFamilly);
    }
}
void dialogAppPref::onScaleVarBgColor()
{
    QColor newColor = selectColorBox(this->scale2BgColor);
    if (this->selectStatut == true )
    {

        qreal alpha = this->scale2BgColor.alphaF();
        this->scale2BgColor = newColor;
        this->scale2BgColor.setAlphaF( alpha );

        this->onFontPreview( tableWidgetMapLegend, MAPLEGEND_SCALEVARCOLOR,1, this->scale2FgColor, this->scale2BgColor, this->scale2FonfFamilly);
    }

}
void dialogAppPref::onScaleVarTransparency()
{

    bool ok_select = false;
    float transparency = QInputDialog::getDouble(this,"Transparence", "Entrez la transparence (0...1) :", this->scale2BgColor.alphaF(), 0, 1, 2, &ok_select); // label, value, min, max, decimales

    if (ok_select == true )
    {
        this->scale2BgColor.setAlphaF(transparency);
        this->onFontPreview( tableWidgetMapLegend, MAPLEGEND_SCALEVARCOLOR,1, this->scale2FgColor, this->scale2BgColor, this->scale2FonfFamilly);
    }
}


// printer setting
void dialogAppPref::onMapPoiFontSelect()
{
    bool okButton;
    QFont mapFonfFamilly;

    mapFonfFamilly = QFontDialog::getFont(&okButton, mapPoiFontFamilly);
    if (okButton)
    {
        mapPoiFontFamilly = mapFonfFamilly;
        this->onFontPreview( tableWidgetLegend, PRINTER_MAPPOICOLOR ,1, this->mapPoiFontColorFg, this->mapPoiFontColorBg, this->mapPoiFontFamilly);

    }
}
void dialogAppPref::onMapPoiFontColor()
{
    QColor newColor = selectColorBox( this->mapPoiFontColorFg );
    if (this->selectStatut == true )
    {
        this->mapPoiFontColorFg = newColor;
        this->onFontPreview( tableWidgetLegend, PRINTER_MAPPOICOLOR ,1, this->mapPoiFontColorFg, this->mapPoiFontColorBg, this->mapPoiFontFamilly);
    }
}
void dialogAppPref::onMapPoiBgColor()
{
    QColor newColor = selectColorBox(this->mapPoiFontColorBg);
    if (this->selectStatut == true )
    {

        qreal alpha = this->mapPoiFontColorBg.alphaF();
        this->mapPoiFontColorBg = newColor;
        this->mapPoiFontColorBg.setAlphaF( alpha );

        this->onFontPreview( tableWidgetLegend, PRINTER_MAPPOICOLOR ,1, this->mapPoiFontColorFg, this->mapPoiFontColorBg, this->mapPoiFontFamilly);
    }

}
void dialogAppPref::onMapPoiTransparency()
{

    bool ok_select = false;
    float transparency = QInputDialog::getDouble(this,"Transparence", "Entrez la transparence (0...1) :", this->mapPoiFontColorBg.alphaF(), 0, 1, 2, &ok_select); // label, value, min, max, decimales

    if (ok_select == true )
    {
        this->mapPoiFontColorBg.setAlphaF(transparency);
        this->onFontPreview( tableWidgetLegend, PRINTER_MAPPOICOLOR ,1, this->mapPoiFontColorFg, this->mapPoiFontColorBg, this->mapPoiFontFamilly);
    }
}
// --2
void dialogAppPref::onListPoiFontSelect()
{
    bool okButton;
    QFont mapFonfFamilly;

    mapFonfFamilly = QFontDialog::getFont(&okButton, listPoiFontFamilly);
    if (okButton)
    {
        listPoiFontFamilly = mapFonfFamilly;
        this->onFontPreview( tableWidgetLegend, PRINTER_LISTPOICOLOR ,1, this->listPoiFontColorFg, this->listPoiFontColorBg, this->listPoiFontFamilly);

    }
}
void dialogAppPref::onListPoiFontColor()
{
    QColor newColor = selectColorBox( this->listPoiFontColorFg );
    if (this->selectStatut == true )
    {
        this->listPoiFontColorFg = newColor;
        this->onFontPreview( tableWidgetLegend, PRINTER_LISTPOICOLOR ,1, this->listPoiFontColorFg, this->listPoiFontColorBg, this->listPoiFontFamilly);
    }
}
void dialogAppPref::onListPoiBgColor()
{
    QColor newColor = selectColorBox(this->listPoiFontColorBg);
    if (this->selectStatut == true )
    {

        qreal alpha = this->listPoiFontColorBg.alphaF();
        this->listPoiFontColorBg = newColor;
        this->listPoiFontColorBg.setAlphaF( alpha );

        this->onFontPreview( tableWidgetLegend, PRINTER_LISTPOICOLOR ,1, this->listPoiFontColorFg, this->listPoiFontColorBg, this->listPoiFontFamilly);
    }

}
void dialogAppPref::onListPoiTransparency()
{

    bool ok_select = false;
    float transparency = QInputDialog::getDouble(this,"Transparence", "Entrez la transparence (0...1) :", this->scale2BgColor.alphaF(), 0, 1, 2, &ok_select); // label, value, min, max, decimales

    if (ok_select == true )
    {
        this->listPoiFontColorBg.setAlphaF(transparency);
        this->onFontPreview( tableWidgetLegend, PRINTER_LISTPOICOLOR ,1, this->listPoiFontColorFg, this->listPoiFontColorBg, this->listPoiFontFamilly);
    }
}

// 3

void dialogAppPref::onMapLegendFontSelect()
{
    bool okButton;
    QFont mapFonfFamilly;

    mapFonfFamilly = QFontDialog::getFont(&okButton, legendPoiFontFamilly);
    if (okButton)
    {
        legendPoiFontFamilly = mapFonfFamilly;
        this->onFontPreview( tableWidgetLegend, PRINTER_MAPLEGENDCOLOR ,1, this->legendPoiFontColorFg, this->legendPoiFontColorBg, this->legendPoiFontFamilly);

    }
}
void dialogAppPref::onMapLegendFontColor()
{
    QColor newColor = selectColorBox( this->legendPoiFontColorFg );
    if (this->selectStatut == true )
    {
        this->legendPoiFontColorFg = newColor;
        this->onFontPreview( tableWidgetLegend, PRINTER_MAPLEGENDCOLOR ,1, this->legendPoiFontColorFg, this->legendPoiFontColorBg, this->legendPoiFontFamilly);

    }
}
void dialogAppPref::onMapLegendBgColor()
{
    QColor newColor = selectColorBox(this->legendPoiFontColorBg);
    if (this->selectStatut == true )
    {

        qreal alpha = this->listPoiFontColorBg.alphaF();
        this->legendPoiFontColorBg = newColor;
        this->legendPoiFontColorBg.setAlphaF( alpha );

        this->onFontPreview( tableWidgetLegend, PRINTER_MAPLEGENDCOLOR ,1, this->legendPoiFontColorFg, this->legendPoiFontColorBg, this->legendPoiFontFamilly);

    }

}
void dialogAppPref::onMapLegendTransparency()
{

    bool ok_select = false;
    float transparency = QInputDialog::getDouble(this,"Transparence", "Entrez la transparence (0...1) :", this->scale2BgColor.alphaF(), 0, 1, 2, &ok_select); // label, value, min, max, decimales

    if (ok_select == true )
    {
        this->legendPoiFontColorBg.setAlphaF(transparency);
        this->onFontPreview( tableWidgetLegend, PRINTER_MAPLEGENDCOLOR ,1, this->legendPoiFontColorFg, this->legendPoiFontColorBg, this->legendPoiFontFamilly);

    }
}



// ------------
void dialogAppPref::onFontPreview( QTableWidget *arrayWidget, int lig, int col, QColor txtColor, QColor bgColor , QFont markerFont )
{

    QString fontMarkerPreviewValue;
    fontMarkerPreviewValue = markerFont.family() + ", " + QString("%1").arg(markerFont.pointSize());

    arrayWidget->setItem(lig,col, new QTableWidgetItem( fontMarkerPreviewValue ) );

    arrayWidget->item(lig,col)->setFont( markerFont  );
    arrayWidget->item(lig,col)->setTextColor ( txtColor );
    bgColor.setAlphaF( bgColor.alphaF());
    arrayWidget->item(lig,col)->setBackgroundColor( bgColor );

}

QColor dialogAppPref::selectColorBox( QColor currentColor )
{
    QColor mapColor = QColorDialog::getColor(currentColor, this );
    if( mapColor.isValid() )
    {
        this->selectStatut = true;
        return mapColor;
    }
    this->selectStatut = false ;
    return QColor(Qt::yellow);
}

void dialogAppPref::displayWptCol()
{
    tableWidgetColDispo->setRowCount(COL_WPT_COUNT );

    for (int j= 0 ; j < COL_WPT_COUNT ; j++ )
    {
        QCheckBox *colVisibleChk  = new QCheckBox("");
        if (localColBase[j].colvisible == true ) colVisibleChk->setChecked(true);
        tableWidgetColDispo->setCellWidget(j,0, colVisibleChk);

        tableWidgetColDispo->setItem(j,1, new QTableWidgetItem( QString("%1").arg(localColBase[j].name )) );
        tableWidgetColDispo->setItem(j,2, new QTableWidgetItem( QString("%1").arg(localColBase[j].id )) );
        tableWidgetColDispo->setItem(j,3, new QTableWidgetItem( QString("%1").arg(localColBase[j].len )) );
        tableWidgetColDispo->setItem(j,4, new QTableWidgetItem( QString("%1").arg(localColBase[j].tooltip )) );
    }
}


void dialogAppPref::ctxMenuMapDispo( const QPoint &pos )
{
    wptColLig = tableWidgetMapSrc->rowAt(pos.y());

    debugp(  "tableWidgetMapSrc menu " +  QString::number(wptColLig) );
    QMenu *menu = new QMenu;

    if (wptColLig > 0)
        menu->addAction("Haut", this, SLOT(mapDispoColTop(  )));
    if (wptColLig > 0)
        menu->addAction("Monter", this, SLOT(mapDispoColUp()));
    if (wptColLig < tableWidgetMapSrc->rowCount()-1)
        menu->addAction("Descendre", this, SLOT(mapDispoColDown()));
    if (wptColLig < tableWidgetMapSrc->rowCount()-1)
        menu->addAction("Bas", this, SLOT(mapDispoColBottom()));

    menu->addSeparator();

    menu->addAction( QString::fromUtf8("Insérer"), this, SLOT(mapDispoColInsert()));
    menu->addAction( QString::fromUtf8("Supprimer"), this, SLOT(mapDispoColSupp()));

    menu->exec(this->mapToGlobal(pos));

}
void dialogAppPref::mapDispoColSupp()
{

    int rowIndex = wptColLig ;

    QMessageBox Msgbox(this);
    Msgbox.setText( QString::fromUtf8("Supprimer cette carte ?"));
    Msgbox.setInformativeText( QString::fromUtf8("Confirmez pour suppimer : \"") + tableWidgetMapSrc->item(rowIndex,0)->text() + "\"");
    Msgbox.setStandardButtons( QMessageBox::Cancel | QMessageBox::Yes );

    //Msgbox.setIconPixmap(QPixmap(iconDir + "icon_info.png"));
    Msgbox.setWindowTitle("");
    Msgbox.setDefaultButton(QMessageBox::Cancel);


    switch( Msgbox.exec() )
    {
    case QMessageBox::Yes:
        tableWidgetMapSrc->removeRow(rowIndex);

    }

}
void dialogAppPref::mapDispoColInsert()
{

    int rowIndex = wptColLig ;

    tableWidgetMapSrc->insertRow(rowIndex);

    //updateKbShortcut();

}
void dialogAppPref::mapDispoColTop()
{
    int rowIndex = wptColLig ;

    tableWidgetMapSrc->insertRow(0);
    rowIndex++;

    tableWidgetMapSrc->setItem(0, 0, tableWidgetMapSrc->takeItem(rowIndex,0) );
    tableWidgetMapSrc->setItem(0, 1, tableWidgetMapSrc->takeItem(rowIndex,1) );
    tableWidgetMapSrc->setItem(0, 2, tableWidgetMapSrc->takeItem(rowIndex,2) );
    tableWidgetMapSrc->setItem(0, 3, tableWidgetMapSrc->takeItem(rowIndex,3) );
    tableWidgetMapSrc->setItem(0, 4, tableWidgetMapSrc->takeItem(rowIndex,4) );
    tableWidgetMapSrc->setItem(0, 5, tableWidgetMapSrc->takeItem(rowIndex,5) );
    tableWidgetMapSrc->setItem(0, 6, tableWidgetMapSrc->takeItem(rowIndex,6) );
    tableWidgetMapSrc->setItem(0, 7, tableWidgetMapSrc->takeItem(rowIndex,7) );
    tableWidgetMapSrc->setItem(0, 8, tableWidgetMapSrc->takeItem(rowIndex,8) );

    tableWidgetMapSrc->removeRow(rowIndex);

}
void dialogAppPref::mapDispoColBottom(  )
{
    int rowIndex = wptColLig ;
    int lastRow = tableWidgetMapSrc->rowCount() ;

    tableWidgetMapSrc->insertRow(lastRow);

    tableWidgetMapSrc->setItem(lastRow, 0, tableWidgetMapSrc->takeItem(rowIndex,0) );
    tableWidgetMapSrc->setItem(lastRow, 1, tableWidgetMapSrc->takeItem(rowIndex,1) );
    tableWidgetMapSrc->setItem(lastRow, 2, tableWidgetMapSrc->takeItem(rowIndex,2) );
    tableWidgetMapSrc->setItem(lastRow, 3, tableWidgetMapSrc->takeItem(rowIndex,3) );
    tableWidgetMapSrc->setItem(lastRow, 4, tableWidgetMapSrc->takeItem(rowIndex,4) );
    tableWidgetMapSrc->setItem(lastRow, 5, tableWidgetMapSrc->takeItem(rowIndex,5) );
    tableWidgetMapSrc->setItem(lastRow, 6, tableWidgetMapSrc->takeItem(rowIndex,6) );
    tableWidgetMapSrc->setItem(lastRow, 7, tableWidgetMapSrc->takeItem(rowIndex,7) );
    tableWidgetMapSrc->setItem(lastRow, 8, tableWidgetMapSrc->takeItem(rowIndex,8) );

    tableWidgetMapSrc->removeRow(rowIndex);
}
void dialogAppPref::mapDispoColUp(  )
{
    int rowIndex = wptColLig -1;

    tableWidgetMapSrc->insertRow(rowIndex + 2);

    tableWidgetMapSrc->setItem(rowIndex + 2, 0, tableWidgetMapSrc->takeItem(rowIndex,0) );
    tableWidgetMapSrc->setItem(rowIndex + 2, 1, tableWidgetMapSrc->takeItem(rowIndex,1) );
    tableWidgetMapSrc->setItem(rowIndex + 2, 2, tableWidgetMapSrc->takeItem(rowIndex,2) );
    tableWidgetMapSrc->setItem(rowIndex + 2, 3, tableWidgetMapSrc->takeItem(rowIndex,3) );
    tableWidgetMapSrc->setItem(rowIndex + 2, 4, tableWidgetMapSrc->takeItem(rowIndex,4) );
    tableWidgetMapSrc->setItem(rowIndex + 2, 5, tableWidgetMapSrc->takeItem(rowIndex,5) );
    tableWidgetMapSrc->setItem(rowIndex + 2, 6, tableWidgetMapSrc->takeItem(rowIndex,6) );
    tableWidgetMapSrc->setItem(rowIndex + 2, 7, tableWidgetMapSrc->takeItem(rowIndex,7) );
    tableWidgetMapSrc->setItem(rowIndex + 2, 8, tableWidgetMapSrc->takeItem(rowIndex,8) );

    tableWidgetMapSrc->removeRow(rowIndex);

}
void dialogAppPref::mapDispoColDown(  )
{
    int rowIndex = wptColLig ;

    tableWidgetMapSrc->insertRow(rowIndex );

    tableWidgetMapSrc->setItem(rowIndex , 0, tableWidgetMapSrc->takeItem(rowIndex+2,0) );
    tableWidgetMapSrc->setItem(rowIndex , 1, tableWidgetMapSrc->takeItem(rowIndex+2,1) );
    tableWidgetMapSrc->setItem(rowIndex , 2, tableWidgetMapSrc->takeItem(rowIndex+2,2) );
    tableWidgetMapSrc->setItem(rowIndex , 3, tableWidgetMapSrc->takeItem(rowIndex+2,3) );
    tableWidgetMapSrc->setItem(rowIndex , 4, tableWidgetMapSrc->takeItem(rowIndex+2,4) );
    tableWidgetMapSrc->setItem(rowIndex , 5, tableWidgetMapSrc->takeItem(rowIndex+2,5) );
    tableWidgetMapSrc->setItem(rowIndex , 6, tableWidgetMapSrc->takeItem(rowIndex+2,6) );
    tableWidgetMapSrc->setItem(rowIndex , 7, tableWidgetMapSrc->takeItem(rowIndex+2,7) );
    tableWidgetMapSrc->setItem(rowIndex , 8, tableWidgetMapSrc->takeItem(rowIndex+2,8) );

    tableWidgetMapSrc->removeRow(rowIndex+2);

}

void dialogAppPref::ctxMenuColDispo( const QPoint &pos )
{
    wptColLig = tableWidgetColDispo->rowAt(pos.y());

    debugp( "ctx menu " +  QString::number(wptColLig ));
    QMenu *menu = new QMenu;
    if (wptColLig > 0)
        menu->addAction("Monter", this, SLOT(wptDispoColUp()));
    if (wptColLig < COL_WPT_COUNT-1)
        menu->addAction("Descendre", this, SLOT(wptDispoColDown()));
    menu->exec(this->mapToGlobal(pos));

}
void dialogAppPref::wptDispoColUp(  )
{
    debugp("ctx menu up" +  QString::number( wptColLig));

    colsDisposition::colBaseType colBaseTmp;

    colBaseTmp.id = localColBase[wptColLig-1].id;
    colBaseTmp.name = localColBase[wptColLig-1].name;
    colBaseTmp.tooltip = localColBase[wptColLig-1].tooltip;
    colBaseTmp.len = localColBase[wptColLig-1].len;
    colBaseTmp.colvisible = localColBase[wptColLig-1].colvisible;

    localColBase[wptColLig-1].id = localColBase[wptColLig].id ;
    localColBase[wptColLig-1].name = localColBase[wptColLig].name;
    localColBase[wptColLig-1].tooltip = localColBase[wptColLig].tooltip;
    localColBase[wptColLig-1].len = localColBase[wptColLig].len;
    localColBase[wptColLig-1].colvisible = localColBase[wptColLig].colvisible;

    localColBase[wptColLig].id = colBaseTmp.id ;
    localColBase[wptColLig].name = colBaseTmp.name;
    localColBase[wptColLig].tooltip = colBaseTmp.tooltip;
    localColBase[wptColLig].len = colBaseTmp.len;
    localColBase[wptColLig].colvisible = colBaseTmp.colvisible;

    displayWptCol();

}
void dialogAppPref::wptDispoColDown(  )
{
    debugp( "ctx menu down "   +   QString::number(wptColLig ) );
    colsDisposition::colBaseType colBaseTmp;

    colBaseTmp.id = localColBase[wptColLig+1].id;
    colBaseTmp.name = localColBase[wptColLig+1].name;
    colBaseTmp.tooltip = localColBase[wptColLig+1].tooltip;
    colBaseTmp.len = localColBase[wptColLig+1].len;
    colBaseTmp.colvisible = localColBase[wptColLig+1].colvisible;

    localColBase[wptColLig+1].id = localColBase[wptColLig].id ;
    localColBase[wptColLig+1].name = localColBase[wptColLig].name;
    localColBase[wptColLig+1].tooltip = localColBase[wptColLig].tooltip;
    localColBase[wptColLig+1].len = localColBase[wptColLig].len;
    localColBase[wptColLig+1].colvisible = localColBase[wptColLig].colvisible;

    localColBase[wptColLig].id = colBaseTmp.id ;
    localColBase[wptColLig].name = colBaseTmp.name;
    localColBase[wptColLig].tooltip = colBaseTmp.tooltip;
    localColBase[wptColLig].len = colBaseTmp.len;
    localColBase[wptColLig].colvisible = colBaseTmp.colvisible;

    displayWptCol();

}

void dialogAppPref::okClicked()
{

    QKeyEvent event(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
    QApplication::sendEvent(tableWidgetAppPref, &event);

    QCheckBox *mcb;
    bool checkCell;

    mcb = qobject_cast<QCheckBox*>(tableWidgetAppPref->cellWidget(11, 1));
    if (mcb->checkState())
    {
        debugp("Raz liste fichiers recents");
        m_parent->recentFileClear();
        m_parent->recentFiles.clear();
        //m_parent->recentFileCreateMenu();
        //m_parent->recentF
    }

    mcb = qobject_cast<QCheckBox*>(tableWidgetAppPref->cellWidget(8, 1));
    if (mcb->checkState())
        checkCell = true;
    else
        checkCell = false;
    m_parent->confirmQuit = checkCell;


    mcb = qobject_cast<QCheckBox*>(tableWidgetAppPref->cellWidget(9, 1));
    if (mcb->checkState())
        checkCell = true;
    else
        checkCell = false;
    debugModeRun = checkCell;

    m_parent->clipSep = tableWidgetAppPref->item(7, 1 )->text();

    m_parent->recentFilesMax = tableWidgetAppPref->item(10, 1 )->text().toInt();

    // General
    m_parent->mapZoomToUpper = tableWidgetAppPref->item(3, 1 )->text().toInt();
    m_parent->mapZoomToLower = tableWidgetAppPref->item(4, 1 )->text().toInt();
    m_parent->dateTimeFormat = tableWidgetAppPref->item(5, 1 )->text();
    m_parent->mapGotoFromClipboard  = tableWidgetAppPref->item(6, 1 )->text();

    // Maps disposition
    QTableWidgetItem *item ;
    QString dlgMapName;
    QString dlgMapPath;
    int dlgMinzomm;
    int dlgMaxzomm;
    QString dlgMapLicence;
    QString dlgMapUseragent;
    QString dlgMapReferer;
    int dlgMapKey;
    int dlgMapKeyZoom;

    m_parent->mapChooseWidget.clearAll();
    bool oldState = m_parent->mapChooseWidget.blockSignals(true);
    debugp( "Mpp count = " + QString::number(tableWidgetMapSrc->rowCount()));

    for (int row = 0 ; row < tableWidgetMapSrc->rowCount() ; row++)
    {
        item = tableWidgetMapSrc->item(row, 0);
        dlgMapName ="[Nouvelle carte]";
        if (item && !item->text().isEmpty())
            dlgMapName = item->text();

        dlgMapPath="";
        item = tableWidgetMapSrc->item(row, 8);
        if (item && !item->text().isEmpty())
            dlgMapPath = item->text();

        dlgMinzomm=0;
        item = tableWidgetMapSrc->item(row, 3);
        if (item && !item->text().isEmpty())
            dlgMinzomm = item->text().toInt();

        dlgMaxzomm = 19;
        item = tableWidgetMapSrc->item(row, 2);
        if (item && !item->text().isEmpty())
            dlgMaxzomm = item->text().toInt();

        item = tableWidgetMapSrc->item(row, 5);
        dlgMapLicence ="";
        if (item && !item->text().isEmpty())
            dlgMapLicence = item->text();

        item = tableWidgetMapSrc->item(row, 6);
        dlgMapUseragent ="";
        if (item && !item->text().isEmpty())
            dlgMapUseragent = item->text();

        item = tableWidgetMapSrc->item(row, 7);
        dlgMapReferer ="";
        if (item && !item->text().isEmpty())
            dlgMapReferer = item->text();

        dlgMapKey=0;
        item = tableWidgetMapSrc->item(row, 3);
        if (item && !item->text().isEmpty())
            dlgMapKey = item->text().toInt();

        dlgMapKeyZoom=0;
        item = tableWidgetMapSrc->item(row, 4);
        if (item && !item->text().isEmpty())
            dlgMapKeyZoom = item->text().toInt();

        debugp("--- Add map " + dlgMapName + " #" +QString::number(row));
        debugp(" path=" + dlgMapPath);
        debugp(" min zoom=" + QString::number(dlgMinzomm));
        debugp(" maxzoom=" + QString::number(dlgMaxzomm));
        debugp("" + dlgMapLicence);
        debugp("" + dlgMapUseragent);
        debugp("" + dlgMapReferer);
        debugp(" key=" + QString::number(dlgMapKey));
        debugp(" keyzoom=" + QString::number(dlgMapKeyZoom));


        m_parent->mapChooseWidget.add(
            dlgMapName , // mapName
            dlgMapPath,
            dlgMinzomm,
            dlgMaxzomm,
            dlgMapLicence,
            dlgMapUseragent,
            dlgMapReferer,
            dlgMapKey,
            dlgMapKeyZoom
        );

        debugp("add map done " + dlgMapName);
    }
    debugp("end loop add ");
    m_parent->mapChooseWidget.blockSignals(oldState );

    // Col disposition
    for (int row = 0 ; row < COL_WPT_COUNT ; row++)
    {
        mcb = qobject_cast<QCheckBox*>(tableWidgetColDispo->cellWidget(row, 0));
        if (mcb->checkState()) checkCell = true;
        else checkCell = false;

        m_parent->tableWpt->m_colsDisposition.setColBase(
            row,
            tableWidgetColDispo->item(row, 2)->text().toInt(),
            tableWidgetColDispo->item(row, 1)->text(),
            tableWidgetColDispo->item(row, 4)->text(),
            checkCell,
            tableWidgetColDispo->item(row, 3)->text().toInt());
    }
    m_parent->tableWpt->setRowCount(0);
    m_parent->tableWpt->initDisplay();
    m_parent->tableWpt->showWpt();

    // Maps cache
    m_parent->slippyMapCacheFilePath = tableWidgetMapCache->item(0, 1)->text();

    // Search web
    m_parent->searchInternetUrl = tableWidgetWebSearch->item(0,1)->text();
    m_parent->searchInternetMapUrl = tableWidgetWebSearch->item(1,1)->text();

    // Markers
    QDir tmpDir(tableWidgetMarker->item(0,1)->text());
    m_parent->iconSymbolName=tmpDir.dirName();
    m_parent->projectData.m_iconSymbolRootDir = m_parent->iconSymbolRootDir + m_parent->iconSymbolName;
    m_parent->markers->setIconDir( m_parent->iconSymbolRootDir + m_parent->iconSymbolName);

    m_parent->markers->markerFgColor = this->markerFgColor;
    m_parent->markers->markerBgColor= this->markerBgColor;
    m_parent->markers->markerFonfFamilly = this->markerFonfFamilly;

    mcb = qobject_cast<QCheckBox*>(tableWidgetMarker->cellWidget(2, 1));
    if (mcb->checkState())
        checkCell = true;
    else
        checkCell = false;

    m_parent->setWptDisplayLabel(checkCell );
    m_parent->setMarkerFont();

    // Map legend
    m_parent->mapZoomVal->bgColor = this->zoomBgColor ;
    m_parent->mapZoomVal->fontColor = this->zoomFgColor ;
    m_parent->mapZoomVal->font = this->zoomFonfFamilly ;
    m_parent->mapZoomControl->fontColor = this->zoomFgColor;
    m_parent->mapZoomControl->bgColor = this->zoomBgColor;
    mcb = qobject_cast<QCheckBox*>(tableWidgetMapLegend->cellWidget(MAPLEGEND_ZOOMDISPLAY, 1));
    if (mcb->checkState())
        checkCell = true;
    else
        checkCell = false;
    m_parent->mapZoomVal->zoomDisplayVal = checkCell;

    mcb = qobject_cast<QCheckBox*>(tableWidgetMapLegend->cellWidget(MAPLEGEND_ZOOMCTRLDISPLAY, 1));
    if (mcb->checkState())
        checkCell = true;
    else
        checkCell = false;
    m_parent->mapZoomControl->display = checkCell;

    // Cross
    m_parent->centerMark->sizeCross = tableWidgetMapLegend->item(MAPLEGEND_CROSSSIZE,1)->text().toInt();
    mcb = qobject_cast<QCheckBox*>(tableWidgetMapLegend->cellWidget(MAPLEGEND_CROSSDISPLAY, 1));
    if (mcb->checkState())
        checkCell = true;
    else
        checkCell = false;
    m_parent->centerMark->display= checkCell;

    m_parent->centerMark->bgColor = this->crossBgColor ;
    m_parent->centerMark->fontColor = this->crossFgColor ;
    m_parent->centerMark->fontColor.setAlphaF( m_parent->centerMark->bgColor.alphaF() );
    m_parent->centerMark->font = this->crossFonfFamilly ;

    // Legend scale
    m_parent->mapScale->scale1fonfFamilly  = this->scale1FonfFamilly ;
    m_parent->mapScale->scale1bgColor = this->scale1BgColor  ;
    m_parent->mapScale->scale1fontColor = this->scale1FgColor  ;
    mcb = qobject_cast<QCheckBox*>(this->tableWidgetMapLegend->cellWidget(MAPLEGEND_SCALEFIXDISPLAY, 1));
    if (mcb->checkState())
        checkCell = true;
    else
        checkCell = false;
    m_parent->mapScale->display1 = checkCell;
    m_parent->mapScale->scale1MarginX = this->tableWidgetMapLegend->item(MAPLEGEND_SCALEFIXOFFSETX,1)->text().toInt();    // 10
    m_parent->mapScale->scale1MarginY = this->tableWidgetMapLegend->item(MAPLEGEND_SCALEFIXOFFSETY,1)->text().toInt();  // 20
    m_parent->mapScale->scale1Precision = this->tableWidgetMapLegend->item(MAPLEGEND_SCALEFIXPREC,1)->text().toInt(); ;
    m_parent->mapScale->scale1PixelLength = this->tableWidgetMapLegend->item(MAPLEGEND_SCALEFIXLEN,1)->text().toDouble();

    m_parent->mapScale->scale2fonfFamilly  = this->scale2FonfFamilly ;
    m_parent->mapScale->scale2bgColor = this->scale2BgColor  ;
    m_parent->mapScale->scale2fontColor = this->scale2FgColor  ;
    mcb = qobject_cast<QCheckBox*>(tableWidgetMapLegend->cellWidget(MAPLEGEND_SCALEVARDISPLAY, 1));
    if (mcb->checkState())
        checkCell = true;
    else
        checkCell = false;
    m_parent->mapScale->display2 = checkCell;
    m_parent->mapScale->scale2MarginX = this->tableWidgetMapLegend->item(MAPLEGEND_SCALEVAROFFSETX,1)->text().toInt();    // 10
    m_parent->mapScale->scale2MarginY = this->tableWidgetMapLegend->item(MAPLEGEND_SCALEVAROFFSETY,1)->text().toInt();  // 20
    m_parent->mapScale->scale2Precision = this->tableWidgetMapLegend->item(MAPLEGEND_SCALEVARPREC,1)->text().toInt();
    m_parent->mapScale->scale2MetersLength = tableWidgetMapLegend->item(MAPLEGEND_SCALEVARLEN,1)->text().toDouble();


    // find / replace color bg
    m_parent->foundColor = foundColor;
    m_parent->lineSelectColor = lineSelectColor;
    m_parent->lineEditColor = lineEditColor;

    m_parent->printMarginBottom =  tableWidgetLegend->item(PRINTER_MARGIN_BOTTOM,1)->text().toInt();
    m_parent->printMarginLeft = tableWidgetLegend->item(PRINTER_MARGIN_LEFT,1)->text().toInt();
    m_parent->printMarginTop = tableWidgetLegend->item(PRINTER_MARGIN_TOP,1)->text().toInt();
    m_parent->printMarginRight = tableWidgetLegend->item(PRINTER_MARGIN_RIGHT,1)->text().toInt();
    m_parent->printResolution = tableWidgetLegend->item(PRINTER_RESOLUTION,1)->text().toInt();

    mcb = qobject_cast<QCheckBox*>(tableWidgetLegend->cellWidget(PRINTER_MODEDEBUG, 1));
    if (mcb->checkState())
        checkCell = true;
    else
        checkCell = false;
    m_parent->printDebugMode = checkCell;

    mcb = qobject_cast<QCheckBox*>(tableWidgetLegend->cellWidget(PRINTER_POIMAP, 1));
    if (mcb->checkState())
        checkCell = true;
    else
        checkCell = false;
    m_parent->printMapPoi = checkCell;

    mcb = qobject_cast<QCheckBox*>(tableWidgetLegend->cellWidget(PRINTER_POILIST, 1));
    if (mcb->checkState())
        checkCell = true;
    else
        checkCell = false;
    m_parent->printListPoi = checkCell;

    mcb = qobject_cast<QCheckBox*>(tableWidgetLegend->cellWidget(PRINTER_BORDER, 1));
    if (mcb->checkState())
        checkCell = true;
    else
        checkCell = false;
    m_parent->printMapBorder =checkCell;

    m_parent->printMapPoiColorFg = this->mapPoiFontColorFg  ;
    m_parent->printMapPoiColorBg = this->mapPoiFontColorBg ;
    m_parent->printMapPoiFont = this->mapPoiFontFamilly ;
    m_parent->printListPoiColorFg = this->listPoiFontColorFg ;
    m_parent->printListPoiColorBg = this->listPoiFontColorBg ;
    m_parent->printListPoiFont = this->listPoiFontFamilly ;
    m_parent->printMapLegendColorFg = this->legendPoiFontColorFg  ;
    m_parent->printMapLegendColorBg = this->legendPoiFontColorBg ;
    m_parent->printMapLegendFont = this->legendPoiFontFamilly ;


    /*
        printMapPoiColorFg = customPrintDialog.mapColor  ;
        printMapPoiColorBg = customPrintDialog.mapBgColor  ;
        printMapPoiColorFg.alphaF() = customPrintDialog.poiMapFontTransparency->setText( QString::number() );
        printMapPoiFont = customPrintDialog.mapFonfFamilly  ;

        printListPoiColorFg = customPrintDialog.listColor  ;
        printListPoiColorBg = customPrintDialog.listBgColor  ;
        printListPoiColorFg.alphaF() = customPrintDialog.poiListFontTransparency->setText( QString::number() );
        printListPoiFont = customPrintDialog.listFonfFamilly  ;

        printMapLegendColorFg = customPrintDialog.legendColor  ;
        printMapLegendColorBg = customPrintDialog.legendBgColor  ;
        printMapLegendColorFg.alphaF() = customPrintDialog.poiMapFontTransparency->setText( QString::number() );
        printMapLegendFont = customPrintDialog.legendFonfFamilly  ;
        */

    QDialog::accept();

}

void dialogAppPref::cancelClicked()
{
    QDialog::accept();
}

qlonglong dialogAppPref::sizeOfdir(QString path)
{
    qlonglong suma = 0;

    QDir dir(path);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    if(!dir.exists())
    {
        return 1;
    }
    QFileInfoList sList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);

    foreach(QFileInfo ruta, sList)
    {
        if(ruta.isDir())
        {
            suma += sizeOfdir( ruta.path() + "/" + ruta.completeBaseName()+"/");
        }
        else
        {
            suma += ruta.size();
        }
    }
    return suma;

}

int dialogAppPref::CountFiles(QString path)
{
    int suma = 0;
    QDir dir(path);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    if(!dir.exists())
    {
        return 1;
    }
    QFileInfoList sList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);

    foreach(QFileInfo ruta, sList)
    {
        if(ruta.isDir())
        {
            suma += CountFiles(ruta.path() + "/" + ruta.completeBaseName()+"/");
        }
        suma++;
    }
    return suma;
}

#include "dialogAppPref.moc"



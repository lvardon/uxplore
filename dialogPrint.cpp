#include "global.h"
#include "dialogPrint.h"
#include "mainWindow.h"


#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QTableWidget>
#include <QFontMetrics>
#include <QtPrintSupport/QPrinterInfo>

#include "projectData.h"

dialogPrint::dialogPrint(mainWindow *parent) : QDialog(parent)
{
    resize(500, 300); // W x H

    scaleEquiv scalElem;

    scalElem.zoom = 2;
    scalElem.scalePrint = 40000;
    scalElem.scaleLib = "1/ 40 000 000";
    scaleEquivList.append( scalElem );
    scalElem.zoom = 3;
    scalElem.scalePrint = 20000;
    scalElem.scaleLib = "1/ 20 000 000";
    scaleEquivList.append( scalElem );
    scalElem.zoom = 4;
    scalElem.scalePrint = 20000;
    scalElem.scaleLib = "1/ 20 000 000";
    scaleEquivList.append( scalElem );
    scalElem.zoom = 5;
    scalElem.scalePrint = 16000;
    scalElem.scaleLib = "1/ 16 000 000";
    scaleEquivList.append( scalElem );
    scalElem.zoom = 6;
    scalElem.scalePrint = 8000;
    scalElem.scaleLib = "1/ 8 000 000";
    scaleEquivList.append( scalElem );
    scalElem.zoom = 7;
    scalElem.scalePrint = 4000;
    scalElem.scaleLib = "1/ 4 000 000";
    scaleEquivList.append( scalElem );
    scalElem.zoom = 8;
    scalElem.scalePrint = 2000;
    scalElem.scaleLib = "1/ 2 000 000";
    scaleEquivList.append( scalElem );
    scalElem.zoom = 9;
    scalElem.scalePrint = 1000 ;
    scalElem.scaleLib = "1/ 1 000 000";
    scaleEquivList.append( scalElem );
    scalElem.zoom = 10;
    scalElem.scalePrint = 400;
    scalElem.scaleLib = "1/ 400 000";
    scaleEquivList.append( scalElem );
    scalElem.zoom = 11;
    scalElem.scalePrint = 200 ;
    scalElem.scaleLib = "1/ 200 000";
    scaleEquivList.append( scalElem );
    scalElem.zoom = 12;
    scalElem.scalePrint = 100;
    scalElem.scaleLib = "1/ 100 000";
    scaleEquivList.append( scalElem );
    scalElem.zoom = 13;
    scalElem.scalePrint = 80;
    scalElem.scaleLib = "1/ 80 000";
    scaleEquivList.append( scalElem );
    scalElem.zoom = 14;
    scalElem.scalePrint = 50;
    scalElem.scaleLib = "1/ 50 000";
    scaleEquivList.append( scalElem );
    scalElem.zoom = 15;
    scalElem.scalePrint = 25;
    scalElem.scaleLib = "1/ 25 000 (topo)";
    scaleEquivList.append( scalElem );
    scalElem.zoom = 16;
    scalElem.scalePrint = 12;
    scalElem.scaleLib = "1/ 12 000";
    scaleEquivList.append( scalElem );
    scalElem.zoom = 17;
    scalElem.scalePrint = 6;
    scalElem.scaleLib = "1/ 6 000";
    scaleEquivList.append( scalElem );
    scalElem.zoom = 18;
    scalElem.scalePrint = 3;
    scalElem.scaleLib = "1/ 3 000";
    scaleEquivList.append( scalElem );
    scalElem.zoom = 19;
    scalElem.scalePrint = 1.5;
    scalElem.scaleLib = "1/ 1 500";
    scaleEquivList.append( scalElem );

    paperSizeEquiv paperSizeElem;

    paperSizeElem.paperSizeLib = "A0, 841 x 1189 mm";
    paperSizeElem.paperW=841;
    paperSizeElem.paperH=1189;
    paperSizeElem.paperSize = QPrinter::A0;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "A1, 594 x 841 mm";
    paperSizeElem.paperW=594;
    paperSizeElem.paperH=841;
    paperSizeElem.paperSize = QPrinter::A1;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "A2, 420 x 594 mm";
    paperSizeElem.paperW=420;
    paperSizeElem.paperH=594;
    paperSizeElem.paperSize = QPrinter::A2;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "A3, 297 x 420 mm";
    paperSizeElem.paperW=297;
    paperSizeElem.paperH=420;
    paperSizeElem.paperSize = QPrinter::A3;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "A4, 210 x 297 mm";
    paperSizeElem.paperW=210;
    paperSizeElem.paperH=297;
    paperSizeElem.paperSize = QPrinter::A4;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "A5, 148 x 210 mm";
    paperSizeElem.paperW=148;
    paperSizeElem.paperH=210;
    paperSizeElem.paperSize = QPrinter::A5;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "A6, 105 x 148 mm";
    paperSizeElem.paperW=105;
    paperSizeElem.paperH=148;
    paperSizeElem.paperSize = QPrinter::A6;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "A7, 74 x 105 mm";
    paperSizeElem.paperW=74;
    paperSizeElem.paperH=105;
    paperSizeElem.paperSize = QPrinter::A7;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "A8, 52 x 74 mm";
    paperSizeElem.paperW=52;
    paperSizeElem.paperH=74;
    paperSizeElem.paperSize = QPrinter::A8;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "A9, 37 x 52 mm";
    paperSizeElem.paperW=37;
    paperSizeElem.paperH=52;
    paperSizeElem.paperSize = QPrinter::A9;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "B0, 1000 x 1414 mm";
    paperSizeElem.paperW=1000;
    paperSizeElem.paperH=1414;
    paperSizeElem.paperSize = QPrinter::B0;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "B1, 707 x 1000 mm";
    paperSizeElem.paperW=707;
    paperSizeElem.paperH=1000;
    paperSizeElem.paperSize = QPrinter::B1;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "B2, 500 x 707 mm";
    paperSizeElem.paperW=500;
    paperSizeElem.paperH=707;
    paperSizeElem.paperSize = QPrinter::B2;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "B3, 353 x 500 mm";
    paperSizeElem.paperW=353;
    paperSizeElem.paperH=500;
    paperSizeElem.paperSize = QPrinter::B3;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "B4, 250 x 353 mm";
    paperSizeElem.paperW=250;
    paperSizeElem.paperH=353;
    paperSizeElem.paperSize = QPrinter::B4;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "B5, 176 x 250 mm";
    paperSizeElem.paperW=176;
    paperSizeElem.paperH=250;
    paperSizeElem.paperSize = QPrinter::B5;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "B6, 125 x 176 mm";
    paperSizeElem.paperW=125;
    paperSizeElem.paperH=176;
    paperSizeElem.paperSize = QPrinter::B6;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "B7, 88 x 125 mm";
    paperSizeElem.paperW=88;
    paperSizeElem.paperH=125;
    paperSizeElem.paperSize = QPrinter::B7;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "B8, 62 x 88 mm";
    paperSizeElem.paperW=62;
    paperSizeElem.paperH=88;
    paperSizeElem.paperSize = QPrinter::B8;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "B9, 33 x 62 mm";
    paperSizeElem.paperW=33;
    paperSizeElem.paperH=62;
    paperSizeElem.paperSize = QPrinter::B9;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "B10, 31 x 44 mm";
    paperSizeElem.paperW=31;
    paperSizeElem.paperH=44;
    paperSizeElem.paperSize = QPrinter::B10;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "C5E, 163 x 229 mm";
    paperSizeElem.paperW=163;
    paperSizeElem.paperH=229;
    paperSizeElem.paperSize = QPrinter::C5E;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "Comm10E, 105 x 241 mm";
    paperSizeElem.paperW=105;
    paperSizeElem.paperH=241;
    paperSizeElem.paperSize = QPrinter::Comm10E;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "DLE, 110 x 220 mm";
    paperSizeElem.paperW=110;
    paperSizeElem.paperH=220;
    paperSizeElem.paperSize = QPrinter::DLE;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "Executive, 190.5 x 254 mm";
    paperSizeElem.paperW=190.5;
    paperSizeElem.paperH=254;
    paperSizeElem.paperSize = QPrinter::Executive;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "Folio, 210 x 330 mm";
    paperSizeElem.paperW=210;
    paperSizeElem.paperH=330;
    paperSizeElem.paperSize = QPrinter::Folio;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "Ledger, 431.8 x 279.4 mm";
    paperSizeElem.paperW=431.8;
    paperSizeElem.paperH=279.4;
    paperSizeElem.paperSize = QPrinter::Ledger;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "Legal, 215.9 x 355.6 mm";
    paperSizeElem.paperW=215.9;
    paperSizeElem.paperH=355.6;
    paperSizeElem.paperSize = QPrinter::Legal;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "Letter, 215.9 x 279.4 mm";
    paperSizeElem.paperW=215.9;
    paperSizeElem.paperH=279.4;
    paperSizeElem.paperSize = QPrinter::Letter;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = "Tabloid, 279.4 x 431.8 mm";
    paperSizeElem.paperW=279.4;
    paperSizeElem.paperH=431.8;
    paperSizeElem.paperSize = QPrinter::Tabloid;
    paperSizeEquivList.append( paperSizeElem );
    paperSizeElem.paperSizeLib = QString::fromUtf8("Personnalisé");
    paperSizeElem.paperW=0;
    paperSizeElem.paperH=0;
    paperSizeElem.paperSize = QPrinter::Custom;
    paperSizeEquivList.append( paperSizeElem );

    m_parent = parent;

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout();

    // Ok cancel buttons
    QPushButton *button;
    QDialogButtonBox *buttonBox = new QDialogButtonBox( Qt::Horizontal );

    button = new QPushButton( "Imprimer" );
    connect( button, SIGNAL(clicked()), this, SLOT(okClicked()) );
    buttonBox->addButton( button, QDialogButtonBox::AcceptRole );

    button = new QPushButton( "Annuler" );
    connect( button, SIGNAL(clicked()), this, SLOT(cancelClicked()) );
    buttonBox->addButton( button, QDialogButtonBox::RejectRole );

    // Tab list creation
    QTabWidget *ongletsWidget = new QTabWidget(this);
    QWidget *ongletConfig = new QWidget;
    QWidget *ongletOption = new QWidget;
    QWidget *ongletColor = new QWidget;
    QWidget *ongletAdvanced = new QWidget;

    // Config tab content --------------------------------------------------------------
    QFormLayout *layoutConfig = new QFormLayout();
    layoutConfig->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    qDebug() << "Printer names via availablePrinterNames method";
    QStringList name_list = QPrinterInfo::availablePrinterNames();
    foreach ( QString name, name_list)
    {
        debugp( "Printer name = " + name );
    }
     // was necessary to add /printsupport/windowsprintersupport.dll in , the aaplet whence is launched.
    /*
    unsigned int def_index = 0;
    list_prn_info prn_list = QPrinterInfo:: availablePrinters ();
    std:: for_each (prn_list.begin (), prn_list.end (), [this, &def_index] (const QPrinterInfo& pi)
    {
        ui-> printers_comboBox-> addItem (pi.printerName ());
        if (pi.isDefault ())
        {
            def_index = ui-> printers_comboBox-> count ()-1;
        }
    });
    */
    qDebug() << "\nPrinter names via availablePrinters method";
    QList<QPrinterInfo> info_list = QPrinterInfo::availablePrinters();
    foreach ( QPrinterInfo info, info_list )
    {
        debugp( "info.printerName() = " + info.printerName() );
    }


    comboBoxPrinterList = new QComboBox;
    comboBoxPrinterList->addItem(tr("Fichier Pdf"));
    QList<QPrinterInfo> printerList=QPrinterInfo::availablePrinters();
    foreach (QPrinterInfo printerInfo, printerList)
    {
        debugp( "Printer name = " + printerInfo.printerName() );
        comboBoxPrinterList->addItem(printerInfo.printerName());
    }
    layoutConfig->addRow("Imprimante:", comboBoxPrinterList);
    comboBoxPrinterList->setCurrentIndex(0);
    connect(comboBoxPrinterList , SIGNAL(currentIndexChanged(int)),this,SLOT(onComboBoxPrinterList(int)));


    // Textbox pdf file name
    QHBoxLayout *layoutPdf = new QHBoxLayout();
    pdfName = new QLineEdit();
    layoutPdf->addWidget( pdfName);
    pdfSelect = new QPushButton("...");
    layoutPdf->addWidget(pdfSelect);
    layoutConfig->addRow("Nom du fichier Pdf:", layoutPdf);
    connect(pdfSelect, SIGNAL(clicked()), SLOT(onPdfSelect()));

    QHBoxLayout *layoutPaperSize = new QHBoxLayout();
    comboBoxPaperSize = new QComboBox;
    QList<paperSizeEquiv> paperSizeEquivListItem;
    foreach ( paperSizeEquiv paperSizeEquivListItem, paperSizeEquivList)
    {
        comboBoxPaperSize->addItem( paperSizeEquivListItem.paperSizeLib );
    }
    layoutPaperSize->addWidget(comboBoxPaperSize);
    connect(comboBoxPaperSize , SIGNAL(currentIndexChanged(int)),this,SLOT(onComboBoxPaperSize(int)));

    paperW = new QLineEdit();
    paperH = new QLineEdit();
    layoutPaperSize->addWidget(paperW);
    layoutPaperSize->addWidget(paperH);
    layoutConfig->addRow("Taille (lxh) :", layoutPaperSize);

    paperW->setText("");
    paperH->setText("");
    paperW->setDisabled(true);
    paperH->setDisabled(true);

    comboBoxPaperOrientation = new QComboBox;
    comboBoxPaperOrientation->addItem( "Paysage" );
    comboBoxPaperOrientation->addItem( "Portrait" );
    layoutConfig->addRow("Orientation:", comboBoxPaperOrientation);

    ongletConfig->setLayout(layoutConfig);

    cbprintBw = new QCheckBox("");
    layoutConfig->addRow( QString::fromUtf8("Imprimer en noir&&blanc:"), cbprintBw);

    cbOpenPdf= new QCheckBox("");
    layoutConfig->addRow( QString::fromUtf8("Ouvrir le fichier pdf:"), cbOpenPdf);

    // Option tab content --------------------------------------------------------------
    QFormLayout *layoutOption = new QFormLayout();
    layoutOption->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    comboBoxScale = new QComboBox;
    QList<scaleEquiv> scaleEquivListItem;
    foreach ( scaleEquiv scaleEquivListItem, scaleEquivList)
    {
        comboBoxScale->addItem( scaleEquivListItem.scaleLib);
    }
    layoutOption->addRow("Echelle de la carte:", comboBoxScale);
    connect(comboBoxScale , SIGNAL(currentIndexChanged(int)),this,SLOT(oncomboBoxScaleSelect(int)));


    cbMapBorderPrint = new QCheckBox("");
    cbMapBorderPrint->setCheckState(Qt::Checked);
    layoutOption->addRow( QString::fromUtf8("Imprimer le cadre:"), cbMapBorderPrint);

    cbMapPoiPrint = new QCheckBox("");
    cbMapPoiPrint->setCheckState(Qt::Checked);
    layoutOption->addRow( QString::fromUtf8("Imprimer les balises sur la carte:"), cbMapPoiPrint);

    cbPagePoi = new QCheckBox("");
    cbPagePoi->setCheckState(Qt::Checked);
    layoutOption->addRow( QString::fromUtf8("Imprimer la liste des balises:"), cbPagePoi);

    QHBoxLayout *layoutMargins = new QHBoxLayout();
    marginLeft = new QLineEdit();
    marginTop = new QLineEdit();
    marginRight = new QLineEdit();
    marginBottom = new QLineEdit();
    layoutMargins->addWidget(marginLeft);
    layoutMargins->addWidget(marginTop);
    layoutMargins->addWidget(marginRight);
    layoutMargins->addWidget(marginBottom);
    layoutOption->addRow("Marges en mm (G, H, D, B) :", layoutMargins);

    ongletOption->setLayout(layoutOption);

    // Font & color tab content --------------------------------------------------------------
    QFormLayout *layoutColor = new QFormLayout();
    layoutColor->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QHBoxLayout *layoutMapPoiColor = new QHBoxLayout();
    /*
    mapColor = QColor(0,0,0);
    mapBgColor = QColor(255,255,255);
    mapFonfFamilly.setFamily("FreeSans");
    mapFonfFamilly.setPointSize(6);
    */
    poiMapFontFamilly = new QLabel();
    poiMapFontFamilly->setFixedSize(200, 20);
    layoutMapPoiColor->addWidget(poiMapFontFamilly);
    poiMapFontFamilly->setText(mapFonfFamilly.family()  + ", " + QString::number(mapFonfFamilly.pointSize()) );

    QPushButton *mapPoiFontSelect = new QPushButton("Police");
    layoutMapPoiColor->addWidget(mapPoiFontSelect );
    connect(mapPoiFontSelect , SIGNAL(clicked()), SLOT(onMapGetFont()));

    QPushButton *mapPoiColorSelect = new QPushButton("Couleur texte");
    layoutMapPoiColor->addWidget(mapPoiColorSelect  );
    connect(mapPoiColorSelect , SIGNAL(clicked()), SLOT(onMapGetColor()));

    QPushButton *mapPoiBgColorSelect = new QPushButton("Couleur fond");
    layoutMapPoiColor->addWidget(mapPoiBgColorSelect  );
    connect(mapPoiBgColorSelect , SIGNAL(clicked()), SLOT(onMapGetBgColor()));

    poiMapFontTransparency = new QLineEdit();
    layoutMapPoiColor->addWidget(poiMapFontTransparency);


    layoutColor->addRow("Afficher les balises sur la carte", layoutMapPoiColor);


    QHBoxLayout *layoutListPoiColor = new QHBoxLayout();

    /*
        listColor = QColor(0,0,0);
        listBgColor = QColor(255,255,255);
        listFonfFamilly.setFamily("FreeSans");
        listFonfFamilly.setPointSize(10);
    */
    poiListFontFamilly = new QLabel();
    poiListFontFamilly->setFixedSize(200, 20);
    layoutListPoiColor->addWidget(poiListFontFamilly);
    poiListFontFamilly->setText(listFonfFamilly.family()  + ", " + QString::number(listFonfFamilly.pointSize()) );

    QPushButton *listPoiFontSelect = new QPushButton("Police");
    layoutListPoiColor->addWidget(listPoiFontSelect );
    connect(listPoiFontSelect , SIGNAL(clicked()), SLOT(onListGetFont()));

    QPushButton *listPoiColorSelect = new QPushButton("Couleur texte");
    layoutListPoiColor->addWidget(listPoiColorSelect  );
    connect(listPoiColorSelect , SIGNAL(clicked()), SLOT(onListGetColor()));

    QPushButton *listPoiBgColorSelect = new QPushButton("Couleur de fond");
    layoutListPoiColor->addWidget(listPoiBgColorSelect  );
    connect(listPoiBgColorSelect , SIGNAL(clicked()), SLOT(onListGetBgColor()));
    layoutColor->addRow("Liste des balises:", layoutListPoiColor);


    QHBoxLayout *layoutLegendColor = new QHBoxLayout();
    /*
    legendColor = QColor(0,0,0);
    legendBgColor = QColor(255,255,255);
    legendFonfFamilly.setFamily("FreeSans");
    legendFonfFamilly.setPointSize(10);
    */
    mapLegendFontFamilly = new QLabel();
    mapLegendFontFamilly->setFixedSize(200, 20);
    layoutLegendColor->addWidget(mapLegendFontFamilly);
    mapLegendFontFamilly->setText(legendFonfFamilly.family()  + ", " + QString::number(legendFonfFamilly.pointSize()) );

    QPushButton *legendFontSelect = new QPushButton("Police");
    layoutLegendColor->addWidget(legendFontSelect  );
    connect(legendFontSelect  , SIGNAL(clicked()), SLOT(onLegendGetFont()));

    QPushButton *legendColorSelect = new QPushButton("Couleur texte");
    layoutLegendColor->addWidget(legendColorSelect  );
    connect(legendColorSelect, SIGNAL(clicked()), SLOT(onLegendGetColor()));

    QPushButton *legendBgColorSelect = new QPushButton("Couleur fond");
    layoutLegendColor->addWidget(legendBgColorSelect  );
    connect(legendBgColorSelect, SIGNAL(clicked()), SLOT(onLegendGetBgColor()));
    layoutColor->addRow( QString::fromUtf8("Légende"), layoutLegendColor);


    ongletColor->setLayout(layoutColor);

    // Advanced tab content --------------------------------------------------------------
    QFormLayout *layoutAdvanced = new QFormLayout();
    layoutAdvanced->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    printerDpi = new QLineEdit();
    printerDpi->setMaxLength(5);
    layoutAdvanced->addRow( QString::fromUtf8("Résolution :"), printerDpi);

    scalePrint = new QLineEdit();
    scalePrint->setMaxLength(5);
    layoutAdvanced->addRow( QString::fromUtf8("Echelle :"), scalePrint);

    cbDebugMode = new QCheckBox("");
    layoutAdvanced->addRow( QString::fromUtf8("Mode debug:"), cbDebugMode);

    ongletAdvanced->setLayout(layoutAdvanced);


    // Add tabs
    ongletsWidget->addTab(ongletConfig, QString::fromUtf8("Imprimante"));
    ongletsWidget->addTab(ongletOption, QString::fromUtf8("Options"));
    ongletsWidget->addTab(ongletColor, QString::fromUtf8("Police"));
    ongletsWidget->addTab(ongletAdvanced, QString::fromUtf8("Avancé"));

    mainLayout->addWidget(ongletsWidget);

    // Final layout
    mainLayout->addWidget( buttonBox );
    setLayout(mainLayout);

    setWindowTitle( QString::fromUtf8("Imprimer la carte"));

    //setDefaultValues();


    scaleFactor = 1;

}

void dialogPrint::showEvent( QShowEvent* event )
{

    QWidget::showEvent( event );

    onMapFontPreview( poiMapFontFamilly, mapColor, mapBgColor , mapFonfFamilly );
    onMapFontPreview( poiListFontFamilly, listColor, listBgColor , listFonfFamilly );
    onMapFontPreview( mapLegendFontFamilly, legendColor, legendBgColor , legendFonfFamilly );
    poiMapFontTransparency->setText(  QString::number(mapBgColor.alphaF(), 'g', 2) );

    comboBoxPaperSize->setCurrentIndex(4);
}

void dialogPrint::setDefaultValues( void )
{


    /*
    marginLeft->setText("10");
    marginTop->setText("10");
    marginRight->setText("10");
    marginBottom->setText("10");

    printerDpi->setText("1200");

    comboBoxScale->setCurrentIndex( getScaleIndexByZoom(10)  );
    scalePrint->setText( QString::number(getScalePrintByZoom(10) , 'f', 5) );


    QPalette sample_palette;

    sample_palette.setColor(QPalette::Window, mapBgColor);
    sample_palette.setColor(QPalette::WindowText, mapColor);
    poiMapFontFamilly->setAutoFillBackground(true);
    poiMapFontFamilly->setPalette(sample_palette);
    poiMapFontTransparency->setText("0");

    sample_palette.setColor(QPalette::Window, listBgColor);
    sample_palette.setColor(QPalette::WindowText, listColor);
    poiListFontFamilly->setAutoFillBackground(true);
    poiListFontFamilly->setPalette(sample_palette);

    sample_palette.setColor(QPalette::Window, legendBgColor);
    sample_palette.setColor(QPalette::WindowText, legendColor);
    mapLegendFontFamilly->setAutoFillBackground(true);
    mapLegendFontFamilly->setPalette(sample_palette);

    poiMapFontTransparency->setText("0.85");
    */
}

int dialogPrint::getScaleIndexByZoom( int zoom )
{

    for( int i=0; i<scaleEquivList.count(); ++i )
    {
        if (zoom == scaleEquivList[i].zoom ) return i;
    }
    return 0;
}
float dialogPrint::getScalePrintByZoom( int zoom )
{

    for( int i=0; i<scaleEquivList.count(); ++i )
    {
        if (zoom == scaleEquivList[i].zoom ) return scaleEquivList[i].scalePrint;
    }
    return 1;
}
float dialogPrint::getScalePrintByIndex( int index )
{

    for( int i=0; i<scaleEquivList.count(); ++i )
    {
        if (index == i ) return scaleEquivList[i].scalePrint;
    }
    return 1;
}

void dialogPrint::setScalePrintByZoom( int zoom )
{

    comboBoxScale->setCurrentIndex( getScaleIndexByZoom(zoom)  );
}

void dialogPrint::onPdfSelect()
{
    QString filters = QString("Fichiers Pdf (*.pdf);;Text files (*.txt);;All files (*.*)");
    QString defaultFilter = QString("Fichiers Pdf (*.pdf)");
    QString currentpdfMapName = pdfName->text();

    QString fileSaveName = QFileDialog::getSaveFileName(this, QString::fromUtf8("Fichier Pdf à créer"), currentpdfMapName,filters, &defaultFilter);

    if ( fileSaveName != QString::null)
        pdfName->setText( fileSaveName );

}

void dialogPrint::oncomboBoxScaleSelect (int index)
{
    scalePrint->setText( QString::number( getScalePrintByIndex(index) , 'f', 5 ) );

}

void dialogPrint::onComboBoxPrinterList(int index)
{
    if (index==0 )
    {
        pdfName->setDisabled(false);
        pdfSelect->setDisabled(false);
        cbOpenPdf->setDisabled(false);

    }
    else
    {
        pdfName->setDisabled(true);
        pdfSelect->setDisabled(true);
        cbOpenPdf->setDisabled(true);
    }

}


void dialogPrint::onComboBoxPaperSize(int index)
{

    paperW->setText( QString::number(paperSizeEquivList[index].paperW ));
    paperH->setText( QString::number(paperSizeEquivList[index].paperH ));

    if (paperSizeEquivList[index].paperSize == QPrinter::Custom )
    {
        paperW->setDisabled(false);
        paperH->setDisabled(false);

    }
    else
    {
        paperW->setDisabled(true);
        paperH->setDisabled(true);
    }

}

void dialogPrint::onMapGetFont( void )
{
    bool ok;
    QFont newFont;
    newFont = QFontDialog::getFont(&ok, mapFonfFamilly);
    if (ok)
    {
        mapFonfFamilly = newFont;
        onMapFontPreview( poiMapFontFamilly, mapColor, mapBgColor , mapFonfFamilly );
    }

}
void dialogPrint::onMapGetColor( void )
{
    QColor newColor;
    newColor = QColorDialog::getColor(mapColor, this );
    if( newColor.isValid() )
    {
        mapColor = newColor;
        onMapFontPreview( poiMapFontFamilly,mapColor, mapBgColor , mapFonfFamilly );
    }

}
void dialogPrint::onMapGetBgColor( void )
{
    QColor newColor;
    newColor = QColorDialog::getColor(mapBgColor, this );
    if( newColor.isValid() )
    {
        mapBgColor = newColor;
        onMapFontPreview( poiMapFontFamilly,mapColor, mapBgColor , mapFonfFamilly );
    }

}

void dialogPrint::onMapFontPreview( QLabel*widget,  QColor txtColor, QColor bgColor , QFont markerFont )
{

    QPalette sample_palette;
    sample_palette.setColor(QPalette::Window, bgColor);
    sample_palette.setColor(QPalette::WindowText, txtColor);

    widget->setAutoFillBackground(true);
    widget->setPalette(sample_palette);
    widget->setText(markerFont.family()  + ", " + QString::number(markerFont.pointSize()) );
    widget->setFont(markerFont);


    /*
    poiMapFontFamilly->setAutoFillBackground(true);
    poiMapFontFamilly->setPalette(sample_palette);
    poiMapFontFamilly->setText(markerFont.family()  + ", " + QString::number(markerFont.pointSize()) );
    poiMapFontFamilly->setFont(markerFont);
    */

}



void dialogPrint::onListGetFont( void )
{
    bool ok;
    listFonfFamilly = QFontDialog::getFont(&ok, listFonfFamilly);
    if (ok)
    {
        poiListFontFamilly->setText(listFonfFamilly.family()  + ", " + QString::number(listFonfFamilly.pointSize()) );
        poiListFontFamilly->setFont(listFonfFamilly);

    }

}
void dialogPrint::onListGetColor( void )
{
    listColor = QColorDialog::getColor(Qt::yellow, this );
    if( listColor.isValid() )
    {
        QPalette sample_palette;
        sample_palette.setColor(QPalette::Window, listBgColor);
        sample_palette.setColor(QPalette::WindowText, listColor);
        poiListFontFamilly->setAutoFillBackground(true);
        poiListFontFamilly->setPalette(sample_palette);
    }

}
void dialogPrint::onListGetBgColor( void )
{
    listBgColor = QColorDialog::getColor(Qt::yellow, this );
    if( listBgColor.isValid() )
    {
        QPalette sample_palette;
        sample_palette.setColor(QPalette::Window, listBgColor);
        sample_palette.setColor(QPalette::WindowText, listColor);
        poiListFontFamilly->setAutoFillBackground(true);
        poiListFontFamilly->setPalette(sample_palette);
    }

}

void dialogPrint::onLegendGetFont( void )
{
    bool ok;
    legendFonfFamilly = QFontDialog::getFont(&ok, legendFonfFamilly);
    if (ok)
    {
        mapLegendFontFamilly->setText(legendFonfFamilly.family()  + ", " + QString::number(legendFonfFamilly.pointSize()) );
        mapLegendFontFamilly->setFont(legendFonfFamilly);

    }

}

void dialogPrint::onLegendGetColor( void )
{
    legendColor = QColorDialog::getColor(Qt::yellow, this );
    if( listColor.isValid() )
    {
        QPalette sample_palette;
        sample_palette.setColor(QPalette::Window, legendBgColor);
        sample_palette.setColor(QPalette::WindowText, legendColor);
        mapLegendFontFamilly->setAutoFillBackground(true);
        mapLegendFontFamilly->setPalette(sample_palette);
    }

}
void dialogPrint::onLegendGetBgColor( void )
{
    legendBgColor = QColorDialog::getColor(Qt::yellow, this );
    if( listBgColor.isValid() )
    {
        QPalette sample_palette;
        sample_palette.setColor(QPalette::Window, legendBgColor);
        sample_palette.setColor(QPalette::WindowText, legendColor);
        mapLegendFontFamilly->setAutoFillBackground(true);
        mapLegendFontFamilly->setPalette(sample_palette);
    }

}
double dialogPrint::mmToPixels(QPrinter *printer, int mm)
{

    return mm * 0.039370147 * printer->resolution();
}

double dialogPrint::pixelsTomm(QPrinter* printer, int pixels)
{
    return pixels / ( printer->resolution()  * 0.039370147 ) ;
}

double dialogPrint::long2tilex(double lon, int z)
{
    return (double)((lon + 180.0) / 360.0 * pow(2.0, z));
}

double dialogPrint::lat2tiley(double lat, int z)
{
    return (double)((1.0 - log( tan(lat * M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, z));
}

QPointF dialogPrint::mapProject ( double lat,  double lon , int zoom, int tileSize )
{

    QPointF retPos;

    double tileX = long2tilex( lon, zoom );
    double tileY = lat2tiley( lat, zoom );

    retPos.setX( tileX );
    retPos.setY( tileY );

    return retPos;
}

void dialogPrint::doPrint( void )
{

    debugp("--------------- Start printing ------" );

    uiPause( true );

    QPrinter printer(QPrinter::HighResolution);
    setupPrinter( &printer );
    QPainter painter( &printer );
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    setupScale( &painter );

    setupBorder( &printer );
    setupSlippyMap( &printer );

    painter.save();
    painter.scale( scaleFactor , scaleFactor );
    renderMap( &printer, &painter );

    renderBorder( &printer, &painter );
    painter.restore();

    setupPrintData(); // Current date, number of poi

    renderLegend( &printer, &painter );

    QRectF corderCoo = QRectF( QPointF(topLeftX, topLeftY) , QPointF(bottomRightX, bottomRightY) );
    painter.save();
    painter.setClipRect(corderCoo);
    renderPoiText( &printer, &painter );
    painter.restore();

    renderPoiList( &printer, &painter );

    painter.end();
    uiPause( false );
    QDialog::accept();

    debugp( "--------------- End printing ------" );


}

void dialogPrint::uiPause( bool uiState  )
{
    if (uiState == true)
    {
        this->setCursor(Qt::WaitCursor);
        slippyMap->PrintingMap  = true;

        progress.setWindowModality(Qt::ApplicationModal);
        progress.setLabelText("Impression de la carte en cours ...");
        progress.setCancelButton(0);
        progress.setRange(0,0);
        progress.setMinimumDuration(0);
        progress.show();
    }
    else
    {
        slippyMap->PrintingMap  = false;
        progress.cancel();
        this->setCursor(Qt::ArrowCursor);
    }
}

bool dialogPrint::cancelClicked()
{
    close();
    return false;
}
bool dialogPrint::okClicked()
{

    QDialog::accept();
    return true;

}

void dialogPrint::setupBorder( QPrinter *printer)
{

    topLeftX =  ( 0 + mmToPixels( printer, marginTop->text().toInt() ) ) / scaleFactor;
    topLeftY =  ( 0 + mmToPixels( printer,marginLeft->text().toInt() ) ) / scaleFactor;
    topRightX = ( printer->pageRect(QPrinter::DevicePixel).width() - mmToPixels( printer, marginTop->text().toInt() ) )  / scaleFactor;
    topRightY = ( 0 + mmToPixels( printer, marginLeft->text().toInt()) ) / scaleFactor;;
    bottomLeftX =  ( 0 + mmToPixels( printer,marginTop->text().toInt() ) )  / scaleFactor; ;
    bottomLeftY =  ( printer->pageRect(QPrinter::DevicePixel).height() - mmToPixels( printer,marginLeft->text().toInt() )) / scaleFactor;
    bottomRightX = ( printer->pageRect(QPrinter::DevicePixel).width() - mmToPixels( printer,marginTop->text().toInt() ) ) /scaleFactor;
    bottomRightY = ( printer->pageRect(QPrinter::DevicePixel).height() - mmToPixels( printer,marginLeft->text().toInt() ) )/scaleFactor  ;

    debugp( "Page wxh " +  QString::number( printer->pageRect(QPrinter::DevicePixel).width() ) + " x " + QString::number( printer->pageRect(QPrinter::DevicePixel).height()) );
    debugp( "Border : " +  QString::number(bottomLeftX) + "," +  QString::number(bottomLeftY) + " - " +  QString::number(bottomRightX) +"," +  QString::number(bottomRightY) );

    mapBorderCoordinate = QRectF( QPointF(topLeftX, topLeftY) , QPointF(bottomRightX, bottomRightY) );

}

void dialogPrint::setupSlippyMap( QPrinter *printer  )
{

    const QSizeF pageSize = printer->paperRect().size();
    double pageWidthMm = pixelsTomm( printer, pageSize.width() );
    double pageHeightMm = pixelsTomm( printer, pageSize.height() );

    slippyMap->printerPageW = mmToPixels( printer, pageWidthMm);
    slippyMap->printerPageH = mmToPixels( printer, pageHeightMm);

    slippyMap->pageW = pageWidthMm;
    slippyMap->pageH = pageHeightMm;
    slippyMap->printCenter = false;
    slippyMap->printPoi = true;
    slippyMap->printScale = false;
    slippyMap->printZoom = false;
    slippyMap->printZoomVal = false;
    slippyMap->printBox = false;
    slippyMap->printTile = true;
}

void dialogPrint::setupScale(QPainter *painter)
{
    QRectF r = slippyMap->getArea();
    QPointF srcPoint;
    srcPoint =  QPointF( r.left(), r.bottom());
    QPointF dstPoint;
    dstPoint =  QPointF( r.right(), r.bottom());
    double mapSideDistance  = Great_Circle_Distance( srcPoint, dstPoint ); // Metre
    scaleFactor = ( ( mapSideDistance * ( printerDpi->text().toInt()  / 2.54) ) ) / ( 10 * scalePrint->text().toFloat() * slippyMap->width() )  ;
    scaleLen = ( mapSideDistance / slippyMap->width()  ) * ( 1 * ( printerDpi->text().toInt()  / 2.54) / scaleFactor );

}

void dialogPrint::setupPrinter(QPrinter *printer)
{
    int comboIndex;

    comboIndex = comboBoxPrinterList->currentIndex();
    if (comboIndex == 0 )
    {
        printer->setOutputFormat(QPrinter::PdfFormat);
        printer->setDocName(pdfName->text());
        printer->setOutputFileName(pdfName->text());
    }

    comboIndex =comboBoxPaperSize->currentIndex();
    if ( paperSizeEquivList[comboIndex].paperSize != QPrinter::Custom )
    {
        printer->setPaperSize( paperSizeEquivList[comboIndex].paperSize );
    }
    else
    {
        printer->setPaperSize(QSizeF( paperSizeEquivList[comboIndex].paperW , paperSizeEquivList[comboIndex].paperH), QPrinter::Millimeter);
    }

    comboIndex = comboBoxPaperOrientation->currentIndex();
    if (comboIndex == 0)
        printer->setOrientation( QPrinter::Landscape);
    else
        printer->setOrientation( QPrinter::Portrait);

    if (cbprintBw->isChecked() )
    {
        printer->setColorMode( QPrinter::GrayScale );
    }

    printer->setResolution(printerDpi->text().toInt());
    printer->setPageMargins(marginLeft->text().toInt(), marginTop->text().toInt(), marginRight->text().toInt(), marginBottom->text().toInt(),QPrinter::Millimeter);
    printer->setFullPage( true );


}

void dialogPrint::setupPrintData()
{

    int PoiOnMap = 0;

    projectDatas::wptType *m_wpt;

    poiCount = 0;

    for(int rowData = 0; rowData < projectData->m_wptList.size(); rowData++)
    {
        m_wpt = projectData->getWptFromId2( rowData +1 );
        if (m_wpt==NULL)
        {
            debugp("m_wpt is NULL");
            continue;
        }

        if (m_wpt->m_selected == false) continue;

        PoiOnMap = 0;
        if ( m_wpt->m_lat.toDouble() <= slippyMap->printedTopLeft.ry() && m_wpt->m_lat.toDouble() >= slippyMap->printedBottomRight.ry() )
            PoiOnMap +=2;
        if ( m_wpt->m_lon.toDouble() >= slippyMap->printedTopLeft.rx() && m_wpt->m_lon.toDouble() <= slippyMap->printedBottomRight.rx() )
            PoiOnMap +=4;

        if (PoiOnMap == 6) poiCount++;
    }


    QDate cd = QDate::currentDate();
    QTime ct = QTime::currentTime();
    printDate = QString (" %1 %2").arg( cd.toString("dd MMM yyyy"), ct.toString("hh:mm") ) ;

}

void dialogPrint::renderMap(QPrinter *printer, QPainter *painter)
{

    int drawTileBorder = 1;

    slippyMap->render( painter,
                       mmToPixels(printer, marginLeft->text().toInt()), mmToPixels(printer,marginTop->text().toInt()), mmToPixels(printer,marginRight->text().toInt()), mmToPixels(printer,marginBottom->text().toInt()), // margins
                       printerDpi->text().toInt(), // printer resolution (dpi)
                       scaleFactor, scaleLen,
                       slippyMap->getZoom(),
                       drawTileBorder, // draw over margin (bool)
                       printer->pageRect().width(), printer->pageRect().height() ,
                       mapBorderCoordinate, // map border coordinate (QRectF)
                       cbDebugMode->isChecked()
                     );

}

void dialogPrint::renderPoiText(QPrinter *printer, QPainter *painter)
{

    QString lineToPrintPoi;
    QRectF rLegendCursorPoi;
    QPoint offsetPixmap;
    QPoint offsetText;
    QRect currentTextPoi;
    QString strKey("offsetPrint/");
    QFont fontPoi ;

    if (cbMapPoiPrint->isChecked() == false)  return;

    QSettings * settings = 0;
    settings = new QSettings( iconFullPath + "/symbol.ini", QSettings::IniFormat );

    debugp("Icons full path = " + iconFullPath + "/symbol.ini" );
    debugp("Page WxH " +   QString::number(printer->pageRect(QPrinter::DevicePixel).width()) + " x " +  QString::number(printer->pageRect(QPrinter::DevicePixel).height() ) );

    topLeftX =  mmToPixels( printer, marginTop->text().toInt() )  ;
    topLeftY =  mmToPixels( printer,marginLeft->text().toInt() )  ;

    debugp( "topleft X,Y = " +   QString::number(topLeftX) + "," +  QString::number(topLeftY));

    int X = settings->value( strKey + "icoX", 0).toInt();
    int Y = settings->value( strKey + "icoY", 0).toInt();
    offsetPixmap = QPoint(X,Y);
    debugp( "offset Text X,Y = " + QString::number(X) + "," +  QString::number(Y));

    X = settings->value( strKey + "txtX", 0).toInt();
    Y = settings->value( strKey + "txtY", 0).toInt();
    debugp("offset Icon X,Y = " + QString::number(X) + "," + QString::number(Y));
    offsetText = QPoint(X,Y);

    fontPoi = QFont( mapFonfFamilly.family(), mapFonfFamilly.pointSize() );
    fontPoi.setPointSize( mapFonfFamilly.pointSize()  );

    painter->setPen(QPen( mapColor ));
    painter->setFont( fontPoi );

    debugp("map scaleFactor = " + QString::number( scaleFactor) );

    int PoiOnMap = 0;
    projectDatas::wptType *m_wpt;

    painter->setOpacity( poiMapFontTransparency->text().toFloat());
    for(int rowData = 0; rowData < projectData->m_wptList.size(); rowData++)
    {
        m_wpt = projectData->getWptFromId2( rowData +1 );

        if (m_wpt==NULL)
        {
            debugp("m_wpt is NULL");
            continue;
        }

        if (m_wpt->m_selected == false) continue;

        PoiOnMap = 0;
        if ( m_wpt->m_lat.toDouble() <= slippyMap->printedTopLeft.ry() && m_wpt->m_lat.toDouble() >= slippyMap->printedBottomRight.ry() )
            PoiOnMap +=2;
        if ( m_wpt->m_lon.toDouble() >= slippyMap->printedTopLeft.rx() && m_wpt->m_lon.toDouble() <= slippyMap->printedBottomRight.rx() )
            PoiOnMap +=4;

        if (PoiOnMap == 6)
        {
            debugp( "Poi lat,lon="  + QString::number( m_wpt->m_lat.toDouble()) + "," + QString::number( m_wpt->m_lon.toDouble())  +  "zoom=" +  QString::number( slippyMap->getZoom()) );
            QPointF posPoint = mapProject( m_wpt->m_lat.toDouble(), m_wpt->m_lon.toDouble(), slippyMap->getZoom() , 256); // x,y = Tile ids
            debugp("Poi pos x,y=" + QString::number( posPoint.x()) + "," +  posPoint.y() );
            int tileX =  floor(posPoint.x() ) ;
            int tileY =  floor(posPoint.y() ) ;
            double poiDecalX = (posPoint.x() - floor(posPoint.x() ) ) * 256 ;
            double poiDecalY = (posPoint.y() - floor(posPoint.y() ) ) * 256 ;
            QPointF posTile = slippyMap->getPrintMapTileXY(tileX, tileY, slippyMap->getZoom());
            debugp( "pos tile tileX,Y =" + QString::number( tileX ) + "," + QString::number( tileY) +" -> @" + QString::number( posTile.x()) + "," +QString::number(  posTile.y()) ) ;
            debugp( "offsetPixmap.r = " + QString::number( offsetPixmap.rx()) + "," + QString::number(  offsetPixmap.ry())  + " -" + QString::number( mmToPixels(printer,offsetPixmap.rx())) + "," + QString::number( mmToPixels(printer,offsetPixmap.ry())));
            debugp( "offsetText.r = " + QString::number( offsetText.rx()) + "," + QString::number( offsetText.ry()) + " -" + QString::number( mmToPixels(printer, offsetText.rx())) + "," + QString::number( mmToPixels(printer, offsetText.ry()) ));
            lineToPrintPoi = m_wpt->m_name + " ";

            QPixmap pixmapPng( iconFullPath + "/" + m_wpt->m_sym + ".png");
            debugp( "pix file = " + iconFullPath + "/" + m_wpt->m_sym + ".png" );

            QFileInfo checkFile(iconFullPath + "/" + m_wpt->m_sym + ".png");
            if ( !checkFile.exists() || !checkFile.isFile())
            {
                pixmapPng.load( iconFullPath + "/default.png");
            }

            //painter->setOpacity(0.5);
            //painter->setOpacity(poiMapFontTransparency->text().toFloat()));
            painter->drawPixmap (
                topLeftX + (posTile.x() * scaleFactor) + (poiDecalX * scaleFactor) + (offsetPixmap.rx()*scaleFactor) , //+  ((posTile.x() + poiDecalX) *scaleFactor) + (offsetPixmap.rx()*scaleFactor) , // mmToPixels(printer,offsetPixmap.rx()) ,
                topLeftY + (posTile.y() * scaleFactor) + (poiDecalY * scaleFactor) + (offsetPixmap.ry()*scaleFactor) , //+  ((posTile.y() + poiDecalY) *scaleFactor) + (offsetPixmap.ry()*scaleFactor) , // mmToPixels(printer,offsetPixmap.ry()) ,
                pixmapPng.width() * scaleFactor * 1, pixmapPng.height() * scaleFactor * 1,
                pixmapPng);
            //painter->setOpacity(1.00);

            if (m_parent->markers->markerDisplayName == true )
            {
                currentTextPoi = painter->fontMetrics().boundingRect(lineToPrintPoi);

                rLegendCursorPoi = QRect( topLeftX + ((posTile.x() + poiDecalX) *scaleFactor) + (offsetText.rx()*scaleFactor) , //mmToPixels(printer, offsetText.rx()),
                                          topLeftY + ((posTile.y() + poiDecalY) *scaleFactor) - currentTextPoi.height() + painter->fontMetrics().height() + (offsetText.ry()*scaleFactor) , // mmToPixels(printer, offsetText.ry()) - currentTextPoi.height() + painter->fontMetrics().height() ,
                                          currentTextPoi.width() ,
                                          currentTextPoi.height());

                painter->drawText( rLegendCursorPoi ,Qt::AlignVCenter, lineToPrintPoi);
                //painter->fillRect( rLegendCursorPoi, QBrush(QColor(10, 100, 200, 50)));
                painter->fillRect( rLegendCursorPoi, mapBgColor);
                debugp("draw text " +lineToPrintPoi + " @" + QString::number( rLegendCursorPoi.x()) + "," + QString::number( rLegendCursorPoi.y()));
            }

        }
    }
    painter->setOpacity(1);


}
void dialogPrint::renderBorder(QPrinter *printer, QPainter *painter)
{
    if (cbMapBorderPrint->isChecked() == false)  return;

    bool overrideMargin = true;

    if (overrideMargin==true)
    {
        painter->fillRect( 0,0, printer->pageRect(QPrinter::DevicePixel).width()/scaleFactor,topRightY , Qt::white); // top
        painter->fillRect( 0,0, topLeftX, bottomLeftY , Qt::white); // left
        painter->fillRect( 0, bottomLeftY , printer->pageRect(QPrinter::DevicePixel).width()/scaleFactor, printer->pageRect(QPrinter::DevicePixel).height()/scaleFactor, Qt::white); // bottom
        painter->fillRect( topRightX, topRightY , printer->pageRect(QPrinter::DevicePixel).width()/scaleFactor, printer->pageRect(QPrinter::DevicePixel).height()/scaleFactor, Qt::white); // right
    }

    // Horizontal top
    painter->setPen(QPen(QColor("black")));
    painter->drawLine( topLeftX , topLeftY,  topRightX, topRightY);

    // left vertical
    painter->setPen(QPen(QColor("black")));
    painter->drawLine( topLeftX , topLeftY,  bottomLeftX, bottomLeftY );

    // Right vertical
    painter->setPen(QPen(QColor("black")));
    painter->drawLine( topRightX, topRightY, bottomRightX, bottomRightY);

    // Horizontal bottom
    painter->setPen(QPen(QColor("black")));
    painter->drawLine( bottomLeftX , bottomLeftY,  bottomRightX, bottomRightY);
}

void dialogPrint::renderLegend(QPrinter *printer, QPainter *painter)
{
    // Legend Common vars
    QRect rLegendCursor;
    QRect currentText;
    QFont fontLegend ;
    QString lineToPrint;
    int lastTxtX = 0;

    qreal marginLeftMm;
    qreal marginRightMm;
    qreal marginTopMm;
    qreal marginBottomMm;

    marginLeftMm = pixelsTomm( printer, marginLeft->text().toInt());
    marginRightMm = pixelsTomm( printer, marginRight->text().toInt());
    marginTopMm = pixelsTomm( printer, marginTop->text().toInt());
    marginBottomMm = pixelsTomm( printer, marginBottom->text().toInt());

    topLeftX =  ( 0 + mmToPixels( printer, marginTop->text().toInt() ) ) ;
    topLeftY =  ( 0 + mmToPixels( printer,marginLeft->text().toInt() ) ) ;
    topRightX = ( printer->pageRect(QPrinter::DevicePixel).width() - mmToPixels( printer, marginTop->text().toInt() ) )  ;
    topRightY = ( 0 + mmToPixels( printer, marginLeft->text().toInt()) ) ;
    bottomLeftX =  ( 0 + mmToPixels( printer,marginTop->text().toInt() ) )  ;
    bottomLeftY =  ( printer->pageRect(QPrinter::DevicePixel).height() - mmToPixels( printer,marginLeft->text().toInt() )) ;
    bottomRightX = ( printer->pageRect(QPrinter::DevicePixel).width() - mmToPixels( printer,marginTop->text().toInt() ) ) ;
    bottomRightY = ( printer->pageRect(QPrinter::DevicePixel).height() - mmToPixels( printer,marginLeft->text().toInt() ) );

    lastTxtX = bottomLeftX;

    fontLegend = QFont( legendFonfFamilly.family(), legendFonfFamilly.pointSize() );
    fontLegend.setPointSize( legendFonfFamilly.pointSize()  );

    painter->setPen(QPen( legendColor ));
    painter->setFont( fontLegend );

    // Print scale value print
    /*char lineToPrintChar[255];
    sprintf( lineToPrintChar , "1/%'.0f  ", (scaleLen /10 ) * 1000 );
    lineToPrint  = lineToPrintChar;
    */
    lineToPrint = QString("1/%1  ").arg((scaleLen /10 ) * 1000 , 0, 'f', 0) ;

    currentText = painter->fontMetrics().boundingRect(lineToPrint);
    rLegendCursor = QRect( lastTxtX + 0, bottomLeftY, currentText.width() , currentText.height());
    lastTxtX += currentText.width();
    painter->fillRect( rLegendCursor, QBrush(QColor(0, 0, 0, 0)));
    painter->drawText( rLegendCursor, Qt::AlignVCenter| Qt::AlignLeft, lineToPrint );

    // Graphic scale ladder compute len pixels
    int lenFor1cm =  mmToPixels( printer, 10) ; // 1 * ( (printerResolution  / 2.54) )  / scaleFactor ) ; // length for 1cm in pixel unit

    // Graphic scale draw (1cm)
    int lineHeight = painter->fontMetrics().height() ;
    int halfLineHeight = painter->fontMetrics().height()/2 ;
    int HlineThick = 4*4;
    int VlineThick = 2*4;
    int VlineReduce = 5*4;
    painter->setBrush(Qt::VerPattern);
    painter->drawRect(lastTxtX+1, bottomLeftY + halfLineHeight, lenFor1cm, HlineThick);
    painter->fillRect(lastTxtX, bottomLeftY+VlineReduce , VlineThick , lineHeight-VlineReduce, QBrush(QColor(0,0,0,255))); // Left side
    painter->fillRect(lastTxtX + lenFor1cm , bottomLeftY +VlineReduce, VlineThick , lineHeight-VlineReduce, QBrush(QColor(0,0,0,255))); // Right side
    lastTxtX += lenFor1cm;

    // Graphic scale legend print
    double scalLenH = (double)(( double((scaleLen / 10)) * 1000) / 100); // meters
    if (scalLenH < 1000 )
        lineToPrint = QString (" %1 m ").arg( QString::number( scalLenH , 'f', 0)) ;
    else
        lineToPrint = QString (" %1 km ").arg( QString::number( scalLenH / 1000 , 'f', 0)) ;
    currentText = painter->fontMetrics().boundingRect(lineToPrint);
    rLegendCursor = QRect( lastTxtX , bottomLeftY, currentText.width() , currentText.height());
    lastTxtX += currentText.width();
    painter->fillRect( rLegendCursor, QBrush(QColor(0, 0, 0, 0)));
    painter->drawText( rLegendCursor, Qt::AlignVCenter| Qt::AlignLeft, lineToPrint );

    // Current date print
    currentText = painter->fontMetrics().boundingRect(printDate);
    rLegendCursor = QRect( lastTxtX , bottomLeftY, currentText.width() , currentText.height());
    painter->fillRect( rLegendCursor, QBrush(QColor(0, 0, 0, 0)));
    lastTxtX += currentText.width();

    painter->drawText( rLegendCursor, Qt::AlignVCenter| Qt::AlignLeft, printDate );

    // Map bound
    lineToPrint = QString (" Top-left = %1,%2 / Bottom-right = %3, %4").arg( QString::number( slippyMap->printedTopLeft.ry() , 'f', 5),
                  QString::number( slippyMap->printedTopLeft.rx() , 'f', 5) ,
                  QString::number( slippyMap->printedBottomRight.ry() , 'f', 5) ,
                  QString::number( slippyMap->printedBottomRight.rx() , 'f', 5)
                                                                           ) ;
    currentText = painter->fontMetrics().boundingRect(lineToPrint);
    rLegendCursor = QRect( lastTxtX , bottomLeftY, currentText.width() , currentText.height());
    lastTxtX += currentText.width();
    painter->fillRect( rLegendCursor, QBrush(QColor(0, 0, 0, 0)));
    painter->drawText( rLegendCursor, Qt::AlignVCenter| Qt::AlignLeft, lineToPrint );

    double zoneMapWmm = printer->pageRect(QPrinter::Millimeter).width()-marginLeftMm-marginRightMm;
    double zoneMapHmm = printer->pageRect(QPrinter::Millimeter).height()-marginTopMm-marginBottomMm;

    if ((zoneMapWmm/10) * (scalLenH) < 1000  || (zoneMapHmm/10) * (scalLenH) < 1000)
        lineToPrint = QString (" Carte lxh: %1 x %2 m ").arg( QString::number( (zoneMapWmm/10) * (scalLenH), 'f', 1) , QString::number( (zoneMapHmm/10) * (scalLenH) , 'f', 1)) ;
    else
    {
        int precKm = 1;
        if (  ((zoneMapWmm/10) * (scalLenH))/1000 > 100) precKm = 0;

        lineToPrint = QString (" Carte lxh: %1 x %2 Km ").arg( QString::number( ((zoneMapWmm/10) * (scalLenH))/1000, 'f', precKm) , QString::number( ((zoneMapHmm/10) * (scalLenH) / 1000) , 'f', precKm)) ;
    }

    currentText = painter->fontMetrics().boundingRect(lineToPrint);
    rLegendCursor = QRect( lastTxtX , bottomLeftY, currentText.width() , currentText.height());
    lastTxtX += currentText.width();
    painter->fillRect( rLegendCursor, QBrush(QColor(0, 0, 0, 0)));
    painter->drawText( rLegendCursor, Qt::AlignVCenter| Qt::AlignLeft, lineToPrint );

    lineToPrint= QString (" %3 poi. ").arg(poiCount);
    currentText = painter->fontMetrics().boundingRect(lineToPrint);
    rLegendCursor = QRect( lastTxtX , bottomLeftY, currentText.width() , currentText.height());
    lastTxtX += currentText.width();
    painter->fillRect( rLegendCursor, QBrush(QColor(0, 0, 0, 0)));
    painter->drawText( rLegendCursor, Qt::AlignVCenter| Qt::AlignLeft, lineToPrint );

    // Map name
    lineToPrint = QString (" %1").arg( mapLibName );
    currentText = painter->fontMetrics().boundingRect(lineToPrint);
    rLegendCursor = QRect( lastTxtX , bottomLeftY, currentText.width() , currentText.height());
    lastTxtX += currentText.width();
    painter->fillRect( rLegendCursor, QBrush(QColor(0, 0, 0, 0)));
    painter->drawText( rLegendCursor, Qt::AlignVCenter| Qt::AlignLeft, lineToPrint );

}

void dialogPrint::renderPoiList(QPrinter *printer, QPainter *painter)
{

    if (cbPagePoi->isChecked() == false)  return;

    // Current text to print
    QString lineToPrint;

    // Page margins
    int pageMargintTop;
    int pageMargintRight;
    int pageMargintBottom;
    int pageMargintLeft;

    // Page size (pixel)
    int pageTotalW;
    int pageTotalH;

    // Space between lines
    int lineSpace = 120;

    // Current print position
    QPoint printAt;

    int pageNumber;

    // W var
    QRect textSize;
    bool newPageFlag;

    // Start init

    pageMargintTop=400;
    pageMargintBottom=400;
    pageMargintLeft=400;
    pageMargintRight=400;

    pageTotalW=painter->device()->width();
    pageTotalH=painter->device()->height();

    printAt.setX(pageMargintLeft);
    printAt.setY(pageMargintTop );

    //painter->setPen(QPen( legendColor ));
    //painter->setFont( legendFonfFamilly );

    painter->setFont(listFonfFamilly);
    painter->setPen(QPen(listColor, 12, Qt::DotLine, Qt::RoundCap));


    painter->setRenderHint(QPainter::Antialiasing);

    pageNumber = 1;

    // End init

    projectDatas::wptType *m_wpt;
    int PoiOnMap = 0;

    //painter.setFont(QFont("YaHei Consolas Hybrid", 40, -1, false));
    // painter->setPen(QPen(listColor, 12, Qt::DotLine, Qt::RoundCap));

    //painter.setColor(palette().color(QPalette::Highlight));
    //painter.setPen(p);

    // Todo check if at least one Poi to print
    newPage( printer, painter,  &printAt, lineSpace, poiCount, pageNumber, pageMargintLeft, pageMargintTop , pageMargintRight, pageMargintBottom, pageTotalW, pageTotalH);


    for(int rowData = 0; rowData < projectData->m_wptList.size(); rowData++)
    {
        m_wpt = projectData->getWptFromId2( rowData +1 );

        if (m_wpt==NULL)
        {
            debugp("m_wpt is NULL");
            continue;
        }
        if (m_wpt->m_selected == false) continue;

        PoiOnMap = 0;
        if ( m_wpt->m_lat.toDouble() <= slippyMap->printedTopLeft.ry() && m_wpt->m_lat.toDouble() >= slippyMap->printedBottomRight.ry() )
            PoiOnMap +=2;
        if ( m_wpt->m_lon.toDouble() >= slippyMap->printedTopLeft.rx() && m_wpt->m_lon.toDouble() <= slippyMap->printedBottomRight.rx() )
            PoiOnMap +=4;

        //painter->setOpacity( poiMapFontTransparency->text().toFloat());
        if (PoiOnMap == 6)
        {
            // Pixmap
            int pixScale = 1;

            QPixmap pixmapPng( iconFullPath + "/" + m_wpt->m_sym + ".png");
            debugp( +"pix file = " + iconFullPath + "/" + m_wpt->m_sym + ".png" );

            QFileInfo checkFile(iconFullPath + "/" + m_wpt->m_sym + ".png");
            if ( !checkFile.exists() || !checkFile.isFile())
            {
                pixmapPng.load( iconFullPath + "/default.png");
            }

            //painter->setOpacity(0.5);
            //painter->setOpacity(poiMapFontTransparency->text().toFloat()));
            painter->drawPixmap (
                printAt.x(),
                printAt.y() ,
                pixmapPng.width() * scaleFactor * pixScale , pixmapPng.height() * scaleFactor * pixScale ,
                pixmapPng);

            printAt.setX( printAt.x() + (pixmapPng.width() * scaleFactor ) );
            printAt.setY( printAt.y() + ((pixmapPng.height() * scaleFactor))*0.3 ) ;

            // Text
            lineToPrint= QString ("%1 | Lat,lon=%2 , %3").arg( m_wpt->m_name ).arg( m_wpt->m_lat).arg( m_wpt->m_lon);
            newPageFlag = printTextAt( painter, &printAt, lineToPrint, lineSpace, pageMargintLeft , pageMargintBottom, pageTotalH, true);

            painter->drawLine( printAt.x(), printAt.y(), pageTotalW-pageMargintRight , printAt.y());

            if (newPageFlag==true)
            {
                pageNumber++;
                newPage( printer, painter,  &printAt, lineSpace, poiCount, pageNumber, pageMargintLeft, pageMargintTop , pageMargintRight, pageMargintBottom, pageTotalW, pageTotalH);
                newPageFlag=false;

            }

        }
    }

    return;
}

//#define UNUSED(expr) do { (void)(expr); } while (0)
void  dialogPrint::newPage( QPrinter *printer, QPainter *painter,  QPoint *printAt, int lineSpace, int poiCount, int pageNumber, int pageMargintLeft, int pageMargintTop , int pageMargintRight, int pageMargintBottom, int pageTotalW, int pageTotalH)
{

    //int dummy;
    bool drawPageBorer = false;

    printer->newPage();

    if (drawPageBorer== true)
    {
        //painter->setBrush(QBrush(QColor( 100,100,100, 0.85)));
        painter->drawRect(pageMargintTop  , pageMargintLeft , pageTotalW - (pageMargintRight+pageMargintLeft) , pageTotalH- (pageMargintBottom+pageMargintTop) );
    }

    printAt->setX(pageMargintLeft );
    printAt->setY(pageMargintTop );

    QString lineToPrint;
    lineToPrint= QString ("Page : %1 | %2 | %3 poi.").arg( pageNumber ).arg( printDate ).arg(poiCount) ;
    printTextAt( painter,  printAt, lineToPrint, lineSpace, pageMargintLeft, pageMargintBottom, pageTotalH, true );

    painter->drawLine( printAt->x(), printAt->y(), pageTotalW-pageMargintRight , printAt->y());

}

bool dialogPrint::printTextAt( QPainter *painter,  QPoint *printAt, QString lineToPrint, int lineSpace, int pageMargintLeft, int pageMargintBottom, int pageTotalH, bool newlineAfter )
{

    QRect textSize;
    QPoint tmpAt;
    bool newPageFlag;

    newPageFlag=false;

    textSize = painter->fontMetrics().boundingRect(lineToPrint);

    tmpAt.setX( printAt->x());
    tmpAt.setY( printAt->y() + textSize.height() );

    painter->drawText( tmpAt,  lineToPrint );

    if (newlineAfter == false)
    {
        // Same line case
        printAt->setX( printAt->x() + textSize.width());
    }
    else
    {
        // New line case
        printAt->setX( pageMargintLeft );
        printAt->setY( printAt->y() + textSize.height() + lineSpace ) ;
    }

    // Return true for new page
    if ( printAt->y() >= pageTotalH-pageMargintBottom-textSize.height()) newPageFlag=true;

    return newPageFlag;
}


#include "dialogPrint.moc"



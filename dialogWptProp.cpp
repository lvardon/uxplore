#include "dialogWptProp.h"

#include "projectData.h"

#include "mainWindow.h"

#include <QDebug>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLineEdit>
#include <QCheckBox>

#define COL_LINK_EDIT_WIDGET   0
#define COL_LINK_VIEW_WIDGET   1
#define COL_LINK_VALUE_WIDGET   2

dialogWptProp::dialogWptProp() : QDialog(0)
{
    QPalette palette;

    QGridLayout *layout = new QGridLayout();

    QLabel *m_idLabel = new QLabel("Id :", this);
    m_idLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel *m_nameLabel = new QLabel("Nom :", this);
    m_nameLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QLabel *m_groupLabel = new QLabel("Groupe :", this);
    m_groupLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    palette = m_groupLabel->palette();
    palette.setColor(QPalette::WindowText,QColor(QColor(255,0,0)));
    m_groupLabel->setPalette(palette);

    QLabel *m_noteLabel = new QLabel("Note :", this);
    m_noteLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    palette = m_noteLabel->palette();
    palette.setColor(QPalette::WindowText,QColor(QColor(255,0,0)));
    m_noteLabel->setPalette(palette);


    QLabel *m_latLabel = new QLabel("Latitude :", this);
    m_latLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel *m_lonLabel = new QLabel("Longitude :", this);
    m_lonLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel *m_eleLabel = new QLabel("Elevation :", this);
    m_eleLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel *m_timeLabel = new QLabel("Horodatage :", this);
    m_timeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel *m_magvarLabel = new QLabel( QString::fromUtf8("Orientation magnétique :"), this);
    m_magvarLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel *m_geoidheightLabel = new QLabel( QString::fromUtf8("Hauteur du géoïde :"), this);
    m_geoidheightLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel *m_cmtLabel = new QLabel("Commentaire  :", this);
    m_cmtLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel *m_descLabel = new QLabel("Description :", this);
    m_descLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel *m_srcLabel = new QLabel("Appareil :", this);
    m_srcLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel *m_fixLabel = new QLabel("Type Gps :", this);
    m_fixLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel *m_satLabel = new QLabel("Nombre de satellites :", this);
    m_satLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel *m_dgpsidLabel = new QLabel("Mesure Gps :", this);
    m_dgpsidLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel *m_linksLabel = new QLabel("Liens :", this);
    m_linksLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel *m_symLabel = new QLabel("Symbole :", this);
    m_symLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel *m_typeLabel = new QLabel("Type de point :", this);
    m_typeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel *m_hdopLabel = new QLabel("hdop :", this);
    m_hdopLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel *m_vdopLabel = new QLabel("vdop :", this);
    m_vdopLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel *m_pdopLabel = new QLabel("pdop :", this);
    m_pdopLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel *m_extensionsLabel = new QLabel("Extentions :", this);
    m_extensionsLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_idQ = new QLineEdit(this);
    m_idQ->setDisabled(true);
    m_idQ->setFixedWidth(15*6);
    m_nameQ = new QLineEdit(this);
    m_groupQ = new QLineEdit(this);
    m_groupQ->setFixedWidth(15*10);

    m_noteQ = new QLineEdit(this);
    m_latQ = new QLineEdit(this);
    m_latQ->setFixedWidth(15*7);
    m_lonQ = new QLineEdit(this);
    m_lonQ->setFixedWidth(15*7);
    m_eleQ = new QLineEdit(this);
    m_eleQ->setFixedWidth(15*15);
    m_timeQ = new QDateTimeEdit();
    m_timeQ->setDisplayFormat("dd/MM/yyyy hh:mm");

    m_timeQ->setCalendarPopup(true);

    m_timeQ->setFixedWidth(15*12);
    m_magvarQ = new QLineEdit(this);
    m_magvarQ->setFixedWidth(15*15);
    m_geoidheightQ = new QLineEdit(this);
    m_geoidheightQ->setFixedWidth(15*15);
    m_cmtQ = new QTextEdit(this);
    m_descQ = new QTextEdit(this);
    m_srcQ = new QLineEdit(this);
    m_fixQ = new QLineEdit(this);
    m_satQ = new QLineEdit(this);
    m_satQ->setFixedWidth(15*5);
    m_dgpsidQ = new QLineEdit(this);

    m_linksQ = new QTableWidget(0, 0, this);
    m_linksQ->setEditTriggers(QAbstractItemView::AllEditTriggers);
    m_linksQ->horizontalHeader()->setStretchLastSection(true);
    m_linksQ->verticalHeader()->hide();
    m_linksQ->horizontalHeader()->hide();
    m_linksQ->setColumnCount(3);
    m_linksQ->setRowCount(0);

    m_symQ = new QLineEdit(this);
    m_symQ->setFixedWidth(15*18);
    m_symQIcon  = new QPushButton();
    m_typeQ = new QLineEdit(this);
    m_hdopQ = new QLineEdit(this);
    m_hdopQ->setFixedWidth(15*6);
    m_vdopQ = new QLineEdit(this);
    m_vdopQ->setFixedWidth(15*6);
    m_pdopQ = new QLineEdit(this);
    m_pdopQ->setFixedWidth(15*6);
    m_extensionsQ = new QLineEdit(this);

    layout->addWidget(m_idLabel,  0, 0, 1, 1) ;
    layout->addWidget(m_nameLabel, 1, 0, 1, 1) ;
    layout->addWidget(m_groupLabel, 2, 0, 1, 1) ;
    layout->addWidget(m_noteLabel, 3, 0, 1, 1) ;
    layout->addWidget(m_latLabel, 4, 0, 1, 1) ;
    layout->addWidget(m_lonLabel,  5, 0, 1, 1) ;
    layout->addWidget(m_eleLabel, 6, 0, 1, 1);
    layout->addWidget(m_timeLabel, 7, 0, 1, 1);
    layout->addWidget(m_magvarLabel, 8, 0, 1, 1);
    layout->addWidget(m_geoidheightLabel, 9, 0, 1, 1);
    layout->addWidget(m_cmtLabel, 10, 0, 1, 1);
    layout->addWidget(m_descLabel, 11, 0, 1, 1);
    layout->addWidget(m_srcLabel, 12, 0, 1, 1);
    layout->addWidget(m_fixLabel, 13, 0, 1, 1);
    layout->addWidget(m_satLabel, 14, 0, 1, 1);
    layout->addWidget(m_dgpsidLabel, 15, 0, 1, 1);
    layout->addWidget(m_linksLabel, 16, 0, 1, 1);
    layout->addWidget(m_symLabel, 17, 0, 1, 1);
    layout->addWidget(m_typeLabel, 18, 0, 1, 1);
    layout->addWidget(m_hdopLabel, 19, 0, 1, 1);
    layout->addWidget(m_vdopLabel, 20, 0, 1, 1);
    layout->addWidget(m_pdopLabel, 21, 0, 1, 1);
    layout->addWidget(m_extensionsLabel, 22, 0, 1, 1);


    layout->addWidget(m_idQ, 0, 1, 1, 1);
    layout->addWidget(m_nameQ, 1, 1, 1, 1);
    layout->addWidget(m_groupQ, 2, 1, 1, 1);
    layout->addWidget(m_noteQ, 3, 1, 1, 1);
    layout->addWidget(m_latQ, 4, 1, 1, 1);
    layout->addWidget(m_lonQ, 5, 1, 1, 1);
    layout->addWidget(m_eleQ, 6, 1, 1, 1);
    layout->addWidget(m_timeQ, 7, 1, 1, 1);
    layout->addWidget(m_magvarQ, 8, 1, 1, 1);
    layout->addWidget(m_geoidheightQ, 9, 1, 1, 1);
    layout->addWidget(m_cmtQ, 10, 1, 1, 1);
    layout->addWidget(m_descQ, 11, 1, 1, 1);
    layout->addWidget(m_srcQ, 12, 1, 1, 1);
    layout->addWidget(m_fixQ, 13, 1, 1, 1);
    layout->addWidget(m_satQ, 14, 1, 1, 1);
    layout->addWidget(m_dgpsidQ, 15, 1, 1, 1);
    layout->addWidget(m_linksQ, 16, 1, 1, 1);
    layout->addWidget(m_symQ, 17, 1, 1, 1);
    layout->addWidget(m_symQIcon, 17, 2, 1, 1);
    layout->addWidget(m_typeQ, 18, 1, 1, 1);
    layout->addWidget(m_hdopQ, 19, 1, 1, 1);
    layout->addWidget(m_vdopQ, 20, 1, 1, 1);
    layout->addWidget(m_pdopQ, 21, 1, 1, 1);
    layout->addWidget(m_extensionsQ, 22, 1, 1, 1);

    m_idQ->setToolTip(QString::fromUtf8("Identifiant interne"));
    m_nameQ->setToolTip(QString::fromUtf8("Nom principal du way point (label)"));
    m_groupQ->setToolTip(QString::fromUtf8("Groupe du point"));
    m_noteQ->setToolTip(QString::fromUtf8("Note"));
    m_latQ->setToolTip(QString::fromUtf8("Latitude"));
    m_lonQ->setToolTip(QString::fromUtf8("Longitude"));
    m_eleQ->setToolTip(QString::fromUtf8("Altitude en mètres"));
    m_timeQ->setToolTip(QString::fromUtf8("Date de creation/modification du way point. (Univeral Coordinated Time : UTC)"));
    m_magvarQ->setToolTip(QString::fromUtf8("L'orientation magnétique du récepteur GPS en degrés"));
    m_geoidheightQ->setToolTip(QString::fromUtf8("La hauteur du géoïde (niveau de la mer) au-dessus de l'ellipsoïde WGS 84, tel que décrit dans la spécification des messages NMEA GGA"));
    m_cmtQ->setToolTip(QString::fromUtf8("Commentaire"));
    m_descQ->setToolTip(QString::fromUtf8("Description"));
    m_srcQ->setToolTip(QString::fromUtf8("Le modèle et le fabricant de l'appareil à l'origine de la géolocalisation du point"));
    m_fixQ->setToolTip(QString::fromUtf8("Le type de GPS ayant permis l'obtention du point (fix)  : none'|'2d'|'3d'|'dgps'|'pps"));
    m_satQ->setToolTip(QString::fromUtf8("Le nombre de satellites acquis lors du calcul des coordonnées"));
    m_dgpsidQ->setToolTip(QString::fromUtf8("Des informations sur les mesures DGPS comme la durée depuis la dernière mise à jour (<ageofdgpsdata>) ou le l'identifiant de la station"));
    m_linksQ->setToolTip(QString::fromUtf8("Liens internet d'informations sur le way point"));
    m_symQ->setToolTip(QString::fromUtf8("Le nom exact du symbole illustrant le point sur l'appareil GPS (fichier icone)"));
    m_typeQ->setToolTip(QString::fromUtf8("Le type de point (catégorie)"));
    m_hdopQ->setToolTip(QString::fromUtf8("Précision (<hdop>, <vdhop>, <pdop> voir Geometric_dilution_of_precision)"));
    m_vdopQ->setToolTip(QString::fromUtf8("Précision (<hdop>, <vdhop>, <pdop> voir Geometric_dilution_of_precision)"));
    m_pdopQ->setToolTip(QString::fromUtf8("Précision (<hdop>, <vdhop>, <pdop> voir Geometric_dilution_of_precision)"));
    m_extensionsQ->setToolTip(QString::fromUtf8("Optionnellement : des extensions d'un autre espace de noms"));


    QPushButton *button;
    QDialogButtonBox *ButtonBox = new QDialogButtonBox( Qt::Horizontal );

    button = new QPushButton( "Ok" );
    connect( button, SIGNAL(clicked()), this, SLOT(okClicked()) );
    ButtonBox->addButton( button, QDialogButtonBox::AcceptRole );

    button = new QPushButton( "Annuler" );
    connect( button, SIGNAL(clicked()), this, SLOT(cancelClicked()) );
    ButtonBox->addButton( button, QDialogButtonBox::RejectRole );

    layout->addWidget( ButtonBox , 23, 1, 1 ,1);
    setLayout(layout);
    resize(600, 220);
    setWindowTitle( QString::fromUtf8("Propriétés du way point"));

}

void dialogWptProp::okClicked()
{


    //QKeyEvent event(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
    //QApplication::sendEvent(m_linksQ, &event);

    wpt = m_projectDatas->getWptFromId2( curentWptId );
    wpt->m_name = m_nameQ->text() ;
    wpt->m_group = m_groupQ->text() ;
    wpt->m_note = m_noteQ->text() ;
    wpt->m_lat =  m_latQ->text();
    wpt->m_lon =  m_lonQ->text();
    wpt->m_ele =  m_eleQ->text();
    wpt->m_time = m_timeQ->dateTime();
    wpt->m_magvar = m_magvarQ ->text();
    wpt->m_geoidheight = m_geoidheightQ->text();
    wpt->m_cmt =  m_cmtQ->toPlainText();
    wpt->m_desc =  m_descQ->toPlainText();
    wpt->m_src =  m_srcQ->text();
    wpt->m_fix = m_fixQ->text();
    wpt->m_sat = m_satQ->text();
    wpt->m_dgpsid = m_dgpsidQ->text();

    wpt->m_links.clear();
    for (int i=0; i<m_linksQ->rowCount()-1; ++i)
    {
        QString libLink = m_linksQ->item(i,COL_LINK_VALUE_WIDGET)->text();
        if ( libLink.trimmed() != "")
        {
            wpt->m_links.append( libLink );
        }
    }

    wpt->m_sym = m_symQ->text();
    wpt->m_type = m_typeQ->text();
    wpt->m_hdop = m_hdopQ->text();
    wpt->m_vdop = m_vdopQ->text();
    wpt->m_pdop = m_pdopQ->text();
    wpt->m_extensions = m_extensionsQ->text();

    dataSaved = true;

    QDialog::accept();
}

void dialogWptProp::cancelClicked()
{
    dataSaved = false;
    QDialog::accept();
}

void dialogWptProp::onMlinksViewClicked(void)
{
    int i = sender()->property("TableRowIndex").toInt() ;
    debugp("View : " +  QString::number(i) + " :" + m_linksQ->item(i,COL_LINK_VALUE_WIDGET)->text() );
    if ( m_linksQ->item(i,COL_LINK_VALUE_WIDGET)->text() != "")
    {
        QString url = m_linksQ->item(i,COL_LINK_VALUE_WIDGET)->text();
        QDesktopServices::openUrl(QUrl(url));
    }
}

void dialogWptProp::appendLinkTableLine( QTableWidget *tbl, int idexLine, QString editBtnLabel, QString viewBtnLabel, QString valueLink )
{
    // Edit + / - button
    QPushButton *linkEditButton;
    linkEditButton = new QPushButton();
    linkEditButton->setText(editBtnLabel);
    linkEditButton->setProperty("TableRowIndex", idexLine);

    m_linksQ->setCellWidget(idexLine, COL_LINK_EDIT_WIDGET, linkEditButton);
    connect(linkEditButton, SIGNAL(clicked()) , this , SLOT(onMlinksClicked()));

    // View button
    if (viewBtnLabel != "")
    {
        QPushButton *linkViewButton;
        linkViewButton = new QPushButton();
        linkViewButton->setText(viewBtnLabel);
        linkViewButton->setProperty("TableRowIndex", idexLine);
        debugp( "TableRowIndex="  + QString::number(idexLine ) );
        m_linksQ->setCellWidget(idexLine, COL_LINK_VIEW_WIDGET, linkViewButton);
        connect(linkViewButton, SIGNAL(clicked()) , this , SLOT(onMlinksViewClicked()));
    }

    // Value
    m_linksQ->setItem(idexLine,COL_LINK_VALUE_WIDGET,new QTableWidgetItem( valueLink ));
}

void dialogWptProp::updateParent( void )
{

    dataSaved = false;

    mainWindow *p_mainWindow;

    p_mainWindow = dynamic_cast<mainWindow*>(QApplication::activeWindow());

    if (p_mainWindow == 0)
    {
        debugp("Parent is not MainWindow !");
    }
    else
        p_mainWindow->OnProjectDataUnSaved();

}

void dialogWptProp::onMlinksClicked(void)
{
    int i = sender()->property("TableRowIndex").toInt() ;

    if (i+1 < m_linksQ->rowCount())
    {
        m_linksQ->removeRow(i);
    }
    else
    {
        if (m_linksQ->item(i,COL_LINK_VALUE_WIDGET)->text().trimmed() == "")
        {
            return;
        }

        // New table line
        i++;
        m_linksQ->setRowCount(m_linksQ->rowCount()+1);
        appendLinkTableLine( m_linksQ, i, "+", "" ,"" );

        // Change previous label button from + to -
        if (i-1 >=0 )
        {
            QPushButton *prevButton = (QPushButton*)m_linksQ->cellWidget(i-1,COL_LINK_EDIT_WIDGET);
            prevButton->setText("-");

            QPushButton *linkViewButton;
            linkViewButton = new QPushButton();
            linkViewButton->setText("...");
            linkViewButton->setProperty("TableRowIndex", i-1);
            m_linksQ->setCellWidget(i-1, COL_LINK_VIEW_WIDGET, linkViewButton);
            connect(linkViewButton, SIGNAL(clicked()) , this , SLOT(onMlinksViewClicked()));
        }
    }

    // Re-order table TableRowIndex
    for (int i=0; i<m_linksQ->rowCount(); ++i)
    {
        QPushButton *prevEditButton = (QPushButton*)m_linksQ->cellWidget(i,COL_LINK_EDIT_WIDGET);
        prevEditButton->setProperty("TableRowIndex", i);
        if (i +1 < m_linksQ->rowCount())
        {
            QPushButton *prevViewButton = (QPushButton*)m_linksQ->cellWidget(i,COL_LINK_VIEW_WIDGET);
            prevViewButton->setProperty("TableRowIndex", i);
        }
    }

}

void dialogWptProp::showEvent( QShowEvent* event )
{

    wpt = m_projectDatas->getWptFromId2( curentWptId );

    if (wpt==NULL)
    {
        debugp("wpt is NULL");
        return;
    }

    QWidget::showEvent( event );

    m_idQ->setText( QString::number(wpt->m_id));
    m_nameQ->setText(wpt->m_name);
    m_groupQ->setText(wpt->m_group);
    m_noteQ->setText( wpt->m_note);
    m_latQ->setText(wpt->m_lat);
    m_lonQ->setText(wpt->m_lon);
    m_eleQ->setText(wpt->m_ele);
    m_timeQ->setDateTime( wpt->m_time );
    m_magvarQ->setText(wpt->m_magvar);
    m_geoidheightQ->setText(wpt->m_geoidheight);
    m_cmtQ->setText(wpt->m_cmt);
    m_descQ->setText(wpt->m_desc);
    m_srcQ->setText(wpt->m_src);
    m_fixQ->setText(wpt->m_fix);
    m_satQ->setText(wpt->m_sat);
    m_dgpsidQ->setText(wpt->m_dgpsid);
    m_linksQ->setRowCount(wpt->m_links.count()+1);
    QString linkCurr;
    int i=0;
    foreach(linkCurr, wpt->m_links)
    {
        appendLinkTableLine( m_linksQ, i, "-", "..." ,linkCurr );
        i++;
    }
    appendLinkTableLine( m_linksQ, i, "+", "" , "" );

    m_linksQ->setItem(i,1,new QTableWidgetItem( "" ));
    m_linksQ->horizontalHeader()->resizeSection(COL_LINK_EDIT_WIDGET, 20);
    m_linksQ->horizontalHeader()->resizeSection(COL_LINK_VIEW_WIDGET, 30);
    m_linksQ->horizontalHeader()->resizeSection(COL_LINK_VALUE_WIDGET, 200);
    m_symQ->setText(wpt->m_sym);

    QPixmap* pixmap = new QPixmap(iconSymbolRootDir + "/" + wpt->m_sym);

    QIcon icon(*pixmap);
    QSize iconSize(pixmap->width(), pixmap->height());
    m_symQIcon->setIconSize(iconSize);
    m_symQIcon->setIcon(icon);
    connect(m_symQIcon, SIGNAL(clicked()) , this , SLOT(onSymQIconClicked()));

    m_typeQ->setText(wpt->m_type);
    m_hdopQ->setText(wpt->m_hdop);
    m_vdopQ->setText(wpt->m_vdop);
    m_pdopQ->setText(wpt->m_pdop);
    m_extensionsQ->setText(wpt->m_extensions);

}

void dialogWptProp::onSymQIconClicked()
{

    QString importFilters = QString::fromUtf8("Images (*.png);;All files (*.*)");
    QString importDefaultFilter = QString::fromUtf8("Images (*.png)");


    QString importSymFileName = QFileDialog::getOpenFileName(this, "Selectionner l'icône", iconSymbolRootDir, importFilters, &importDefaultFilter);

    if ( importSymFileName  == "")
    {
        return;
    }

    QPixmap* pixmap = new QPixmap( importSymFileName);
    QIcon icon(*pixmap);
    QSize iconSize(pixmap->width(), pixmap->height());
    m_symQIcon->setIconSize(iconSize);
    m_symQIcon->setIcon(icon);

    m_symQ->setText(QFileInfo(importSymFileName).completeBaseName());
}


#include "dialogWptProp.moc"




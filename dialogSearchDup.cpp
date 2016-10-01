#include "global.h"
#include "mainWindow.h"
#include "projectData.h"

#include "dialogSearchDup.h"

#include <QDebug>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPalette>


#define HILIGHT_ACTION_NOTHING  0
#define HILIGHT_ACTION_COLOR    1
#define HILIGHT_ACTION_CHECK    2
#define HILIGHT_ACTION_UNCHECK  3


dialogSearchDup::dialogSearchDup(mainWindow  *parent) : QDialog(parent)
{

    m_parent = parent;

    resize(400, 180); // W x H
    QGridLayout *layout = new QGridLayout();

    QPushButton *button;
    QDialogButtonBox *ButtonBox = new QDialogButtonBox( Qt::Horizontal );

    button = new QPushButton( "Grouper les doublons" );
    connect( button, SIGNAL(clicked()), this, SLOT(okClicked()) );
    ButtonBox->addButton( button, QDialogButtonBox::AcceptRole );

    button = new QPushButton( "Fermer" );
    connect( button, SIGNAL(clicked()), this, SLOT(cancelClicked()) );
    ButtonBox->addButton( button, QDialogButtonBox::RejectRole );

    QLabel *strSearchLabel = new QLabel("Rayon (metres) :", this);
    strSearchLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    strSearch = new QLineEdit(this);
    layout->addWidget(strSearchLabel, 0 , 0) ;
    layout->addWidget(strSearch, 0 , 1);
    strSearch->setFixedWidth(15*7);
    strSearch->setText("1000");
    strSearch->setToolTip("Grouper les points dans un rayon de X metres");

    QLabel *filterLabel = new QLabel(trUtf8("Filtre"), this);
    filterLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    filterSelectedListBox = new QComboBox(this);
    filterSelectedListBox->addItem(trUtf8("Tous"));
    filterSelectedListBox->addItem(trUtf8("Sélectionnés"));
    filterSelectedListBox->addItem(trUtf8("Non sélectionnés"));
    layout->addWidget(filterLabel, 1 , 0) ;
    layout->addWidget(filterSelectedListBox, 1 , 1);

    QLabel *actionLabel = new QLabel(trUtf8("Action"), this);
    actionLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    actionSelectedListBox = new QComboBox(this);
    actionSelectedListBox->addItem(trUtf8("Ne rien faire"));
    actionSelectedListBox->addItem(trUtf8("Couleur"));
    actionSelectedListBox->addItem(trUtf8("Grouper et sélectionner la ligne"));
    actionSelectedListBox->addItem(trUtf8("Grouper et dé-selectionner la ligne"));
    layout->addWidget(actionLabel, 2 , 0) ;
    layout->addWidget(actionSelectedListBox, 2 , 1);
    actionSelectedListBox->setCurrentIndex(1);

    QLabel *strResultSearchLabel = new QLabel( trUtf8("Résultat"), this);
    strResultSearchLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(strResultSearchLabel, 3 , 0);
    strResultSearch = new QLabel( trUtf8(""), this);
    layout->addWidget(strResultSearch, 3 , 1);

    layout->addWidget( ButtonBox , 99 , 1);

    setLayout(layout);

    setWindowTitle( QString::fromUtf8("Grouper les doublons"));
}

void dialogSearchDup::okClicked()
{
    debugp ("Enter search duplicate");

    qlonglong countCheckLine = 0;
    qlonglong countCheckDup = 0;
    QList<projectDatas::wptType>::iterator itMain = m_projectDatas->m_wptList.begin();
    QList<projectDatas::wptType>::iterator it = m_projectDatas->m_wptList.begin();
    projectDatas::wptType *m_wpt;
    projectDatas::wptType *m_wptMain;
    int rowMain;
    int row;
    QPointF markerPosRef;
    QPointF markerPosCurrent;
    double currentDist;
    double maxDist;
    int currentGroup = 0;

    strResultSearch->setText( QString::fromUtf8("Calcul en cours ..."));

    maxDist = strSearch->text().toInt(); // meters

    this->setCursor(Qt::WaitCursor);
    row=0;
    while (it != m_projectDatas->m_wptList.end())
    {
        m_wpt = m_projectDatas->getWptFromId2( row +1 );

            if (m_wpt==NULL) {
                debugp("m_wpt is NULL");
                continue;
            }
        m_wptEdit->hilightWpt( row , Qt::white , Qt::black);
        m_projectDatas->m_wptList[row].m_group = "";
        m_wptEdit->displayWptLine( row, m_wpt);

        ++it;
        row++;
    }

    rowMain=0;

    while (itMain != m_projectDatas->m_wptList.end())
    {
        row=0;
        it = m_projectDatas->m_wptList.begin();

        m_wptMain = m_projectDatas->getWptFromId2( rowMain +1 );

        // Apply data filter
        if ( filterSelectedListBox->currentIndex() == 2 && m_projectDatas->m_wptList[rowMain].m_selected == true) continue;
        if ( filterSelectedListBox->currentIndex() == 1 && m_projectDatas->m_wptList[rowMain].m_selected == false) continue;

        debugp("--- Ref Duplicate for : name=" + m_projectDatas->m_wptList[rowMain].m_name + " lat=" + m_projectDatas->m_wptList[rowMain].m_lat + " lon=" + m_projectDatas->m_wptList[rowMain].m_lon);
        markerPosRef.setX( m_projectDatas->m_wptList[rowMain].m_lon.toDouble() );
        markerPosRef.setY( m_projectDatas->m_wptList[rowMain].m_lat.toDouble() );

        while (it != m_projectDatas->m_wptList.end())
        {
            if (m_projectDatas->m_wptList[row].m_id != m_projectDatas->m_wptList[rowMain].m_id && m_projectDatas->m_wptList[row].m_group == "")
            {
                m_wpt = m_projectDatas->getWptFromId2( row +1 );
                //m_wptEdit->hilightWpt( row , Qt::white , Qt::black);

                markerPosCurrent.setX( m_projectDatas->m_wptList[row].m_lon.toDouble() );
                markerPosCurrent.setY( m_projectDatas->m_wptList[row].m_lat.toDouble() );
                currentDist = Great_Circle_Distance( markerPosRef, markerPosCurrent);

                if ( currentDist <= maxDist )
                {
                    countCheckDup++;
                    debugp ("IN : name=" + m_projectDatas->m_wptList[row].m_name + " lat=" + m_projectDatas->m_wptList[row].m_lat + " lon=" + m_projectDatas->m_wptList[row].m_lon + " D=" + QString::number(currentDist));
                    if ( m_projectDatas->m_wptList[rowMain].m_group == "")
                    {
                        currentGroup++;
                        m_projectDatas->m_wptList[rowMain].m_group = QString::number(currentGroup);
                        m_projectDatas->m_wptList[row].m_group = QString::number(currentGroup);

                        hilighttLine( rowMain, m_wptMain, actionSelectedListBox->currentIndex() );
                        hilighttLine( row, m_wpt, actionSelectedListBox->currentIndex() );

                        countCheckLine+=2;


                    }
                    else
                    {
                        m_projectDatas->m_wptList[row].m_group  = m_projectDatas->m_wptList[rowMain].m_group ;
                        hilighttLine( row, m_wpt, actionSelectedListBox->currentIndex() );
                        countCheckLine++;

                    }
                }
            }
            ++it;
            row++;
        }

        ++itMain;
        rowMain++;
    }

    this->setCursor(Qt::ArrowCursor);

    QPalette pal = QPalette(strResultSearch->palette());
    pal.setColor(QPalette::WindowText,QColor(Qt::darkGreen));
    strResultSearch->setPalette(pal);
    strResultSearch->setText( QString::number(countCheckLine) + " lignes " + QString::number(countCheckDup) + " doublons. " + QString::number(currentGroup) + " groupes" );

}


void dialogSearchDup::hilighttLine( int row, projectDatas::wptType *m_wpt, int action )
{

    if (action == HILIGHT_ACTION_COLOR )
    {
        debugp (" Colorize row #" + QString::number( row)) ;
        m_wptEdit->hilightWpt( row , Qt::red , Qt::white );
    }

    if (action == HILIGHT_ACTION_CHECK )
    {
        debugp ( "Check row #" + QString::number(row )) ;
        m_wpt->m_selected = true;
        m_parent->tableWpt->displayWptLine( row, m_wpt);
        m_wptEdit->updateMarkerByWpt(m_wpt);
        m_parent->slippyMap.update();
    }

    if (action == HILIGHT_ACTION_UNCHECK )
    {
        debugp ( "UnCheck row #" + QString::number(row )) ;
        m_wpt->m_selected = false;
        m_wptEdit->displayWptLine( row, m_wpt);
        m_wptEdit->updateMarkerByWpt(m_wpt);
        m_parent->slippyMap.update();

    }

}

void dialogSearchDup::cancelClicked()
{
    QDialog::accept();
    m_parent->dialogSearchDupDisplayed=false;
}

void dialogSearchDup::showEvent( QShowEvent* event )
{

    QWidget::showEvent( event );


}
void dialogSearchDup::setFormatGpx( projectDatas *p_projectDatas)
{
    m_projectDatas = p_projectDatas;

}

void dialogSearchDup::setWptEdit( wptEdit *p_wptedit )
{
    m_wptEdit = p_wptedit;

}

void dialogSearchDup::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
        QDialog::keyPressEvent(e);
    else
    {
         m_parent->dialogSearchDupDisplayed=false;
        QDialog::accept();
    }
}

void dialogSearchDup::closeEvent(QCloseEvent *event)
{
    //emit dialogClosed();
    m_parent->dialogSearchDupDisplayed=false;
    QDialog::accept();
}


#include "dialogSearchDup.moc"



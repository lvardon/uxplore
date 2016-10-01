#include "global.h"
#include "mainWindow.h"
#include "projectData.h"

#include "dialogFind.h"

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
#define HILIGHT_ACTION_SEARCH  4

dialogFind::dialogFind(mainWindow  *parent) : QDialog(parent)
{

    m_parent = parent;

    foundColor = m_parent->foundColor;
    noColor = QColor(Qt::white);
    errorColor = QColor(Qt::red);

    resize(400, 350); // W x H
    QGridLayout *layout = new QGridLayout();

    QPushButton *button;
    QDialogButtonBox *ButtonBox = new QDialogButtonBox( Qt::Horizontal );

    button = new QPushButton( "Chercher" );
    connect( button, SIGNAL(clicked()), this, SLOT(okClicked()) );
    ButtonBox->addButton( button, QDialogButtonBox::AcceptRole );

    button = new QPushButton( "Fermer" );
    connect( button, SIGNAL(clicked()), this, SLOT(cancelClicked()) );
    ButtonBox->addButton( button, QDialogButtonBox::RejectRole );

    QLabel *strSearchLabel = new QLabel("Chercher", this);
    strSearchLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    strSearch = new QLineEdit(this);
    layout->addWidget(strSearchLabel, 0 , 0) ;
    layout->addWidget(strSearch, 0 , 1);
    connect(strSearch, SIGNAL(textChanged(const QString &)), this, SLOT(onStrSearchChange(const QString &)));


    QLabel *regexCheckBoxLabel = new QLabel("Regex", this);
    regexCheckBoxLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    regexCheckBox = new QCheckBox();
    layout->addWidget(regexCheckBoxLabel, 1 , 0) ;
    layout->addWidget(regexCheckBox, 1 , 1);
    connect(regexCheckBox, SIGNAL(stateChanged(int)), this, SLOT(OnRegexCheckBox(int)));

    QLabel *caseCheckBoxLabel = new QLabel("Tenir compte de la case", this);
    caseCheckBoxLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    caseCheckBox = new QCheckBox();
    caseCheckBox ->setChecked(true);
    layout->addWidget(caseCheckBoxLabel, 2 , 0) ;
    layout->addWidget(caseCheckBox, 2 , 1);
    connect(caseCheckBox, SIGNAL(stateChanged(int)), this, SLOT(OnCaseCheckBox(int)));

    QLabel *accentCheckBoxLabel = new QLabel("Tenir compte des accents", this);
    accentCheckBoxLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    accentCheckBox = new QCheckBox();
    accentCheckBox->setChecked(true);
    layout->addWidget(accentCheckBoxLabel, 3 , 0) ;
    layout->addWidget(accentCheckBox, 3 , 1);
    connect(accentCheckBox, SIGNAL(stateChanged(int)), this, SLOT(OnAccentCheckBox(int)));


    colWpt = new QListWidget(this);
    colWpt->addItem(trUtf8("Nom"));
    colWpt->item(0)->setData(Qt::UserRole, COL_WPT_NAME);
    colWpt->addItem(trUtf8("Note"));
    colWpt->item(1)->setData(Qt::UserRole , COL_WPT_NOTE);
    colWpt->addItem(trUtf8("Groupe"));
    colWpt->item(2)->setData(Qt::UserRole , COL_WPT_GROUP);
    colWpt->addItem(trUtf8("Latitude"));
    colWpt->item(3)->setData(Qt::UserRole , COL_WPT_LAT);
    colWpt->addItem(trUtf8("Longitude"));
    colWpt->item(4)->setData(Qt::UserRole , COL_WPT_LON);
    colWpt->addItem(trUtf8("Elevation"));
    colWpt->item(5)->setData(Qt::UserRole , COL_WPT_ELE);
    colWpt->addItem(trUtf8("Horodatage"));
    colWpt->item(6)->setData(Qt::UserRole , COL_WPT_TIME);
    colWpt->addItem(trUtf8("Orientation magnétique"));
    colWpt->item(7)->setData(Qt::UserRole , COL_WPT_MAGVAR);
    colWpt->addItem(trUtf8("Hauteur du géoïde"));
    colWpt->item(8)->setData(Qt::UserRole , COL_WPT_GEOIDHEIGHT);
    colWpt->addItem(trUtf8("Commentaire"));
    colWpt->item(9)->setData(Qt::UserRole , COL_WPT_CMT);
    colWpt->addItem(trUtf8("Description"));
    colWpt->item(10)->setData(Qt::UserRole , COL_WPT_DESC);
    colWpt->addItem(trUtf8("Appareil"));
    colWpt->item(11)->setData(Qt::UserRole , COL_WPT_SRC);
    colWpt->addItem(trUtf8("Type Gps"));
    colWpt->item(12)->setData(Qt::UserRole , COL_WPT_FIX);
    colWpt->addItem(trUtf8("Nombre de satellites"));
    colWpt->item(13)->setData(Qt::UserRole , COL_WPT_SAT);
    colWpt->addItem(trUtf8("Mesure Gps"));
    colWpt->item(14)->setData(Qt::UserRole , COL_WPT_DGPSID);
    colWpt->addItem(trUtf8("Liens"));
    colWpt->item(15)->setData(Qt::UserRole , COL_WPT_LINKS);
    colWpt->addItem(trUtf8("Symbole"));
    colWpt->item(16)->setData(Qt::UserRole , COL_WPT_SYM);
    colWpt->addItem(trUtf8("Type"));
    colWpt->item(17)->setData(Qt::UserRole , COL_WPT_TYPE);
    colWpt->addItem(trUtf8("hdop"));
    colWpt->item(18)->setData(Qt::UserRole , COL_WPT_HDOP);
    colWpt->addItem(trUtf8("vdop"));
    colWpt->item(19)->setData(Qt::UserRole , COL_WPT_VDOP);
    colWpt->addItem(trUtf8("pdop"));
    colWpt->item(20)->setData(Qt::UserRole , COL_WPT_PDOP);
    colWpt->addItem(trUtf8("Extentions"));
    colWpt->item(21)->setData(Qt::UserRole , COL_WPT_EXTENTIONS);

    colWpt->setSelectionMode(QAbstractItemView::ExtendedSelection);
    colWpt->item(0)->setSelected(true);
    //colWpt->item(9)->setSelected(true);
    //colWpt->item(10)->setSelected(true);
    layout->addWidget(colWpt, 4 , 1);

    QLabel *colSearchInLabel = new QLabel(trUtf8("Rechercher dans"), this);
    colSearchInLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(colSearchInLabel, 4 , 0);

    QLabel *filterLabel = new QLabel(trUtf8("Filtre"), this);
    filterLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    filterSelectedListBox = new QComboBox(this);
    filterSelectedListBox->addItem(trUtf8("Tous"));
    filterSelectedListBox->addItem(trUtf8("Sélectionnés"));
    filterSelectedListBox->addItem(trUtf8("Non sélectionnés"));
    layout->addWidget(filterLabel, 5 , 0) ;
    layout->addWidget(filterSelectedListBox, 5 , 1);

    QLabel *actionLabel = new QLabel(trUtf8("Action"), this);
    actionLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    actionSelectedListBox = new QComboBox(this);
    actionSelectedListBox->addItem(trUtf8("Ne rien faire"));
    actionSelectedListBox->addItem(trUtf8("Couleur"));
    actionSelectedListBox->addItem(trUtf8("Selectionner la ligne"));
    actionSelectedListBox->addItem(trUtf8("Dé-selectionner la ligne"));
    actionSelectedListBox->addItem(trUtf8("Chercher"));
    layout->addWidget(actionLabel, 6 , 0) ;
    layout->addWidget(actionSelectedListBox, 6 , 1);
    actionSelectedListBox->setCurrentIndex(HILIGHT_ACTION_SEARCH);
    connect(actionSelectedListBox, SIGNAL(activated(int)), this, SLOT(onActionSelectedListBox(int)));

    QLabel *strResultSearchLabel = new QLabel( trUtf8("Résultat"), this);
    strResultSearchLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(strResultSearchLabel, 7 , 0);
    strResultSearch = new QLabel( trUtf8(""), this);
    layout->addWidget(strResultSearch, 7 , 1);

    layout->addWidget( ButtonBox , 99 , 1);

    setLayout(layout);

    setWindowTitle( QString::fromUtf8("Chercher"));
}

void dialogFind::onActionSelectedListBox(int index)
{

    debugp("Index=" + QString::number( index));

    if (index  == HILIGHT_ACTION_NOTHING || index  == HILIGHT_ACTION_CHECK || index  == HILIGHT_ACTION_UNCHECK || index  == HILIGHT_ACTION_SEARCH || index  == HILIGHT_ACTION_COLOR  )
    {
        if (regexCheckBox->checkState() == false)
        {
            caseCheckBox->setEnabled(true);
            accentCheckBox->setEnabled(true);

        }
        regexCheckBox->setEnabled(true);
        strSearch->setEnabled(true);

    }


}

void dialogFind::onStrSearchChange( const QString &strNewval  )
{
    strSearch->setToolTip(  getPreviewStrSearch(strSearch->text()) );

}

void dialogFind::OnCaseCheckBox(int state)
{
    strSearch->setToolTip(  getPreviewStrSearch(strSearch->text()) );

}
void dialogFind::OnAccentCheckBox(int state)
{

    strSearch->setToolTip(  getPreviewStrSearch(strSearch->text()) );

}
QString dialogFind::getPreviewStrSearch( QString strValue )
{
    QString strRet;

    strRet = strValue;

    if (accentCheckBox->checkState() == Qt::Unchecked)
    {
        strRet = removeAccents(strRet);
    }
    if (caseCheckBox->checkState() == Qt::Checked)
    {
        strRet = strRet.toLower();
    }

    return strRet;
}


void dialogFind::OnRegexCheckBox(int state)
{
    if (state == Qt::Checked)
    {
        caseCheckBox->setEnabled(false);
        accentCheckBox->setEnabled(false);
    }
    else
    {
        caseCheckBox->setEnabled(true);
        accentCheckBox->setEnabled(true);
    }
}

void dialogFind::okClicked()
{
    projectDatas::wptType *m_wpt;
    int rowAttrib;
    int colIndex;
    int countOcc;
    QString stringSrc;
    qlonglong countCheck = 0;
    qlonglong countCheckLine = 0;
    QRegExp rxItem;


    this->setCursor(Qt::WaitCursor);

    QPalette pal = QPalette(strResultSearch->palette());
    pal.setColor(QPalette::WindowText,QColor(Qt::blue));
    strResultSearch->setPalette(pal);

    strResultSearch->setText( QString::fromUtf8("Calcul en cours ..."));

    if (actionSelectedListBox->currentIndex() == HILIGHT_ACTION_CHECK || actionSelectedListBox->currentIndex()  == HILIGHT_ACTION_UNCHECK || actionSelectedListBox->currentIndex()  == HILIGHT_ACTION_SEARCH || actionSelectedListBox->currentIndex()  == HILIGHT_ACTION_COLOR  )
    {
        if (strSearch->text() == "" )
        {
            strResultSearch->setText( QString::fromUtf8("Chaine vide."));
            this->setCursor(Qt::ArrowCursor);
            return;
        }

    }

    bool oldState = m_wptEdit->blockSignals(true);
    // Foreach wpt data to search in
    for(int rowData = 0; rowData < m_wptEdit->rowCount(); rowData++)
    {
        // Get current line id and reset wpt line color
        m_wpt = m_projectDatas->getWptFromId2( rowData +1 );

        if (m_wpt==NULL)
        {
            debugp("m_wpt is NULL");
            continue;
        }
        // Apply data filter
        if ( filterSelectedListBox->currentIndex() == 2 && m_wpt->m_selected == true) continue;
        if ( filterSelectedListBox->currentIndex() == 1 && m_wpt->m_selected == false) continue;

        // Foreach attribute to search in ...
        for(rowAttrib = 0; rowAttrib < colWpt->count(); rowAttrib++)
        {
            QListWidgetItem *item = colWpt->item(rowAttrib);

            // Do not search here (col not selected)
            if (item->isSelected() == false) continue;

            colIndex = item->data(Qt::UserRole).toInt();

            m_wptEdit->item( rowData, m_parent->tableWpt->m_colsDisposition.getWptColIndexByMeta(colIndex))->setBackground( noColor );
            stringSrc=getCellValue( colIndex, m_wpt);

            if (actionSelectedListBox->currentIndex() == HILIGHT_ACTION_CHECK || actionSelectedListBox->currentIndex()  == HILIGHT_ACTION_UNCHECK || actionSelectedListBox->currentIndex()  == HILIGHT_ACTION_SEARCH || actionSelectedListBox->currentIndex()  == HILIGHT_ACTION_COLOR  )
            {
                countOcc=0;
                if (regexCheckBox->isChecked() == false)
                {
                    countOcc = findStr( stringSrc, strSearch->text(), accentCheckBox->isChecked(), caseCheckBox->isChecked() );
                    debugp("occ count = " + QString::number( countOcc));
                }
                else
                {
                    rxItem = QRegExp( strSearch->text() );

                    if (rxItem.isValid())
                    {
                        if( rxItem.indexIn( stringSrc ) != -1 )
                        {
                            int pos = 0;
                            while (( pos = rxItem.indexIn(stringSrc, pos)) != -1)
                            {
                                ++countOcc ;
                                pos += rxItem.matchedLength();
                            }
                        }
                    }
                    else
                    {
                        QPalette pal = QPalette(strResultSearch->palette());
                        pal.setColor(QPalette::WindowText,QColor(errorColor));
                        strResultSearch->setPalette(pal);
                        strResultSearch->setText( rxItem.errorString() );
                        this->setCursor(Qt::ArrowCursor);
                        return;
                    }
                }
            }
            else
            {
                countOcc =1;
            }

            // Actual find
            if (countOcc > 0)
            {
                countCheck+=countOcc;
                countCheckLine++;

                if (actionSelectedListBox->currentIndex() == HILIGHT_ACTION_COLOR )
                    m_wptEdit->item( rowData, m_parent->tableWpt->m_colsDisposition.getWptColIndexByMeta(colIndex))->setBackground( foundColor );

                if (actionSelectedListBox->currentIndex() == HILIGHT_ACTION_CHECK )
                {
                    m_wpt->m_selected = true;
                    m_parent->tableWpt->displayWptLine( rowData, m_wpt);
                    m_wptEdit->updateMarkerByWpt(m_wpt);
                    m_parent->slippyMap.update();
                }

                if (actionSelectedListBox->currentIndex() == HILIGHT_ACTION_UNCHECK )
                {
                    debugp("deselect line (to false) ");
                    m_wpt->m_selected = false;
                    m_wptEdit->displayWptLine( rowData, m_wpt);
                    m_wptEdit->updateMarkerByWpt(m_wpt);
                    m_parent->slippyMap.update();
                }
                if (actionSelectedListBox->currentIndex() == HILIGHT_ACTION_SEARCH )
                {
                }

            }
        }

    }

    m_wptEdit->blockSignals(oldState );

    debugp("-------------- End search");

    pal.setColor(QPalette::WindowText,QColor(Qt::darkGreen));
    strResultSearch->setPalette(pal);

    strResultSearch->setText(QString::number(countCheck) + " occurences. " + QString::number(countCheckLine) + " lignes");

    this->setCursor(Qt::ArrowCursor);

}

int dialogFind::findStr( QString strSearch, QString strNeedle,  bool accentSensi, bool caseSensi)
{

    int findStrCount;
    QString strRetW;
    int indexSearchFrom;
    Qt::CaseSensitivity cs = Qt::CaseSensitive;

    findStrCount = 0;
    indexSearchFrom=0;

    debugp( "strSearch=[" + strSearch + "] strNeedle=[" + strNeedle + "] accentSensi=" + accentSensi + " caseSensi=" + caseSensi);

    if (caseSensi== true) cs = Qt::CaseSensitive;
    else cs = Qt::CaseInsensitive;

    if (accentSensi == false)
    {
        strSearch = removeAccents( strSearch );
        strNeedle = removeAccents( strNeedle );
    }
    debugp("strSearch=[" + strSearch + "] / needle=" + strNeedle );

    while ((indexSearchFrom = strSearch.indexOf(strNeedle, indexSearchFrom, cs)) != -1)
    {
        debugp( "found tag at index position : " +  QString::number(indexSearchFrom));
        ++indexSearchFrom;
        findStrCount++;
    }

    return findStrCount ;
}


QString  dialogFind::doFind( QString strSearch, QString strNeedle,  QString strBy, bool accentSensi, bool caseSensi)
{

    int findStrCount;
    QString strRetW;
    int indexSearchFrom;
    int prevIndexSearchFrom;
    Qt::CaseSensitivity cs = Qt::CaseSensitive;
    QString retStr;
    QString refStr;

    findStrCount = 0;
    indexSearchFrom=0;

    refStr = strSearch;
    retStr = ""; //strSearch;

    debugp( "doFind : strSearch=[" + strSearch + "] strNeedle=[" + strNeedle + "] strBy = [" + strBy+ "]  accentSensi=" +accentSensi+ " caseSensi=" + caseSensi);

    if (caseSensi== true) cs = Qt::CaseSensitive;
    else cs = Qt::CaseInsensitive;

    if (accentSensi == false)
    {
        refStr = removeAccents(refStr);
        strNeedle = removeAccents( strNeedle );
    }

    prevIndexSearchFrom = 0;
    int lenpart=-1;

    while ((indexSearchFrom = refStr.indexOf(strNeedle, indexSearchFrom, cs)) != -1)
    {
        if (lenpart==-1)   // Premier passage
        {
            lenpart = indexSearchFrom;
            prevIndexSearchFrom = 0; //strBy.length();
        }
        else   lenpart = indexSearchFrom - prevIndexSearchFrom  ;

        debugp( "@" + QString::number(prevIndexSearchFrom) + " lenpart=" + QString::number(lenpart)) ;
        debugp( "add part =" + strSearch.mid( prevIndexSearchFrom, lenpart) );
        retStr = retStr + strSearch.mid( prevIndexSearchFrom, lenpart) + strBy;

        indexSearchFrom += 1 ;
        prevIndexSearchFrom = indexSearchFrom + strNeedle.length() - 1;
        findStrCount++;
    }

    debugp( "end loop: prevIndexSearchFrom= " + QString::number(prevIndexSearchFrom) + " len str=" + QString::number(strSearch.length()));
    debugp( "add part (final) =" + strSearch.mid( prevIndexSearchFrom, strSearch.length()-prevIndexSearchFrom));
    retStr = retStr + strSearch.mid( prevIndexSearchFrom, strSearch.length()-prevIndexSearchFrom) ;
    return retStr ;
}

QString dialogFind::getCellValue( int colIndex, projectDatas::wptType *m_wpt)
{

    switch ( colIndex  )
    {

    case  COL_WPT_NAME :
        return m_wpt->m_name;
        break;

    case COL_WPT_NOTE :
        return m_wpt->m_note;
        break;

    case COL_WPT_GROUP :
        return m_wpt->m_group;
        break;

    case COL_WPT_LAT :
        return m_wpt->m_lat;
        break;

    case COL_WPT_LON :
        return m_wpt->m_lon;
        break;

    case COL_WPT_ELE :
        return m_wpt->m_ele;
        break;

    case COL_WPT_TIME :
        return m_wpt->m_time.toString("dd/MM/yyyy HH:mm");
        break;

    case COL_WPT_MAGVAR :
        return m_wpt->m_magvar;
        break;

    case COL_WPT_GEOIDHEIGHT :
        return m_wpt->m_geoidheight;
        break;

    case COL_WPT_DESC :
        return m_wpt->m_desc;
        break;

    case COL_WPT_CMT :
        return m_wpt->m_cmt;
        break;

    case COL_WPT_SRC :
        return m_wpt->m_src;
        break;

    case COL_WPT_FIX :
        return m_wpt->m_fix;
        break;

    case COL_WPT_SAT :
        return m_wpt->m_sat;
        break;

    case COL_WPT_DGPSID :
        return m_wpt->m_dgpsid;
        break;

    case COL_WPT_LINKS :
        //return m_wpt->m_links;
        return "";
        break;

    case COL_WPT_SYM :
        return m_wpt->m_sym;
        break;

    case COL_WPT_TYPE :
        return m_wpt->m_type;
        break;

    case COL_WPT_HDOP :
        return m_wpt->m_hdop;
        break;

    case COL_WPT_VDOP :
        return m_wpt->m_vdop;
        break;

    case COL_WPT_PDOP :
        return m_wpt->m_pdop;
        break;

    case COL_WPT_EXTENTIONS :
        //return m_wpt->m_extensions;
        break;
    }
    return "";

}

void dialogFind::cancelClicked()
{
    QDialog::accept();
    m_parent->dialogFindDisplayed=false;
}

void dialogFind::showEvent( QShowEvent* event )
{

    QWidget::showEvent( event );


}
void dialogFind::setFormatGpx( projectDatas *p_projectDatas)
{
    m_projectDatas = p_projectDatas;

}

void dialogFind::setWptEdit( wptEdit *p_wptedit )
{
    m_wptEdit = p_wptedit;

}


QString dialogFind::removeAccents(QString s)
{
    QString diacriticLetters_;
    QStringList noDiacriticLetters_;

    if (diacriticLetters_.isEmpty())
    {
        diacriticLetters_ = QString::fromUtf8("ŠŒŽšœžŸ¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ");
        noDiacriticLetters_ << "S"<<"OE"<<"Z"<<"s"<<"oe"<<"z"<<"Y"<<"Y"<<"u"<<"A"<<"A"<<"A"<<"A"<<"A"<<"A"<<"AE"<<"C"<<"E"<<"E"<<"E"<<"E"<<"I"<<"I"<<"I"<<"I"<<"D"<<"N"<<"O"<<"O"<<"O"<<"O"<<"O"<<"O"<<"U"<<"U"<<"U"<<"U"<<"Y"<<"s"<<"a"<<"a"<<"a"<<"a"<<"a"<<"a"<<"ae"<<"c"<<"e"<<"e"<<"e"<<"e"<<"i"<<"i"<<"i"<<"i"<<"o"<<"n"<<"o"<<"o"<<"o"<<"o"<<"o"<<"o"<<"u"<<"u"<<"u"<<"u"<<"y"<<"y";
    }

    QString output = "";
    for (int i = 0; i < s.length(); i++)
    {
        QChar c = s[i];
        int dIndex = diacriticLetters_.indexOf(c);
        if (dIndex < 0)
        {
            output.append(c);
        }
        else
        {
            QString findment = noDiacriticLetters_[dIndex];
            output.append(findment);
        }
    }

    return output;
}

void dialogFind::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
        QDialog::keyPressEvent(e);
    else
    {
        m_parent->dialogFindDisplayed=false;
        QDialog::accept();
    }
}

void dialogFind::closeEvent(QCloseEvent *event)
{
    //emit dialogClosed();
    m_parent->dialogFindDisplayed=false;
    QDialog::accept();
}

#include "dialogFind.moc"



#include "mainWindow.h"
#include "projectData.h"

#include "dialogCsv.h"
#include "wptEdit.h"

#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPalette>
#include <QFile>
#include <QStringList>
#include <QDebug>

// Position des champs modifiables
#define COLDSP_INDEX_NAME 2
#define COLDSP_INDEX_FORMAT 3

dialogCsvImport::dialogCsvImport(mainWindow  *parent, int modeInOut) : QDialog(parent)
{

    m_parent = parent;
    modeImportExport = modeInOut;
    maxSampleDataRow = 7;

    resize(900, 550); // W x H
    QGridLayout *layout = new QGridLayout();

    QDialogButtonBox *ButtonBox = new QDialogButtonBox( Qt::Horizontal );

    QPushButton *buttonRefresh;
    buttonRefresh = new QPushButton( "Recharger les données" );
    connect( buttonRefresh, SIGNAL(clicked()), this, SLOT(refreshClicked()) );
    ButtonBox->addButton( buttonRefresh, QDialogButtonBox::RejectRole );

    QPushButton *buttonCancel;
    buttonCancel = new QPushButton( "Annuler" );
    connect( buttonCancel, SIGNAL(clicked()), this, SLOT(cancelClicked()) );
    ButtonBox->addButton( buttonCancel, QDialogButtonBox::RejectRole );

    QPushButton *buttonOk;
    buttonOk = new QPushButton( "Ok" );
    connect( buttonOk, SIGNAL(clicked()), this, SLOT(okClicked()) );
    ButtonBox->addButton( buttonOk, QDialogButtonBox::RejectRole );

    // Tab Wpt Columns Disposition -----------------------------------------------------------------------------------------
    debugp ( "csv #enter col display");

    tableWidgetColDispo = new QTableWidget(0, 0, this);
    tableWidgetColDispo->setAlternatingRowColors(true);
    tableWidgetColDispo->horizontalHeader()->setStretchLastSection(true);
    tableWidgetColDispo->setColumnCount(maxSampleDataRow);
    debugp ( "csv init done. 6 cols.");

    tableWidgetColDispo->setContextMenuPolicy(Qt::CustomContextMenu);
    connect( tableWidgetColDispo, SIGNAL( customContextMenuRequested(const QPoint &) ), this, SLOT( ctxMenuColDispoUp(const QPoint &) ) );

    // Sample datas
    debugp ( "init sample data qtablewidget");
    tableWidgetSampleData = new QTableWidget(0, 0, this);
    tableWidgetSampleData ->setAlternatingRowColors(true);
    tableWidgetSampleData->setContextMenuPolicy(Qt::CustomContextMenu);
    connect( tableWidgetSampleData, SIGNAL( customContextMenuRequested(const QPoint &) ), this, SLOT( ctxMenuWidgetSampleData(const QPoint &) ) );

    fistLineIsHeaderCheck = new QCheckBox( QString::fromUtf8("La première ligne contient les entêtes de colonnes"));
    fistLineIsHeaderCheck->setChecked( true );
    connect(fistLineIsHeaderCheck, SIGNAL(clicked(bool)), this, SLOT(onFistLineIsHeaderCheck(bool)));

    // Autres paramètres du fichier csv
    QLabel *csvSepFieldLabel = new QLabel("Séparateur de champ:", this);
    csvSepFieldLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    csvSepField = new QLineEdit(this);
    csvSepField->setFixedWidth(15*4);

    QLabel *csvSepTextLabel = new QLabel("Séparateur de texte:", this);
    csvSepTextLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    csvSepText = new QLineEdit(this);
    csvSepText->setFixedWidth(15*4);

    tableWidgetSampleData ->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout->addWidget( tableWidgetSampleData ,0, 0, 1, 0);

    tableWidgetColDispo->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout->addWidget( tableWidgetColDispo ,1, 0, 1, 0);

    fistLineIsHeaderCheck ->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout->addWidget( fistLineIsHeaderCheck , 2 , 0, 1 ,0 );

    layout->addWidget( csvSepFieldLabel , 3 , 0);
    layout->addWidget( csvSepField , 3 , 1);

    layout->addWidget( csvSepTextLabel , 4 , 0);
    layout->addWidget( csvSepText , 4 , 1);

    layout->addWidget( ButtonBox , 5 , 1);

    setLayout(layout);

    setWindowTitle( QString::fromUtf8("Import Csv"));

    connect(tableWidgetColDispo, SIGNAL(cellChanged(int, int)), this, SLOT(onCellEditedTableWidgetColDispo(int, int)));

    debugp ( "End object creation");
}

void dialogCsvImport::initCsvParam( csvParam *csvParamValues )
{
    debugp("Init fired !");

    csvParamdlgVal = csvParamValues;

    csvSepField->setText(csvParamdlgVal->csvSepFieldValue);
    csvSepText->setText(csvParamdlgVal->csvSepTextValue);

    if (csvParamdlgVal->fistLineIsHeader == true)
        fistLineIsHeaderCheck->setChecked( true );
    else fistLineIsHeaderCheck->setChecked( false );

    // Colonnes à importer / exporter, Context menu sorted
    contextMenuLibElem contextMenuLibListeNode;

    for (int j= 0 ; j < COL_WPT_COUNT ; j++ )
    {
        contextMenuLibListeNode.id =  csvParamValues->columns.m_colBase[j].id;
        contextMenuLibListeNode.name = csvParamValues->columns.m_colBase[j].name ;
        contextMenuLibListeNode.colvisible = false ;
        contextMenuLibListeNode.format = csvParamValues->columns.m_colBase[j].format ;
        contextMenuLibListeNode.tooltip = csvParamValues->columns.m_colBase[j].tooltip ;
        contextMenuLibListeNode.len = csvParamValues->columns.m_colBase[j].len ;
        contextMenuLibListeNode.order = -1 ;

        contextMenuLibListe.append (contextMenuLibListeNode );
    }

    qSort(contextMenuLibListe);

}

void dialogCsvImport::showEvent( QShowEvent* event )
{
    QWidget::showEvent( event );

    debugp(" show fired !");

    displayWptColImport();
    displayTableWidgetSampleDataImport();

}

bool dialogCsvImport::okClicked()
{

    for (int j= 0 ; j < COL_WPT_COUNT ; j++ )
    {
        if (contextMenuLibListe[j].order != -1)
        {
            QDialog::accept();
            return true;
        }
    }

    m_parent->projectData.lastErrorText = QString::fromUtf8("Aucune correspondance de champ n'a été trouvée");
    close();
    return false;
}

bool dialogCsvImport::cancelClicked()
{
    m_parent->projectData.lastErrorText = "";
    close();
    return false;
}

void dialogCsvImport::refreshClicked()
{
    debugp("reload");

    if (csvParamdlgVal->csvSepFieldValue != csvSepField->text() )
    {
        QCheckBox *mcb;
        for (int row = 0 ; row < COL_WPT_COUNT ; row++)
        {
            mcb = qobject_cast<QCheckBox*>(tableWidgetColDispo->cellWidget(row, 0));
            mcb->setChecked(false);

            contextMenuLibListe[row].colvisible = false;
            contextMenuLibListe[row].order = -1;
        }
    }

    csvParamdlgVal->csvSepFieldValue = csvSepField->text();
    csvParamdlgVal->csvSepTextValue = csvSepText->text();

    displayTableWidgetSampleData();

}


void dialogCsvImport::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
        QDialog::keyPressEvent(e);
    else
    {
        debugp("dialog closed by esc key");
        QDialog::accept();
    }
}
void dialogCsvImport::ctxMenuColDispoUp( const QPoint &pos )
{
    wptColLig = tableWidgetColDispo->rowAt(pos.y());
    QMenu *menu = new QMenu;
    menu->exec(QCursor::pos());
}

int dialogCsvImport::contextMenuLibElemPosByColPos( int currentColPos )
{

    int currentColAssignation = -1;
    for (int rowCurrent = 0 ; rowCurrent < COL_WPT_COUNT ; rowCurrent++)
    {
        if (contextMenuLibListe[rowCurrent].order == currentColPos )
        {
            currentColAssignation = rowCurrent;
            break;
        }
    }
    return currentColAssignation;
}


int dialogCsvImport::contextMenuLibElemPosById( int colId )
{
    int currentColAssignation = -1;
    for (int rowCurrent = 0 ; rowCurrent < COL_WPT_COUNT ; rowCurrent++)
    {
        if (contextMenuLibListe[rowCurrent].id == colId )
        {
            currentColAssignation = rowCurrent;
            break;
        }
    }
    return currentColAssignation;
}


void dialogCsvImport::ctxMenuWidgetSampleData( const QPoint &pos )
{
    currentCol = tableWidgetSampleData->columnAt(pos.x());

    debugp(  "contextmenu menu col selected =" +  QString::number(currentCol ) );
    QMenu *menu = new QMenu;

    int countChecked;
    countChecked=0;
    for (int j= 0 ; j < COL_WPT_COUNT ; j++ )
    {
        if (contextMenuLibListe[j].colvisible == true)
            countChecked++;
    }

    menu->addAction( QString::fromUtf8("Tout selectionner") , this, SLOT(colCsvColSelectAll()));
    menu->addAction( QString::fromUtf8("Tout de-selectionner"), this, SLOT(colCsvColunSelectAll()));
    menu->addSeparator();

    if (!countChecked)
        menu->actions().at(CSV_CTXMENU_UNSELECTALL)->setEnabled(false);
    if (countChecked == COL_WPT_COUNT)
        menu->actions().at(CSV_CTXMENU_SELECTALL)->setEnabled(false);

    int currentColAssignation = contextMenuLibElemPosByColPos( currentCol );
    if (currentColAssignation != -1)
    {
        if (contextMenuLibListe[currentColAssignation].colvisible == false)
            menu->addAction( QString::fromUtf8("Selectionner \"") + contextMenuLibListe[currentColAssignation].name +"\"", this, SLOT(colCsvColSelectCurrentCol()));
        else
            menu->addAction( QString::fromUtf8("Dé-Selectionner \"") + contextMenuLibListe[currentColAssignation].name +"\"", this, SLOT(colCsvColSelectCurrentCol()));

        menu->addSeparator();

    }


    QActionGroup* actions1 = new QActionGroup(menu);
    actions1->setExclusive(false);

    for (int j= 0 ; j < COL_WPT_COUNT ; j++ )
    {
        QAction *actionMenuItem  = menu->addAction( contextMenuLibListe[j].name  );
        actionMenuItem->setData( contextMenuLibListe[j].id );
        actions1->addAction( actionMenuItem );
        if (contextMenuLibListe[j].colvisible == true)
        {
            actionMenuItem->setCheckable(true);
            actionMenuItem->setChecked(true);
        }
        else
        {
            actionMenuItem->setCheckable(false);
            actionMenuItem->setChecked(false);
        }

    }
    connect(actions1, SIGNAL(triggered(QAction*)), SLOT(onSetDefcol(QAction*)));
    menu->exec(this->mapToGlobal(pos));

}

// context menu sample data assign column
void dialogCsvImport::onSetDefcol(QAction *action)
{
    int idSetcol = action->data().toInt();

    debugp("Col id selected, to set here id = " + QString::number(idSetcol));

    //debugp("Prev id Found @" + QString::number(currentCol) + " :" + QString::number(contextMenuLibListe[currentCol].id));

    int prevColPos = -1;
    for (int rowCurrent = 0 ; rowCurrent < COL_WPT_COUNT ; rowCurrent++)
    {
        if (contextMenuLibListe[rowCurrent].order == currentCol )
        {
            prevColPos = rowCurrent;
            break;
        }
    }
    if (prevColPos != -1)
    {
        debugp("Prev id Found @" + QString::number(prevColPos) + " :" + QString::number(contextMenuLibListe[prevColPos].id));
    }

    for (int rowCurrent = 0 ; rowCurrent < COL_WPT_COUNT ; rowCurrent++)
    {
        //debugp("Col ? " + QString::number(rowCurrent) + " / " + QString::number(idSetcol) );
        if (contextMenuLibListe[rowCurrent].id == idSetcol )
        {
            debugp("Found, currentCol @" + QString::number(currentCol));

            if (contextMenuLibListe[rowCurrent].colvisible == false)
            {
                contextMenuLibListe[rowCurrent].order = currentCol;
                contextMenuLibListe[rowCurrent].colvisible = true;
                if (prevColPos != -1)
                {
                    contextMenuLibListe[prevColPos].order = -1;
                    contextMenuLibListe[prevColPos].colvisible = false;
                }
            }
            else
            {
                contextMenuLibListe[rowCurrent].order = -1;
                contextMenuLibListe[rowCurrent].colvisible = false;
            }
            displayWptColImport();
            displayTableWidgetSampleData();
            return;
        }

    }

}
void dialogCsvImport::doSelectByCurrentcol(  int idSetcol  )
{

    int rowCurrent;
    int rowSrc=0;
    for (rowCurrent = 0 ; rowCurrent < COL_WPT_COUNT ; rowCurrent++)
    {
        if (rowCurrent == currentCol)
        {
            //debugp ("Col name Src =" + csvDef.columns.m_colBase[rowCurrent].name + "," + QString::number(rowSrc));
            rowSrc = rowCurrent;
            debugp ("Col name Src =" + contextMenuLibListe[rowCurrent].name + "," + QString::number(rowSrc));
        }

    }
    int rowDst=0;
    for (rowCurrent = 0 ; rowCurrent < COL_WPT_COUNT ; rowCurrent++)
    {
        if (contextMenuLibListe[rowCurrent].id == idSetcol )
        {
            rowDst = rowCurrent;
            debugp ("Col name Dst =" + contextMenuLibListe[rowCurrent].name + ", rowdst=" + QString::number(rowDst));
        }

    }

    if (contextMenuLibListe[rowDst].colvisible == false)
    {

        for (int j = 0 ; j< COL_WPT_COUNT ; j++)
        {
            if ( contextMenuLibListe[j].order == rowSrc )
            {
                contextMenuLibListe[j].order = -1;
                contextMenuLibListe[j].colvisible = false;
                break;
            }
        }

        contextMenuLibListe[rowDst].colvisible = true;
        contextMenuLibListe[rowDst].order = rowSrc;

    }
    else
    {
        contextMenuLibListe[rowDst].colvisible = false;
        contextMenuLibListe[rowDst].order = -1;
    }

    displayTableWidgetSampleData();

}


void dialogCsvImport::onCellEditedTableWidgetColDispo(int row , int col)
{

    debugp("on cell edited row, col : " + QString::number(row) + ", " + QString::number(col));

    if (col == COLDSP_INDEX_NAME )
    {
        //debugp("new val = " + tableWidgetColDispo->item(row, 1)->text() );
        contextMenuLibListe[row].name = tableWidgetColDispo->item(row, COLDSP_INDEX_NAME)->text();
        displayTableWidgetSampleData();
    }

    if (col == COLDSP_INDEX_FORMAT )
    {
        debugp("new val fmt = " + tableWidgetColDispo->item(row, COLDSP_INDEX_FORMAT)->text() );
        contextMenuLibListe[row].format = tableWidgetColDispo->item(row, COLDSP_INDEX_FORMAT)->text();
        displayTableWidgetSampleData();
    }


}

void dialogCsvImport::colCsvColToRight(void)
{
    debugp("To Right col " + QString::number(currentCol));
    swapCol( currentCol, currentCol+1);
}
void dialogCsvImport::colCsvColToLeft(void)
{
    debugp("To Left col " + QString::number(currentCol));
    swapCol( currentCol, currentCol-1 );
}

void dialogCsvImport::swapCol( int oldPos, int newpos)
{

    debugp("Swap col : " + QString::number(oldPos) + " -> " + QString::number(newpos));
    std::swap( contextMenuLibListe[oldPos], contextMenuLibListe[newpos]);
    displayTableWidgetSampleData();
}

void dialogCsvImport::moveCol( int oldPos, int newpos)
{

    debugp("moveCol col : " + QString::number(oldPos) + " -> " + QString::number(newpos));

    contextMenuLibListe.insert(newpos, contextMenuLibListe.at(oldPos) );
    contextMenuLibListe.remove(oldPos+1);

    displayTableWidgetSampleData();
}

void dialogCsvImport::colCsvColSelectCurrentCol(  )
{
    debugp(  "Select col=" +  QString::number(currentCol ) );


    int currentColAssignation = contextMenuLibElemPosByColPos( currentCol );
    debugp("currentColAssignation =" + QString::number(currentColAssignation ));
    contextMenuLibListe[currentColAssignation ].order = -1;
    contextMenuLibListe[currentColAssignation ].colvisible = false;
    displayTableWidgetSampleData();
}


void dialogCsvImport::colCsvColunSelectAll()
{
    QCheckBox *mcb;

    for (int row = 0 ; row < COL_WPT_COUNT ; row++)
    {
        mcb = qobject_cast<QCheckBox*>(tableWidgetColDispo->cellWidget(row, 0));
        if (mcb)
            mcb->setChecked(false);

        contextMenuLibListe[row].colvisible = false;
        contextMenuLibListe[row].order = -1;
    }

    displayTableWidgetSampleData();

}
void dialogCsvImport::colCsvColSelectAll()
{

    QCheckBox *mcb;

    for (int row = 0 ; row < COL_WPT_COUNT ; row++)
    {
        mcb = qobject_cast<QCheckBox*>(tableWidgetColDispo->cellWidget(row, 0));
        if (mcb)
            mcb->setChecked(true);
        contextMenuLibListe[row].colvisible = true;
        contextMenuLibListe[row].order = row;
    }

    displayTableWidgetSampleData();

}

// Display information on colons meta datas
void dialogCsvImport::displayWptColImport()
{

    debugp("Enter meta data");

    bool oldState = tableWidgetColDispo->blockSignals(true);

    tableWidgetColDispo->setRowCount(COL_WPT_COUNT );

    for (int j= 0 ; j < COL_WPT_COUNT ; j++ )
    {
        QCheckBox *colVisibleChk  = new QCheckBox("");

        if ( contextMenuLibListe[j].colvisible == true) colVisibleChk->setChecked(true);
        colVisibleChk->setDisabled(true);

        tableWidgetColDispo->setCellWidget(j,0, colVisibleChk);

        QString libOrder;
        if ( contextMenuLibListe[j].order ==-1) libOrder = "";
        else libOrder = QString::number(contextMenuLibListe[j].order+1);

        tableWidgetColDispo->setItem(j,1,   new QTableWidgetItem( QString("%1").arg( libOrder ) )) ;
        tableWidgetColDispo->item(j, 1 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        tableWidgetColDispo->item(j, 1 )->setTextColor(QColor(Qt::blue));
        tableWidgetColDispo->setItem(j,2, new QTableWidgetItem( QString("%1").arg( contextMenuLibListe[j].name  )) );
        tableWidgetColDispo->setItem(j,3, new QTableWidgetItem( QString("%1").arg( contextMenuLibListe[j].format  )) );
        tableWidgetColDispo->setItem(j,4, new QTableWidgetItem( QString("%1").arg( contextMenuLibListe[j].id )) );
        tableWidgetColDispo->setItem(j,5, new QTableWidgetItem( QString("%1").arg( contextMenuLibListe[j].len )) );
        tableWidgetColDispo->setItem(j,6, new QTableWidgetItem( QString("%1").arg( contextMenuLibListe[j].tooltip )) );
        tableWidgetColDispo->item(j, 6 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        tableWidgetColDispo->item(j, 6 )->setTextColor(QColor(Qt::blue));
        tableWidgetColDispo->item(j, 6 )->setTextColor(QColor(Qt::blue));
    }

    // Header
    tableWidgetColDispo->setHorizontalHeaderLabels(QString("Selection;Position;Nom;Format;Id;Longueur;Description").split(";"));

    tableWidgetColDispo->hideColumn(4);
    tableWidgetColDispo->hideColumn(5);

    tableWidgetColDispo->setColumnWidth(0, 40);
    tableWidgetColDispo->setColumnWidth(1, 50);
    tableWidgetColDispo->setColumnWidth(2, 160);
    tableWidgetColDispo->setColumnWidth(3, 90);
    tableWidgetColDispo->setColumnWidth(4, 160);
    tableWidgetColDispo->setColumnWidth(5, 160);
    tableWidgetColDispo->setColumnWidth(6, 160);


    tableWidgetColDispo->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Champ selectionné"));
    tableWidgetColDispo->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Position dns le fichier"));
    tableWidgetColDispo->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Nom du champ"));
    tableWidgetColDispo->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Format des données"));
    tableWidgetColDispo->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Identifiant"));
    tableWidgetColDispo->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Longueur du champ"));
    tableWidgetColDispo->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Description"));

    tableWidgetColDispo->blockSignals(oldState);
    debugp("End meta data");

}

void dialogCsvImport::displayTableWidgetSampleData(void)
{
    displayTableWidgetSampleDataImport();

    // Scroll to colX
    QTableWidgetItem  *tableWidgetItemVisible = tableWidgetSampleData->itemAt(0, currentCol);
    if (tableWidgetItemVisible)
        tableWidgetSampleData->scrollToItem(tableWidgetSampleData->item(tableWidgetItemVisible->row(), currentCol));
}

void  dialogCsvImport::displayTableWidgetSampleDataImport(void)
{

    int dspCol;
    int rowData;

    QString fieldFmt ;

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0, "csv import error : ", file.errorString());
    }

    QTextStream in(&file);

    tableWidgetSampleData->setRowCount(0);

    // Calculer le nombre maximum de colonnes
    fileMaxFields=0;
    //while( !in.atEnd()  && ( rowData <= maxSampleDataRow || maxSampleDataRow == -1) )
    while( !in.atEnd()  && maxSampleDataRow == -1 )
    {
        QString line = in.readLine();
        QStringList fields = line.split(QRegExp(csvParamdlgVal->csvSepFieldValue), QString::KeepEmptyParts );
        if ( fileMaxFields < fields.count() ) fileMaxFields = fields.count() ;
    }
    in.seek(0);

    debugp("Set table widget max col = " + QString::number(fileMaxFields));

    displayTableWidgetSampleDataHeaderImport( fileMaxFields );
    tableWidgetSampleData->setColumnCount(fileMaxFields);

    rowData=0;
    rowDisplay=0;
    while( !in.atEnd()  && ( rowData <= maxSampleDataRow || maxSampleDataRow == -1) )
    {
        QString line = in.readLine();
        //debugp("Line=" + line);

        if ( rowData == 0 && csvParamdlgVal->fistLineIsHeader == true)
        {
            debugp("Skip Line 0 ");
            rowData++;
        }
        else
        {
            int row  = tableWidgetSampleData->rowCount();
            tableWidgetSampleData->insertRow( row );

            //debugp("cut line fields");
            QStringList fields = line.split(QRegExp(csvParamdlgVal->csvSepFieldValue), QString::KeepEmptyParts );
            dspCol=0;

            int colorCell = 0;
            foreach (const QString &field, fields)
            {
                fieldFmt = field;

                debugp("field fieldFmt: " + fieldFmt + "  len = " + QString::number(fieldFmt.length()) + " vs " + QString::number(csvParamdlgVal->csvSepTextValue.length())   );

                if (fieldFmt.length() >= csvParamdlgVal->csvSepTextValue.length() &&
                        fieldFmt.mid(0,csvParamdlgVal->csvSepTextValue.length()) == csvParamdlgVal->csvSepTextValue &&
                        fieldFmt.mid(fieldFmt.length() - 1 , csvParamdlgVal->csvSepTextValue.length()) == csvParamdlgVal->csvSepTextValue)
                {
                    fieldFmt.remove(0, csvParamdlgVal->csvSepTextValue.length());
                    fieldFmt.chop( csvParamdlgVal->csvSepTextValue.length() );
                }

                colorCell = 0;

                //int idxcol = dspCol;

                for (int j = 0 ; j< COL_WPT_COUNT ; j++)
                {

                    if (contextMenuLibListe[j].order == dspCol)
                    {
                        //idxcol = j;
                        colorCell = 1;
                        break;
                    }
                }

                tableWidgetSampleData->setItem(rowDisplay,dspCol, new QTableWidgetItem( QString("%1").arg(fieldFmt )) );

                if (colorCell==0)
                {
                    tableWidgetSampleData->item(rowDisplay, dspCol)->setForeground( QColor(127, 127, 127) );
                }
                else
                {

                    QTableWidgetItem* myItem;
                    myItem = tableWidgetSampleData->item(rowDisplay,dspCol);
                    myItem->setForeground(Qt::red);
                }
                dspCol++;

            } // end foreach fields

            rowData++;
            rowDisplay++;
        }
    }

    file.close();

    setWindowTitle( QString::fromUtf8("Import Csv (") + QString::number(rowDisplay) + " lignes)");

    return ;

}

int dialogCsvImport::displayTableWidgetSampleDataHeaderImport(int colmaxCount )
{
    QString colDesc;
    colDesc="";
    QString sepCol="";
    QStringList tmpLib;

    colDesc="";
    sepCol = "";
    QString libHead;
    for (int rowPos = 0 ; rowPos < colmaxCount ; rowPos++)
    {
        libHead="";
        for (int j = 0 ; j< COL_WPT_COUNT ; j++)
        {
            //debugp("add lib @" + QString::number(rowPos) + ": order=" + QString::number(contextMenuLibListe[j].order) + " :" + contextMenuLibListe[j].name);
            if ( contextMenuLibListe[j].order == rowPos ) //&& contextMenuLibListe[j].order != -1 )
            {
                libHead =  contextMenuLibListe[j].name;
                break;
            }
            else
            {
                libHead="";
            }
        }
        colDesc +=  sepCol + libHead;
        sepCol = ";";
    }
    debugp( "ColDesc = " + colDesc + " , colmaxCount=" + QString::number(colmaxCount));

    tableWidgetSampleData->setColumnCount(0);
    tableWidgetSampleData->setColumnCount(colmaxCount);
    tableWidgetSampleData->setHorizontalHeaderLabels(colDesc.split(";"));

    return colmaxCount;
}


int dialogCsvImport::displayTableWidgetSampleDataHeader(void)
{
    QString colDesc;
    colDesc="";
    int rowCount=0;
    QString sepCol="";
    QString tmpLib[COL_WPT_COUNT];

    for (int rowPos = 0 ; rowPos < COL_WPT_COUNT ; rowPos++)
    {
        for (int j = 0 ; j< COL_WPT_COUNT ; j++)
        {
            if ( contextMenuLibListe[j].order == rowPos )
            {
                tmpLib[rowPos] =  contextMenuLibListe[j].name;
                //debugp("add lib = "+ QString::number(j) + " :" + contextMenuLibListe[j].name);
                break;
            }
        }

        rowCount++;
    }

    colDesc="";
    for (int rowPos = 0 ; rowPos < COL_WPT_COUNT ; rowPos++)
    {

        sepCol = ";";
    }

    debugp( "ColDesc = " + colDesc + " , rowCount=" + QString::number(rowCount));

    tableWidgetSampleData->setColumnCount(0);
    tableWidgetSampleData->setColumnCount(rowCount);
    tableWidgetSampleData->setHorizontalHeaderLabels(colDesc.split(";"));

    return rowCount;
}

void dialogCsvImport::onFistLineIsHeaderCheck(bool)
{
    if (fistLineIsHeaderCheck->checkState())
        csvParamdlgVal->fistLineIsHeader=true;
    else csvParamdlgVal->fistLineIsHeader=false;

    displayTableWidgetSampleData();
}

void dialogCsvImport::onCsvSepField(const QString &newVal)
{
    debugp("new val = " + newVal);
    csvParamdlgVal->csvSepFieldValue = newVal;
    displayTableWidgetSampleData();
}

void dialogCsvImport::oncsvSepText(const QString &newVal)
{
    debugp("new val = " + newVal);
    csvParamdlgVal->csvSepTextValue = newVal;
    displayTableWidgetSampleData();
}

void dialogCsvImportHeaderView::onSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    bool oldState = this->blockSignals(true);

    debugp(" section moved : logicalIndex=" +  QString::number(logicalIndex) + " oldIndex=" +  QString::number(oldVisualIndex) + " newIndex=" +  QString::number(newVisualIndex) ) ;

    m_parent->moveCol(oldVisualIndex, newVisualIndex);

    this->blockSignals(oldState);

}


// ---------------------------------------------------------------------------------------------


dialogCsvExport::dialogCsvExport(mainWindow  *parent, int modeInOut) : QDialog(parent)
{

    m_parent = parent;

    maxSampleDataRow = 7;

    resize(900, 550); // W x H
    QGridLayout *layout = new QGridLayout();

    QDialogButtonBox *ButtonBox = new QDialogButtonBox( Qt::Horizontal );

    QPushButton *buttonRefresh;
    buttonRefresh = new QPushButton( "Recharger les données" );
    connect( buttonRefresh, SIGNAL(clicked()), this, SLOT(refreshClicked()) );
    ButtonBox->addButton( buttonRefresh, QDialogButtonBox::RejectRole );

    QPushButton *buttonCancel;
    buttonCancel = new QPushButton( "Annuler" );
    connect( buttonCancel, SIGNAL(clicked()), this, SLOT(cancelClicked()) );
    ButtonBox->addButton( buttonCancel, QDialogButtonBox::RejectRole );

    QPushButton *buttonOk;
    buttonOk = new QPushButton( "Ok" );
    connect( buttonOk, SIGNAL(clicked()), this, SLOT(okClicked()) );
    ButtonBox->addButton( buttonOk, QDialogButtonBox::RejectRole );

    // Tab Wpt Columns Disposition -----------------------------------------------------------------------------------------
    debugp ( "csv #enter col display");

    tableWidgetColDispo = new QTableWidget(0, 0, this);
    tableWidgetColDispo->setAlternatingRowColors(true);
    tableWidgetColDispo->horizontalHeader()->setStretchLastSection(true);
    tableWidgetColDispo->setColumnCount(maxSampleDataRow);
    debugp ( "csv init done. 6 cols.");

    tableWidgetColDispo->setContextMenuPolicy(Qt::CustomContextMenu);
    connect( tableWidgetColDispo, SIGNAL( customContextMenuRequested(const QPoint &) ), this, SLOT( ctxMenuColDispoUp(const QPoint &) ) );

    // Sample datas
    debugp ( "init sample data qtablewidget");
    tableWidgetSampleData = new QTableWidget(0, 0, this);
    tableWidgetSampleData ->setAlternatingRowColors(true);
    tableWidgetSampleData->setContextMenuPolicy(Qt::CustomContextMenu);
    connect( tableWidgetSampleData, SIGNAL( customContextMenuRequested(const QPoint &) ), this, SLOT( ctxMenuWidgetSampleData(const QPoint &) ) );

    fistLineIsHeaderCheck = new QCheckBox( QString::fromUtf8("La première ligne contient les entêtes de colonnes"));
    fistLineIsHeaderCheck->setChecked( true );
    connect(fistLineIsHeaderCheck, SIGNAL(clicked(bool)), this, SLOT(onFistLineIsHeaderCheck(bool)));

    // Autres paramètres du fichier csv
    QLabel *csvSepFieldLabel = new QLabel("Séparateur de champ:", this);
    csvSepFieldLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    csvSepField = new QLineEdit(this);
    csvSepField->setFixedWidth(15*4);

    QLabel *csvSepTextLabel = new QLabel("Séparateur de texte:", this);
    csvSepTextLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    csvSepText = new QLineEdit(this);
    csvSepText->setFixedWidth(15*4);

    tableWidgetSampleData ->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout->addWidget( tableWidgetSampleData ,0, 0, 1, 0);

    tableWidgetColDispo->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout->addWidget( tableWidgetColDispo ,1, 0, 1, 0);

    fistLineIsHeaderCheck ->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout->addWidget( fistLineIsHeaderCheck , 2 , 0, 1 ,0 );

    layout->addWidget( csvSepFieldLabel , 3 , 0);
    layout->addWidget( csvSepField , 3 , 1);

    layout->addWidget( csvSepTextLabel , 4 , 0);
    layout->addWidget( csvSepText , 4 , 1);

    layout->addWidget( ButtonBox , 5 , 1);

    setLayout(layout);


    setWindowTitle( QString::fromUtf8("Export Csv"));
    dialogCsvExportHeaderViewObj = new dialogCsvExportHeaderView();
    dialogCsvExportHeaderViewObj->m_parent  = this;
    tableWidgetSampleData->setHorizontalHeader(dialogCsvExportHeaderViewObj);


    connect(tableWidgetColDispo, SIGNAL(cellChanged(int, int)), this, SLOT(onCellEditedTableWidgetColDispo(int, int)));

    //colCsvColSelectAll();

    debugp ( "End object creation");
}

void dialogCsvExport::initCsvParam( csvParam *csvParamValues )
{
    debugp("Init fired !");

    csvParamdlgVal = csvParamValues;

    csvSepField->setText(csvParamdlgVal->csvSepFieldValue);
    csvSepText->setText(csvParamdlgVal->csvSepTextValue);

    if (csvParamdlgVal->fistLineIsHeader == true)
        fistLineIsHeaderCheck->setChecked( true );
    else fistLineIsHeaderCheck->setChecked( false );

    // Colonnes à importer / exporter, Context menu sorted
    contextMenuLibElem contextMenuLibListeNode;

    for (int j= 0 ; j < COL_WPT_COUNT ; j++ )
    {
        contextMenuLibListeNode.id =  csvParamValues->columns.m_colBase[j].id;
        contextMenuLibListeNode.name = csvParamValues->columns.m_colBase[j].name ;
        contextMenuLibListeNode.colvisible = false ;
        contextMenuLibListeNode.format = csvParamValues->columns.m_colBase[j].format ;
        contextMenuLibListeNode.tooltip = csvParamValues->columns.m_colBase[j].tooltip ;
        contextMenuLibListeNode.len = csvParamValues->columns.m_colBase[j].len ;
        contextMenuLibListeNode.order = -1 ;

        contextMenuLibListe.append (contextMenuLibListeNode );
    }

    qSort(contextMenuLibListe);

    colCsvColSelectAll();

}

void dialogCsvExport::showEvent( QShowEvent* event )
{

    QWidget::showEvent( event );

    debugp(" show fired !");

    displayWptColExport();
    displayTableWidgetSampleData();
    //colCsvColSelectAll();

}



bool dialogCsvExport::okClicked()
{

    for (int j= 0 ; j < COL_WPT_COUNT ; j++ )
    {
        if (contextMenuLibListe[j].order != -1)
        {
            QDialog::accept();
            return true;
        }
    }

    m_parent->projectData.lastErrorText = QString::fromUtf8("Aucune correspondance de champ n'a été trouvée");
    close();
    return false;
}

bool dialogCsvExport::cancelClicked()
{
    m_parent->projectData.lastErrorText = "";
    close();
    return false;
}

void dialogCsvExport::refreshClicked()
{
    debugp("reload");

    if (csvParamdlgVal->csvSepFieldValue != csvSepField->text() )
    {
        QCheckBox *mcb;
        for (int row = 0 ; row < COL_WPT_COUNT ; row++)
        {
            mcb = qobject_cast<QCheckBox*>(tableWidgetColDispo->cellWidget(row, 0));
            mcb->setChecked(false);

            contextMenuLibListe[row].colvisible = false;
            contextMenuLibListe[row].order = -1;
        }

    }

    csvParamdlgVal->csvSepFieldValue = csvSepField->text();
    csvParamdlgVal->csvSepTextValue = csvSepText->text();

    displayTableWidgetSampleData();

}


void dialogCsvExport::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
        QDialog::keyPressEvent(e);
    else
    {
        debugp("dialog closed by esc key");
        QDialog::accept();
    }
}
void dialogCsvExport::ctxMenuColDispoUp( const QPoint &pos )
{

    wptColLig = tableWidgetColDispo->rowAt(pos.y());

    QMenu *menu = new QMenu;

    menu->exec(QCursor::pos());

}

int dialogCsvExport::contextMenuLibElemPosByColPos( int currentColPos )
{

    int currentColAssignation = -1;
    for (int rowCurrent = 0 ; rowCurrent < COL_WPT_COUNT ; rowCurrent++)
    {
        if (contextMenuLibListe[rowCurrent].order == currentColPos )
        {
            currentColAssignation = rowCurrent;
            break;
        }
    }
    return currentColAssignation;
}


int dialogCsvExport::contextMenuLibElemPosById( int colId )
{
    int currentColAssignation = -1;
    for (int rowCurrent = 0 ; rowCurrent < COL_WPT_COUNT ; rowCurrent++)
    {
        if (contextMenuLibListe[rowCurrent].id == colId )
        {
            currentColAssignation = rowCurrent;
            break;
        }
    }
    return currentColAssignation;
}


void dialogCsvExport::ctxMenuWidgetSampleData( const QPoint &pos )
{
    currentCol = tableWidgetSampleData->columnAt(pos.x());

    debugp(  "contextmenu menu col selected =" +  QString::number(currentCol ) );
    QMenu *menu = new QMenu;

    int countChecked;
    countChecked=0;
    for (int j= 0 ; j < COL_WPT_COUNT ; j++ )
    {
        if (contextMenuLibListe[j].colvisible == true)
            countChecked++;
    }

    menu->addAction( QString::fromUtf8("Tout selectionner") , this, SLOT(colCsvColSelectAll()));
    menu->addAction( QString::fromUtf8("Tout de-selectionner"), this, SLOT(colCsvColunSelectAll()));
    menu->addSeparator();

    if (!countChecked)
        menu->actions().at(CSV_CTXMENU_UNSELECTALL)->setEnabled(false);
    if (countChecked == COL_WPT_COUNT)
        menu->actions().at(CSV_CTXMENU_SELECTALL)->setEnabled(false);



    menu->addAction( QString::fromUtf8("<-- Vers la gauche") , this, SLOT(colCsvColToLeft()));
    menu->addAction( QString::fromUtf8("Vers la droite -->"), this, SLOT(colCsvColToRight()));
    menu->addSeparator();

    if (currentCol==0)
        menu->actions().at(CSV_CTXMENU_MOVELEFT)->setEnabled(false);
    if (currentCol==COL_WPT_COUNT-1)
        menu->actions().at(CSV_CTXMENU_MOVERIGHT)->setEnabled(false);

    if (contextMenuLibListe[currentCol].colvisible == false)
        menu->addAction( QString::fromUtf8("Selectionner \"") + contextMenuLibListe[currentCol].name +"\"", this, SLOT(colCsvColSelectCurrentCol()));
    else
        menu->addAction( QString::fromUtf8("Dé-Selectionner \"") + contextMenuLibListe[currentCol].name +"\"", this, SLOT(colCsvColSelectCurrentCol()));

    menu->addSeparator();



    QActionGroup* actions1 = new QActionGroup(menu);
    actions1->setExclusive(false);

    for (int j= 0 ; j < COL_WPT_COUNT ; j++ )
    {
        QAction *actionMenuItem  = menu->addAction( contextMenuLibListe[j].name  );
        actionMenuItem->setData( contextMenuLibListe[j].id );
        actions1->addAction( actionMenuItem );
        if (contextMenuLibListe[j].colvisible == true)
        {
            actionMenuItem->setCheckable(true);
            actionMenuItem->setChecked(true);
        }
        else
        {
            actionMenuItem->setCheckable(false);
            actionMenuItem->setChecked(false);
        }

    }
    connect(actions1, SIGNAL(triggered(QAction*)), SLOT(onSetDefcol(QAction*)));
    menu->exec(this->mapToGlobal(pos));

}

// context menu sample data assign column
void dialogCsvExport::onSetDefcol(QAction *action)
{
    int idSetcol = action->data().toInt();

    debugp("Col id selected, to set here id = " + QString::number(idSetcol));

    debugp("Prev id Found @" + QString::number(currentCol) + " :" + QString::number(contextMenuLibListe[currentCol].id));

    int prevColPos = -1;
    for (int rowCurrent = 0 ; rowCurrent < COL_WPT_COUNT ; rowCurrent++)
    {
        if (contextMenuLibListe[rowCurrent].order == currentCol )
        {
            prevColPos = rowCurrent;
            break;
        }
    }
    if (prevColPos != -1)
    {
        debugp("Prev id Found @" + QString::number(prevColPos) + " :" + QString::number(contextMenuLibListe[prevColPos].id));
    }

    for (int rowCurrent = 0 ; rowCurrent < COL_WPT_COUNT ; rowCurrent++)
    {
        //debugp("Col ? " + QString::number(rowCurrent) + " / " + QString::number(idSetcol) );
        if (contextMenuLibListe[rowCurrent].id == idSetcol )
        {
            debugp("Found, currentCol @" + QString::number(currentCol));

            if (contextMenuLibListe[rowCurrent].colvisible == false)
            {
                contextMenuLibListe[rowCurrent].order = currentCol;
                contextMenuLibListe[rowCurrent].colvisible = true;
                if (prevColPos != -1)
                {
                    contextMenuLibListe[prevColPos].order = -1;
                    contextMenuLibListe[prevColPos].colvisible = false;
                }
            }
            else
            {
                contextMenuLibListe[rowCurrent].order = -1;
                contextMenuLibListe[rowCurrent].colvisible = false;
            }
            displayWptColExport();
            displayTableWidgetSampleData();
            return;
        }

    }
    //displayWptColExport();
}
void dialogCsvExport::doSelectByCurrentcol(  int idSetcol  )
{

    int rowCurrent;
    int rowSrc=0;
    for (rowCurrent = 0 ; rowCurrent < COL_WPT_COUNT ; rowCurrent++)
    {
        if (rowCurrent == currentCol)
        {
            //debugp ("Col name Src =" + csvDef.columns.m_colBase[rowCurrent].name + "," + QString::number(rowSrc));
            rowSrc = rowCurrent;
            debugp ("Col name Src =" + contextMenuLibListe[rowCurrent].name + "," + QString::number(rowSrc));
        }

    }
    int rowDst=0;
    for (rowCurrent = 0 ; rowCurrent < COL_WPT_COUNT ; rowCurrent++)
    {
        if (contextMenuLibListe[rowCurrent].id == idSetcol )
        {
            rowDst = rowCurrent;
            debugp ("Col name Dst =" + contextMenuLibListe[rowCurrent].name + ", rowdst=" + QString::number(rowDst));
        }

    }

    if (contextMenuLibListe[rowDst].colvisible == false)
    {

        for (int j = 0 ; j< COL_WPT_COUNT ; j++)
        {
            if ( contextMenuLibListe[j].order == rowSrc )
            {
                contextMenuLibListe[j].order = -1;
                contextMenuLibListe[j].colvisible = false;
                break;
            }
        }

        contextMenuLibListe[rowDst].colvisible = true;
        contextMenuLibListe[rowDst].order = rowSrc;

    }
    else
    {
        contextMenuLibListe[rowDst].colvisible = false;
        contextMenuLibListe[rowDst].order = -1;
    }

    displayWptColExport();
    displayTableWidgetSampleData();

}


void dialogCsvExport::onCellEditedTableWidgetColDispo(int row , int col)
{

    debugp("on cell edited row, col : " + QString::number(row) + ", " + QString::number(col));

    if (col == COLDSP_INDEX_NAME )
    {
        //debugp("new val = " + tableWidgetColDispo->item(row, 1)->text() );
        contextMenuLibListe[row].name = tableWidgetColDispo->item(row, COLDSP_INDEX_NAME)->text();
        displayTableWidgetSampleData();
    }

    if (col == COLDSP_INDEX_FORMAT )
    {
        debugp("new val fmt = " + tableWidgetColDispo->item(row, COLDSP_INDEX_FORMAT)->text() );
        contextMenuLibListe[row].format = tableWidgetColDispo->item(row, COLDSP_INDEX_FORMAT)->text();
        displayTableWidgetSampleData();
    }


}

void dialogCsvExport::colCsvColToRight(void)
{
    debugp("To Right col " + QString::number(currentCol));
    swapCol( currentCol, currentCol+1);

}
void dialogCsvExport::colCsvColToLeft(void)
{
    debugp("To Left col " + QString::number(currentCol));
    swapCol( currentCol, currentCol-1 );

}

void dialogCsvExport::swapCol( int oldPos, int newpos)
{

    debugp("Swap col : " + QString::number(oldPos) + " -> " + QString::number(newpos));
    std::swap( contextMenuLibListe[oldPos], contextMenuLibListe[newpos]);
    displayTableWidgetSampleData();
}

void dialogCsvExport::moveCol( int oldPos, int newpos)
{

    debugp("moveCol col : " + QString::number(oldPos) + " -> " + QString::number(newpos));

    contextMenuLibListe.insert(newpos, contextMenuLibListe.at(oldPos) );
    contextMenuLibListe.remove(oldPos+1);

    displayTableWidgetSampleData();
}

void dialogCsvExport::colCsvColSelectCurrentCol(  )
{
    debugp(  "Select col=" +  QString::number(currentCol ) );
    doSelectByCurrentcol( contextMenuLibListe[currentCol].id );
}


void dialogCsvExport::colCsvColunSelectAll()
{
    QCheckBox *mcb;

    for (int row = 0 ; row < COL_WPT_COUNT ; row++)
    {
        mcb = qobject_cast<QCheckBox*>(tableWidgetColDispo->cellWidget(row, 0));
        if (mcb)
            mcb->setChecked(false);

        debugp(  "Unselect @" +  QString::number(row) );

        contextMenuLibListe[row].colvisible = false;
        contextMenuLibListe[row].order = -1;
    }

    displayTableWidgetSampleData();
    displayWptColExport();

}
void dialogCsvExport::colCsvColSelectAll()
{

    QCheckBox *mcb;

    for (int row = 0 ; row < COL_WPT_COUNT ; row++)
    {
        mcb = qobject_cast<QCheckBox*>(tableWidgetColDispo->cellWidget(row, 0));
        if (mcb)
            mcb->setChecked(true);
        contextMenuLibListe[row].colvisible = true;
        contextMenuLibListe[row].order = row;
    }

    displayTableWidgetSampleData();
    displayWptColExport();

}


// Display information on colons meta datas
void dialogCsvExport::displayWptColExport()
{

    debugp("Enter meta data");

    bool oldState = tableWidgetColDispo->blockSignals(true);

    tableWidgetColDispo->setRowCount(COL_WPT_COUNT );

    for (int j= 0 ; j < COL_WPT_COUNT ; j++ )
    {
        QCheckBox *colVisibleChk  = new QCheckBox("");

        if ( contextMenuLibListe[j].colvisible == true) colVisibleChk->setChecked(true);
        colVisibleChk->setDisabled(true);

        tableWidgetColDispo->setCellWidget(j,0, colVisibleChk);

        QString libOrder;
        if ( contextMenuLibListe[j].order ==-1) libOrder = "";
        else libOrder = QString::number(contextMenuLibListe[j].order+1);

        tableWidgetColDispo->setItem(j,1,   new QTableWidgetItem( QString("%1").arg( libOrder ) )) ;
        tableWidgetColDispo->item(j, 1 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        tableWidgetColDispo->item(j, 1 )->setTextColor(QColor(Qt::blue));
        tableWidgetColDispo->setItem(j,2, new QTableWidgetItem( QString("%1").arg( contextMenuLibListe[j].name  )) );
        tableWidgetColDispo->setItem(j,3, new QTableWidgetItem( QString("%1").arg( contextMenuLibListe[j].format  )) );
        tableWidgetColDispo->setItem(j,4, new QTableWidgetItem( QString("%1").arg( contextMenuLibListe[j].id )) );
        tableWidgetColDispo->setItem(j,5, new QTableWidgetItem( QString("%1").arg( contextMenuLibListe[j].len )) );
        tableWidgetColDispo->setItem(j,6, new QTableWidgetItem( QString("%1").arg( contextMenuLibListe[j].tooltip )) );
        tableWidgetColDispo->item(j, 6 )->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        tableWidgetColDispo->item(j, 6 )->setTextColor(QColor(Qt::blue));
        tableWidgetColDispo->item(j, 6 )->setTextColor(QColor(Qt::blue));
    }

    // Header
    tableWidgetColDispo->setHorizontalHeaderLabels(QString("Selection;Position;Nom;Format;Id;Longueur;Description").split(";"));

    tableWidgetColDispo->hideColumn(4);
    tableWidgetColDispo->hideColumn(5);

    tableWidgetColDispo->setColumnWidth(0, 40);
    tableWidgetColDispo->setColumnWidth(1, 50);
    tableWidgetColDispo->setColumnWidth(2, 160);
    tableWidgetColDispo->setColumnWidth(3, 90);
    tableWidgetColDispo->setColumnWidth(4, 160);
    tableWidgetColDispo->setColumnWidth(5, 160);
    tableWidgetColDispo->setColumnWidth(6, 160);


    tableWidgetColDispo->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Champ selectionné"));
    tableWidgetColDispo->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Position dns le fichier"));
    tableWidgetColDispo->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Nom du champ"));
    tableWidgetColDispo->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Format des données"));
    tableWidgetColDispo->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Identifiant"));
    tableWidgetColDispo->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Longueur du champ"));
    tableWidgetColDispo->horizontalHeaderItem(0)->setToolTip( QString::fromUtf8("Description"));

    tableWidgetColDispo->blockSignals(oldState);
    debugp("End meta data");

}

void dialogCsvExport::displayTableWidgetSampleData(void)
{

    projectDatas::wptType *m_wpt;

    tableWidgetSampleData->setRowCount(0);

    displayTableWidgetSampleDataHeader();

    int dspCol=0;
    QString m_linksCsv;

    for(int rowData = 0; rowData < m_parent->projectData.m_wptList.size() && (rowData < maxSampleDataRow || maxSampleDataRow ==-1) ; rowData++)
    {
        m_wpt = m_parent->projectData.getWptFromId2( rowData +1  );

        if (m_wpt==NULL)
        {
            debugp("m_wpt is NULL for rowid = " + QString::number(rowData) + " / " + QString::number(maxSampleDataRow) + " size=" + QString::number(m_parent->projectData.m_wptList.size() ) );
            continue;
        }

        /*if (m_wpt->m_selected == false) {
            debugp("row false " + QString::number(rowData));
        }
        */

        int row  = tableWidgetSampleData->rowCount();
        tableWidgetSampleData->insertRow( row );

        dspCol=0;
        for (int col = 0 ; col < COL_WPT_COUNT ; col++ )
        {
            tableWidgetSampleData->setItem(rowData,col, new QTableWidgetItem( QString("")) );

            switch (contextMenuLibListe[col].id)
            {

            case  COL_WPT_NAME :
                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( QString( contextMenuLibListe[col].format).arg( m_wpt->m_name )) );
                break;

            case COL_WPT_NOTE :
                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( QString( contextMenuLibListe[col].format).arg( m_wpt->m_note )) );
                break;

            case COL_WPT_GROUP :
                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( QString( contextMenuLibListe[col].format).arg( m_wpt->m_group )) );
                break;

            case COL_WPT_LAT :
                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( QString( contextMenuLibListe[col].format).arg( m_wpt->m_lat )) );
                break;

            case COL_WPT_LON :
                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( QString( contextMenuLibListe[col].format).arg( m_wpt->m_lon )) );
                break;

            case COL_WPT_ELE :
                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( QString( contextMenuLibListe[col].format).arg( m_wpt->m_ele )) );
                break;

            case COL_WPT_TIME :
                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( m_wpt->m_time.toString(contextMenuLibListe[col].format) ) );
                break;

            case COL_WPT_MAGVAR :
                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( QString( contextMenuLibListe[col].format).arg( m_wpt->m_magvar )) );
                break;

            case COL_WPT_GEOIDHEIGHT :
                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( QString( contextMenuLibListe[col].format).arg( m_wpt->m_geoidheight )) );
                break;

            case COL_WPT_DESC :
                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( QString( contextMenuLibListe[col].format).arg( m_wpt->m_desc )) );
                break;

            case COL_WPT_CMT :
                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( QString( contextMenuLibListe[col].format).arg( m_wpt->m_cmt )) );

                break;

            case COL_WPT_SRC :
                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( QString( contextMenuLibListe[col].format).arg( m_wpt->m_src )) );
                break;

            case COL_WPT_FIX :
                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( QString( contextMenuLibListe[col].format).arg( m_wpt->m_fix )) );
                break;

            case COL_WPT_SAT :
                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( QString( contextMenuLibListe[col].format).arg( m_wpt->m_sat )) );
                break;

            case COL_WPT_DGPSID :
                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( QString( contextMenuLibListe[col].format).arg( m_wpt->m_dgpsid )) );
                break;

            case COL_WPT_LINKS :
                m_linksCsv="";
                for (QStringList::iterator it = m_wpt->m_links.begin(); it != m_wpt->m_links.end(); ++it)
                {
                    QString current = *it;
                    m_linksCsv += current + "," ;
                }

                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( QString( contextMenuLibListe[col].format).arg( m_linksCsv )) );
                break;

            case COL_WPT_SYM :
                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( QString( contextMenuLibListe[col].format).arg( m_wpt->m_sym )) );
                break;

            case COL_WPT_TYPE :
                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( QString( contextMenuLibListe[col].format).arg( m_wpt->m_type )) );
                break;

            case COL_WPT_HDOP :
                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( QString( contextMenuLibListe[col].format).arg( m_wpt->m_hdop )) );
                break;

            case COL_WPT_VDOP :
                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( QString( contextMenuLibListe[col].format).arg( m_wpt->m_vdop )) );
                break;

            case COL_WPT_PDOP :
                tableWidgetSampleData->setItem(rowData,dspCol, new QTableWidgetItem( QString( contextMenuLibListe[col].format).arg( m_wpt->m_pdop )) );
                break;

            case COL_WPT_EXTENTIONS :
                //return m_wpt->m_extensions;
                break;
            }

            if ( contextMenuLibListe[col].colvisible == false)
            {
                tableWidgetSampleData->item(rowData, dspCol)->setForeground( QColor(127, 127, 127) );
                tableWidgetSampleData->horizontalHeaderItem(dspCol)->setForeground( QColor(127, 127, 127) );
            }
            else
            {

                tableWidgetSampleData->item(rowData, dspCol)->setForeground( Qt::red );
                tableWidgetSampleData->horizontalHeaderItem(dspCol)->setForeground(Qt::red);
            }

            dspCol++;

        } // end for col

    } // end for rowData


// Scroll to colX
    QTableWidgetItem  *tableWidgetItemVisible = tableWidgetSampleData->itemAt(0, currentCol);
    if (tableWidgetItemVisible)
        tableWidgetSampleData->scrollToItem(tableWidgetSampleData->item(tableWidgetItemVisible->row(), currentCol));

}

int dialogCsvExport::displayTableWidgetSampleDataHeaderExport(int colmaxCount )
{
    QString colDesc;
    colDesc="";
    QString sepCol="";
    QStringList tmpLib;

    colDesc="";
    sepCol = "";
    QString libHead;
    for (int rowPos = 0 ; rowPos < colmaxCount ; rowPos++)
    {
        libHead="";
        for (int j = 0 ; j< COL_WPT_COUNT ; j++)
        {
            //debugp("add lib @" + QString::number(rowPos) + ": order=" + QString::number(contextMenuLibListe[j].order) + " :" + contextMenuLibListe[j].name);
            if ( contextMenuLibListe[j].order == rowPos ) //&& contextMenuLibListe[j].order != -1 )
            {
                libHead =  contextMenuLibListe[j].name;
                break;
            }
            else
            {
                libHead="";
            }
        }
        colDesc +=  sepCol + libHead;
        sepCol = ";";
    }
    debugp( "ColDesc 1 = " + colDesc + " , colmaxCount=" + QString::number(colmaxCount));

    tableWidgetSampleData->setColumnCount(0);
    tableWidgetSampleData->setColumnCount(colmaxCount);
    tableWidgetSampleData->setHorizontalHeaderLabels(colDesc.split(";"));

    return colmaxCount;
}

int dialogCsvExport::displayTableWidgetSampleDataHeader(void)
{
    QString colDesc;
    colDesc="";
    QString sepCol="";
    QStringList tmpLib;

    colDesc="";
    sepCol = "";
    QString libHead;

    for (int rowPos = 0 ; rowPos < COL_WPT_COUNT ; rowPos++)
    {
        colDesc +=  sepCol + contextMenuLibListe[rowPos].name;
        sepCol = ";";

    }

    debugp( "ColDesc = " + colDesc + " , COL_WPT_COUNT=" + QString::number(COL_WPT_COUNT));

    tableWidgetSampleData->setColumnCount(0);
    tableWidgetSampleData->setColumnCount(COL_WPT_COUNT);
    tableWidgetSampleData->setHorizontalHeaderLabels(colDesc.split(";"));

    return COL_WPT_COUNT;
}

void dialogCsvExport::onFistLineIsHeaderCheck(bool)
{
    if (fistLineIsHeaderCheck->checkState())
        csvParamdlgVal->fistLineIsHeader=true;
    else csvParamdlgVal->fistLineIsHeader=false;

    displayTableWidgetSampleData();
}

void dialogCsvExport::onCsvSepField(const QString &newVal)
{
    debugp("new val = " + newVal);
    csvParamdlgVal->csvSepFieldValue = newVal;
    displayTableWidgetSampleData();
}

void dialogCsvExport::oncsvSepText(const QString &newVal)
{
    debugp("new val = " + newVal);
    csvParamdlgVal->csvSepTextValue = newVal;
    displayTableWidgetSampleData();
}

void dialogCsvExportHeaderView::onSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    bool oldState = this->blockSignals(true);

    debugp(" section moved : logicalIndex=" +  QString::number(logicalIndex) + " oldIndex=" +  QString::number(oldVisualIndex) + " newIndex=" +  QString::number(newVisualIndex) ) ;

    m_parent->moveCol(oldVisualIndex, newVisualIndex);

    this->blockSignals(oldState);

}



#include "dialogCsv.moc"


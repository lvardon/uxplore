#include "global.h"
#include "wptEdit.h"
#include "dialogWptProp.h"
#include "mainWindow.h"
#include "projectData.h"

#include <QDialog>
#include <QCheckBox>
#include <QTableWidget>
#include <QMenu>
#include <QDesktopServices>
#include <QUrl>


wptEdit::wptEdit(  QWidget *parent ) : QTableWidget(parent)
{

    p_mainWindow = parent;

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect( this, SIGNAL( customContextMenuRequested(const QPoint &) ), this, SLOT( ctxMenu(const QPoint &) ) );

    curentWptId = -1;

    mapZoomToUpper = 5;
    mapZoomToLower = 15;

    hlClr = Qt::black; // highlight color to set

    const QColor txtClr = Qt::white; // highlighted text color to set

    QPalette p = palette();
    p.setColor(QPalette::Highlight, hlClr);
    p.setColor(QPalette::HighlightedText, txtClr);
    setPalette(p);

    WptEditHeaderViewObj = new WptEditHeaderView();
    this->setHorizontalHeader(WptEditHeaderViewObj);

    connect(this, SIGNAL(cellChanged(int, int)), this, SLOT(wptCellEdited(int, int)));
    connect(this, SIGNAL(cellClicked(int,int)), this, SLOT(wptCellClic(int,int)));

    this->setAlternatingRowColors(true);

    //this->QTableWidgetItem(1);
}
void wptEdit::wptCellClic(int row,int col)
{
    debugp("cell clic = " + QString::number(row) + "," + QString::number( col ) ) ;
}

void wptEdit::initDisplay(bool setColSize)
{
    this->setColumnCount( 0 );
    this->setHorizontalHeaderLabels( QStringList());

    this->setColumnCount( COL_WPT_COUNT );

    this->setHorizontalHeaderLabels( m_colsDisposition.getWptColDisposition().split(";"));


    for (int colIndex=0; colIndex < COL_WPT_COUNT; colIndex++)
    {
        this->horizontalHeaderItem(colIndex)->setToolTip( m_colsDisposition.getWptColTooltip(colIndex));


        int lenBefore =  m_colsDisposition.getWptColLen(colIndex);

        if ( m_colsDisposition.m_colBase[colIndex].colvisible == false )
        {
            hideColumn(colIndex);
        }
        else
        {
            showColumn(colIndex);

        }

        this->setColumnWidth(colIndex, lenBefore);

    }

    this->horizontalHeader()->setStretchLastSection(true);
}

void wptEdit::setFormatGpx( projectDatas *p_projectDatas)
{
    m_projectDatas = p_projectDatas;
    WptEditHeaderViewObj->m_projectDatas = m_projectDatas;
    WptEditHeaderViewObj->parent = this;
    WptEditHeaderViewObj->m_colsDisposition = &m_colsDisposition;

}

void wptEdit::setMarkers(SlippyMarkers *p_markers)
{
    m_markers = p_markers;
}

void wptEdit::setMap( SlippyMapWidget *p_map )
{
    m_map = p_map;
}

void wptEdit::showWpt( void )
{
    projectDatas::wptType wptCurr;

    this->setRowCount(m_projectDatas->m_wptList.size());

    bool oldState = this->blockSignals(true);

    QList<projectDatas::wptType>::iterator it = m_projectDatas->m_wptList.begin();
    qlonglong index_list;
    index_list=0;
    int row=0;
    while (it != m_projectDatas->m_wptList.end())
    {
        displayWptLine( row, &m_projectDatas->m_wptList[row]);
        ++it;
        index_list++;
        row++;
    }

    this->blockSignals(oldState);

}

void wptEdit::addNewWpt( QPointF lonlat)
{
    int lineNumber=rowCount();

    debugp("addNewWpt line "  + QString::number(lineNumber) + " / " + QString::number( lonlat.rx()) + "," + QString::number( lonlat.ry() ));

    projectDatas::wptType wpt;
    projectDatas::wptType *wptCreated;

    wpt.m_selected = true;
    wpt.m_name = "Nouveau " + QString::number(m_projectDatas->m_wptList.count()+1);
    wpt.m_lat = QString::number( lonlat.rx() );
    wpt.m_lon = QString::number( lonlat.ry() );

    qlonglong idPoint = m_projectDatas->addNewWpt( wpt.m_selected,  wpt.m_lat ,  wpt.m_lon ,  wpt.m_name );
    wptCreated = m_projectDatas->getWptFromId2( idPoint );

    setRowCount( m_projectDatas->m_wptList.size());
    if (wptCreated != NULL) displayWptLine( lineNumber, wptCreated );

    scrollToBottom();

    if (wptCreated != NULL) refreshMarkerByWpt( wptCreated );
    m_map->update();

}

void wptEdit::displayWptLine( int lineNumber, projectDatas::wptType *wpt)
{
    int colMetaByPos;

    bool oldState = this->blockSignals(true);
    for (int colNumber=0; colNumber < COL_WPT_COUNT; colNumber++)
    {
        colMetaByPos = m_colsDisposition.getWptColId(colNumber);

        if (colMetaByPos == COL_WPT_ID )
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem( QString::number(wpt->m_id)));

            continue;
        };
        if (colMetaByPos == COL_WPT_SELECTED )
        {
            QTableWidgetItem *checkItemLine = new QTableWidgetItem();
            checkItemLine->data(Qt::CheckStateRole);
            if (wpt->m_selected  == false)
                checkItemLine->setCheckState(Qt::Unchecked);
            else
                checkItemLine->setCheckState(Qt::Checked);

            this->setItem( lineNumber,colNumber, checkItemLine);
            continue;
        };
        if (colMetaByPos == COL_WPT_NOTE )
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem(wpt->m_note));
            continue;
        };
        if (colMetaByPos == COL_WPT_GROUP)
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem(wpt->m_group));
            continue;
        };
        if (colMetaByPos == COL_WPT_LAT )
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem(wpt->m_lat));
            continue;
        };
        if (colMetaByPos == COL_WPT_LON )
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem(wpt->m_lon));
            continue;
        };
        if (colMetaByPos == COL_WPT_NAME )
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem(wpt->m_name));
            continue;
        };
        if (colMetaByPos == COL_WPT_ELE)
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem(wpt->m_ele));
            continue;
        };

        if (colMetaByPos == COL_WPT_TIME )
        {
            QDateTimeEdit * dateTimeWidget = new QDateTimeEdit( wpt->m_time );

            mainWindow *p_mainWindow = dynamic_cast<mainWindow*>(QApplication::activeWindow());;
            if (p_mainWindow == 0)
            {
                debugp ("Parent is not MainWindow !");
                dateTimeWidget->setDisplayFormat("dd/MM/yyyy hh:mm.ss");
            }

            else
                dateTimeWidget->setDisplayFormat(p_mainWindow->dateTimeFormat);

            dateTimeWidget->setFrame(false);
            dateTimeWidget->setCalendarPopup(true);
            this->setCellWidget(lineNumber, colNumber, dateTimeWidget);

            QSignalMapper* signalMapper = new QSignalMapper(this);
            signalMapper->setMapping(dateTimeWidget, QString("%1-%2").arg(lineNumber).arg(colNumber));
            connect(dateTimeWidget, SIGNAL(dateTimeChanged(QDateTime)), signalMapper,SLOT(map() ) );
            connect(signalMapper, SIGNAL(mapped(const QString &)),this, SLOT(wptDateChanged(const QString &)));
            continue;
        };
        if (colMetaByPos == COL_WPT_MAGVAR )
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem(wpt->m_magvar));
            continue;
        };
        if (colMetaByPos == COL_WPT_GEOIDHEIGHT )
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem(wpt->m_geoidheight));
            continue;
        };
        if (colMetaByPos == COL_WPT_CMT)
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem(wpt->m_cmt));
            continue;
        };
        if (colMetaByPos == COL_WPT_DESC )
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem(wpt->m_desc));
            continue;
        };
        if (colMetaByPos == COL_WPT_SRC )
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem(wpt->m_src));
            continue;
        };
        if (colMetaByPos == COL_WPT_FIX )
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem(wpt->m_fix));
            continue;
        };
        if (colMetaByPos == COL_WPT_SAT )
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem(wpt->m_sat));
            continue;
        };
        if (colMetaByPos == COL_WPT_DGPSID )
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem(wpt->m_dgpsid));
            continue;
        };
        if (colMetaByPos == COL_WPT_LINKS )
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem(""));
            continue;
        };
        if (colMetaByPos == COL_WPT_SYM )
        {

            QString imgPath  = m_projectDatas->m_iconSymbolRootDir + "/" +wpt->m_sym + ".png";
            QImage *img = new QImage();
            bool loaded = img->load(imgPath);
            if (loaded)
            {

                QTableWidgetItem *thumbnail = new QTableWidgetItem;
                thumbnail->setData(Qt::DecorationRole, QPixmap::fromImage(*img));
                this->setItem(lineNumber, colNumber, thumbnail);
            }
            else
            {

                this->setItem(lineNumber, colNumber, new QTableWidgetItem( wpt->m_sym ));
            }
            continue;
        };
        if (colMetaByPos == COL_WPT_TYPE )
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem(wpt->m_type));
            continue;
        };
        if (colMetaByPos == COL_WPT_HDOP )
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem(wpt->m_hdop));
            continue;
        };
        if (colMetaByPos == COL_WPT_VDOP )
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem(wpt->m_vdop));
            continue;
        };
        if (colMetaByPos == COL_WPT_PDOP )
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem(wpt->m_pdop));
            continue;
        };
        if (colMetaByPos == COL_WPT_EXTENTIONS  )
        {
            this->setItem(lineNumber, colNumber,new QTableWidgetItem(""));
            continue;
        };

    }
    this->blockSignals(oldState);
}


void wptEdit::removeSelected()
{
    projectDatas::wptType wptCurr;
    QList <projectDatas::wptType> idRemoveList;

    this->setRowCount(m_projectDatas->m_wptList.size());

    bool oldState = this->blockSignals(true);

    QList<projectDatas::wptType>::iterator it = m_projectDatas->m_wptList.begin();
    int row=0;
    while (it != m_projectDatas->m_wptList.end())
    {
        if (m_projectDatas->m_wptList[row].m_selected == true)
        {
            debugp("remove selected  id=" + QString::number(m_projectDatas->m_wptList[row].m_id) + " name=" + m_projectDatas->m_wptList[row].m_name );
            idRemoveList.append( m_projectDatas->m_wptList[row] );
        }
        ++it;
        row++;
    }

    foreach( wptCurr , idRemoveList )
    {
        m_projectDatas->removeWptById( wptCurr.m_id );
        refreshMarkerByWpt( &wptCurr );
    }
    m_map->update();
    m_projectDatas->reIndexwpt();

    this->blockSignals(oldState);
}

void wptEdit::refreshMarkers(void)
{

    int colCheck = m_colsDisposition.getWptColIndexByMeta(COL_WPT_SELECTED);

    for (int row = 0 ; row < this->rowCount() ; ++row)
    {
        if ( this->item(row, colCheck)->checkState() )
        {
            this->item(row, colCheck)->setCheckState(Qt::Unchecked);
            this->item(row, colCheck)->setCheckState(Qt::Checked);
            debugp(" refresh marker : check line "  + QString::number(row) );

        }
    }
}


void wptEdit::reindexMarkers( void )
{
    QList<QTableWidgetItem*> itemList;
    QList<projectDatas::wptType>::iterator it = m_projectDatas->m_wptList.begin();
    QPointF markerPos;
    QString fileSymbolName;

    m_markers->clearAllMarkers();
    qlonglong index_list;
    index_list=0;
    int row=0;
    while (it != m_projectDatas->m_wptList.end())
    {
        if (m_projectDatas->m_wptList[row].m_selected == true)
        {
            fileSymbolName = m_projectDatas->m_iconSymbolRootDir + "/" + m_projectDatas->m_wptList[row].m_sym + ".png";
            QFileInfo checkFile(fileSymbolName);

            if ( !checkFile.exists() || !checkFile.isFile())
                fileSymbolName = "default" ; // .png
            else
                fileSymbolName = checkFile.completeBaseName();

            markerPos.setX( m_projectDatas->m_wptList[row].m_lon.toDouble() );
            markerPos.setY( m_projectDatas->m_wptList[row].m_lat.toDouble() );
            m_markers->addMarker( m_projectDatas->m_wptList[row].m_id , markerPos, m_projectDatas->m_wptList[row].m_name, fileSymbolName );

            debugp("re create marker on map , check id=" + QString::number(m_projectDatas->m_wptList[row].m_id) + " name=" + m_projectDatas->m_wptList[row].m_name + " lat=" + m_projectDatas->m_wptList[row].m_lat + " fileSymbolName=" + fileSymbolName );

        }

        ++it;
        index_list++;
        row++;
    }

}

void wptEdit::showAllMarkersOnMap()
{
    int colCheck = m_colsDisposition.getWptColIndexByMeta(COL_WPT_SELECTED);

    for (int row = 0 ; row < this->rowCount() ; ++row)
    {
        this->item(row, colCheck)->setCheckState(Qt::Checked);
    }

}

void wptEdit::hideAllMarkersOnMap()
{

    int colCheck = m_colsDisposition.getWptColIndexByMeta(COL_WPT_SELECTED);

    for (int row = 0 ; row < this->rowCount() ; ++row)
    {
        this->item(row, colCheck)->setCheckState(Qt::Unchecked);
    }

}

void wptEdit::showAllMarkersByMap( bool wptCheck )
{

    mainWindow *p_mainWindow;

    p_mainWindow = dynamic_cast<mainWindow*>(QApplication::activeWindow());;
    if (p_mainWindow == 0)
    {
        debugp ("Parent is not MainWindow !");
    }

    else
    {
        QRectF area = p_mainWindow->slippyMap.getArea();
        int colCheck = m_colsDisposition.getWptColIndexByMeta(COL_WPT_SELECTED);
        projectDatas::wptType *m_wpt;

        debugp("p_mainWindow->projectData.m_wptList.size()= " +  QString::number(p_mainWindow->projectData.m_wptList.size()) );
        for(int rowData = 0; rowData < p_mainWindow->projectData.m_wptList.size(); rowData++)
        {
            m_wpt = p_mainWindow->projectData.getWptFromId2( rowData +1 );

            if (m_wpt==NULL)
            {
                debugp ("m_wpt is NULL");
                continue;
            }

            debugp(" rowData=" + rowData );
            debugp(" name =" + m_wpt->m_name );
            debugp("------- POINT " + QString::number(m_wpt->m_lat.toDouble() ));
            debugp("------- POINT " + QString::number(m_wpt->m_lon.toDouble() ));
            debugp("------- area " + QString::number(area.bottom() ));
            debugp("------- area " + QString::number(area.top() ));
            debugp("------- area " + QString::number(area.left() ));
            debugp("------- area " + QString::number(area.right() ));

            int PoiOnMap = 0;
            if ( m_wpt->m_lat.toDouble() >= area.bottom() && m_wpt->m_lat.toDouble() <= area.top() )
                PoiOnMap +=2;
            if ( m_wpt->m_lon.toDouble() >= area.left() && m_wpt->m_lon.toDouble() <= area.right() )
                PoiOnMap +=4;

            debugp(" check rowData=" + rowData );

            if (PoiOnMap == 6)
            {
                debugp( "lat,lon = " + QString::number(m_wpt->m_lat.toDouble()) + "," + QString::number(m_wpt->m_lon.toDouble()) ) ;
                if (wptCheck== true)
                    this->item(rowData, colCheck)->setCheckState(Qt::Checked);
                else
                    this->item(rowData, colCheck)->setCheckState(Qt::Unchecked);
            }
            debugp(" check rowData END =" + rowData );
        }

    }

}

void wptEdit::inverseAllMarkersOnMap()
{
    QList<QTableWidgetItem*> itemList;
    int colCheck = m_colsDisposition.getWptColIndexByMeta(COL_WPT_SELECTED);

    for (int row = 0 ; row < this->rowCount() ; ++row)
    {
        if ( this->item(row, colCheck)->checkState() )
        {
            this->item(row, colCheck)->setCheckState(Qt::Unchecked);

        }
        else
        {
            this->item(row, colCheck)->setCheckState(Qt::Checked);
        }
    }

}

void wptEdit::hilightWpt( qlonglong rowWpt, QColor cellBgColor , QColor cellTxtColor)
{
    if (m_projectDatas->m_wptList.size() == 0 ) return;
    bool oldState = this->blockSignals(true);
    for (int colNumber=0; colNumber < COL_WPT_COUNT; colNumber++)
    {
        QWidget *cellWidgetPtr = this->cellWidget(rowWpt,colNumber);
        if ( !cellWidgetPtr )
        {
            this->item(rowWpt, colNumber)->setBackgroundColor(cellBgColor);
            this->item(rowWpt, colNumber)->setTextColor (cellTxtColor);
        }
    }

    this->blockSignals(oldState);
    this->scrollToItem(this->item(rowWpt, 1));

}

void wptEdit::wptDateChanged(QString coordinates )
{

    QStringList rowcol = coordinates.split("-");
    int row = rowcol[0].toInt();
    int col = rowcol[1].toInt();

    QDateTimeEdit *qDtVal = (QDateTimeEdit*)(this->cellWidget(row,col));
    debugp("dateTime changed = " +   QString::number(row)  + "," +   QString::number(col) );
    debugp("New  qDtVal->dateTime()=" +  qDtVal->dateTime().toString() );

    wptCellEdited(row, col);

}

void wptEdit::wptCellEdited(int row,int col)
{

    int colMeta =  m_colsDisposition.getWptColId(col);

    projectDatas::wptType *curWpt = m_projectDatas->getWptFromId2( row +1 );
    if (curWpt == NULL) return;

    debugp("Before : curWpt->m_id = " +  QString::number(curWpt->m_id) + ", " + "curWpt->m_name = " + curWpt->m_name + " selected=" + curWpt->m_selected + "lat,lon=" + curWpt->m_lat + "," + curWpt->m_lon);

    bool oldState = this->blockSignals(true);

    if (colMeta == COL_WPT_ID )
    {
        // NO , not editable : curWpt->m_id = this->item(row,col)->text().toInt();

    };
    if (colMeta == COL_WPT_SELECTED )
    {
        if (curWpt->m_selected == false)
        {
            curWpt->m_selected = true;
            debugp("wptCellEdited; Check box : on ; add marker : id=" +  QString::number(curWpt->m_id) +  ", name="  + curWpt->m_name + "lon, lat=" + curWpt->m_lon + "," + curWpt->m_lat );
            refreshMarkerByWpt(curWpt );
            m_map->update();
        }
        else
        {
            curWpt->m_selected = false;

            debugp("wptCellEdited; Check box : OFF - remove marker ");
            m_markers->removeById(curWpt->m_id);
            m_map->update();
        }

    };
    if (colMeta == COL_WPT_NOTE )
    {
        curWpt->m_note = QString(this->item(row,col)->text());
    };
    if (colMeta == COL_WPT_GROUP )
    {
        curWpt->m_group = QString(this->item(row,col)->text());
    };
    if (colMeta == COL_WPT_LAT )
    {
        curWpt->m_lat = QString(this->item(row,col)->text());

        QPointF markerP;
        markerP.setX( curWpt->m_lon.toDouble() );
        markerP.setY( curWpt->m_lat.toDouble() );

        m_markers->setMarkerPosById(curWpt->m_id, markerP);
        refreshMarkerByWpt(curWpt );
        m_map->update();
    };
    if (colMeta == COL_WPT_LON )
    {
        curWpt->m_lon = QString(this->item(row,col)->text());

        QPointF markerP;
        markerP.setX( curWpt->m_lon.toDouble() );
        markerP.setY( curWpt->m_lat.toDouble() );

        m_markers->setMarkerPosById(curWpt->m_id, markerP);
        refreshMarkerByWpt(curWpt );
        m_map->update();
    };
    if (colMeta == COL_WPT_NAME )
    {
        curWpt->m_name = QString(this->item(row,col)->text());
        m_markers->setLabelMarkerById(curWpt->m_id , curWpt->m_name);
        refreshMarkerByWpt(curWpt );
        m_map->update();

    };
    if (colMeta == COL_WPT_ELE )
    {
        curWpt->m_ele = QString(this->item(row,col)->text());
    };
    if (colMeta == COL_WPT_TIME )
    {
        QDateTimeEdit *qDtVal = (QDateTimeEdit*)(this->cellWidget(row,col));
        curWpt->m_time = qDtVal->dateTime();
        debugp("New  qDtVal->dateTime()="  +   qDtVal->dateTime().toString() );
    };
    if (colMeta == COL_WPT_MAGVAR )
    {
        curWpt->m_magvar = QString(this->item(row,col)->text());
    };
    if (colMeta == COL_WPT_GEOIDHEIGHT )
    {
        curWpt->m_geoidheight = QString(this->item(row,col)->text());
    };
    if (colMeta == COL_WPT_CMT )
    {
        curWpt->m_cmt = QString(this->item(row,col)->text());
    };
    if (colMeta == COL_WPT_DESC )
    {
        curWpt->m_desc = QString(this->item(row,col)->text());
    };
    if (colMeta == COL_WPT_SRC )
    {
        curWpt->m_src = QString(this->item(row,col)->text());
    };
    if (colMeta == COL_WPT_FIX )
    {
        curWpt->m_fix = QString(this->item(row,col)->text());
    };
    if (colMeta == COL_WPT_SAT )
    {
        curWpt->m_sat = QString(this->item(row,col)->text());
    };
    if (colMeta == COL_WPT_DGPSID )
    {
        curWpt->m_dgpsid = QString(this->item(row,col)->text());
    };
    if (colMeta == COL_WPT_LINKS )
    {
        // curWpt->m_links = QString(this->item(row,col)->text());
    };
    if (colMeta == COL_WPT_SYM )
    {
        //curWpt->m_sym = QString(this->item(row,col)->text());
    };
    if (colMeta == COL_WPT_TYPE )
    {
        curWpt->m_type = QString(this->item(row,col)->text());
    };
    if (colMeta == COL_WPT_HDOP )
    {
        curWpt->m_hdop = QString(this->item(row,col)->text());
    };
    if (colMeta == COL_WPT_VDOP )
    {
        curWpt->m_vdop = QString(this->item(row,col)->text());
    };
    if (colMeta == COL_WPT_PDOP )
    {
        curWpt->m_pdop = QString(this->item(row,col)->text());
    };
    if (colMeta == COL_WPT_EXTENTIONS )
    {
        //curWpt->m_extensions = QString(this->item(row,col)->text());
    };

    updateParent();


    this->blockSignals(oldState);

    debugp ("wptCellEdited end");

}

void wptEdit::refreshMarkerByWpt( projectDatas::wptType *curWpt )
{

    QPointF markerP;
    markerP.setX( curWpt->m_lon.toDouble() );
    markerP.setY( curWpt->m_lat.toDouble());
    QString fileSymbolName;
    fileSymbolName = m_projectDatas->m_iconSymbolRootDir + "/" + curWpt->m_sym + ".png";
    QFileInfo checkFile(fileSymbolName);

    debugp("refresh marker on map id=" +  QString::number(curWpt->m_id) + " name=" + curWpt->m_name );

    m_markers->removeById(curWpt->m_id);

    if ( !checkFile.exists() || !checkFile.isFile())
        fileSymbolName = "default" ; // .png
    else
        fileSymbolName = checkFile.completeBaseName();

    m_markers->addMarker( curWpt->m_id,
                          markerP,
                          curWpt->m_name ,
                          //"number_1" // symbol icon png
                          fileSymbolName
                        );

}

void wptEdit::updateMarkerByWpt( projectDatas::wptType *curWpt )
{

    QPointF markerP;
    markerP.setX( curWpt->m_lon.toDouble() );
    markerP.setY( curWpt->m_lat.toDouble());
    QString fileSymbolName;
    fileSymbolName = m_projectDatas->m_iconSymbolRootDir + "/" + curWpt->m_sym + ".png";
    QFileInfo checkFile(fileSymbolName);

    debugp("refresh marker on map id (upd) =" +  QString::number(curWpt->m_id) + " name=" + curWpt->m_name + " TO : " + curWpt->m_selected );

    m_markers->removeById(curWpt->m_id);

    if (curWpt->m_selected == true)
    {
        if ( !checkFile.exists() || !checkFile.isFile())
            fileSymbolName = "default" ; // .png
        else
            fileSymbolName = checkFile.completeBaseName();

        m_markers->addMarker( curWpt->m_id,
                              markerP,
                              curWpt->m_name ,
                              //"number_1" // symbol icon png
                              fileSymbolName
                            );

    }

}

void wptEdit::ctxMenu(const QPoint &pos)
{

    int clicLig = rowAt(pos.y());

    if (clicLig <0)  return; // Clic outside datas

    curentWptId = m_projectDatas->m_wptList.at(clicLig).m_id ;

    QAction *ctxAction;
    QMenu *menu = new QMenu;
    ctxAction = menu->addAction( QString::fromUtf8( "Propriétés :") + m_projectDatas->m_wptList.at(clicLig).m_name + " #" + QString::number(curentWptId), this, SLOT(sheetEditProp()) );
    ctxAction->setIcon(QIcon(iconDir + "tool_properties.png"));

    ctxAction=menu->addAction("Centrer et zoomer +", this, SLOT(sheetZoomItemUpper()));
    ctxAction->setIcon(QIcon(iconDir + "tool_ctx_zoomplus.png"));
    ctxAction=menu->addAction("Centrer et zoomer -", this, SLOT(sheetZoomItemLower()));
    ctxAction->setIcon(QIcon(iconDir + "tool_ctx_zoomminus.png"));



    ctxAction=menu->addAction( QString::fromUtf8("Copier les données"), this, SLOT(sheetCopyItem()));
    ctxAction->setIcon(QIcon(iconDir + "tool_copydata.png"));

    ctxAction=menu->addAction( QString::fromUtf8("Copier les coordonnées"), this, SLOT(sheetCopyCoordItem()));
    ctxAction->setIcon(QIcon( iconDir + "tool_ctx_copycoord.png"));

    QString libWebSearch;

    libWebSearch = "Rechercher : " + searchInternetUrl;

    libWebSearch.replace("%1", m_projectDatas->m_wptList.at(clicLig).m_lat);
    libWebSearch.replace("%2", m_projectDatas->m_wptList.at(clicLig).m_lon);
    libWebSearch.replace("%3", m_projectDatas->m_wptList.at(clicLig).m_name);
    libWebSearch.replace("%4", m_projectDatas->m_wptList.at(clicLig).m_desc);
    libWebSearch.replace("%5", m_projectDatas->m_wptList.at(clicLig).m_cmt);
    libWebSearch.replace("%6", m_projectDatas->m_wptList.at(clicLig).m_note);

    ctxAction = menu->addAction( libWebSearch, this, SLOT(sheetOpenUrl()));
    ctxAction->setIcon(QIcon(iconDir + "tool_ctx_web.png"));

    ctxAction = menu->addAction("Supprimer", this, SLOT(sheetRemoveItem()));
    ctxAction->setIcon(QIcon(iconDir + "tool_delete.png"));

    QList<projectDatas::wptType>::iterator it = m_projectDatas->m_wptList.begin();
    qlonglong index_list;
    index_list=0;
    while (it != m_projectDatas->m_wptList.end())
    {
        if (m_projectDatas->m_wptList[index_list].m_id == curentWptId)
        {
            menu->addSeparator ();
            QString linkCurr;
            foreach(linkCurr, m_projectDatas->m_wptList[index_list].m_links)
            {
                debugp("link(s)=" + linkCurr );
                QAction *action = menu->addAction( QString::fromUtf8("Ouvrir : ") + linkCurr, this, SLOT(sheetOpenWptUrl()) );
                action->setData( linkCurr );
            }

            break;
        }
        ++it;
        index_list++;
    }

    menu->exec(this->mapToGlobal(pos));
}

void wptEdit::sheetNULL( void )
{
    return;
}

void wptEdit::sheetCopyCoordItem(void)
{
    if (curentWptId != -1)
    {
        const projectDatas::wptType *curWpt = m_projectDatas->getWptFromId( curentWptId );
        if (curWpt != NULL)
        {
            QClipboard *myClip = QApplication::clipboard();
            myClip->setText(QString (curWpt->m_lat + "," + curWpt->m_lon) );
        }
    }
}
void wptEdit::sheetCopyItem(void)
{
    QClipboard *myClip = QApplication::clipboard();
    QString sep;
    QString clipData;

    mainWindow *p_mainWindow;

    p_mainWindow = dynamic_cast<mainWindow*>(QApplication::activeWindow());;
    if (p_mainWindow == 0)
    {
        debugp ("wptedit : Parent is not MainWindow !");
        sep = ";";
    }
    else
        sep = p_mainWindow->clipSep;

    // Stocke la selection faite par l'utilisateur
    QItemSelectionModel *selection = selectionModel();

    // Recupere une liste contenant les indexes
    // de la selection faite par l'utilisateur
    QModelIndexList selectionList = selection->selectedIndexes();

    int lineIndex=0;
    foreach (QModelIndex index, selectionList)
    {
        debugp("add clip selection " + QString::number(index.row()) + " :" + index.data().toString());



        if ( lineIndex != index.row())
        {
            clipData += "\n";
        }

        clipData += index.data().toString() + sep;
        lineIndex = index.row();
    }
    myClip->setText( clipData  );

    //QMessageBox::information(this, "Eléments sélectionnés", clipData);

}
void wptEdit::sheetRemoveItem(void)
{

    projectDatas::wptType *wpt  = m_projectDatas->getWptFromId2( curentWptId );

    if (wpt==NULL)
    {
        debugp("wpt is NULL");
        return;
    }

    QMessageBox Msgbox(this);
    Msgbox.setText( QString::fromUtf8("Supprimer ce Waypoint ?"));
    Msgbox.setInformativeText( QString::fromUtf8("Confirmez pour supprimer ce Waypoint : ") + "\n\n\"" + wpt->m_name + "\"");
    Msgbox.setStandardButtons( QMessageBox::Cancel | QMessageBox::Yes );
    Msgbox.setIconPixmap(QPixmap(iconDir + "icon_info.png"));
    Msgbox.setWindowTitle("");
    Msgbox.setDefaultButton(QMessageBox::Cancel);

    switch( Msgbox.exec() )
    {
    case QMessageBox::Yes:

        debugp("Remove row id#" +  QString::number( curentWptId ) +" id=" + m_markers->getMarkerLabelById( curentWptId ) );

        m_projectDatas->removeWptById( curentWptId );

        m_projectDatas->reIndexwpt();

        removeRow(curentWptId-1);

        m_map->markersPtr->removeMarkerById( curentWptId );
        m_map->update();

        if ( curentWptId== m_map->pastedWptId )
            m_map->pastedWptId=-1;

        dynamic_cast<mainWindow*>(p_mainWindow)->OnProjectDataUnSaved();

    }

}
void wptEdit::wptSetPos( qlonglong wptId, QPointF newPos)
{
    m_projectDatas->wptSetPos( wptId, QString::number(newPos.rx()), QString::number(newPos.ry()) );
    showWpt();

}

void wptEdit::sheetEditProp(void)
{
    dialogWptProp dlg;

    mainWindow *mainWindowApp;
    mainWindowApp = dynamic_cast<mainWindow*>(QApplication::activeWindow());
    if (mainWindowApp == 0)
    {
        debugp ("Parent is not MainWindow !");
    }
    else hlClr = mainWindowApp->lineEditColor;

    //hilightWpt( curentWptId-1 , Qt::red, Qt::white);
    hilightWpt( curentWptId-1 , hlClr, Qt::white);

    dlg.curentWptId = curentWptId;
    dlg.m_projectDatas = m_projectDatas;
    dlg.iconSymbolRootDir = m_iconSymbolRootDir;

    dlg.exec();
    debugp("After wpt edit, dataSaved=" + dlg.dataSaved );

    if (dlg.dataSaved == false) return;

    projectDatas::wptType *curWpt;
    curWpt = m_projectDatas->getWptFromId2( curentWptId );

    if (curWpt==NULL)
    {
        debugp("curWpt is NULL");
        return;
    }

    showWpt();

    m_markers->setLabelMarkerById( curWpt->m_id , curWpt->m_name);
    m_markers->setSymbolMarkerById( curWpt->m_id , curWpt->m_sym);
    refreshMarkerByWpt(curWpt );
    m_map->update();

    hilightWpt( curentWptId-1 , Qt::white, Qt::black);

    dynamic_cast<mainWindow*>(p_mainWindow)->OnProjectDataUnSaved();

}


void wptEdit::updateParent( void )
{
    debugp ("updateparent start");

    dataSaved = false;

    mainWindow *p_mainWindow;

    p_mainWindow = dynamic_cast<mainWindow*>(QApplication::activeWindow());;
    if (p_mainWindow == 0)
    {
        debugp ("wptedit : Parent is not MainWindow !");
    }
    else
        p_mainWindow->OnProjectDataUnSaved();

    debugp("wptedit : updateparent end ");

}

void wptEdit::sheetOpenUrl(void)
{
    if (curentWptId != -1)
    {
        const projectDatas::wptType *curWpt = m_projectDatas->getWptFromId( curentWptId );
        if (curWpt != NULL)
        {
            debugp("search net = " +  searchInternetUrl + curWpt->m_name);
            QString url = searchInternetUrl;
            url.replace("%1", curWpt->m_lat);
            url.replace("%2", curWpt->m_lon);
            url.replace("%3", curWpt->m_name);
            url.replace("%4", curWpt->m_desc);
            url.replace("%5", curWpt->m_cmt);
            url.replace("%6", curWpt->m_note);

            QDesktopServices::openUrl(QUrl(url));

        }
    }
}

void wptEdit::sheetOpenWptUrl(void)
{
    const QAction * action = qobject_cast<const QAction *>(this->sender());
    debugp("Action parameter = " + action->data().value<QString>() );
    QString url = action->data().value<QString>();
    QDesktopServices::openUrl(QUrl(url));

}

void wptEdit::sheetZoomItemUpper(void)
{

    if (curentWptId != -1)
    {
        const projectDatas::wptType *curWpt = m_projectDatas->getWptFromId( curentWptId );
        if (curWpt != NULL)
        {
            QPointF lonlat = QPointF( curWpt->m_lon.toDouble(), curWpt->m_lat.toDouble() );
            m_map->setLonLat( lonlat, mapZoomToUpper  );

        }
    }
}

void wptEdit::sheetZoomItemLower(void)
{
    if (curentWptId != -1)
    {
        const projectDatas::wptType *curWpt = m_projectDatas->getWptFromId( curentWptId );
        if (curWpt != NULL)
        {
            QPointF lonlat = QPointF( curWpt->m_lon.toDouble(), curWpt->m_lat.toDouble() );
            m_map->setLonLat( lonlat, mapZoomToLower  );

        }
    }
}





void wptEdit::searchDuplicate()
{
    debugp("Enter search duplicate");

    QList<projectDatas::wptType>::iterator itMain = m_projectDatas->m_wptList.begin();
    QList<projectDatas::wptType>::iterator it = m_projectDatas->m_wptList.begin();
    int rowMain;
    int row;
    QPointF markerPosRef;
    QPointF markerPosCurrent;
    double currentDist;
    double maxDist;
    int currentGroup = 0;

    maxDist = 1000; // meters

    this->setCursor(Qt::WaitCursor);
    row=0;
    while (it != m_projectDatas->m_wptList.end())
    {
        m_projectDatas->m_wptList[row].m_group = "";
        ++it;
        row++;
    }

    rowMain=0;

    while (itMain != m_projectDatas->m_wptList.end())
    {
        row=0;
        it = m_projectDatas->m_wptList.begin();

        debugp("--- REF Duplicate for : name=" + m_projectDatas->m_wptList[rowMain].m_name + " lat=" + m_projectDatas->m_wptList[rowMain].m_lat + "lon=" + m_projectDatas->m_wptList[rowMain].m_lon);
        markerPosRef.setX( m_projectDatas->m_wptList[rowMain].m_lon.toDouble() );
        markerPosRef.setY( m_projectDatas->m_wptList[rowMain].m_lat.toDouble() );

        while (it != m_projectDatas->m_wptList.end())
        {
            if (m_projectDatas->m_wptList[row].m_id != m_projectDatas->m_wptList[rowMain].m_id)
            {
                markerPosCurrent.setX( m_projectDatas->m_wptList[row].m_lon.toDouble() );
                markerPosCurrent.setY( m_projectDatas->m_wptList[row].m_lat.toDouble() );
                currentDist = Great_Circle_Distance( markerPosRef, markerPosCurrent);

                if ( currentDist <= maxDist )
                {
                    debugp("In : name=" + m_projectDatas->m_wptList[row].m_name + " Lat=" + m_projectDatas->m_wptList[row].m_lat + " Lon=" + m_projectDatas->m_wptList[row].m_lon + " D=" +  QString::number(currentDist) );
                    if ( m_projectDatas->m_wptList[rowMain].m_group == "")
                    {
                        currentGroup++;
                        m_projectDatas->m_wptList[rowMain].m_group = QString::number(currentGroup);
                        m_projectDatas->m_wptList[row].m_group = QString::number(currentGroup);
                    }
                    else
                    {
                        m_projectDatas->m_wptList[row].m_group  = m_projectDatas->m_wptList[rowMain].m_group ;

                    }
                }
            }

            ++it;
            row++;
        }

        ++itMain;
        rowMain++;
    }

    showWpt();

    this->setCursor(Qt::ArrowCursor);

}

int  wptEdit::getSelectedCount(void)
{

    debugp("Enter getSelectedCount");

    int retVal=0;

    QList<projectDatas::wptType>::iterator itMain = m_projectDatas->m_wptList.begin();
    QList<projectDatas::wptType>::iterator it = m_projectDatas->m_wptList.begin();
    int rowMain;


    this->setCursor(Qt::WaitCursor);
    rowMain=0;

    while (itMain != m_projectDatas->m_wptList.end())
    {
        it = m_projectDatas->m_wptList.begin();

        if (m_projectDatas->m_wptList[rowMain].m_selected == true)
        {
            retVal++;
        }

        ++itMain;
        rowMain++;
    }

    this->setCursor(Qt::ArrowCursor);
    return retVal;
}

void wptEdit::computeMinMaxCoo( void )
{
    debugp("Enter computeMinMaxCoo");

    QList<projectDatas::wptType>::iterator itMain = m_projectDatas->m_wptList.begin();
    QList<projectDatas::wptType>::iterator it = m_projectDatas->m_wptList.begin();
    int rowMain;
    //QPointF markerPosRef;
    //QPointF markerPosCurrent;

    this->setCursor(Qt::WaitCursor);
    rowMain=0;

    minCoo.setX(180); // lon
    minCoo.setY(90); // lat

    maxCoo.setX(-180); // lon
    maxCoo.setY(-90); // lat

    while (itMain != m_projectDatas->m_wptList.end())
    {
        it = m_projectDatas->m_wptList.begin();

        if (m_projectDatas->m_wptList[rowMain].m_selected == true)
        {
            debugp("--- dump poi : name=" + m_projectDatas->m_wptList[rowMain].m_name + " selected=" + m_projectDatas->m_wptList[rowMain].m_selected + " lat=" + m_projectDatas->m_wptList[rowMain].m_lat + " lon=" + m_projectDatas->m_wptList[rowMain].m_lon);

            if ( m_projectDatas->m_wptList[rowMain].m_lat.toDouble() < minCoo.y()  )
                minCoo.setY( m_projectDatas->m_wptList[rowMain].m_lat.toDouble()  );
            if ( m_projectDatas->m_wptList[rowMain].m_lat.toDouble() > maxCoo.y()  )
                maxCoo.setY( m_projectDatas->m_wptList[rowMain].m_lat.toDouble()  );

            if ( m_projectDatas->m_wptList[rowMain].m_lon.toDouble() < minCoo.x()  )
                minCoo.setX( m_projectDatas->m_wptList[rowMain].m_lon.toDouble()  );
            if ( m_projectDatas->m_wptList[rowMain].m_lon.toDouble() > maxCoo.x()  )
                maxCoo.setX( m_projectDatas->m_wptList[rowMain].m_lon.toDouble()  );
        }

        ++itMain;
        rowMain++;
    }


    this->setCursor(Qt::ArrowCursor);


}



void WptEditHeaderView::onSectionMoved(int logicalIndex, int oldIndex, int newIndex)
{

    bool oldState = this->blockSignals(true);
    debugp(" logicalIndex=" +  QString::number(logicalIndex) + " oldIndex=" +  QString::number(oldIndex) + " newIndex=" +  QString::number(newIndex) ) ;

    // Show all sections, before moving.
    for (int colIndex=0; colIndex < COL_WPT_COUNT; colIndex++) parent->showColumn(colIndex);

    parent->m_colsDisposition.moveCol( logicalIndex, newIndex);

    //Re-display sections , hide sections , display rows
    parent->setRowCount(0);
    parent->initDisplay();
    parent->showWpt();

    this->blockSignals(oldState);
}

void WptEditHeaderView::onSectionResized(int logicalIndex, int oldSize, int newSize)
{
    parent->m_colsDisposition.setWptColLen(logicalIndex, newSize);
}


void WptEditHeaderView::onSectionClicked(int index)
{
    debugp("sort header : Index: " +  QString::number(index) + " ColMeta = " +  QString::number(m_colsDisposition->m_colBase[index].id ));

    if (parent->rowCount() == 0 ) return;

    this->setCursor(Qt::WaitCursor);
    this->clearSelection();

    bool doSort = false;
    int colMeta = m_colsDisposition->getWptColId(index);


    if (colMeta == COL_WPT_ID )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_SELECTED )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_NOTE )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_GROUP )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_LAT )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_LON )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_NAME )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_ELE )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_TIME )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_MAGVAR )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_GEOIDHEIGHT )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_CMT )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_DESC )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_SRC )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_FIX )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_SAT )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_DGPSID )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_LINKS )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_SYM )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_TYPE )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_HDOP )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_VDOP )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_PDOP )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };
    if (colMeta == COL_WPT_EXTENTIONS )
    {
        m_projectDatas->m_currentColSort = colMeta;
        doSort = true;
    };

    if (doSort == true)
    {
        m_projectDatas->sortWpt( m_projectDatas->m_currentColSort ,  m_projectDatas->m_currentColSortOrder );
        parent->showWpt();
        parent->reindexMarkers();
    }

    if (lastSortIndex != -1)
    {

        QTableWidgetItem* itemRemoveIcon = new QTableWidgetItem();
        QIcon emptyIcon;
        emptyIcon =  QIcon( QIcon(qApp->applicationDirPath() + "/icon/" +  "empty.png") );
        itemRemoveIcon->setIcon( emptyIcon );
        itemRemoveIcon->setText( parent->m_colsDisposition.getWptColName(lastSortIndex) );
        parent->setHorizontalHeaderItem(lastSortIndex, itemRemoveIcon);
    }

    QTableWidgetItem* item = new QTableWidgetItem();
    if (m_projectDatas->m_currentColSortOrder)
        item->setIcon(*(new QIcon( QIcon(qApp->applicationDirPath() + "/icon/" +  "downarrow.png") )));
    else item->setIcon(*(new QIcon( QIcon(qApp->applicationDirPath() + "/icon/" +  "uparrow.png") )));
    item->setText( parent->m_colsDisposition.getWptColName(index) );
    parent->setHorizontalHeaderItem(index, item);

    lastSortIndex=index;

    this->setCursor(Qt::ArrowCursor);
    this->clearSelection();

}

colsDisposition::colsDisposition( void )
{
}

void colsDisposition::setColBase(int indexColBase, int id, QString name, QString tooltip, bool colvisible, int len)
{

    m_colBase[indexColBase].id = id;
    m_colBase[indexColBase].name = name;
    m_colBase[indexColBase].tooltip = tooltip;
    m_colBase[indexColBase].colvisible = colvisible;
    if (len>0) // Qt bug : len is 0 when column is hidden
        m_colBase[indexColBase].len = len;

}
void colsDisposition::moveCol(int srcIndex, int dstIndex)
{
    colBaseType tmpColBase;

    debugp("srcIndex, dstIndex : " +  QString::number(srcIndex) + " " + QString::number( dstIndex) ) ;

    colBaseType     m_colBaseTmp[COL_WPT_COUNT];

    int decal;
    decal = 0;

    int colMin;
    int colMax;
    colMin = qMin( srcIndex, dstIndex );
    colMax = qMax( srcIndex, dstIndex );
    qDebug() <<" colFrom, to = " << colMin << ", " << colMax;
    if (srcIndex > dstIndex ) decal = -1;
    else decal = 1;

    for (int i = colMin; i <= colMax; ++i)
    {
        if ( i == dstIndex )
        {
            debugp("< [" + QString::number( i ) + "]" + " <- [" +  QString::number(srcIndex) + "] <---" +   m_colBase[srcIndex].name);
            m_colBaseTmp[i].id =  m_colBase[srcIndex].id;
            m_colBaseTmp[i].name =  m_colBase[srcIndex].name;
            m_colBaseTmp[i].format =  m_colBase[srcIndex].format;
            m_colBaseTmp[i].colvisible =  m_colBase[srcIndex].colvisible;
            m_colBaseTmp[i].len =  m_colBase[srcIndex].len;
            m_colBaseTmp[i].tooltip =  m_colBase[srcIndex].tooltip;

            continue;
        }
        else
        {
            debugp("< [" +   QString::number(i) + "]" + " <- [" +  QString::number(i + decal) + "] +- " + m_colBase[i+decal].name );
            m_colBaseTmp[i].id =  m_colBase[i+decal].id;
            m_colBaseTmp[i].name =  m_colBase[i+decal].name;
            m_colBaseTmp[i].format =  m_colBase[i+decal].format;
            m_colBaseTmp[i].colvisible =  m_colBase[i+decal].colvisible;
            m_colBaseTmp[i].len =  m_colBase[i+decal].len;
            m_colBaseTmp[i].tooltip =  m_colBase[i+decal].tooltip;
            continue;
        }
    }
    for (int i = 0; i < COL_WPT_COUNT; ++i)
    {
        if (i < colMin )
        {
            debugp("< [" +  QString::number(i) + "]" + " <- [" +  QString::number(i) + "] = " + m_colBase[i].name );
            m_colBaseTmp[i].id =  m_colBase[i].id;
            m_colBaseTmp[i].name =  m_colBase[i].name;
            m_colBaseTmp[i].format =  m_colBase[i].format;
            m_colBaseTmp[i].colvisible =  m_colBase[i].colvisible;
            m_colBaseTmp[i].len =  m_colBase[i].len;
            m_colBaseTmp[i].tooltip =  m_colBase[i].tooltip;
            continue;
        }

        if (i >  colMax)
        {
            debugp("< [" +  QString::number(i) + "]" + " <- [" +  QString::number(i) + "] . " + m_colBase[i].name);
            m_colBaseTmp[i].id =  m_colBase[i].id;
            m_colBaseTmp[i].name =  m_colBase[i].name;
            m_colBaseTmp[i].format =  m_colBase[i].format;
            m_colBaseTmp[i].colvisible =  m_colBase[i].colvisible;
            m_colBaseTmp[i].len =  m_colBase[i].len;
            m_colBaseTmp[i].tooltip =  m_colBase[i].tooltip;
            continue;
        }
    }


    for (int i = 0; i < COL_WPT_COUNT; ++i)
    {
        debugp( "#" +  QString::number(i) + ": " + m_colBaseTmp[i].name );
        m_colBase[i].id =  m_colBaseTmp[i].id;
        m_colBase[i].name =  m_colBaseTmp[i].name;
        m_colBase[i].format =  m_colBaseTmp[i].format;
        m_colBase[i].colvisible =  m_colBaseTmp[i].colvisible;
        m_colBase[i].len =  m_colBaseTmp[i].len;
        m_colBase[i].tooltip =  m_colBaseTmp[i].tooltip;
    }

    debugp("return");
}

void colsDisposition::swapCol(int oldIndex, int newIndex)
{
    debugp("Enter swap " + QString::number(oldIndex) + " <-> " + QString::number(newIndex) );

    colBaseType tmpColBase;

    tmpColBase.id = m_colBase[newIndex].id ;
    tmpColBase.name = m_colBase[newIndex].name ;
    tmpColBase.format = m_colBase[newIndex].format ;
    tmpColBase.tooltip = m_colBase[newIndex].tooltip ;
    tmpColBase.colvisible = m_colBase[newIndex].colvisible;
    if (m_colBase[newIndex].len) // Qt bug : len is 0 when column is hidden
        tmpColBase.len = m_colBase[newIndex].len;

    m_colBase[newIndex].id = m_colBase[oldIndex].id ;
    m_colBase[newIndex].name = m_colBase[oldIndex].name ;
    m_colBase[newIndex].format = m_colBase[oldIndex].format ;
    m_colBase[newIndex].tooltip = m_colBase[oldIndex].tooltip ;
    m_colBase[newIndex].colvisible = m_colBase[oldIndex].colvisible;
    if (m_colBase[oldIndex].len) // Qt bug : len is 0 when column is hidden
        m_colBase[newIndex].len = m_colBase[oldIndex].len;

    m_colBase[oldIndex].id = tmpColBase.id ;
    m_colBase[oldIndex].name = tmpColBase.name ;
    m_colBase[oldIndex].format = tmpColBase.format ;
    m_colBase[oldIndex].tooltip = tmpColBase.tooltip ;
    m_colBase[oldIndex].colvisible = tmpColBase.colvisible;
    if (m_colBase[oldIndex].len) // Qt bug : len is 0 when column is hidden
        m_colBase[oldIndex].len = tmpColBase.len;


}
int colsDisposition::getWptColIdVisible(int indexColBase)
{
    int colCpt=-1;

    for (int i = 0; i < COL_WPT_COUNT; ++i)
    {
        if (m_colBase[i].colvisible == true ) colCpt++;
        if (colCpt == indexColBase)
        {
            return m_colBase[i].id;
        }
    }
    return -1;
}

int colsDisposition::getWptColIndexByMeta(int colMeta)
{
    for (int i = 0; i < COL_WPT_COUNT; ++i)
    {
        if (m_colBase[i].id == colMeta ) return i;
    }
    return -1;
}


int colsDisposition::getWptColId(int indexColBase)
{
    return m_colBase[indexColBase].id;
}
QString colsDisposition::getWptColName(int indexColBase)
{
    //debugp("get col namle dipos #" + QString::number(indexColBase)):
    //return "ZZZZ";
    return m_colBase[indexColBase].name;
}
QString colsDisposition::getWptColTooltip(int indexColBase)
{
    return m_colBase[indexColBase].tooltip;
}
int colsDisposition::getWptColLen(int indexColBase)
{
    return m_colBase[indexColBase].len;
}
bool colsDisposition::getWptColVisibility(int indexColBase)
{
    return m_colBase[indexColBase].colvisible;
}
void colsDisposition::setWptColId(int indexColBase, int id)
{
    m_colBase[indexColBase].id = id;
}
void colsDisposition::setWptColName(int indexColBase, QString name)
{
    m_colBase[indexColBase].name = name;
}
void colsDisposition::setWptColTooltip(int indexColBase, QString tooltip)
{
    m_colBase[indexColBase].tooltip = tooltip;
}
void colsDisposition::setWptColLen(int indexColBase, int len )
{
    if (len > 0) // Qt bug : len is 0 when column is hidden
        m_colBase[indexColBase].len = len ;
}
void colsDisposition::setWptColVisibility(int indexColBase, bool colvisible)
{
    m_colBase[indexColBase].colvisible = colvisible;
}


bool colsDisposition::getWptColVisibilityByMeta(int colMetaByPos)
{
    for (int i = 0; i < COL_WPT_COUNT; ++i)
    {
        if (m_colBase[i].id == colMetaByPos)
        {
            return m_colBase[i].colvisible;
        }

    }
    return false;
}
QString colsDisposition::getWptColNameByMeta(int colMetaByPos)
{
    for (int i = 0; i < COL_WPT_COUNT; ++i)
    {
        if (m_colBase[i].id == colMetaByPos)
        {
            return m_colBase[i].name;
        }

    }
    return "";
}

bool colsDisposition::moveUpCol( int colIndex )
{
    colBaseType colBaseTmp;


    if (colIndex < 0 ) return false;

    debugp("Moving up ..." +  QString::number(colIndex) );

    colBaseTmp.id = getWptColId(colIndex-1);
    colBaseTmp.name = getWptColName(colIndex-1);
    colBaseTmp.tooltip = getWptColTooltip(colIndex-1);
    colBaseTmp.len = getWptColLen(colIndex-1);
    colBaseTmp.colvisible = getWptColVisibility(colIndex-1);

    setWptColId(colIndex-1, getWptColId(colIndex));
    setWptColName(colIndex-1, getWptColName(colIndex));
    setWptColTooltip(colIndex-1, getWptColTooltip(colIndex));
    setWptColLen(colIndex-1, getWptColLen(colIndex) );
    setWptColVisibility(colIndex-1, getWptColVisibility(colIndex));

    setWptColId(colIndex, colBaseTmp.id);
    setWptColName(colIndex, colBaseTmp.name);
    setWptColTooltip(colIndex, colBaseTmp.tooltip);
    setWptColLen(colIndex, colBaseTmp.len );
    setWptColVisibility(colIndex, colBaseTmp.colvisible);

    return true;

}
bool colsDisposition::moveDownCol( int colIndex )
{
    colBaseType colBaseTmp;

    if (colIndex <= 0 ) return false;
    if (colIndex > COL_WPT_COUNT) return false;

    debugp("Moving down ..." +  QString::number( colIndex ));

    colBaseTmp.id = getWptColId(colIndex+1);
    colBaseTmp.name = getWptColName(colIndex+1);
    colBaseTmp.tooltip = getWptColTooltip(colIndex+1);
    colBaseTmp.len = getWptColLen(colIndex+1);
    colBaseTmp.colvisible = getWptColVisibility(colIndex+1);

    setWptColId(colIndex+1, getWptColId(colIndex));
    setWptColName(colIndex+1, getWptColName(colIndex));
    setWptColTooltip(colIndex+1, getWptColTooltip(colIndex));
    setWptColLen(colIndex+1, getWptColLen(colIndex) );
    setWptColVisibility(colIndex+1, getWptColVisibility(colIndex));

    setWptColId(colIndex, colBaseTmp.id);
    setWptColName(colIndex, colBaseTmp.name);
    setWptColTooltip(colIndex, colBaseTmp.tooltip);
    setWptColLen(colIndex, colBaseTmp.len );
    setWptColVisibility(colIndex, colBaseTmp.colvisible);
    return true;
}

QString colsDisposition::getWptColDisposition()
{
    QString colDispRet;
    QString colDispRetSep= "";


    for (int i = 0; i < COL_WPT_COUNT; ++i)
    {

        colDispRet = colDispRet + colDispRetSep  + m_colBase[i].name;
        colDispRetSep = ";";
    }

    return colDispRet;
}


int colsDisposition::colWptCountVisible()
{
    int colCountVisible;

    colCountVisible=0;
    for (int i = 0; i < COL_WPT_COUNT; ++i)
    {
        if (m_colBase[i].colvisible == true)
        {
            colCountVisible++;
        }

    }
    return colCountVisible;
}


#include "wptEdit.moc"


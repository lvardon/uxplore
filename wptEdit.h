#ifndef WPTEDIT_H
#define WPTEDIT_H

#include "global.h"
#include "projectData.h"
#include "maplib/SlippyMapWidget.h"
#include "maplib/MapChooseWidget.h"

#include <QTableWidget>
#include <QtGui>

class WptEditHeaderView;

class colsDisposition
{

public :

    struct colBaseStruct
    {
        int id;
        QString name;
        QString format;
        QString tooltip;
        bool colvisible;
        int len;

    };

    colsDisposition(void);

    typedef colBaseStruct colBaseType;
    colBaseType     m_colBase[COL_WPT_COUNT];
    void setColBase(int indexColBase, int id, QString name, QString tooltip, bool colvisible, int len);
    void swapCol(int oldIndex, int newIndex);
    void moveCol(int oldIndex, int newIndex);
    int getWptColIndexByMeta(int colMeta);
    int getWptColId(int indexColBase);
    int getWptColIdVisible(int indexColBase);
    QString getWptColName(int indexColBase);
    QString getWptColTooltip(int indexColBase) ;
    int getWptColLen(int indexColBase) ;
    bool getWptColVisibility(int indexColBase) ;
    int colWptCountVisible();

    QString getWptColDisposition();
    bool getWptColVisibilityByMeta(int colMetaByPos);
    QString getWptColNameByMeta(int colMetaByPos);
    void setWptColId(int indexColBase, int id);
    void setWptColName(int indexColBase, QString name);
    void setWptColTooltip(int indexColBase, QString tooltip);
    void setWptColLen(int indexColBase, int len );
    void setWptColVisibility(int indexColBase, bool visibility);
    void displayWptLine( int lineNumber, projectDatas::wptType wptToDsp );

    bool moveUpCol( int colIndex );
    bool moveDownCol( int colIndex );



};


class wptEdit : public QTableWidget
{
    Q_OBJECT

public:

    bool dataSaved;
    QWidget *p_mainWindow;

    wptEdit( QWidget *parent= 0 );
    QString iconDir;

    colsDisposition m_colsDisposition;

    void showWpt( void );
    void initDisplay(bool setColSize=false) ;
    void addNewWpt( QPointF lonlat);
    void refreshMarkers( void );
    void reindexMarkers( void );
    void removeSelected();
    void setFormatGpx( projectDatas *p_projectDatas);
    void setMarkers(SlippyMarkers *p_markers);
    void setMap( SlippyMapWidget *p_map );
    void showAllMarkersOnMap();
    void hideAllMarkersOnMap();
    void inverseAllMarkersOnMap();
    void showAllMarkersByMap( bool wptCheck );
    void hilightWpt( qlonglong rowWpt, QColor cellBgColor , QColor cellTxtColor);
    void displayWptLine( int lineNumber , projectDatas::wptType *wpt);
    void refreshMarkerByWpt( projectDatas::wptType *curWpt );
    void updateMarkerByWpt( projectDatas::wptType *curWpt );
    void searchDuplicate();
    int  getSelectedCount(void);

    int mapZoomToUpper;
    int mapZoomToLower;
    QString m_iconSymbolRootDir;
    qlonglong curentWptId;

    QString searchInternetUrl;
    WptEditHeaderView * WptEditHeaderViewObj;

    QColor hlClr;

    QPointF minCoo;
    QPointF maxCoo;
    void computeMinMaxCoo( void );


public slots:
    void wptCellEdited(int row,int col);
    void wptCellClic(int row,int col) ;
    void wptDateChanged(QString coordinates);

    void ctxMenu(const QPoint &pos);

    void sheetNULL( void );
    void sheetCopyCoordItem(void);
    void sheetCopyItem(void);
    void sheetRemoveItem(void);
    void sheetEditProp(void);
    void sheetOpenUrl(void);
    void sheetOpenWptUrl(void);
    void sheetZoomItemUpper(void);
    void sheetZoomItemLower(void);
    void wptSetPos( qlonglong wptId, QPointF newPos);


private :
    void updateParent( void );

    projectDatas *m_projectDatas;
    SlippyMarkers *m_markers;
    SlippyMapWidget *m_map;



};

#include <QtWidgets/QHeaderView>

class WptEditHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    WptEditHeaderView():QHeaderView(Qt::Horizontal)
    {
        setSectionsClickable(true);
        lastSortIndex = -1;
        connect(this,SIGNAL(sectionClicked(int)),this,SLOT(onSectionClicked(int)));
        connect(this,SIGNAL(sectionResized(int,int,int)),this, SLOT(onSectionResized(int,int,int)));
        connect(this,SIGNAL(sectionMoved(int,int,int)),this, SLOT(onSectionMoved(int,int,int)));

        this->setSectionsMovable( true );

    };
    ~WptEditHeaderView() {};
    projectDatas *m_projectDatas;
    colsDisposition * m_colsDisposition;

    wptEdit * parent;
    int lastSortIndex;

public slots:
    void onSectionClicked(int index);
    void onSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);
private slots:
    void onSectionResized(int, int, int);

};


#endif // WPTEDIT_H


#ifndef DIALOGCSV_H
#define DIALOGCSV_H

#include "global.h"

class mainWindow;
class dialogCsvImport;
class dialogCsvExport;

#include <QtGui>
#include <QDebug>

#include <QtWidgets/QHeaderView>

// Paramétrage de la structure du fichier csv
class csvParam
{

public :

    colsDisposition columns;

    bool fistLineIsHeader;
    QString csvSepFieldValue;
    QString csvSepTextValue;

};


class dialogCsvImportHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    dialogCsvImportHeaderView():QHeaderView(Qt::Horizontal)
    {
        setSectionsClickable(true);
        this->setSectionsMovable( true );

        this->setDragEnabled(true);
        this->setDragDropMode(QTableView::InternalMove);
        this->setSectionsMovable(true);
        this->setDropIndicatorShown(true);
        this->setDragEnabled(true);
        this->setDragDropMode(QTableView::InternalMove);

        connect(this,SIGNAL(sectionMoved(int,int,int)),this, SLOT(onSectionMoved(int,int,int)));

    };
    ~dialogCsvImportHeaderView() {};

    dialogCsvImport *m_parent;


public slots:
    void onSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);
private slots:

};


class dialogCsvImport: public QDialog
{
    Q_OBJECT

public:

    dialogCsvImport( mainWindow  *parent = 0 , int modeInOut = CSV_MODE_EXPORT );

    ~dialogCsvImport()
    {

    }

    QString fileName;
    int rowDisplay;
    QString lastErrorTxt;

    QVector<contextMenuLibElem> contextMenuLibListe;
    int maxSampleDataRow;
    int modeImportExport = CSV_MODE_EXPORT;
    void initCsvParam( csvParam *csvParamValues  );

    csvParam *csvParamdlgVal;

    QCheckBox *fistLineIsHeaderCheck;

    QTableWidget *tableWidgetColDispo;
    QTableWidget *tableWidgetSampleData;
    QLineEdit *csvSepField;
    QLineEdit *csvSepText;
    int fileMaxFields;

    QString csvSepFieldValue;
    QString csvSepTextValue;

    void showEvent( QShowEvent* event );

    void displayTableWidgetSampleData();
    void displayTableWidgetSampleDataExport(void);
    void displayTableWidgetSampleDataImport(void);
    int displayTableWidgetSampleDataHeaderImport(int colmaxCount );


    void displayWptColImport();

    int wptColLig;
    int currentCol ;

    dialogCsvImportHeaderView *dialogCsvImportHeaderViewObj;

    void swapCol( int oldPos, int newpos);
    void moveCol( int oldPos, int newpos);

    int contextMenuLibElemPosByColPos( int currentColPos );
    int contextMenuLibElemPosById( int colId );

protected:
    void closeEvent(QCloseEvent *e)
    {

    }

public slots:

    bool okClicked();
    bool cancelClicked();
    void refreshClicked();
    void onCsvSepField(const QString &);
    void oncsvSepText(const QString &);
    void ctxMenuColDispoUp( const QPoint &pos );
    void ctxMenuWidgetSampleData( const QPoint &pos);
    void onSetDefcol( QAction* );
    void colCsvColSelectAll();
    void colCsvColunSelectAll();
    void colCsvColSelectCurrentCol();

    void onCellEditedTableWidgetColDispo(int, int);
    int displayTableWidgetSampleDataHeader(void);
    void onFistLineIsHeaderCheck(bool);
    void doSelectByCurrentcol(   int idSetcol );

    void colCsvColToRight(void);
    void colCsvColToLeft(void);

private :
    mainWindow      *m_parent;
    void keyPressEvent(QKeyEvent *e);

};



// ------------------------------ Export



class dialogCsvExportHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    dialogCsvExportHeaderView():QHeaderView(Qt::Horizontal)
    {
        setSectionsClickable(true);
        this->setSectionsMovable( true );

        this->setDragEnabled(true);
        this->setDragDropMode(QTableView::InternalMove);
        this->setSectionsMovable(true);
        this->setDropIndicatorShown(true);
        this->setDragEnabled(true);
        this->setDragDropMode(QTableView::InternalMove);

        connect(this,SIGNAL(sectionMoved(int,int,int)),this, SLOT(onSectionMoved(int,int,int)));

    };
    ~dialogCsvExportHeaderView() {};

    dialogCsvExport *m_parent;


public slots:
    void onSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);
private slots:

};




class dialogCsvExport: public QDialog
{
    Q_OBJECT

public:

    dialogCsvExport( mainWindow  *parent = 0 , int modeInOut = CSV_MODE_EXPORT );

    ~dialogCsvExport()
    {

    }

    QString fileName;
    QString lastErrorTxt;

    QVector<contextMenuLibElem> contextMenuLibListe;
    int maxSampleDataRow;
    int modeExportExport = CSV_MODE_EXPORT;
    void initCsvParam( csvParam *csvParamValues  );

    csvParam *csvParamdlgVal;

    QCheckBox *fistLineIsHeaderCheck;

    QTableWidget *tableWidgetColDispo;
    QTableWidget *tableWidgetSampleData;
    QLineEdit *csvSepField;
    QLineEdit *csvSepText;
    int fileMaxFields;

    QString csvSepFieldValue;
    QString csvSepTextValue;

    void showEvent( QShowEvent* event );

    void displayTableWidgetSampleData();
    int displayTableWidgetSampleDataHeaderExport(int colmaxCount );


    void displayWptColExport();

    int wptColLig;
    int currentCol ;

    dialogCsvExportHeaderView *dialogCsvExportHeaderViewObj;

    void swapCol( int oldPos, int newpos);
    void moveCol( int oldPos, int newpos);

    int contextMenuLibElemPosByColPos( int currentColPos );
    int contextMenuLibElemPosById( int colId );

protected:
    void closeEvent(QCloseEvent *e)
    {

    }

public slots:

    bool okClicked();
    bool cancelClicked();
    void refreshClicked();
    void onCsvSepField(const QString &);
    void oncsvSepText(const QString &);
    void ctxMenuColDispoUp( const QPoint &pos );
    void ctxMenuWidgetSampleData( const QPoint &pos);
    void onSetDefcol( QAction* );
    void colCsvColSelectAll();
    void colCsvColunSelectAll();
    void colCsvColSelectCurrentCol();

    void onCellEditedTableWidgetColDispo(int, int);
    int displayTableWidgetSampleDataHeader(void);
    void onFistLineIsHeaderCheck(bool);
    void doSelectByCurrentcol(   int idSetcol );

    void colCsvColToRight(void);
    void colCsvColToLeft(void);

private :
    mainWindow      *m_parent;
    void keyPressEvent(QKeyEvent *e);

};




#endif // DIALOGCSV_H

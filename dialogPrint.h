#ifndef DIALOGPRINT_H
#define DIALOGPRINT_H

#include "mainWindow.h"

#include <QDialog>
#include <QCheckBox>
#include <QTableWidget>
#include <QtPrintSupport/QPrinter>

#include "global.h"
#include "maplib/SlippyMapWidget.h"
#include "projectData.h"

class dialogPrint : public QDialog
{
    Q_OBJECT

public:

    mainWindow *m_parent;

    dialogPrint( mainWindow *parent= 0 );

    QComboBox *comboBoxPrinterList;
    QLineEdit *pdfName ;
    QPushButton *pdfSelect;
    QCheckBox *cbOpenPdf;
    QComboBox *comboBoxPaperSize;
    QLineEdit *paperW;
    QLineEdit *paperH;
    QComboBox *comboBoxPaperOrientation ;
    QCheckBox *cbprintBw;
    QComboBox *comboBoxScale;

    QCheckBox *cbPagePoi;
    QCheckBox *cbMapPoiPrint;
    QCheckBox *cbMapBorderPrint ;
    QLineEdit *marginLeft;
    QLineEdit *marginTop;
    QLineEdit *marginRight;
    QLineEdit *marginBottom;

    QFont mapFonfFamilly;
    QLabel *poiMapFontFamilly;
    QColor mapColor ;
    QColor mapBgColor ;
    QLineEdit *poiMapFontTransparency;

    QFont listFonfFamilly;
    QLabel *poiListFontFamilly;
    QColor listColor ;
    QColor listBgColor ;
    //QLineEdit *poiListFontTransparency;

    QFont legendFonfFamilly;
    QLabel *mapLegendFontFamilly;
    QColor legendColor;
    QColor legendBgColor;

    QLineEdit *printerDpi;
    //bool printBorder;
    QCheckBox *cbDebugMode;
    QLineEdit *scalePrint;

    struct scaleEquiv
    {
        int zoom;
        double scalePrint;
        QString scaleLib;
    };
    QList<scaleEquiv> scaleEquivList;

    struct paperSizeEquiv
    {
        QPrinter::PaperSize paperSize;
        int paperW;
        int paperH;
        QString paperSizeLib;
    };
    QList<paperSizeEquiv> paperSizeEquivList;

    void doPrint(void);
    void setDefaultValues(void);
    int getScaleIndexByZoom( int zoom );
    float getScalePrintByZoom( int zoom );
    float getScalePrintByIndex( int index) ;
    void setScalePrintByZoom( int zoom ) ;
    double mmToPixels(QPrinter* printer, int mm);
    double pixelsTomm(QPrinter* printer, int pixels);
    double long2tilex(double lon, int z);
    double lat2tiley(double lat, int z);
    QPointF mapProject ( double lat,  double lon , int zoom, int tileSize );

    SlippyMapWidget *slippyMap;
    projectDatas *projectData;
    QString iconFullPath;
    QString mapLibName;

    double scaleFactor;
    double scaleLen;
    int topLeftX;
    int topLeftY;
    int topRightX;
    int topRightY;
    int bottomLeftX;
    int bottomLeftY;
    int bottomRightX;
    int bottomRightY;
    QRectF mapBorderCoordinate;

    QProgressDialog progress;
    void uiPause( bool uiState );
    void setupPrinter(QPrinter *printer);
    void setupScale(QPainter *painter) ;
    void setupSlippyMap( QPrinter *printer);
    void setupBorder( QPrinter *printer) ;
    void setupPrintData();
    void renderMap(QPrinter *printer, QPainter *painter);
    void renderPoiText(QPrinter *printer, QPainter *painter);
    void renderBorder(QPrinter *printer, QPainter *painter);
    void renderLegend(QPrinter *printer, QPainter *painter);
    void renderPoiList(QPrinter *printer, QPainter *painter);

    bool printTextAt( QPainter *painter,  QPoint *printAt, QString lineToPrint, int lineSpace, int pageMargintLeft, int pageMargintBottom, int pageTotalH, bool newlineAfter = false );
    void newPage( QPrinter *printer, QPainter *painter,  QPoint *printAt, int lineSpace, int poiCount, int pageNumber, int pageMargintLeft, int pageMargintTop , int pageMargintRight, int pageMargintBottom, int pageTotalW, int pageTotalH);

    QString printDate;
    int poiCount;

    void onMapFontPreview( QLabel*widget, QColor txtColor, QColor bgColor , QFont markerFont );

public slots:
    bool okClicked();
    bool cancelClicked();
    void onPdfSelect(void);
    void onComboBoxPrinterList(int index) ;
    void onComboBoxPaperSize(int index) ;
    void onMapGetFont( void );
    void onMapGetColor( void );
    void onMapGetBgColor( void );
    void onListGetFont( void );
    void onListGetColor( void );
    void onListGetBgColor( void );
    void onLegendGetFont( void );
    void onLegendGetColor( void );
    void onLegendGetBgColor( void );
    void oncomboBoxScaleSelect(int index);

private :
    QTableWidget *tableWidgetMain;
    QTableWidget *tableWidgetOption;
    void showEvent( QShowEvent* event ) ;


};

#endif // DIALOGPRINT_H



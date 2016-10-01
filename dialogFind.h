#ifndef dialogFind_H
#define dialogFind_H

#include "global.h"
class mainWindow;

#include <QtGui>
#include <QDebug>
#include <QtWidgets>

class dialogFind: public QDialog
{
    Q_OBJECT

public:

    dialogFind( mainWindow  *parent = 0 );

    ~dialogFind()
    {

    }
    void setFormatGpx( projectDatas *p_projectDatas);
    void setWptEdit( wptEdit *p_wptedit);
    QString removeAccents(QString s);

    void keyPressEvent(QKeyEvent *e);

    QColor foundColor;
    QColor noColor;
    QColor errorColor;

protected:


public slots:

    void okClicked();
    void cancelClicked();
    void OnRegexCheckBox(int);
    void OnAccentCheckBox(int);
    void OnCaseCheckBox(int);
    void onStrSearchChange(const QString &);
    void onActionSelectedListBox(int);

private :
    mainWindow      *m_parent;
    projectDatas    *m_projectDatas;
    wptEdit         *m_wptEdit;

    QLabel      *strResultSearch;
    QLineEdit   *projNbWpt ;
    QLineEdit   *strSearch ;
    QListWidget *colWpt;
    QCheckBox   *regexCheckBox;
    QCheckBox   *caseCheckBox;
    QCheckBox   *accentCheckBox;
    QComboBox *filterSelectedListBox;
    QComboBox *actionSelectedListBox;

    void showEvent( QShowEvent* event ) ;
    QString getPreviewStrSearch( QString strValue );
    QString doFind( QString strSearch, QString strNeedle,  QString strBy, bool accentSensi, bool caseSensi);
    int findStr( QString strSearch, QString strNeedle,  bool accentSensi, bool caseSensi);
    QString getCellValue( int colIndex, projectDatas::wptType *m_wpt);
    void closeEvent(QCloseEvent *event);

};

#endif // dialogFind_H

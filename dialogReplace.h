#ifndef dialogReplace_H
#define dialogReplace_H

#include "global.h"
class mainWindow;

#include <QtGui>
#include <QDebug>

class dialogReplace: public QDialog
{
    Q_OBJECT

public:

    dialogReplace( mainWindow  *parent = 0 );

    ~dialogReplace()
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
    QLineEdit   *strReplace;
    QListWidget *colWpt;
    QCheckBox   *regexCheckBox;
    QCheckBox   *caseCheckBox;
    QCheckBox   *accentCheckBox;
    QComboBox *filterSelectedListBox;
    QComboBox *actionSelectedListBox;

    void showEvent( QShowEvent* event ) ;
    QString getPreviewStrSearch( QString strValue );
    QString doReplace( QString strSearch, QString strNeedle,  QString strBy, bool accentSensi, bool caseSensi);
    int findStr( QString strSearch, QString strNeedle,  bool accentSensi, bool caseSensi);
    void setCellValue( int colIndex, QString stringSrc, projectDatas::wptType *m_wpt);
    QString getCellValue( int colIndex, projectDatas::wptType *m_wpt);
    void closeEvent(QCloseEvent *event);

};

#endif // dialogReplace_H

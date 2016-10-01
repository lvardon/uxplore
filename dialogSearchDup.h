#ifndef DIALOGSEARCHDUP_H
#define DIALOGSEARCHDUP_H

#include "global.h"
class mainWindow;

#include <QtGui>
#include <QDebug>

class dialogSearchDup: public QDialog
{
    Q_OBJECT

public:

    dialogSearchDup( mainWindow  *parent = 0 );

    ~dialogSearchDup()
    {
    }
    void setFormatGpx( projectDatas *p_projectDatas);
    void setWptEdit( wptEdit *p_wptedit);
    void keyPressEvent(QKeyEvent *e);

protected:


public slots:

    void okClicked();
    void cancelClicked();

private :
    mainWindow      *m_parent;
    projectDatas    *m_projectDatas;
    wptEdit         *m_wptEdit;

    QLabel      *strResultSearch;
    QLineEdit   *projNbWpt ;
    QLineEdit   *strSearch ;
    QListWidget *colWpt;
    QCheckBox   *caseCheckBox;
    QCheckBox   *accentCheckBox;
    QComboBox *filterSelectedListBox;
    QComboBox *actionSelectedListBox;

    void showEvent( QShowEvent* event ) ;
    void hilighttLine( int row, projectDatas::wptType *m_wpt, int action );
    void closeEvent(QCloseEvent *event);

};


#endif // DIALOGSEARCHDUP_H

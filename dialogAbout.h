#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include "global.h"


class mainWindow;

#include <QtGui>
#include <QDebug>

class dialogAbout: public QDialog
{
    Q_OBJECT

public:

    dialogAbout( mainWindow  *parent = 0 );

    ~dialogAbout()
    {

    }

protected:
    /*void closeEvent(QCloseEvent *e)
    {

    }
    */

public slots:

    void okClicked();


private :
    mainWindow      *m_parent;
    void keyPressEvent(QKeyEvent *e);
    void closeEvent(QCloseEvent *event);

};

#endif // DIALOGABOUT_H

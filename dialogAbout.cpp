#include "mainWindow.h"
#include "projectData.h"

#include "dialogAbout.h"

#include <QDebug>
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

dialogAbout::dialogAbout(mainWindow  *parent) : QDialog(parent)
{

    m_parent = parent;

    resize(500, 630); // W x H
    QGridLayout *layout = new QGridLayout();

    QPushButton *button;
    QDialogButtonBox *ButtonBox = new QDialogButtonBox( Qt::Horizontal );

    button = new QPushButton( "Ok" );
    connect( button, SIGNAL(clicked()), this, SLOT(okClicked()) );
    ButtonBox->addButton( button, QDialogButtonBox::RejectRole );

    // FIX : chemin d'accsè au fichier gtml. Pb sous windows
#ifdef _WIN64
    QString file =  "apropos.html";
#elif _WIN32
    QString file =  "apropos.html";
#elif __APPLE__
    QString file = qApp->applicationDirPath() +  QDir::separator() + "apropos.html";
#elif __linux
    QString file = qApp->applicationDirPath() +  QDir::separator() + "apropos.html";
#elif __unix // all unices not caught above
    QString file = qApp->applicationDirPath() +  QDir::separator() + "apropos.html";
#elif __posix
    QString file = qApp->applicationDirPath() +  QDir::separator() + "apropos.html";
#endif

    //file = file.replace("/", "\\");

    file = QDir::toNativeSeparators( file );

    debugp("Affichage de la page : [" + file + "]");

    //QMessageBox Msgbox(this);
    //Msgbox.setText( "Affichage de la page : [" + file + "]" );
    //Msgbox.exec();

    QTextBrowser* browser = new QTextBrowser();

    //browser->setFixedSize(500,500);
    //browser->setSource(*new QUrl(file));
    //browser->setSource( QUrl("C:/Users/localadmin/Desktop/uxplore/uxplore/bin/Debug/apropos.html"));
    browser->setSource(QUrl(file));
    browser->setWindowTitle("A propos");

    layout->addWidget(browser, 0, 0) ;

    layout->addWidget( ButtonBox, 1, 0);

    setLayout(layout);

    setWindowTitle( QString::fromUtf8("A propos"));
}


void dialogAbout::okClicked()
{
    m_parent->dialogAboutDisplayed=false;
    QDialog::accept();
}

void dialogAbout::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
        QDialog::keyPressEvent(e);
    else
    {
        debugp("dialog closed by esc key");
        m_parent->dialogAboutDisplayed=false;
        QDialog::accept();
    }
}
void dialogAbout::closeEvent(QCloseEvent *event)
{
    //emit dialogClosed();
    m_parent->dialogAboutDisplayed=false;
    QDialog::accept();
}

#include "dialogAbout.moc"



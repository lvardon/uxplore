#include <QTextStream>

#include <QCommonStyle>
//typedef QCommonStyle OsStyle;

/*
#include <QPlastiqueStyle>
#include <QCleanlooksStyle>
#include <QWindowsXPStyle>
#include <QWindowsVistaStyle>
#include <QMotifStyle>
#include <QCDEStyle>
*/
#include "global.h"
#include "wptEdit.h"
#include "mainWindow.h"
#include "envParam.h"


char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}


int main(int argc, char *argv[])
{

    QTextStream out(stdout);

    debugp("----- start uXplore --------");

#ifdef _WIN64
    debugp("Win 64");
#elif _WIN32
    debugp("Win 32");
#elif __APPLE__
    debugp("Apple");
#elif __linux
    debugp("Linux");
#elif __unix // all unices not caught above
    debugp("Unix");
#elif __posix
    debugp("Posix");
#endif

    // ----------------------------------------------------------------------------
    envParam *envParamObj = new envParam();

    debugModeRun = envParamObj->envDebug;
    if (debugModeRun ==true)
    {
        debugp("mode Degug activé");
    }
    else
    {
        debugp("mode Degug désactivé");
    }

    if (envParamObj->isValidIni==false)
    {
        debugp("envIniFileName non trouvé, ERREUR = " + envParamObj->envIniFileName );
        return -2;
    }
    else
    {
        debugp("envIniFileName ok = " + envParamObj->envIniFileName);
    }

    if (envParamObj->isValidCache==false)
    {

        debugp("envCacheFilePath non trouvé, ERREUR = " + envParamObj->envCacheFilePath);
        return -3;
    }
    else
    {
        debugp("envCacheFilePath ok = " + envParamObj->envCacheFilePath );
    }
    // ----------------------------------------------------------------------------


    QApplication app(argc, argv);

    //QCoreApplication::addLibraryPath( QCoreApplication::applicationDirPath()  +  "/lib/plugins" );
    //app.addLibraryPath( QCoreApplication::applicationDirPath()  + "/lib/plugins");
    //debugp("plugin path added : " + QCoreApplication::applicationDirPath()  +  "/lib/plugins");


    /*QSettings *appSettings;
    QString appExeName = QFileInfo( QCoreApplication::applicationFilePath() ).fileName();
    appSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope ,appExeName, "pref");
    */
    //debugp( "appSettings filename = " + appSettings->fileName() );

    //app.setStyle(new QApplication::QPlastiqueStyle);
    app.setStyle("fusion");
    //QApplication::setStyle("plastique");
    //QApplication::setStyle(new QPlastiqueStyle);
    //QApplication::setStyle("windowsxp");
    //qDebug() << QApplication::style();


    if(cmdOptionExists(argv, argv+argc, "-h"))
    {
        out << QString(argv[0]) + " [fichier]\n";
        out << "[fichier] : fichier gpp, gpx, kml, kmz, csv.\n";
        out.flush();
        return -1;
    }
    else
    {
        //QTextCodec::setCodecFortrUtf8( QTextCodec::codecForName("UTF-8") );
        //QTextCodec::setCodecForCStrings( QTextCodec::codecForName("UTF-8") );

        debugp("Creation mainWindow");
        mainWindow window;


        QTranslator translator;
        QString locationPath  = QLibraryInfo::location (QLibraryInfo::TranslationsPath);
        translator.load ("qt_" + QLocale::system().name (), locationPath);
        app.installTranslator (&translator);

        debugp("mainWindow init");
        window.init();

        debugp("mainWindow readSetting");
        window.readSettings( envParamObj->envIniFileName, envParamObj->envCacheFilePath);

        /*if(cmdOptionExists(argv, argv+argc, "-q"))
        {
            debugModeRun = false;
            debugp ( "debugModeRun set to : false" );
        }
        */

        app.setStyleSheet("QLineEdit { background-color: white }");
        //app.setStyleSheet("QHeaderView::section { background-color: rgb(255, 85, 127);}");
        //app.setStyleSheet("QHeaderView::section { background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #7F7F7F, stop: 0.5 #9F9F9F,  stop: 0.6 #7F7F7F, stop:1 #5F5F5F);  color: white;  padding: 5px; margin: 0px;  border: 0px solid #6c6c6c; }");
        app.setStyleSheet("QHeaderView::section { background-color: rgb(216, 216, 216); padding: 4px; margin: 0px;  border: 1px solid #9F9F9F; }");


        window.show();
        window.firstDisplay();

        char * filename = NULL;
        if (argv[1] )  filename = argv[1] ;
        debugp ( "Ouvrir le fichier : " + QString(filename) );
        //if ( strlen(filename) > 0 )
        if ( QString(filename).length() > 0 )
        {
            window.doimportImportfile( filename );
        }

        return app.exec();
    }

}

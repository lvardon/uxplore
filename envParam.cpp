#include "envParam.h"

envParam::envParam()
{
    isValidIni = false;
    isValidCache = false;

    envIniFileName="";
    envCacheFilePath="";
    envDebug=false;

    getEnv();
    checkEnv();

}

void envParam::getEnv( void )
{

    QSettings *settingFile;

     // Var debug
    QString envDebugVar ;
    envDebugVar = QProcessEnvironment::systemEnvironment().value("UX_DEBUG","");
    if (envDebugVar=="1") envDebug = true;


    // Var Ini
    QString varUxINIFILE ;
    varUxINIFILE = QProcessEnvironment::systemEnvironment().value("UX_INIFILE","");

    // Ini file name
    // /home/laurent/.config/uXplore/pref.ini
    if (varUxINIFILE!= "")
    {
        settingFile = new QSettings(varUxINIFILE, QSettings::IniFormat);
    }
    else
    {
        settingFile = new QSettings(QSettings::IniFormat, QSettings::UserScope, "uXplore", "pref");
    }
    envIniFileName = settingFile->fileName();


    // var UX_CACHE
    QString varUxCACHE ;
    varUxCACHE = QProcessEnvironment::systemEnvironment().value("UX_CACHEPATH","/tmp");

    // Cache path
    // /home/laurent/.cache/uxplore
    if (varUxCACHE != "")
    {
        envCacheFilePath = varUxCACHE;
    }
    else
    {
        QDir homeDir(QDir::home());
        envCacheFilePath.replace("~", homeDir.absolutePath());
        QDir dir(envCacheFilePath);
        envCacheFilePath = dir.absolutePath();
        if (envCacheFilePath=="")
        {
            QString appExeName = QFileInfo( QCoreApplication::applicationFilePath() ).fileName();
            envCacheFilePath = "~/.cache/" + appExeName + "/";
        }
    }

}

void envParam::checkEnv( void )
{

    QFileInfo *iniCheck  = new QFileInfo( envIniFileName );
    if (iniCheck->exists() && iniCheck->isFile() )
        isValidIni = true;
    else
        isValidIni=false;


    QDir cacheCheck(envCacheFilePath);
    if (cacheCheck.exists())
    {
        isValidCache = true;
    }
    else
    {
        isValidCache = false;

    }

}





#include "envParam.moc"



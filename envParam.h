#ifndef ENVPARAM_H
#define ENVPARAM_H

#include "global.h"

#include <QSettings>
#include <QProcessEnvironment>
#include <QFileInfo>
#include <QApplication>
#include <QDir>


class envParam
{

public:

    envParam( );

    ~envParam()
    {

    }
    QString envIniFileName;
    QString envCacheFilePath;
    bool envDebug;

    bool isValidIni;
    bool isValidCache;

    void getEnv( void );
    void checkEnv( void );


private :

};

#endif // ENVPARAM_H

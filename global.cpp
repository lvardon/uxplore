#include "global.h"
#include <QString>
#include <QDebug>

#include <QDir>
#include <QFileInfo>

#if defined(WIN32)
#include <libgen.h>
#else
#include <libgen.h>
#endif

bool debugModeRun = true;

extern void debugPrint ( const char *file, int line, const char *func, QString message)
{

    if (debugModeRun == true) {

        QFileInfo fi( file );
        QString base = fi.baseName() + "." + fi.completeSuffix() ; // base = "archive"

        qDebug() << base <<": #" << line <<":" << func <<"() :" << message;
    }
}

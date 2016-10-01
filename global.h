#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

extern bool debugModeRun;

extern void debugPrint ( const char *file, int line, const char *func, QString message);

#define debugp(message) debugPrint ( __FILE__, __LINE__, __func__, message);

#endif // GLOBAL_H



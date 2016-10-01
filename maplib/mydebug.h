#ifndef _MYDEBUG_H_
#define _MYDEBUG_H_

/*!
 * \file mydebug.h
 *
 * Debug helpers.
 *
 * \ingroup Debug
 */


/*!
 * \def USE_SYSLOG
 * Turn on qDebug output to syslog if set to 1.
 */
#ifndef USE_SYSLOG
#define USE_SYSLOG 0
#endif


/*!
 * \def	DEBUGLVL
 * Specify level of debug output.
 *
 * Use this while developing code. Can have 4 values:
 *
 * - 0: no debug output
 * - 1: activates macro MYDEBUG
 * - 2: activates macro MYTRACE
 * - 3: activtaes macro MYVERBOSE
 *
 * Activated macros will send their output to myDebug().
 *
 * In the following example, only the text "Some debug message" would be
 * printed:
 *
 * \code
 * #define DEBUGLVL 1
 * include "mydebug.h"
 *
 * void x(int n)
 * {
 *    MYTRACE("x(%d)", n)
 *    MYDEBUG("Some debug message");
 * }
 * \endcode
 */
#ifndef DEBUGLVL
#define DEBUGLVL 2
#endif


#if DEBUGLVL
#  define MYDEBUG myDebug
#else
#  define MYDEBUG 1 ? (void)0 : myDebug
#endif
#if DEBUGLVL > 1
#  define MYTRACE myDebug
#else
#  define MYTRACE 1 ? (void)0 : myDebug
#endif
#if DEBUGLVL > 2
#  define MYVERBOSE myDebug
#else
#  define MYVERBOSE 1 ? (void)0 : myDebug
#endif

/*!
 * \def MYDEBUG
 * Generates debug output if DEBUGLVL is 1 or higher.
 *
 * \def MYTRACE
 * Generates debug output if DEBUGLVL is 2 or higher.
 *
 * \def MYVERBOSE
 * Generates debug output if DEBUGLVL is 3 or higher.
 */


void myDebug(const char* fmt, ...) __attribute__((format (printf, 1, 2)));
bool err(const char *errtxt);
void dump(const void *data, int len, bool with_addr=false);


#endif

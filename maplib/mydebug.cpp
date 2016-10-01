#define DEBUGLVL 0
#include "mydebug.h"

/*!
 * \file mydebug.cpp
 *
 * Debug helpers.
 *
 * \ingroup Debug
 */

#include <qglobal.h>
#include <stdio.h>		// for snprintf
#include <stdlib.h>		// for abort
#include <stdarg.h>


#ifdef USE_SYSLOG
#if defined(WIN32)
#include <syslog.h>		// for syslog
#else
#endif

#endif

/*!
 * Always return false and prints an error text.
 *
 * \param errtxt Error text for qWarning()
 *
 * \return Always false.
 *
 * Use it in code like this:
 *
 * \code
 * bool someFunction()
 * {
 *     if (!predicate)
 *          return err("Predicate isn't fulfilled");
 *     return true;
 * }
 * \endcode
 */
bool err(const char *errtxt)
{
	qWarning("%s", errtxt);
	return false;
}


/*!
 * Replacement Message handler for Qt
 *
 * This message handler can write "Warning" or "Error" before messages (if that
 * message was generated with qWarning() or qCritical(), respectively).
 *
 * It also can send the output to syslog, if \ref USE_SYSLOG is 1.
 *
 * \param type Type of message (QtDebugMsg, QtWarningMsg, ...).
 *
 * \param msg Text message to emit.
 */
static void MessageHandler(QtMsgType type, const char *msg)
{
	const char *t = "Unknown: ";
#if USE_SYSLOG
	int level = LOG_DEBUG;
#endif

	/*switch (type) {
	case QtDebugMsg:
		t = "";
		break;
	case QtWarningMsg:
		t = "Warning: ";
#if USE_SYSLOG
		level = LOG_WARNING;
#endif
		break;
	case QtCriticalMsg:
		t = "Error: ";
		break;
	case QtFatalMsg:
		t = "Fatal: ";
#if USE_SYSLOG
		level = LOG_CRIT;
#endif
		break;
	}
	*/

	// Send to syslog
#if USE_SYSLOG
	openlog("splat2", 0, LOG_USER);
	syslog(level, msg);
	closelog();
#endif

	// Send to stdout
	fputs(t, stdout);
	puts(msg);
	fflush(stdout);

	if (type == QtFatalMsg)
		abort();
}


/*!
 * Send debug output to Qt's MessageHandler.
 * \param fmt C-style format string
 */
void myDebug(const char *fmt, ...)
{
	char buf[256];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	MessageHandler(QtDebugMsg, buf);
	va_end(ap);
}


/*!
 * Handy function to dump arbitrary memory areas.
 *
 * \param data Point to the memory area.
 *
 * \param len Number of bytes to dump.
 *
 * \param with_addr (default false) When false, only the bytes will be
 *                  dumped. If true, then each line gets an position
 *                  (address) indicator. Note that is is not the memory
 *                  address, but the relative offset to the beginning of \a
 *                  data.
 *
 */
void dump(const void *data, int len, bool with_addr)
{
	const unsigned char *p = (const unsigned char *)data;
	int i, thisline;
	int offset = 0;

	while (offset < len) {
		if (with_addr)
			printf("%04x ", offset);
		thisline = len - offset;
		if (thisline > 16) {
			thisline = 16;
		}
		for (i = 0; i < thisline; i++) {
			printf("%02x ", p[i]);
		}
		for (; i < 16; i++) {
			printf("   ");
		}
		for (i = 0; i < thisline; i++) {
			printf("%c", (p[i] >= 0x20 && p[i] < 0x7f) ? p[i] : '.');
		}
		printf("\n");
		offset += thisline;
		p += thisline;
	}
	fflush(stdout);
}


static void init_debug(void) __attribute__((__constructor__));

/*!
 * Installs a MessageHandler as Qt's message handler.
 *
 * This functions is marked as "__attribute__((__constructor__))" and will
 * be automatically called at program start time.
 */
static void init_debug(void)
{
	//qInstallMsgHandler(MessageHandler);
}

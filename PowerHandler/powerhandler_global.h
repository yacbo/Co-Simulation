#ifndef POWERHANDLER_GLOBAL_H
#define POWERHANDLER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(POWERHANDLER_LIBRARY)
#  define POWERHANDLERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define POWERHANDLERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // POWERHANDLER_GLOBAL_H

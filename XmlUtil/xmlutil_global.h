#ifndef XMLUTIL_GLOBAL_H
#define XMLUTIL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(XMLUTIL_LIBRARY)
#  define XMLUTILSHARED_EXPORT Q_DECL_EXPORT
#else
#  define XMLUTILSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // XMLUTIL_GLOBAL_H

#ifndef DECISIONALTH_GLOBAL_H
#define DECISIONALTH_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DECISIONALTH_LIBRARY)
#  define DECISIONALTHSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DECISIONALTHSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DECISIONALTH_GLOBAL_H

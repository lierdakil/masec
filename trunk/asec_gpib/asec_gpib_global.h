#ifndef ASEC_GPIB_GLOBAL_H
#define ASEC_GPIB_GLOBAL_H

#include <QtCore/qglobal.h>

#define VISA

#if defined(ASEC_GPIB_LIBRARY)
#  define ASEC_GPIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ASEC_GPIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ASEC_GPIB_GLOBAL_H

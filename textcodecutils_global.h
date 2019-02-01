#ifndef TEXTCODECUTILS_GLOBAL_H
#define TEXTCODECUTILS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TEXTCODECUTILS_LIBRARY)
#  define TEXTCODECUTILSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TEXTCODECUTILSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TEXTCODECUTILS_GLOBAL_H

#ifndef COLORLINESETDEF_H
#define COLORLINESETDEF_H
#include <qglobal.h>
#if defined(GCOLORLINESET_LIBRARY)
#define GCOLORLINESET_SHARED_EXPORT Q_DECL_EXPORT
#else
#define GCOLORLINESET_SHARED_EXPORT Q_DECL_IMPORT
#endif
#endif // COLORLINESETDEF_H
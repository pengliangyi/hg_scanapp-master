﻿#ifndef CUTDIALOGDEF_H
#define CUTDIALOGDEF_H
#include <qglobal.h>
#if defined(GCUTDIALOG_LIBRARY)
#define GCUTDIALOG_SHARED_EXPORT Q_DECL_EXPORT
#else
#define GCUTDIALOG_SHARED_EXPORT Q_DECL_IMPORT
#endif
#endif // CUTDIALOGDEF_H

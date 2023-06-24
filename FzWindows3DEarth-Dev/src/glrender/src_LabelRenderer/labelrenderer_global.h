#pragma once

#include <QtCore/qglobal.h>

# if defined(LABELRENDERER_LIB)
#  define LABELRENDERER_EXPORT Q_DECL_EXPORT
# else
#  define LABELRENDERER_EXPORT Q_DECL_IMPORT
# endif

#pragma comment(lib,"FzNode.lib")
#include <iostream>

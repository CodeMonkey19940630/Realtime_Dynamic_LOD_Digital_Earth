#pragma once

#include <QtCore/qglobal.h>

# if defined(XXXRENDERER_LIB)
#  define XXXRENDERER_EXPORT Q_DECL_EXPORT
# else
#  define XXXRENDERER_EXPORT Q_DECL_IMPORT
# endif

#pragma comment(lib,"FzNode.lib")
#include <iostream>

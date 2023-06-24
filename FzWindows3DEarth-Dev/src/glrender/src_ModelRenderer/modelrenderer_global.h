#pragma once

#include <QtCore/qglobal.h>

# if defined(MODELRENDERER_LIB)
#  define MODELRENDERER_EXPORT Q_DECL_EXPORT
# else
#  define MODELRENDERER_EXPORT Q_DECL_IMPORT
# endif

#pragma comment(lib,"FzNode.lib")
#include <iostream>
using namespace std;

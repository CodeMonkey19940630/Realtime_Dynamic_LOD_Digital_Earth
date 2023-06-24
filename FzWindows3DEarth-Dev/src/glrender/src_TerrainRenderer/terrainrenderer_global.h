#ifndef TERRAINRENDERER_GLOBAL_H
#define TERRAINRENDERER_GLOBAL_H
#include <QtCore/qglobal.h>

#ifdef TERRAINRENDERER_LIB
# define TERRAINRENDERER_EXPORT Q_DECL_EXPORT
#else
# define TERRAINRENDERER_EXPORT Q_DECL_EXPORT
#endif
#pragma comment(lib,"FzNode.lib")
#include <iostream>
#endif // TERRAINRENDERER_GLOBAL_H

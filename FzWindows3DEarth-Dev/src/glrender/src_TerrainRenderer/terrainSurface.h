//
// terrainSurface.h
//

#pragma once

#include "modelclass.h"
#include "QuadTile.h"
#include "TextureFont.h"
#include <QOpenGLFunctions>
#ifdef __linux__
#include <semaphore.h>
#endif

//////////////////////////////////////////////////////////////////////////
//
class CTerrainSurface : public QThread
{
public:
    CTerrainSurface();
    ~CTerrainSurface();

    bool Initialize(CTextureFont* _font);
	void Render(std::unique_ptr<CameraClass> const &m_Camera, int width, int height);
    void Shutdown();
    void closeThread();

#ifdef __linux__
	pthread_t thread_io[NUMS_THREAD_IO];
	sem_t sem_io;
	pthread_mutex_t mutex_io;
	pthread_cond_t  condition_var;
#else
	HANDLE thread_io[NUMS_THREAD_IO];
	HANDLE sem_io;
	CRITICAL_SECTION mutex_io;

	friend void WINAPI _FileIOThreadProc(LPVOID arg);
#endif

public:
    GLfloat *vtexVerts;	//统一的纹理坐标
	GLuint hTex_PreLoaded[4];
	GLuint stippleTex;

	std::unique_ptr<ModelClass> m_Model;

	QOpenGLFunctions* f_;
public:

    int num;

    volatile bool isStop;//易失性变量，需要用volatile进行申明
    bool m_bDone;
    bool		m_bDisposeDone;
    bool		m_bIOThreadDone;
    int			m_NumIORequests;
public:
    /**
     * 获取cPos周围（含自己）tile kPos数据
     * 0   7   6
     * 1   C   5
     * 2   3   4
     * @param cPos 当前所在tile
     * @param tilePos 返回结果9块tile数组
     */
    void GetTilePos(TILTPOS cPos, TILTPOS (&tilePos)[9]);
    void GetTilePos(TILTPOS cPos, TILTPOS (&tilePos)[25]);
	void genStippleTex();
    /**
     * 填充totalTileMap;
     * 计算ioQueueList
     * @param lat lat 经度，弧度
     * @param lon lon 纬度，弧度
     * @param lev 加载的level
     */
    void ComputeTile( double lat, double lon, int lev );
	void RenderTile(CQuadTile* pTile, GLuint texPreloaded[], bool is3D = true);
	void tileRender(CQuadTile* pTile, GLuint texPreloaded[]);
	void RenderTileGeo(CQuadTile* pTile);
    /**
     * 计算渲染队列m_pTileRenderList
     * @param pTile
     * @param maxlevel
     * @param tanDis
     */
    void RenderQuadTile(CQuadTile* pTile,int maxlevel,double tanDis);
    void ComputeShapeType(CQuadTile* pTile);
	void PickingRayIntersect(
		int screenX,
		int screenY,
		glm::vec3& vec);
	void PickingViewIntersect(
		double lat,
		double lon,
		glm::vec3& vec);
	void GetPosIntersectSphere(glm::dvec3& posInters);
	/**
	* io读取数据子线程函数
	* @return void
	*/
#ifdef __linux
    void* FileIOThreadProc();
#else
    void FileIOThreadProc();
#endif

	GLuint GenTextureSD(unsigned char *data, bool haveAlpha);
	void LoadTileTexData();

};

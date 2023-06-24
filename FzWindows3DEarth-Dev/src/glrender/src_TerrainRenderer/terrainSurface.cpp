
#include "terrainSurface.h"

#ifndef __linux__
#include <process.h>
#endif

//
int maxLevel;   //当前可以加载的最大层级

std::map<TILTPOS, CQuadTile*> totalTileMap;
std::map<TILTPOS, CQuadTile*>	ioQueueList;//子线程队列
std::vector<TILTPOS> curTileVector;
std::vector<CQuadTile*>		m_pQuadTile;//预设数据
std::vector<CQuadTile*>		tilePickList;
std::vector<CQuadTile*>		m_pTileRenderList;//render数据
std::vector<TILTPOS> tilePosList;

std::multimap<int, TILTPOS, std::greater<int>> g_tiltManager;
std::multimap<double, TILTPOS>g_tileIOM;

COLORLISTDATA	g_hc;
COLORLISTDATA	g_hc_0;
COLORTOSHADER	g_hc2Fx;

glm::dvec3 cameraPos;
glm::dvec3 cviewPos;

std::string g_res_FilePath = (std::string)(QCoreApplication::applicationDirPath().toLocal8Bit().data()) + "/../../res/";

extern std::unique_ptr<CameraClass> m_Camera;

//////////////////////////////////////////////
//
#ifdef __linux
void* _FileIOThreadProc(void *arg)
{
    return  ((CTerrainSurface*)arg)->FileIOThreadProc();
}
#else
void WINAPI _FileIOThreadProc(LPVOID arg)
{
	return  ((CTerrainSurface*)arg)->FileIOThreadProc();
}
#endif

//
void LoadTopColorfromDisk()
{
	std::string path;

    path = g_res_FilePath + "Earth/data/topcolor.txt";

    QFile file(QString::fromStdString(path));

	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		return;
	}
	QTextStream in(&file);
	int num = 0;
	int index = 0;

	while (true)
	{
		QString line = in.readLine(0);

		if (line.left(3) == "end")
		{
			break;
		}

		if (line.left(2) == "//")
		{
			index++;
			num = 0;
		}
		else
		{
			QStringList strlist = line.split(",");
			if (index > 0)
			{
				QString str = strlist.at(0);
				g_hc.data[num][index].h = str.toFloat();
				str = strlist.at(1);
				g_hc.data[num][index].r = str.toFloat();
				str = strlist.at(2);
				g_hc.data[num][index].g = str.toFloat();
				str = strlist.at(3);
				g_hc.data[num][index].b = str.toFloat();
				num++;
			}
		}

	}

	file.close();

}

void LoadFixedHeightColorData()
{
	//第 0 套高度-颜色方案
	g_hc_0.data[0][0].h = -8000.0;	g_hc_0.data[0][0].r = 41.0 / 255.0;			g_hc_0.data[0][0].g = 97.0 / 255.0;			g_hc_0.data[0][0].b = 156.0 / 255.0;
	g_hc_0.data[1][0].h = -1000.0;	g_hc_0.data[1][0].r = 89.0 / 255.0;			g_hc_0.data[1][0].g = 148.0 / 255.0;		g_hc_0.data[1][0].b = 204.0 / 255.0;
	g_hc_0.data[2][0].h = -100.0;	g_hc_0.data[2][0].r = 148.0 / 255.0;			g_hc_0.data[2][0].g = 194.0 / 255.0;		g_hc_0.data[2][0].b = 247.0 / 255.0;
	g_hc_0.data[3][0].h = 0.0;		g_hc_0.data[3][0].r = 186.0 / 255.0;			g_hc_0.data[3][0].g = 222.0 / 255.0;		g_hc_0.data[3][0].b = 255.0 / 255.0;
	g_hc_0.data[4][0].h = 0.0;		g_hc_0.data[4][0].r = 0.5098;		g_hc_0.data[4][0].g = 0.6510;		g_hc_0.data[4][0].b = 0.3255;
	g_hc_0.data[5][0].h = 500.0;	g_hc_0.data[5][0].r = 0.1804;		g_hc_0.data[5][0].g = 0.4063;		g_hc_0.data[5][0].b = 0.1047;
	g_hc_0.data[6][0].h = 1000.0;	g_hc_0.data[6][0].r = 0.8784;		g_hc_0.data[6][0].g = 0.7765;		g_hc_0.data[6][0].b = 0.6078;
	g_hc_0.data[7][0].h = 2000.0;	g_hc_0.data[7][0].r = 0.8275;		g_hc_0.data[7][0].g = 0.6;			g_hc_0.data[7][0].b = 0.349;
	g_hc_0.data[8][0].h = 5000.0;	g_hc_0.data[8][0].r = 0.7412;		g_hc_0.data[8][0].g = 0.3961;		g_hc_0.data[8][0].b = 0.0157;
	g_hc_0.data[9][0].h = 6000.0;	g_hc_0.data[9][0].r = 0.5922;		g_hc_0.data[9][0].g = 0.0667;		g_hc_0.data[9][0].b = 0.0135;
	g_hc_0.data[10][0].h = 7000.0;	g_hc_0.data[10][0].r = 0.5922;		g_hc_0.data[10][0].g = 0.0667;		g_hc_0.data[10][0].b = 0.0135;
	g_hc_0.data[11][0].h = 8000.0;	g_hc_0.data[11][0].r = 0.5922;		g_hc_0.data[11][0].g = 0.0667;		g_hc_0.data[11][0].b = 0.0135;
	//第 1 套高度-颜色方案
	g_hc_0.data[0][1].h = -8000.0;	g_hc_0.data[0][1].r = 41.0 / 255.0;			g_hc_0.data[0][1].g = 97.0 / 255.0;			g_hc_0.data[0][1].b = 156.0 / 255.0;
	g_hc_0.data[1][1].h = -1000.0;	g_hc_0.data[1][1].r = 89.0 / 255.0;			g_hc_0.data[1][1].g = 148.0 / 255.0;		g_hc_0.data[1][1].b = 204.0 / 255.0;
	g_hc_0.data[2][1].h = -100.0;	g_hc_0.data[2][1].r = 148.0 / 255.0;			g_hc_0.data[2][1].g = 194.0 / 255.0;		g_hc_0.data[2][1].b = 247.0 / 255.0;
	g_hc_0.data[3][1].h = 0.0;		g_hc_0.data[3][1].r = 186.0 / 255.0;			g_hc_0.data[3][1].g = 222.0 / 255.0;		g_hc_0.data[3][1].b = 255.0 / 255.0;
	g_hc_0.data[4][1].h = 0.0;		g_hc_0.data[4][1].r = 0.5098;		g_hc_0.data[4][1].g = 0.6510;		g_hc_0.data[4][1].b = 0.3255;
	g_hc_0.data[5][1].h = 500.0;	g_hc_0.data[5][1].r = 0.1804;		g_hc_0.data[5][1].g = 0.4063;		g_hc_0.data[5][1].b = 0.1047;
	g_hc_0.data[6][1].h = 1000.0;	g_hc_0.data[6][1].r = 0.8784;		g_hc_0.data[6][1].g = 0.7765;		g_hc_0.data[6][1].b = 0.6078;
	g_hc_0.data[7][1].h = 2000.0;	g_hc_0.data[7][1].r = 0.8275;		g_hc_0.data[7][1].g = 0.6;			g_hc_0.data[7][1].b = 0.349;
	g_hc_0.data[8][1].h = 5000.0;	g_hc_0.data[8][1].r = 0.7412;		g_hc_0.data[8][1].g = 0.3961;		g_hc_0.data[8][1].b = 0.0157;
	g_hc_0.data[9][1].h = 6000.0;	g_hc_0.data[9][1].r = 0.5922;		g_hc_0.data[9][1].g = 0.0667;		g_hc_0.data[9][1].b = 0.0135;
	g_hc_0.data[10][1].h = 7000.0;	g_hc_0.data[10][1].r = 0.5922;		g_hc_0.data[10][1].g = 0.0667;		g_hc_0.data[10][1].b = 0.0135;
	g_hc_0.data[11][1].h = 8000.0;	g_hc_0.data[11][1].r = 0.5922;		g_hc_0.data[11][1].g = 0.0667;		g_hc_0.data[11][1].b = 0.0135;
	//第 2 套高度-颜色方案
	g_hc_0.data[0][2].h = -8000.0;	g_hc_0.data[0][2].r = 41.0 / 255.0;			g_hc_0.data[0][2].g = 97.0 / 255.0;			g_hc_0.data[0][2].b = 156.0 / 255.0;
	g_hc_0.data[1][2].h = -1000.0;	g_hc_0.data[1][2].r = 89.0 / 255.0;			g_hc_0.data[1][2].g = 148.0 / 255.0;		g_hc_0.data[1][2].b = 204.0 / 255.0;
	g_hc_0.data[2][2].h = -100.0;	g_hc_0.data[2][2].r = 148.0 / 255.0;			g_hc_0.data[2][2].g = 194.0 / 255.0;		g_hc_0.data[2][2].b = 247.0 / 255.0;
	g_hc_0.data[3][2].h = 0.0;		g_hc_0.data[3][2].r = 186.0 / 255.0;			g_hc_0.data[3][2].g = 222.0 / 255.0;		g_hc_0.data[3][2].b = 255.0 / 255.0;
	g_hc_0.data[4][2].h = 0.0;		g_hc_0.data[4][2].r = 0.5098;		g_hc_0.data[4][2].g = 0.6510;		g_hc_0.data[4][2].b = 0.3255;
	g_hc_0.data[5][2].h = 2000.0;	g_hc_0.data[5][2].r = 0.1804;		g_hc_0.data[5][2].g = 0.4063;		g_hc_0.data[5][2].b = 0.1047;
	g_hc_0.data[6][2].h = 4000.0;	g_hc_0.data[6][2].r = 0.8784;		g_hc_0.data[6][2].g = 0.7765;		g_hc_0.data[6][2].b = 0.6078;
	g_hc_0.data[7][2].h = 6000.0;	g_hc_0.data[7][2].r = 0.8275;		g_hc_0.data[7][2].g = 0.6;			g_hc_0.data[7][2].b = 0.3490;
	g_hc_0.data[8][2].h = 6500.0;	g_hc_0.data[8][2].r = 0.7412;		g_hc_0.data[8][2].g = 0.3961;		g_hc_0.data[8][2].b = 0.0157;
	g_hc_0.data[9][2].h = 7000.0;	g_hc_0.data[9][2].r = 0.5922;		g_hc_0.data[9][2].g = 0.0667;		g_hc_0.data[9][2].b = 0.0135;
	g_hc_0.data[10][2].h = 7500.0;	g_hc_0.data[10][2].r = 0.5922;		g_hc_0.data[10][2].g = 0.0667;		g_hc_0.data[10][2].b = 0.0135;
	g_hc_0.data[11][2].h = 8000.0;	g_hc_0.data[11][2].r = 0.5922;		g_hc_0.data[11][2].g = 0.0667;		g_hc_0.data[11][2].b = 0.0135;
	//第 3 套高度-颜色方案
	g_hc_0.data[0][3].h = -8000.0;	g_hc_0.data[0][3].r = 41.0 / 255.0;			g_hc_0.data[0][3].g = 97.0 / 255.0;			g_hc_0.data[0][3].b = 156.0 / 255.0;
	g_hc_0.data[1][3].h = -1000.0;	g_hc_0.data[1][3].r = 89.0 / 255.0;			g_hc_0.data[1][3].g = 148.0 / 255.0;		g_hc_0.data[1][3].b = 204.0 / 255.0;
	g_hc_0.data[2][3].h = -100.0;	g_hc_0.data[2][3].r = 148.0 / 255.0;			g_hc_0.data[2][3].g = 194.0 / 255.0;		g_hc_0.data[2][3].b = 247.0 / 255.0;
	g_hc_0.data[3][3].h = 0.0;		g_hc_0.data[3][3].r = 186.0 / 255.0;			g_hc_0.data[3][3].g = 222.0 / 255.0;		g_hc_0.data[3][3].b = 255.0 / 255.0;
	g_hc_0.data[4][3].h = 0.0;		g_hc_0.data[4][3].r = 0.5098;		g_hc_0.data[4][3].g = 0.6510;		g_hc_0.data[4][3].b = 0.3255;
	g_hc_0.data[5][3].h = 400.0;	g_hc_0.data[5][3].r = 0.1804;		g_hc_0.data[5][3].g = 0.4063;		g_hc_0.data[5][3].b = 0.1047;
	g_hc_0.data[6][3].h = 1000.0;	g_hc_0.data[6][3].r = 0.8784;		g_hc_0.data[6][3].g = 0.7765;		g_hc_0.data[6][3].b = 0.6078;
	g_hc_0.data[7][3].h = 2000.0;	g_hc_0.data[7][3].r = 0.8275;		g_hc_0.data[7][3].g = 0.6;			g_hc_0.data[7][3].b = 0.3490;
	g_hc_0.data[8][3].h = 5000.0;	g_hc_0.data[8][3].r = 0.7529;		g_hc_0.data[8][3].g = 0.1255;		g_hc_0.data[8][3].b = 0.0;
	g_hc_0.data[9][3].h = 7000.0;	g_hc_0.data[9][3].r = 1.0;			g_hc_0.data[9][3].g = 1.0;			g_hc_0.data[9][3].b = 1.0;
	g_hc_0.data[10][3].h = 8000.0;	g_hc_0.data[10][3].r = 1.0;			g_hc_0.data[10][3].g = 1.0;			g_hc_0.data[10][3].b = 1.0;
	g_hc_0.data[11][3].h = 8500.0;	g_hc_0.data[11][3].r = 0.5922;		g_hc_0.data[11][3].g = 0.0667;		g_hc_0.data[11][3].b = 0.0135;
	//第 4 套高度-颜色方案
	g_hc_0.data[0][4].h = -8000.0;	g_hc_0.data[0][4].r = 41.0 / 255.0;			g_hc_0.data[0][4].g = 97.0 / 255.0;			g_hc_0.data[0][4].b = 156.0 / 255.0;
	g_hc_0.data[1][4].h = -1000.0;	g_hc_0.data[1][4].r = 89.0 / 255.0;			g_hc_0.data[1][4].g = 148.0 / 255.0;		g_hc_0.data[1][4].b = 204.0 / 255.0;
	g_hc_0.data[2][4].h = -100.0;	g_hc_0.data[2][4].r = 148.0 / 255.0;			g_hc_0.data[2][4].g = 194.0 / 255.0;		g_hc_0.data[2][4].b = 247.0 / 255.0;
	g_hc_0.data[3][4].h = 0.0;		g_hc_0.data[3][4].r = 186.0 / 255.0;			g_hc_0.data[3][4].g = 222.0 / 255.0;		g_hc_0.data[3][4].b = 255.0 / 255.0;
	g_hc_0.data[4][4].h = 0.0;		g_hc_0.data[4][4].r = 0.5098;		g_hc_0.data[4][4].g = 0.6510;		g_hc_0.data[4][4].b = 0.3255;
	g_hc_0.data[5][4].h = 1000.0;	g_hc_0.data[5][4].r = 0.1804;		g_hc_0.data[5][4].g = 0.4063;		g_hc_0.data[5][4].b = 0.1047;
	g_hc_0.data[6][4].h = 2000.0;	g_hc_0.data[6][4].r = 0.8784;		g_hc_0.data[6][4].g = 0.7765;		g_hc_0.data[6][4].b = 0.6078;
	g_hc_0.data[7][4].h = 3000.0;	g_hc_0.data[7][4].r = 0.8275;		g_hc_0.data[7][4].g = 0.6;			g_hc_0.data[7][4].b = 0.3490;
	g_hc_0.data[8][4].h = 4000.0;	g_hc_0.data[8][4].r = 0.7412;		g_hc_0.data[8][4].g = 0.3961;		g_hc_0.data[8][4].b = 0.0157;
	g_hc_0.data[9][4].h = 5000.0;	g_hc_0.data[9][4].r = 0.5922;		g_hc_0.data[9][4].g = 0.0667;		g_hc_0.data[9][4].b = 0.0135;
	g_hc_0.data[10][4].h = 6000.0;	g_hc_0.data[10][4].r = 0.5922;		g_hc_0.data[10][4].g = 0.0667;		g_hc_0.data[10][4].b = 0.0135;
	g_hc_0.data[11][4].h = 8000.0;	g_hc_0.data[11][4].r = 0.5922;		g_hc_0.data[11][4].g = 0.0667;		g_hc_0.data[11][4].b = 0.0135;

	//第 5 套高度-颜色方案
	g_hc_0.data[0][5].h = -8000.0;	g_hc_0.data[0][5].r = 41.0 / 255.0;			g_hc_0.data[0][5].g = 97.0 / 255.0;			g_hc_0.data[0][5].b = 156.0 / 255.0;
	g_hc_0.data[1][5].h = -1000.0;	g_hc_0.data[1][5].r = 89.0 / 255.0;			g_hc_0.data[1][5].g = 148.0 / 255.0;		g_hc_0.data[1][5].b = 204.0 / 255.0;
	g_hc_0.data[2][5].h = -100.0;	g_hc_0.data[2][5].r = 148.0 / 255.0;			g_hc_0.data[2][5].g = 194.0 / 255.0;		g_hc_0.data[2][5].b = 247.0 / 255.0;
	g_hc_0.data[3][5].h = 0.0;		g_hc_0.data[3][5].r = 186.0 / 255.0;			g_hc_0.data[3][5].g = 222.0 / 255.0;		g_hc_0.data[3][5].b = 255.0 / 255.0;
	g_hc_0.data[4][5].h = 0.0;		g_hc_0.data[4][5].r = 0.5098;		g_hc_0.data[4][5].g = 0.6510;		g_hc_0.data[4][5].b = 0.3255;
	g_hc_0.data[5][5].h = 400.0;	g_hc_0.data[5][5].r = 0.1804;		g_hc_0.data[5][5].g = 0.4063;		g_hc_0.data[5][5].b = 0.1047;
	g_hc_0.data[6][5].h = 1000.0;	g_hc_0.data[6][5].r = 0.8784;		g_hc_0.data[6][5].g = 0.7765;		g_hc_0.data[6][5].b = 0.6078;
	g_hc_0.data[7][5].h = 2000.0;	g_hc_0.data[7][5].r = 0.8275;		g_hc_0.data[7][5].g = 0.6;			g_hc_0.data[7][5].b = 0.3490;
	g_hc_0.data[8][5].h = 5000.0;	g_hc_0.data[8][5].r = 0.7529;		g_hc_0.data[8][5].g = 0.1255;		g_hc_0.data[8][5].b = 0.0;
	g_hc_0.data[9][5].h = 7000.0;	g_hc_0.data[9][5].r = 1.0;			g_hc_0.data[9][5].g = 1.0;			g_hc_0.data[9][5].b = 1.0;
	g_hc_0.data[10][5].h = 8000.0;	g_hc_0.data[10][5].r = 1.0;			g_hc_0.data[10][5].g = 1.0;			g_hc_0.data[10][5].b = 1.0;
	g_hc_0.data[11][5].h = 8500.0;	g_hc_0.data[11][5].r = 1.0;		g_hc_0.data[11][5].g = 1.0;		g_hc_0.data[11][5].b = 1.0;

}

void setGLSLColor(int colorIndex)
{
	for (int i = 0; i < HCNUM; i++)
	{
		if (true)
		{
			g_hc2Fx.hcolor0[i].x = g_hc.data[i][colorIndex].r;
			g_hc2Fx.hcolor0[i].y = g_hc.data[i][colorIndex].g;
			g_hc2Fx.hcolor0[i].z = g_hc.data[i][colorIndex].b;
			g_hc2Fx.hcolor0[i].w = g_hc.data[i][colorIndex].h;
		}
		else
		{
			g_hc2Fx.hcolor1[i].x = g_hc.data[i][colorIndex].r;
			g_hc2Fx.hcolor1[i].y = g_hc.data[i][colorIndex].g;
			g_hc2Fx.hcolor1[i].z = g_hc.data[i][colorIndex].b;
			g_hc2Fx.hcolor1[i].w = g_hc.data[i][colorIndex].h;
		}
	}
}


///////////////////////////////////////////////////////////////////////////
//
CTerrainSurface::CTerrainSurface()
{
    isStop = false;

    m_bDone = false;
    m_bDisposeDone = false;
    m_bIOThreadDone = false;
    m_NumIORequests = 0;

    maxLevel = 3;

	m_Model = std::unique_ptr<ModelClass>(new ModelClass());
    m_Model->Initialize();

    //预加载纹理
    std::string path_img = g_res_FilePath + "Earth/images/img_loading.jpg";
    hTex_PreLoaded[0] = LoadTexturePic(path_img.c_str());
    path_img = g_res_FilePath + "Earth/images/img_none.jpg";
    hTex_PreLoaded[1] = LoadTexturePic(path_img.c_str());
    path_img = g_res_FilePath + "Earth/images/img_png_none.png";
    hTex_PreLoaded[2] = LoadTexturePic(path_img.c_str());
    path_img = g_res_FilePath + "Earth/images/img_vec_back.jpg";
    hTex_PreLoaded[3] = LoadTexturePic(path_img.c_str());

#ifdef __linux__
	//创建信号量
	sem_init(&sem_io, 0, 0);

	//初始化互斥锁
	pthread_mutex_init(&mutex_io, nullptr);

	for (int j = 0; j < NUMS_THREAD_IO; j++)
	{
        pthread_create(&thread_io[j], nullptr, _FileIOThreadProc, (void *)this);

	}
#else
	//
	sem_io = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
	InitializeCriticalSectionAndSpinCount(&mutex_io, 4000);
	for (int j = 0; j < NUMS_THREAD_IO; j++)
	{
		thread_io[j] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)_FileIOThreadProc, (LPVOID)this, 0, NULL);

	}
#endif
}

CTerrainSurface::~CTerrainSurface()
{
    closeThread();
    requestInterruption();
    quit();
    wait();

    SAFE_DELETE_ARRAY(vtexVerts);

	for (GLuint &i : hTex_PreLoaded)
	{
		glDeleteTextures(1, &i);
	}

#ifdef __linux__
	pthread_mutex_destroy(&mutex_io);
	sem_destroy(&sem_io);
#else
	DeleteCriticalSection(&mutex_io);
	CloseHandle(sem_io);
#endif

}

void CTerrainSurface::Shutdown()
{
    m_bDone = true;
    // Release

    ioQueueList.clear();
    m_pTileRenderList.clear();

    for (int i=0; i<(int)m_pTileRenderList.size(); i++)
    {
        SAFE_DELETE(m_pQuadTile[i]);
    }
    m_pQuadTile.clear();
    std::map<TILTPOS, CQuadTile*> ::iterator iter;
    for ( iter = totalTileMap.begin(); iter != totalTileMap.end(); )
    {
        CQuadTile *pT = iter->second;
        SAFE_DELETE( pT );
        iter = totalTileMap.erase( iter++ );
    }
    totalTileMap.clear();

}

void CTerrainSurface::closeThread()
{
    isStop = true;
}

bool CTerrainSurface::Initialize(CTextureFont* _font)
{
    //initialize background level 3
    for (int row = 0; row <8; row++)
    {
        for (int col = 0; col <8; col++)
        {
            auto pTile = new CQuadTile();
            pTile->Level = 3;
            pTile->Row = row;
            pTile->Col = col;
            pTile->keyPos=TILTPOS(3,col,row);

            pTile->Initialize();
            
            pTile->LoadVT();

           for ( auto &pts : pTile->multiPts.pts)
           {
               if (pts.geoType == POINT_CAPITAL
                   || pts.geoType == POINT_COUNTRY
                   || pts.geoType == POINT_PROVINCE
                   || pts.geoType == POINT_PCAPITAL
                   || pts.geoType == POINT_CITY
                   || pts.geoType == POINT_COUNTY)
               {
                   LabelInfo linfo;
                   linfo.indexItem = -1;

                   //
                   linfo.keyPos = pTile->keyPos;
                   linfo.name = pts.name;
                   linfo.lDesc.lon = pts.pos.lon;
                   linfo.lDesc.lat = pts.pos.lat;
                   linfo.lDesc.height = 100;
                   linfo.type = 0;
                   linfo.visible = 1;
                   linfo.textureId = 0;
                   linfo.texType = pts.geoType;

                   //                if(pts.geoType == POINT_CAPITAL
                   //                   || pts.geoType == POINT_PCAPITAL)
                   //                {
                   //                    linfo.textureId = texIdList[3];
                   //                }
                   //                if(pts.geoType == POINT_CITY)
                   //                {
                   //                    linfo.textureId = texIdList[4];
                   //                }
                   //
                   //                labelInfos.push_back(linfo);
                   //
                   linfo.type = 2;
                   std::wstring wstr;
                   wstr = s2ws(linfo.name);

                   _font->genFontText(wstr.c_str(), linfo);
                   //
                   labelInfos.push_back(linfo);
               }

           }
            for (auto &line : pTile->multiLs.lines)
            {
                if (line.geoType == LINE_CN_DASH)
                {
                    double distanceMeasured = 0;
                    for (int k = 0; k < line.num - 1; k++)
                    {
                        double angularDistance = ApproxAngularDistance(line.pts[k].lat, line.pts[k].lon, line.pts[k + 1].lat, line.pts[k + 1].lon);
                        distanceMeasured += angularDistance*EARTH_MEANRADIUS;
                    }
                    float density = 30;
                    double distanceStep = 0;
                    for (int k = 0; k < line.num - 1; k++)
                    {
                        double angularDistance = ApproxAngularDistance(line.pts[k].lat, line.pts[k].lon, line.pts[k + 1].lat, line.pts[k + 1].lon);
                        distanceStep += angularDistance*EARTH_MEANRADIUS;
                        line.pts[k + 1].uTex = density*distanceStep / distanceMeasured;
                    }
                }
            }

            m_pQuadTile.push_back(pTile);
            //将3级数据（除南北极）加入totalTileMap
            totalTileMap.insert(std::make_pair(pTile->keyPos, pTile));
            g_globalScalar.tiltLoadedNum++;
        }
    }
    //north pole
    for (int col = 0; col <8; col++)
    {
        auto pTile = new CQuadTile();
        pTile->Level = 3;
        pTile->Row = 0;//在初始化里面用到，设为0
        pTile->Col = col;
        pTile->InitializeNPole();
        pTile->Row = -1;//自定义值

        m_pQuadTile.push_back(pTile);
    }
    //south pole
    for (int col = 0; col <8; col++)
    {
        auto pTile = new CQuadTile();
        pTile->Level = 3;
        pTile->Row = 0;//在初始化里面用到，设为0
        pTile->Col = col;

        pTile->InitializeSPole();

        pTile->Row = -2;//自定义值

        m_pQuadTile.push_back(pTile);

    }
    //
    LoadFixedHeightColorData();
 	LoadTopColorfromDisk();

	setGLSLColor(5);

    ///////////////////////////////////////////////////////////////
    //计算每个顶点的纹理坐标
	vtexVerts = new GLfloat[NUM_PTS_TILE * NUM_PTS_TILE * 2 + (9 + 9 + 7 + 7) * 2];

    for ( int y = 0; y < NUM_PTS_TILE; ++y )
    {
        for ( int x = 0; x < NUM_PTS_TILE; ++x )
        {
            vtexVerts[(( y *NUM_PTS_TILE ) + x)*2] = ( float )x / ( float )( NUM_PTS_TILE - 1 );
            vtexVerts[(( y *NUM_PTS_TILE ) + x)*2 + 1] = 1.0 - ( float )y / ( float )( NUM_PTS_TILE - 1 );
        }
    }

    //
    int base = NUM_PTS_TILE*NUM_PTS_TILE*2;
    int y=0;
    for ( int x = 0; x < NUM_PTS_TILE; ++x )
    {
        vtexVerts[base+x*2] = ( float )x / ( float )( NUM_PTS_TILE - 1 );
        vtexVerts[base+x*2 + 1] = 1.0 - ( float )y / ( float )( NUM_PTS_TILE - 1 );
    }

    base += 9*2;
    y=NUM_PTS_TILE-1;
    for ( int x = 0; x < NUM_PTS_TILE; ++x )
    {
        vtexVerts[base+x*2] = ( float )x / ( float )( NUM_PTS_TILE - 1 );
        vtexVerts[base+x*2 + 1] = 1.0 - ( float )y / ( float )( NUM_PTS_TILE - 1 );
    }

    base += 9*2;
    int x = NUM_PTS_TILE-1;
    for ( int y = 1; y < NUM_PTS_TILE-1; ++y )
    {
        vtexVerts[base+(y-1)*2] = ( float )x / ( float )( NUM_PTS_TILE - 1 );
        vtexVerts[base+(y-1)*2 + 1] = 1.0 - ( float )y / ( float )( NUM_PTS_TILE - 1 );
    }

    base += 7*2;
    x = 0;
    for ( int y = 1; y < NUM_PTS_TILE-1; ++y )
    {
        vtexVerts[base+(y-1)*2] = ( float )x / ( float )( NUM_PTS_TILE - 1 );
        vtexVerts[base+(y-1)*2 + 1] = 1.0 - ( float )y / ( float )( NUM_PTS_TILE - 1 );
    }

	//
	genStippleTex();

    //
    return true;
}

void CTerrainSurface::RenderTile(CQuadTile* pTile, GLuint texPreloaded[], bool is3D)
{
	//
	f_->glUniform1i(surfProgram.uniformLocation("g_col"), pTile->Col);
	f_->glUniform1i(surfProgram.uniformLocation("g_row"), pTile->Row);
	f_->glUniform1i(surfProgram.uniformLocation("g_level"), pTile->Level);

	if (!is3D)
	{
		//
		glm::dvec3 pos_normal = SphericalToMercator(pTile->CenterLatitude, pTile->CenterLongitude, EARTH_MEANRADIUS);
		glm::dvec3 pos_add_2PI = SphericalToMercator(pTile->CenterLatitude, pTile->CenterLongitude + 2.0*PI, EARTH_MEANRADIUS);
		glm::dvec3 pos_sub_2PI = SphericalToMercator(pTile->CenterLatitude, pTile->CenterLongitude - 2.0*PI, EARTH_MEANRADIUS);

		double dist_normal = glm::length(cameraPos - pos_normal);
		double dist_add_2PI = glm::length(cameraPos - pos_add_2PI);
		double dist_sub_2PI = glm::length(cameraPos - pos_sub_2PI);

		if (dist_normal > dist_add_2PI)
		{
			f_->glUniform1i(surfProgram.uniformLocation("offset_type"), 1);
		}
		else if (dist_normal > dist_sub_2PI)
		{
			f_->glUniform1i(surfProgram.uniformLocation("offset_type"), -1);
		}
		else
		{
			f_->glUniform1i(surfProgram.uniformLocation("offset_type"), 0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	pTile->gridIndex = 5;

	if (pTile->Level == 3)
		pTile->shapeType = 0;
	else
		ComputeShapeType(pTile);
	pTile->ComputeWorldMatrix(m_Camera->viewPoint);
	tileRender(pTile, texPreloaded);

}

void CTerrainSurface::tileRender(CQuadTile* pTile, GLuint texPreloaded[])
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	// Load the vertex position
	if (is3D)
	{
		f_->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), pTile->vVertices);
	}
	else
	{
		f_->glVertexAttribPointer(0, 3, GL_FLOAT,
			GL_FALSE, 3 * sizeof(GLfloat), pTile->vVertices2D);
	}
	f_->glEnableVertexAttribArray(0);

	if (texType == 3 || (texType == 1 && (pTile->Row == -1 || pTile->Row == -2)))
	{
		f_->glUniform1i(surfProgram.uniformLocation("tex_type"), texType);
		f_->glActiveTexture(GL_TEXTURE1);

		if (pTile->isNorTexLoaded)
			glBindTexture(GL_TEXTURE_2D, pTile->m_texture);
		else if (pTile->isNorHaveLoaded && !pTile->isNorLoaded)
		{
			f_->glUniform1i(surfProgram.uniformLocation("tex_type"), 1);
			f_->glBindTexture(GL_TEXTURE_2D, texPreloaded[0]);  //此处无地势图
		}
		else
		{
			f_->glUniform1i(surfProgram.uniformLocation("tex_type"), 1);
			f_->glBindTexture(GL_TEXTURE_2D, texPreloaded[0]);  //图片加载中
		}

		f_->glUniform1i(surfProgram.uniformLocation("shaderTexture"), 1);
	}
	else if (texType == 1)
	{
		f_->glActiveTexture(GL_TEXTURE0);

		if (pTile->isSatTexLoaded)
			f_->glBindTexture(GL_TEXTURE_2D, pTile->m_texturePic);
		else if (pTile->isSatHaveLoaded && !pTile->isSatLoaded)
			f_->glBindTexture(GL_TEXTURE_2D, texPreloaded[0]);  //此处无卫星图片
		else
			f_->glBindTexture(GL_TEXTURE_2D, texPreloaded[0]);  //卫星图片加载中

		f_->glUniform1i(surfProgram.uniformLocation("shaderTexture"), 0);
	}

	//
	f_->glUniformMatrix4fv(surfProgram.uniformLocation("u_worldMatrix"), 1, GL_FALSE, (GLfloat *)&pTile->worldMQT[0][0]);


	// Draw the grid
	f_->glDrawElements(GL_TRIANGLE_STRIP, m_Model->s_detailLevel[pTile->gridIndex].tileBodies[pTile->shapeType]->vertex_count, GL_UNSIGNED_SHORT, m_Model->s_detailLevel[pTile->gridIndex].tileBodies[pTile->shapeType]->index_table);


    f_->glDisableVertexAttribArray(0);

}

void CTerrainSurface::RenderTileGeo(CQuadTile* pTile)
{
	//
	f_->glEnable(GL_DEPTH_TEST);

	glm::dvec3 vCamera;
	m_Camera->GetPosition(vCamera);

	//
	f_->glActiveTexture(GL_TEXTURE0);
	f_->glBindTexture(GL_TEXTURE_2D, stippleTex);
	f_->glUniform1i(lineProgram.uniformLocation("u_stippleTexture"), 0);

	//////////////////////////////////////////////////////////////////////////
	f_->glUniform3f(lineProgram.uniformLocation("v3CameraPos"), vCamera.x, vCamera.y, vCamera.z);

	glm::mat4 mat = m_Camera->m_MVPMatrix;
	f_->glUniformMatrix4fv(lineProgram.uniformLocation("WorldViewProj"), 1, GL_FALSE, (GLfloat *)&mat[0][0]);

	if (is3D)
		f_->glUniform1i(lineProgram.uniformLocation("p_type"), 1);
	else
	{
		f_->glUniform1i(lineProgram.uniformLocation("p_type"), 0);

		//
		glm::dvec3 pos_normal = SphericalToMercator(pTile->CenterLatitude, pTile->CenterLongitude, EARTH_MEANRADIUS);
		glm::dvec3 pos_add_2PI = SphericalToMercator(pTile->CenterLatitude, pTile->CenterLongitude + 2.0*PI, EARTH_MEANRADIUS);
		glm::dvec3 pos_sub_2PI = SphericalToMercator(pTile->CenterLatitude, pTile->CenterLongitude - 2.0*PI, EARTH_MEANRADIUS);

		double dist_normal = glm::length(vCamera - pos_normal);
		double dist_add_2PI = glm::length(vCamera - pos_add_2PI);
		double dist_sub_2PI = glm::length(vCamera - pos_sub_2PI);

		if (dist_normal > dist_add_2PI)
		{
			f_->glUniform1i(lineProgram.uniformLocation("offset_type"), 1);
		}
		else if (dist_normal > dist_sub_2PI)
		{
			f_->glUniform1i(lineProgram.uniformLocation("offset_type"), -1);
		}
		else
		{
			f_->glUniform1i(lineProgram.uniformLocation("offset_type"), 0);
		}

	}


	f_->glUniform1f(lineProgram.uniformLocation("lineAlt"), 10000.0);

	f_->glEnableVertexAttribArray(0);
	f_->glEnableVertexAttribArray(1);

	for (auto &line : pTile->multiLs.lines)
	{
		f_->glUniform1f(lineProgram.uniformLocation("lineType"), 0.0);//solid line
		f_->glLineWidth(1.0);
		//其他国界
		if (line.geoType == LINE_COUNTRY)
		{
			f_->glUniform4f(lineProgram.uniformLocation("v4Color"), 255.0 / 255.0, 255 / 255.0, 255 / 255.0, 0.6);
			f_->glVertexAttribPointer(0, 2, GL_FLOAT,
				GL_FALSE, 3 * sizeof(GLfloat), &line.pts[0]);
			f_->glDrawArrays(GL_LINE_STRIP, 0, line.num);
		}
		//省界
		if (line.geoType == LINE_PROVINCE)
		{
			f_->glUniform4f(lineProgram.uniformLocation("v4Color"), 166 / 255.0, 171 / 255.0, 177 / 255.0, 0.8);
			f_->glVertexAttribPointer(0, 2, GL_FLOAT,
				GL_FALSE, 3 * sizeof(GLfloat), &line.pts[0]);
			f_->glDrawArrays(GL_LINE_STRIP, 0, line.num);
		}

		//中国国界
		f_->glLineWidth(3.0);
		f_->glUniform4f(lineProgram.uniformLocation("v4Color"), 224.0 / 255.0, 71 / 255.0, 71 / 255.0, 1.0);
		if (line.geoType == LINE_CN)
		{
			f_->glVertexAttribPointer(0, 2, GL_FLOAT,
				GL_FALSE, 3 * sizeof(GLfloat), &line.pts[0]);
			f_->glDrawArrays(GL_LINE_STRIP, 0, line.num);
		}
		if (line.geoType == LINE_CN_DASH)
		{
			//            glEnable(GL_LINE_STIPPLE);
			//            glLineStipple(1, 0x3F3F);
			f_->glUniform4f(lineProgram.uniformLocation("v4Color"), 224.0 / 255.0, 71 / 255.0, 71 / 255.0, 1.0);
			f_->glUniform1f(lineProgram.uniformLocation("lineType"), 1.0);//dashed line
			f_->glVertexAttribPointer(0, 2, GL_FLOAT,
				GL_FALSE, 3 * sizeof(GLfloat), &line.pts[0]);
			f_->glVertexAttribPointer(1, 1, GL_FLOAT,
				GL_FALSE, 3 * sizeof(GLfloat), &line.pts[0].uTex);
			f_->glDrawArrays(GL_LINE_STRIP, 0, line.num);
			//            glDisable(GL_LINE_STIPPLE);
		}


	}

    f_->glDisableVertexAttribArray(0);
    f_->glDisableVertexAttribArray(1);
	//

}

void CTerrainSurface::Render(std::unique_ptr<CameraClass> const &m_Camera, int width, int height)
{
	if ((!ioQueueList.empty()) && globalBool.semaphore_signal)
	{
		globalBool.semaphore_signal = false;
#ifdef __linux__
		sem_post(&sem_io);
#else
		ReleaseSemaphore(sem_io, 1, NULL);
#endif
	}

	//
	surfProgram.bind();


	/////////////////////////////////////////////////////////////////////
	//
	GLfloat hColor[12][4];
	for (int i = 0; i < 12; i++) {
		hColor[i][0] = g_hc2Fx.hcolor0[i].x;
		hColor[i][1] = g_hc2Fx.hcolor0[i].y;
		hColor[i][2] = g_hc2Fx.hcolor0[i].z;
		hColor[i][3] = g_hc2Fx.hcolor0[i].w;
	}
	f_->glUniform4fv(surfProgram.uniformLocation("g_hColor0"), 12, (GLfloat *)hColor[0]);

	// Load the light direction
	f_->glUniform3f(surfProgram.uniformLocation("u_lightDirection"), _vLightDirection.x,
		_vLightDirection.y, _vLightDirection.z);

	if (is3D)
		f_->glUniform1i(surfProgram.uniformLocation("p_type"), 1);
	else
		f_->glUniform1i(surfProgram.uniformLocation("p_type"), 0);

	f_->glUniform1i(surfProgram.uniformLocation("tex_type"), texType);
	f_->glUniform1i(surfProgram.uniformLocation("label_mark"), label_mark);
	f_->glUniform1i(surfProgram.uniformLocation("flag_sun_light"), isSunLightOpen);
	//////////////////////////////////////////////////////////////////////////
	//
	glm::dmat4 viewM = m_Camera->m_absViewMatrix;
	glm::mat4 projectionMatrix = m_Camera->m_absoluteProjectionMatrix;

	//
	double alt_Camera; // Distance from eye to target
	m_Camera->GetAltitude(alt_Camera);
	double distToCenterOfPlanet = (alt_Camera + EARTH_MEANRADIUS);
	double tangentalDistance = sqrt(distToCenterOfPlanet * distToCenterOfPlanet - EARTH_MEANRADIUS * EARTH_MEANRADIUS);

	m_Camera->GetPosition(cameraPos);
	cviewPos = m_Camera->viewPoint;

	//////////////////////////////////////////////////////////////////////////
	//

	double dist_Camera;

	glm::dvec3 pos_inters;
	GetPosIntersectSphere(pos_inters);

	if (is3D)
	{
		dist_Camera = glm::length(cameraPos - pos_inters);
	}
	else
	{
		glm::dvec3 pos_it = CartesianToSpherical(pos_inters.x, pos_inters.y, pos_inters.z);
		dist_Camera = alt_Camera - (pos_it.x - EARTH_MEANRADIUS);
	}

	double tanDis = 17000000;
	if (dist_Camera > tanDis)
		maxLevel = 3;
	else if (dist_Camera > tanDis / pow(2.0, 4 - 3))
		maxLevel = 4;
	else if (dist_Camera > tanDis / pow(2.0, 5 - 3))
		maxLevel = 5;
	else if (dist_Camera > tanDis / pow(2.0, 6 - 3))
		maxLevel = 6;
	else if (dist_Camera > tanDis / pow(2.0, 7 - 3))
		maxLevel = 7;
	else if (dist_Camera > tanDis / pow(2.0, 8 - 3))
		maxLevel = 8;
	else if (dist_Camera > tanDis / pow(2.0, 9 - 3))
		maxLevel = 9;
	else if (dist_Camera > tanDis / pow(2.0, 10 - 3))
		maxLevel = 10;
	else if (dist_Camera > tanDis / pow(2.0, 11 - 3))
		maxLevel = 11;
	else if (dist_Camera > tanDis / pow(2.0, 12 - 3))
		maxLevel = 12;
	else if (dist_Camera > tanDis / pow(2.0, 13 - 3))
		maxLevel = 13;
	else if (dist_Camera > tanDis / pow(2.0, 14 - 3))
		maxLevel = 14;
	else if (dist_Camera > tanDis / pow(2.0, 15 - 3))
		maxLevel = 15;
	else if (dist_Camera > tanDis / pow(2.0, 16 - 3))
		maxLevel = 16;
	else if (dist_Camera > tanDis / pow(2.0, 17 - 3))
		maxLevel = 17;
	else if (dist_Camera > tanDis / pow(2.0, 18 - 3))
		maxLevel = 18;
	else if (dist_Camera > tanDis / pow(2.0, 19 - 3))
		maxLevel = 19;
	else /*if (dist_Camera > tanDis/pow(2.0,20-3))*/
		maxLevel = 20;

	//////////////////////////////////////////////////////////////////////
	// Load the MVP matrix
	glm::mat4 viewMat = viewM;

	f_->glUniformMatrix4fv(surfProgram.uniformLocation("u_viewMatrix"), 1, GL_FALSE, (GLfloat *)&viewMat[0][0]);
	f_->glUniformMatrix4fv(surfProgram.uniformLocation("u_projectionMatrix"), 1, GL_FALSE, (GLfloat *)&projectionMatrix[0][0]);

	//加载统一的纹理坐标
	f_->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), vtexVerts);
	f_->glEnableVertexAttribArray(1);

	//重新计算渲染队列
	m_pTileRenderList.clear();
	curTileVector.clear();

	//视点中心计算
	double lat, lon;
	m_Camera->GetLatitude(lat);
	m_Camera->GetLongitude(lon);

	lat = lat*180.0 / ((double)PI);
	lon = lon*180.0 / ((double)PI);

	TILTPOS tilePos[NUMS_TILE], cPos;
	cPos.level = 3;
	cPos.row = lat2tiley(lat, 3);
	cPos.col = long2tilex(lon, 3);

	if (cPos.row >= 0 && cPos.row <= 7)
	{
#ifdef __linux__
		int ret = pthread_mutex_lock(&mutex_io);
		if (ret == 0)
#else
		if (TryEnterCriticalSection(&mutex_io))
#endif
		{
			if (maxLevel > 3)
			{
				for (int lv = 3; lv < maxLevel; lv++)
					ComputeTile(lat, lon, lv);

			}
			//将不在当前渲染列表里的tile bInUse置为false
			for (auto &iter : totalTileMap)
			{
				if (iter.second->Level == 3 || !iter.second->bInUse)
					continue;

				auto iter_cur = std::find(curTileVector.begin(), curTileVector.end(), iter.first);
				if (iter_cur == curTileVector.end())
				{
					iter.second->UnuseQuadTile();
				}

			}
#ifdef __linux
			pthread_mutex_unlock(&mutex_io);
#else
			LeaveCriticalSection(&mutex_io);
#endif
		}

		//
		GetTilePos(cPos, tilePos);   //tilePos：3级需加载tile
		tilePosList.clear();
		for (auto &tilePo : tilePos)
		{
			bool isFinded = false;
			for (auto i : tilePosList)
			{
				if (tilePo == i)
				{
					isFinded = true;
					break;
				}
			}
			if (!isFinded)
				tilePosList.push_back(tilePo);

		}

		for (auto &t : tilePosList)
		{
			auto *pTile = m_pQuadTile[t.row * 8 + t.col];

			pTile->Getoffset(cameraPos);

			/**
			* 考虑两极文件加载，靠近两极的主体文件只加载到3级
			* 因此不考虑row=0及row=7的情况，只考虑row=1至6
			*/
			//                    if (tilePosList[t].row > 0 && tilePosList[t].row < 7)
			{
				//仅加载与视锥相交的tile
				if (m_Camera->_viewFrustum.Intersects(pTile->m_BBox))
				{
					//计算m_pTileRenderList
					RenderQuadTile(pTile, maxLevel, tangentalDistance);
				}
			}

		}

	}
	else
	{
		tilePosList.clear();
	}

	//3级其他
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			bool istilePos = false;
			auto *pTile = m_pQuadTile[row * 8 + col];

			for (auto &t : tilePosList)
			{
				if (t.row != 0 || t.row != 7)
				{
					if (pTile->keyPos == t)
					{
						istilePos = true;
						break;
					}

				}

			}

			if (!istilePos)
			{
				pTile->Getoffset(cameraPos);
				if (m_Camera->_viewFrustum.Intersects(pTile->m_BBox))
					m_pTileRenderList.push_back(pTile);
			}

		}
	}

	//3D模式下,将两极数据添加进渲染list
	if (is3D)
	{
		for (int col = 0; col < 16; col++)
		{
			m_pTileRenderList.push_back(m_pQuadTile[8 * 8 + col]);
		}
	}
	//
	//////////////////////////////////////////////////////////////////////////
	//与渲染队列m_pTileRenderList中，tile的交点
	if (is3D)
	{
		if (maxLevel > LEV_TILE_PICK_PT)
		{
			PickingRayIntersect(point_move.x(), point_move.y(), vect_pick);
			float vecLen = glm::length(vect_pick);

			if (fabs(vecLen) > EPSILON1)
			{
				glm::dvec3 pos_inter = CartesianToSpherical(vect_pick.x, vect_pick.y, vect_pick.z);
				elevation_pick = pos_inter.x - EARTH_MEANRADIUS;

			}
			else
				elevation_pick = 0;

		}
		else
		{
			elevation_pick = 0;
		}
	}
	else
	{
		if (maxLevel > LEV_TILE_PICK_PT)
		{
			PickingRayIntersect(point_move.x(), point_move.y(), vect_pick);
			float vecLen = glm::length(vect_pick);

			if (fabs(vecLen) > EPSILON1)
			{
				glm::dvec3 pos_inter = MercatorToSpherical(vect_pick.x, vect_pick.y, vect_pick.z);
				elevation_pick = pos_inter.x - EARTH_MEANRADIUS;

			}
			else
				elevation_pick = 0;

		}
		else
		{
			elevation_pick = 0;
		}
	}

	//
	for (auto pT : m_pTileRenderList)
	{
		RenderTile(pT, hTex_PreLoaded);
	}

	LoadTileTexData();

	surfProgram.release();

	//
	lineProgram.bind();

	for (auto &t : tilePosList)
	{
		auto *pTile = m_pQuadTile[t.row * 8 + t.col];

		pTile->Getoffset(cameraPos);

		//仅加载与视锥相交的tile
		//       if (m_Camera->_viewFrustum.Intersects(pTile->m_BBox))
		{
			RenderTileGeo(pTile);
		}

	}
	for (auto pT : m_pTileRenderList)
	{
		if (pT->Level == 3)
		{
			RenderTileGeo(pT);
		}

	}

	lineProgram.release();

}

void CTerrainSurface::GetTilePos(TILTPOS cPos, TILTPOS (&tilePos)[9])
{
    int maxRC = pow(2,cPos.level)-1;
    //  0   7   6
    //  1   C   5
    //  2   3   4
    for(int i=0; i<8; i++)
        tilePos[i].level = cPos.level;
    //C
    tilePos[8] = cPos;
    //0
    tilePos[0].row = cPos.row - 1;
    if(tilePos[0].row<0)
        tilePos[0].row = 0;
    tilePos[0].col = cPos.col - 1;
    if(tilePos[0].col<0)
        tilePos[0].col = maxRC;
    //1
    tilePos[1].row = cPos.row;
    tilePos[1].col = cPos.col - 1;
    if(tilePos[1].col<0)
        tilePos[1].col = maxRC;
    //2
    tilePos[2].row = cPos.row + 1;
    if(tilePos[2].row>maxRC)
        tilePos[2].row = maxRC;
    tilePos[2].col = cPos.col - 1;
    if(tilePos[2].col<0)
        tilePos[2].col = maxRC;
    //3
    tilePos[3].row = cPos.row + 1;
    if(tilePos[3].row>maxRC)
        tilePos[3].row = maxRC;
    tilePos[3].col = cPos.col;
    //4
    tilePos[4].row = cPos.row + 1;
    if(tilePos[4].row>maxRC)
        tilePos[4].row = maxRC;
    tilePos[4].col = cPos.col + 1;
    if(tilePos[4].col>maxRC)
        tilePos[4].col = 0;
    //5
    tilePos[5].row = cPos.row;
    tilePos[5].col = cPos.col + 1;
    if(tilePos[5].col>maxRC)
        tilePos[5].col = 0;
    //6
    tilePos[6].row = cPos.row - 1;
    if(tilePos[6].row<0)
        tilePos[6].row = 0;
    tilePos[6].col = cPos.col + 1;
    if(tilePos[6].col>maxRC)
        tilePos[6].col = 0;
    //7
    tilePos[7].row = cPos.row - 1;
    if(tilePos[7].row<0)
        tilePos[7].row = 0;
    tilePos[7].col = cPos.col;
}

void CTerrainSurface::GetTilePos(TILTPOS cPos, TILTPOS (&tilePos)[25])
{
    int maxRC = pow(2,cPos.level)-1;
    
    int rowPos = cPos.row;
    
    if(cPos.row<0)
        cPos.row = 0;
    if(cPos.row>maxRC)
        cPos.row = maxRC;
    
    //  0   7   6
    //  1   C   5
    //  2   3   4

// 	10	9	8	23	22
// 	11	0	7	6	21
// 	12	1	C	5	20
// 	13	2	3	4	19
// 	14	15	16	17	18

    for(int i=0; i<24; i++)
        tilePos[i].level = cPos.level;
    //C
    tilePos[24] = cPos;
    
    if(rowPos<0)
    {
        for(int i=0;i<maxRC;i++)
        {
            tilePos[i].col = i;
            tilePos[i].row = 0;
            tilePos[i+maxRC+1].col = i;
            tilePos[i+maxRC+1].row = 1;
            tilePos[i+2*(maxRC+1)].col = i;
            tilePos[i+2*(maxRC+1)].row = 2;
        }
    }
    else if(rowPos>maxRC)
    {
        for(int i=0;i<maxRC;i++)
        {
            tilePos[i].col = i;
            tilePos[i].row = maxRC;
            tilePos[i+maxRC+1].col = i;
            tilePos[i+maxRC+1].row = maxRC-1;
            tilePos[i+2*(maxRC+1)].col = i;
            tilePos[i+2*(maxRC+1)].row = maxRC-2;
        }
    }
    else
    {
        //0
        tilePos[0].row = cPos.row - 1;
        if(tilePos[0].row<0)
            tilePos[0].row = 0;
        tilePos[0].col = cPos.col - 1;
        if(tilePos[0].col<0)
            tilePos[0].col = maxRC;
        //1
        tilePos[1].row = cPos.row;
        tilePos[1].col = cPos.col - 1;
        if(tilePos[1].col<0)
            tilePos[1].col = maxRC;
        //2
        tilePos[2].row = cPos.row + 1;
        if(tilePos[2].row>maxRC)
            tilePos[2].row = maxRC;
        tilePos[2].col = cPos.col - 1;
        if(tilePos[2].col<0)
            tilePos[2].col = maxRC;
        //3
        tilePos[3].row = cPos.row + 1;
        if(tilePos[3].row>maxRC)
            tilePos[3].row = maxRC;
        tilePos[3].col = cPos.col;
        //4
        tilePos[4].row = cPos.row + 1;
        if(tilePos[4].row>maxRC)
            tilePos[4].row = maxRC;
        tilePos[4].col = cPos.col + 1;
        if(tilePos[4].col>maxRC)
            tilePos[4].col = 0;
        //5
        tilePos[5].row = cPos.row;
        tilePos[5].col = cPos.col + 1;
        if(tilePos[5].col>maxRC)
            tilePos[5].col = 0;
        //6
        tilePos[6].row = cPos.row - 1;
        if(tilePos[6].row<0)
            tilePos[6].row = 0;
        tilePos[6].col = cPos.col + 1;
        if(tilePos[6].col>maxRC)
            tilePos[6].col = 0;
        //7
        tilePos[7].row = cPos.row - 1;
        if (tilePos[7].row < 0)
            tilePos[7].row = 0;
        tilePos[7].col = cPos.col;

        //8
        tilePos[8].row = cPos.row - 2;
        if (tilePos[8].row < 0)
            tilePos[8].row = 0;
        tilePos[8].col = cPos.col;

        //9
        tilePos[9].row = cPos.row - 2;
        if (tilePos[9].row < 0)
            tilePos[9].row = 0;
        tilePos[9].col = cPos.col - 1;
        if (tilePos[9].col < 0)
            tilePos[9].col = maxRC;

        //10
        tilePos[10].row = cPos.row - 2;
        if (tilePos[10].row < 0)
            tilePos[10].row = 0;
        tilePos[10].col = cPos.col - 2;
        if (tilePos[10].col < 0)
            tilePos[10].col = maxRC;

        //11
        tilePos[11].row = cPos.row - 1;
        if (tilePos[11].row < 0)
            tilePos[11].row = 0;
        tilePos[11].col = cPos.col - 2;
        if (tilePos[11].col < 0)
            tilePos[11].col = maxRC;

        //12
        tilePos[12].row = cPos.row;
        tilePos[12].col = cPos.col - 2;
        if (tilePos[12].col < 0)
            tilePos[12].col = maxRC;

        //13
        tilePos[13].row = cPos.row + 1;
        if (tilePos[13].row > maxRC)
            tilePos[13].row = maxRC;
        tilePos[13].col = cPos.col - 2;
        if (tilePos[13].col < 0)
            tilePos[13].col = maxRC;

        //14
        tilePos[14].row = cPos.row + 2;
        if (tilePos[14].row > maxRC)
            tilePos[14].row = maxRC;
        tilePos[14].col = cPos.col - 2;
        if (tilePos[14].col < 0)
            tilePos[14].col = maxRC;

        //15
        tilePos[15].row = cPos.row + 2;
        if (tilePos[15].row > maxRC)
            tilePos[15].row = maxRC;
        tilePos[15].col = cPos.col - 1;
        if (tilePos[15].col < 0)
            tilePos[15].col = maxRC;

        //16
        tilePos[16].row = cPos.row + 2;
        if (tilePos[16].row > maxRC)
            tilePos[16].row = maxRC;
        tilePos[16].col = cPos.col;

        //17
        tilePos[17].row = cPos.row + 2;
        if (tilePos[17].row > maxRC)
            tilePos[17].row = maxRC;
        tilePos[17].col = cPos.col + 1;
        if (tilePos[17].col > maxRC)
            tilePos[17].col = 0;

        //18
        tilePos[18].row = cPos.row + 2;
        if (tilePos[18].row > maxRC)
            tilePos[18].row = maxRC;
        tilePos[18].col = cPos.col + 2;
        if (tilePos[18].col > maxRC)
            tilePos[18].col = 0;

        //19
        tilePos[19].row = cPos.row + 1;
        if (tilePos[19].row > maxRC)
            tilePos[19].row = maxRC;
        tilePos[19].col = cPos.col + 2;
        if (tilePos[19].col > maxRC)
            tilePos[19].col = 0;

        //20
        tilePos[20].row = cPos.row;
        tilePos[20].col = cPos.col + 2;
        if (tilePos[20].col > maxRC)
            tilePos[20].col = 0;

        //21
        tilePos[21].row = cPos.row - 1;
        if (tilePos[21].row < 0)
            tilePos[21].row = 0;
        tilePos[21].col = cPos.col + 2;
        if (tilePos[21].col > maxRC)
            tilePos[21].col = 0;

        //22
        tilePos[22].row = cPos.row - 2;
        if (tilePos[22].row < 0)
            tilePos[22].row = 0;
        tilePos[22].col = cPos.col + 2;
        if (tilePos[22].col > maxRC)
            tilePos[22].col = 0;

        //23
        tilePos[23].row = cPos.row - 2;
        if (tilePos[23].row < 0)
            tilePos[23].row = 0;
        tilePos[23].col = cPos.col + 1;
        if (tilePos[23].col > maxRC)
            tilePos[23].col = 0;
    }

}

void CTerrainSurface::genStippleTex()
{
	float arr[] = { 1, 0, 1, 0 };
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &stippleTex);
	glBindTexture(GL_TEXTURE_2D, stippleTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, 4, 1, 0, GL_RED, GL_FLOAT, arr);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CTerrainSurface::ComputeTile( double lat, double lon, int lev )
{
    TILTPOS cPos,tilePos[NUMS_TILE],pPos;
    cPos.level = lev+1;//计算下一级
    cPos.row = lat2tiley(lat, lev+1);
    cPos.col = long2tilex(lon, lev+1);

    GetTilePos(cPos,tilePos);
    tilePosList.clear();
    for(auto &tilePo : tilePos)
    {
        bool isFinded = false;
        for(auto &i : tilePosList)
        {
          if(tilePo==i)
          {
              isFinded = true;
              break;
          }
        }
        if(!isFinded)
            tilePosList.push_back(tilePo);
        
    }

    std::vector<TILTPOS> vPos;  //存放父节点
    for(auto &t : tilePosList)
    {
        pPos = TILTPOS(t.level-1,t.col / 2,t.row / 2);
        auto result = find( vPos.begin( ), vPos.end( ), pPos );
        if ( result == vPos.end( ) )
        {
            auto iter_p = totalTileMap.find(pPos);
            if(iter_p!=totalTileMap.end())
            {
                iter_p->second->ComputeChildren();
            }
            //
            vPos.push_back(pPos);

        }

    }


}

void CTerrainSurface::RenderQuadTile(CQuadTile* pTile,int maxlevel,double tanDis)
{
    if (pTile->Level<=maxlevel && pTile->bInUse)
    {
        //
        if(texType == 1)
        {
            if (pTile->haschildren
                    && pTile->NW
                    && pTile->NE
                    && pTile->SW
                    && pTile->SE
                    && pTile->NW->isInitialized
                    && pTile->NE->isInitialized
                    && pTile->SW->isInitialized
                    && pTile->SE->isInitialized
                    && pTile->NW->isSatTexLoaded
                    && pTile->NE->isSatTexLoaded
                    && pTile->SW->isSatTexLoaded
                    && pTile->SE->isSatTexLoaded

                    && pTile->NW->bInUse
                    && pTile->NE->bInUse
                    && pTile->SW->bInUse
                    && pTile->SE->bInUse

                    && pTile->Level<maxlevel)
            {
                RenderQuadTile(pTile->NW,maxlevel,tanDis);
                RenderQuadTile(pTile->NE,maxlevel,tanDis);
                RenderQuadTile(pTile->SW,maxlevel,tanDis);
                RenderQuadTile(pTile->SE,maxlevel,tanDis);

            }
            else
            {
                float offsetA = pTile->Getoffset(cameraPos);
                pTile->ComputeDistance(cviewPos,offsetA);

                if(pTile->distance<tanDis && m_Camera->_viewFrustum.Intersects(pTile->m_BBox))
                {
                    pTile->lives=0;
                    pTile->maxlevel = maxlevel;
                    m_pTileRenderList.push_back(pTile);

                }


            }


        }
        //
        if(texType == 3)
        {
            if (pTile->haschildren
                    && pTile->NW
                    && pTile->NE
                    && pTile->SW
                    && pTile->SE
                    && pTile->NW->isInitialized
                    && pTile->NE->isInitialized
                    && pTile->SW->isInitialized
                    && pTile->SE->isInitialized
                    && pTile->NW->isNorTexLoaded
                    && pTile->NE->isNorTexLoaded
                    && pTile->SW->isNorTexLoaded
                    && pTile->SE->isNorTexLoaded

                    && pTile->NW->bInUse
                    && pTile->NE->bInUse
                    && pTile->SW->bInUse
                    && pTile->SE->bInUse

                    && pTile->Level<maxlevel)
            {
                RenderQuadTile(pTile->NW,maxlevel,tanDis);
                RenderQuadTile(pTile->NE,maxlevel,tanDis);
                RenderQuadTile(pTile->SW,maxlevel,tanDis);
                RenderQuadTile(pTile->SE,maxlevel,tanDis);

            }
            else
            {
                float offsetA = pTile->Getoffset(cameraPos);
                pTile->ComputeDistance(cviewPos,offsetA);

                if(pTile->distance<tanDis && m_Camera->_viewFrustum.Intersects(pTile->m_BBox))
                {
                    pTile->lives=0;
                    pTile->maxlevel = maxlevel;
                    m_pTileRenderList.push_back(pTile);

                }


            }

        }

    }


}

void CTerrainSurface::ComputeShapeType(CQuadTile* pTile)
{
    pTile->shapeType = 0;

    for( auto &pT : m_pTileRenderList )
    {
        if (pT->Level == pTile->Level-1)
        {
            if (fabs(pTile->South - pT->North)<EPSILON6
                    && pTile->CenterLongitude>pT->West
                    && pTile->CenterLongitude<pT->East)
            {
                pTile->shapeType |= 1;	//top
                continue;
            }

            if (fabs(pTile->East - pT->West)<EPSILON6
                    && pTile->CenterLatitude>pT->South
                    && pTile->CenterLatitude<pT->North)
            {
                pTile->shapeType |= 2;	//right
                continue;
            }

            if (fabs(pTile->North - pT->South)<EPSILON6
                    && pTile->CenterLongitude>pT->West
                    && pTile->CenterLongitude<pT->East)
            {
                pTile->shapeType |= 4;	//bottom
                continue;
            }

            if (fabs(pTile->West - pT->East)<EPSILON6
                    && pTile->CenterLatitude>pT->South
                    && pTile->CenterLatitude<pT->North)
            {
                pTile->shapeType |= 8;	//left
                continue;
            }
        }

    }

}

void CTerrainSurface::PickingRayIntersect(
	int screenX,
	int screenY,
	glm::vec3& vec)
{
	glm::dvec3 v1;
	v1.x = (double)screenX;
	v1.y = (double)screenY;
	v1.z = (double)m_Camera->viewPort.MinDepth;
	v1 = m_Camera->Unproject(v1);

	glm::dvec3 v2;
	v2.x = (double)screenX;
	v2.y = (double)screenY;
	v2.z = (double)m_Camera->viewPort.MaxDepth;
	v2 = m_Camera->Unproject(v2);

	std::multimap<double, glm::vec3> vectIntersMap;
	typedef std::pair <double, glm::vec3>	TPPair;

	glm::dvec3 AB = v2 - v1;
	AB = glm::normalize(AB);

	glm::vec3 orig = glm::vec3(v1);
	glm::vec3 dir = glm::vec3(AB);

	if (is3D)
	{
		bool re = false;

		for (auto *pT : m_pTileRenderList)
		{
			if (pT->vertices != nullptr)
			{
				float offsetA = pT->Getoffset(m_Camera->_position);
				pT->ComputeDistance(m_Camera->viewPoint, offsetA);
				pT->ComputelocalOrigin(offsetA);

				float t, u, v;
				for (int y = 0; y < NUM_PTS_TILE - 1; ++y)
				{
					for (int x = 0; x < NUM_PTS_TILE - 1; ++x)
					{
						glm::vec3 p0 = glm::vec3(pT->vertices[((y *NUM_PTS_TILE) + x) * 3],
							pT->vertices[((y *NUM_PTS_TILE) + x) * 3 + 1],
							pT->vertices[((y *NUM_PTS_TILE) + x) * 3 + 2]);
						glm::vec3 p1 = glm::vec3(pT->vertices[((y *NUM_PTS_TILE) + x + 1) * 3],
							pT->vertices[((y *NUM_PTS_TILE) + x + 1) * 3 + 1],
							pT->vertices[((y *NUM_PTS_TILE) + x + 1) * 3 + 2]);
						glm::vec3 p2 = glm::vec3(pT->vertices[(((y + 1) *NUM_PTS_TILE) + x) * 3],
							pT->vertices[(((y + 1) *NUM_PTS_TILE) + x) * 3 + 1],
							pT->vertices[(((y + 1) *NUM_PTS_TILE) + x) * 3 + 2]);
						glm::vec3 p3 = glm::vec3(pT->vertices[(((y + 1) *NUM_PTS_TILE) + x + 1) * 3],
							pT->vertices[(((y + 1) *NUM_PTS_TILE) + x + 1) * 3 + 1],
							pT->vertices[(((y + 1) *NUM_PTS_TILE) + x + 1) * 3 + 2]);

						re = IntersectTriangle(orig, dir, p0, p2, p1, t, u, v);
						if (re)
						{
							vec = orig + dir * t;
							vectIntersMap.insert(TPPair(t, vec));
						}

						re = IntersectTriangle(orig, dir, p1, p2, p3, t, u, v);
						if (re)
						{
							vec = orig + dir * t;
							vectIntersMap.insert(TPPair(t, vec));
						}

					}
				}

				if (!vectIntersMap.empty())
				{
					vec = vectIntersMap.begin()->second;
					m_Camera->rayIntersected = true;
					vectIntersMap.clear();
					return;
				}
			}
		}
		vec = glm::vec3(0, 0, 0);
		return;

	}
	else
	{
		bool re = false;

		for (auto *pT : m_pTileRenderList)
		{
			if (pT->vertices2D != nullptr)
			{
				float offsetA = pT->Getoffset(m_Camera->_position);
				pT->ComputeDistance(m_Camera->viewPoint, offsetA);
				pT->ComputelocalOrigin(offsetA);

				float t, u, v;
				for (int y = 0; y < NUM_PTS_TILE - 1; ++y)
				{
					for (int x = 0; x < NUM_PTS_TILE - 1; ++x)
					{
						glm::vec3 p0 = glm::vec3(pT->vertices2D[((y *NUM_PTS_TILE) + x) * 3],
							pT->vertices2D[((y *NUM_PTS_TILE) + x) * 3 + 1],
							pT->vertices2D[((y *NUM_PTS_TILE) + x) * 3 + 2]);
						glm::vec3 p1 = glm::vec3(pT->vertices2D[((y *NUM_PTS_TILE) + x + 1) * 3],
							pT->vertices2D[((y *NUM_PTS_TILE) + x + 1) * 3 + 1],
							pT->vertices2D[((y *NUM_PTS_TILE) + x + 1) * 3 + 2]);
						glm::vec3 p2 = glm::vec3(pT->vertices2D[(((y + 1) *NUM_PTS_TILE) + x) * 3],
							pT->vertices2D[(((y + 1) *NUM_PTS_TILE) + x) * 3 + 1],
							pT->vertices2D[(((y + 1) *NUM_PTS_TILE) + x) * 3 + 2]);
						glm::vec3 p3 = glm::vec3(pT->vertices2D[(((y + 1) *NUM_PTS_TILE) + x + 1) * 3],
							pT->vertices2D[(((y + 1) *NUM_PTS_TILE) + x + 1) * 3 + 1],
							pT->vertices2D[(((y + 1) *NUM_PTS_TILE) + x + 1) * 3 + 2]);

						re = IntersectTriangle(orig, dir, p0, p2, p1, t, u, v);
						if (re)
						{
							vec = orig + dir * t;
							vectIntersMap.insert(TPPair(t, vec));
						}

						re = IntersectTriangle(orig, dir, p1, p2, p3, t, u, v);
						if (re)
						{
							vec = orig + dir * t;
							vectIntersMap.insert(TPPair(t, vec));
						}

					}
				}

				if (!vectIntersMap.empty())
				{
					vec = vectIntersMap.begin()->second;
					m_Camera->rayIntersected = true;
					vectIntersMap.clear();
					return;
				}
			}
		}
		vec = glm::vec3(0, 0, 0);
		return;
	}

}


void CTerrainSurface::PickingViewIntersect(
	double lat,
	double lon,
	glm::vec3& vec)
{
	std::multimap<double, glm::vec3> vectIntersMap;
	typedef std::pair <double, glm::vec3>	TPPair;

	glm::vec3 orig = SphericalToCartesian(lat, lon, (double)EARTH_MEANRADIUS + 20000.0);
	glm::vec3 dir;
	dir = glm::normalize(orig);
	dir = -dir;

	std::map<TILTPOS, CQuadTile*> ::iterator iter;

	for (int lev = maxLevel; lev > LEV_TILE_PICK_PT; lev--)
	{
		int row = lat2tiley(lat*180.0 / ((double)PI), lev);
		int col = long2tilex(lon*180.0 / ((double)PI), lev);

		TILTPOS key_pos(lev, col, row);

		bool re = false;

		iter = totalTileMap.find(key_pos);
		//如在缓冲队列里
		if (iter != totalTileMap.end())
		{
			auto *pT = iter->second;

			if (pT->bInUse && pT->isInitialized && pT->vertices != nullptr)
			{
				float offsetA = pT->Getoffset(m_Camera->_position);
				pT->ComputeDistance(m_Camera->viewPoint, offsetA);
				pT->ComputelocalOrigin(offsetA);

				float t, u, v;
				for (int y = 0; y < NUM_PTS_TILE - 1; ++y)
				{
					for (int x = 0; x < NUM_PTS_TILE - 1; ++x)
					{
						glm::vec3 p0 = glm::vec3(pT->vertices[((y *NUM_PTS_TILE) + x) * 3],
							pT->vertices[((y *NUM_PTS_TILE) + x) * 3 + 1],
							pT->vertices[((y *NUM_PTS_TILE) + x) * 3 + 2]);
						glm::vec3 p1 = glm::vec3(pT->vertices[((y *NUM_PTS_TILE) + x + 1) * 3],
							pT->vertices[((y *NUM_PTS_TILE) + x + 1) * 3 + 1],
							pT->vertices[((y *NUM_PTS_TILE) + x + 1) * 3 + 2]);
						glm::vec3 p2 = glm::vec3(pT->vertices[(((y + 1) *NUM_PTS_TILE) + x) * 3],
							pT->vertices[(((y + 1) *NUM_PTS_TILE) + x) * 3 + 1],
							pT->vertices[(((y + 1) *NUM_PTS_TILE) + x) * 3 + 2]);
						glm::vec3 p3 = glm::vec3(pT->vertices[(((y + 1) *NUM_PTS_TILE) + x + 1) * 3],
							pT->vertices[(((y + 1) *NUM_PTS_TILE) + x + 1) * 3 + 1],
							pT->vertices[(((y + 1) *NUM_PTS_TILE) + x + 1) * 3 + 2]);

						re = IntersectTriangle(orig, dir, p0, p2, p1, t, u, v);
						if (re)
						{
							vec = orig + dir * t;
							vectIntersMap.insert(TPPair(t, vec));
						}

						re = IntersectTriangle(orig, dir, p1, p2, p3, t, u, v);
						if (re)
						{
							vec = orig + dir * t;
							vectIntersMap.insert(TPPair(t, vec));
						}

					}
				}

				if (!vectIntersMap.empty())
				{
					vec = vectIntersMap.begin()->second;
					m_Camera->rayIntersected = true;
					vectIntersMap.clear();
					return;
				}


			}


		}


	}

	vec = SphericalToCartesian(lat, lon, (double)EARTH_MEANRADIUS);

}

//利用摄像机经纬度、地球半径解算球面交点
void CTerrainSurface::GetPosIntersectSphere(glm::dvec3& posInters)
{
	glm::vec3 vec;
	PickingViewIntersect(m_Camera->_latitude, m_Camera->_longitude, vec);
	posInters = glm::dvec3(vec.x, vec.y, vec.z);

}

#ifdef __linux
void* CTerrainSurface::FileIOThreadProc()
#else
void CTerrainSurface::FileIOThreadProc()
#endif
{
	m_bIOThreadDone = false;

	while (!m_bDone)
	{
		// Wait for a read or create request
#ifdef __linux__
		sem_wait(&sem_io);
#else
		WaitForSingleObject(sem_io, INFINITE);
#endif
		if (m_bDone)
			break;

		m_NumIORequests++;

#ifdef __linux__
		int ret = pthread_mutex_lock(&mutex_io);
		if (ret == 0)
#else
		if (TryEnterCriticalSection(&mutex_io))
#endif
		{
			//
			if (!ioQueueList.empty())
			{
				if (totalTileMap.size() > MAXTILE_BUFSIZE)
				{
					int count = 0;
					for (auto iter_io = totalTileMap.begin(); iter_io != totalTileMap.end();)
					{
						//单次最大删除个数
						if (count == 5)
							break;
						if (!iter_io->second->bInUse&&iter_io->second->lives > 200)//0 1 2的先保留
						{
							//
							SAFE_DELETE(iter_io->second);
							iter_io = totalTileMap.erase(iter_io);
							g_globalScalar.tiltReleaseNum++;
							count++;
						}
						else
						{
							iter_io++;
						}
					}
				}
				//
				g_tileIOM.clear();
				using TPPair = std::pair <double, TILTPOS>;
				for (auto &pTilt : ioQueueList)
				{
					//                    if(pTilt.second && pTilt.first.level==maxLevel)
					{
						//重新计算每个tile的距离
						float offsetA = pTilt.second->Getoffset(cameraPos);
						pTilt.second->ComputeDistance(cviewPos, offsetA);
						g_tileIOM.insert(TPPair(pTilt.second->distance, pTilt.first));
					}
				}
				//从文件队列里取出需要加载的tile，对于不再使用的tile从队列里删除
				for (auto iter = g_tileIOM.begin(); iter != g_tileIOM.end();)
				{
					auto iter_io = ioQueueList.find(iter->second);
					if (iter_io != ioQueueList.end())
					{
						auto *pTile = iter_io->second;
						ioQueueList.erase(iter_io);
#ifdef __linux__
						pthread_mutex_unlock(&mutex_io);
#else
						LeaveCriticalSection(&mutex_io);
#endif
						if (pTile->bInUse)
						{
							//LoadTexture加载纹理数据
							pTile->LoadTexture();
							break;

						}
					}
					iter = g_tileIOM.erase(iter);

				}


			}
			if (!ioQueueList.empty())
			{
#ifdef __linux__
				sem_post(&sem_io);
#else
				ReleaseSemaphore(sem_io, 1, NULL);
#endif
			}
			else
			{
				globalBool.semaphore_signal = true;
			}

		}

	}
	m_bIOThreadDone = true;

    return ;
}

GLuint CTerrainSurface::GenTextureSD(unsigned char *data, bool haveAlpha)
{
	//
	GLuint    textureId = 0;

	// 加载并生成纹理
	int width = 256;
	int height = 256;

	if (data != nullptr)
	{
		f_->glGenTextures(1, &textureId);
		f_->glBindTexture(GL_TEXTURE_2D, textureId);


		f_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		f_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		f_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		f_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 2);
		// 		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);

		if (haveAlpha)
			f_->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			f_->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		f_->glBindTexture(GL_TEXTURE_2D, 0);

		// 		glGenerateMipmap(GL_TEXTURE_2D);

		//        glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, -1.5);

		//        stbi_image_free(data);
		data = nullptr;
		// 		SAFE_DELETE_ARRAY(data);

	}
	else {
		printf("terrainSurfacedata = NULL");
	}

	return  textureId;
}

void CTerrainSurface::LoadTileTexData()
{
	for (auto pTilt : totalTileMap)
	{
		auto *pT = pTilt.second;
		assert(pT);
		pT->lives++;
		if (pT)
		{
			//卫星
			if (texType == 1)
			{
				/**
				* tile正在使用
				* tile被初始化
				* sat数据已加载
				* sat纹理未生成
				*/
				if (pT->bInUse && pT->isInitialized && pT->isSatLoaded && !pT->isSatTexLoaded)
				{
					assert(pT->data_sat);

					pT->m_texturePic = GenTextureSD(pT->data_sat, false);
					SAFE_DELETE(pT->data_sat);

					pT->isSatTexLoaded = true;
				}

			}
			//标准
			if (texType == 2)
			{

			}
			//地势
			if (texType == 3)
			{
				/**
				* tile正在使用
				* tile被初始化
				* nor数据已加载
				* nor纹理未生成
				*/
				if (pT->bInUse && pT->isInitialized && pT->isNorLoaded && !pT->isNorTexLoaded)
				{
					// Use tightly packed data
					f_->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

					assert(pT->g_TextureRectDataNor);

					// Generate a texture object
					// 创建纹理
					f_->glGenTextures(1, &pT->m_texture);
					f_->glBindTexture(GL_TEXTURE_2D, pT->m_texture);
					// 创建 Nearest 滤波贴图
					f_->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 257, 257, 0, GL_RGB, GL_HALF_FLOAT, pT->g_TextureRectDataNor);

					// Set the filtering mode
					f_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					f_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					f_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					f_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					f_->glBindTexture(GL_TEXTURE_2D, 0);

					SAFE_DELETE_ARRAY(pT->g_TextureRectDataNor);

					pT->isNorTexLoaded = true;
				}

			}

		}
	}


}


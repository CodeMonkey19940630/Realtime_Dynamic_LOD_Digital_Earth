//
// uniHead.h
//

#pragma once

#include "CustomMath.h"

#include <QOpenGLWidget>
//#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>

#include <stdlib.h>
#include <stdio.h>

#include <cstring>

#include <fstream>
#include <iostream>

#include <map>
#include <list>
#include <string>
#include <vector>
#include "GrowableArray.h"
//#include "D3DColor.h"
#include <algorithm>

#include <QPainterPath>
#include <QPainter>
#include <QFont>
#include <QMouseEvent>
#include <QColorDialog>
#include "QTreeWidgetItem"

//#include <QNetworkAccessManager>
//#include <QNetworkReply>
//#include <QNetworkRequest>
#include <QUrl>
#include <QObject>
#include <QFile>
#include <QDir>
#include <QString>
#include <QIODevice>
#include <QTimer>
#include "qeventloop.h"
#include <QProcess>
#include "QTextCodec"
#include "QLabel"
#include "QRadioButton"
#include <QSemaphore>
#include <QThread>
#include <QMutex>
#include <memory>

//#include "Utilities/OpenGLVersion.h"
#define F(c) c *f = (GlobalContext::contextFunc)

using namespace std;

#ifdef __unix
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),  (mode)))==NULL
#endif

typedef signed long     int32;
typedef signed short    int16;
typedef signed char     int8;
typedef unsigned long   uint32;
typedef unsigned short  uint16;
typedef unsigned char   uint8;
typedef unsigned char   byte;

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=nullptr; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=nullptr; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=nullptr; } }
#endif

//#ifndef max
//#define max(a,b)            (((a) > (b)) ? (a) : (b))
//#endif

//#ifndef min
//#define min(a,b)            (((a) < (b)) ? (a) : (b))
//#endif

#define V_RETURN(x) { hr = (x); if( FAILED(hr) ) { return hr; } }

//字体设置
#define TEX_FONT_S 2048	//字符纹理尺寸
#define FONT_LEN_MAX 30	//字符最大数量
#define NUMF_P_ROW 25	//每行最多字符数
#define FONT_SIZE 20	//字体默认大小,32

#define NUMS_TILE 25
#define NUMS_THREAD_IO 1

#define LEV_TILE_PICK_PT 9//6
//UI 图片大小
#define UI_PIC_SIZE	40

//地球地块结构
struct SectionDesc
{
    //一张图片+ 四等分
    //00 01
    //10 11
    //level 当前等分次数 等级
    int        col;
    int        row;
    int        level;
    int        maxLevel;
    //新增尝试
    float    u;
    float    v;
    float    range;
    int        isInside;
    //////////////////////////////////////////////////////////////////////////
    float    LLU;
    float    LLV;
    float    LLRange;
    float    LLAlpha;
};

struct SectionDescLoc
{
    GLint		colLoc;
    GLint		rowLoc;
    GLint		levelLoc;
    GLint		maxLevelLoc;
    //新增尝试
    GLint	uLoc;
    GLint	vLoc;
    GLint	rangeLoc;
    GLint	    isInsideLoc;
    //////////////////////////////////////////////////////////////////////////
    GLint	LLULoc;
    GLint	LLVLoc;
    GLint	LLRangeLoc;
    GLint	LLAlphaLoc;
};

///////////////////////////////////////////////////////////////////////
//飞行器参数
struct AIRCRAFT_PARAM
{
    glm::dmat4 worldMatrix;
    glm::dvec3 pos;

    double yaw;
    double pitch;
    double roll;
};

//球面三角形
struct SPHERETRIANGLE
{
	int index;
	double angle;
	glm::vec3 vtype;

};

//纹理覆盖
struct VertexTypeHatch
{
	glm::vec3 position;
	glm::vec2 texture;
	glm::vec3 normal;
};

//地球地块基础位置，可以作为map结构的key使用
struct TILTPOS
{
    int		level;
    int		col;
    int		row;
    TILTPOS()
    {
        level=0;
        col=0;
        row=0;
    }
    TILTPOS(int levels,int cols,int rows)
    {
        level=levels;
        col=cols;
        row=rows;
    }
    bool operator<(const TILTPOS tp) const
    {
        if(level<tp.level)
            return true;
        else if(level==tp.level)
        {
            if(col<tp.col)
                return true;
            else if(col==tp.col)
            {
                if(row<tp.row)
                    return true;
                else
                    return false;
            }else
                return false;
        }else
            return false;
    }

    bool operator == (const TILTPOS tp) const
    {
        if(row==tp.row && col==tp.col && level==tp.level)
            return true;
        else
            return false;
    }
    bool operator != (const TILTPOS tp) const
    {
        if(row!=tp.row || col!=tp.col || level!=tp.level)
            return true;
        else
            return false;
    }
};
//
//geo type
enum GEO_TYPE
{
    POINT_COUNTRY,//国家
    POINT_CAPITAL,//首都
    POINT_PROVINCE,//省
    POINT_PCAPITAL,//省会
    POINT_CITY,//市
    POINT_COUNTY,//县
    LINE_COUNTRY,//国界线
    LINE_CN,//中国国界线
    LINE_CN_DASH,//中国国界线虚线
    LINE_PROVINCE,//省界
    LINE_CITY,//市界
    LINE_COUNTY,//县界
    LINE_ALERT_48,//48小时警戒线
    LINE_ALERT_24,//24小时警戒线
    GEO_INVALID//无效
};

struct Point2F
{
    Point2F()
    {
        lat = 0.;
        lon = 0.;
        uTex = 0;
    }
    Point2F(float latIn, float lonIn)
    {
        lon = lonIn;
        lat = latIn;
    }
    float  lat;
    float  lon;
    float uTex;
};

//
struct V2Point
{
    V2Point()
    {
        len = 0;
        geoType = GEO_INVALID;
    }
    GEO_TYPE    geoType;
    int len;
    std::string	name;
    Point2F	pos;
};
//
struct V2Line
{
    V2Line()
    {
        geoType = GEO_INVALID;
        num = 0;
    }
    GEO_TYPE    geoType;
    int         num;
    std::vector<Point2F> pts;
};
// multi point
struct V2MultiPoint
{
    V2MultiPoint()
    {
        num = 0;
    }
    int num;                    //number of lines
    std::vector<V2Point> pts;
};
// multiline
struct V2MultiLine
{
    V2MultiLine()
    {
        num = 0;
    }
    int num;                    //number of lines
    std::vector<V2Line> lines;
};

/**
 * 数据下载通用存储结构
 */
struct MemoryStruct
{
    char *memory;
    size_t size;
    MemoryStruct()
    {
        memory = (char *)malloc(1);
        size = 0;
    }
    ~MemoryStruct()
    {
        free(memory);
        memory = nullptr;
        size = 0;
    }
};

struct GlobalScalar
{
    GlobalScalar(void)
    {
        MaxLevelPerFrame=5;
        ColorProjectIndex=0;
        tiltLoadedNum=0;
        tiltReleaseNum=0;
    }
    int MaxLevelPerFrame;
    int ColorProjectIndex;
    int tiltLoadedNum;
    int tiltReleaseNum;
};

struct HEIGHTCOLOR
{
    HEIGHTCOLOR(void)
    {
        h=0.0;
        r=0.0;
        g=0.0;
        b=0.0;
    }
    HEIGHTCOLOR(float hv, int val)
    {
        h = hv;
        hex = val;
        r = (0xff << 16 & val) >> 16;
        g = (0xff << 8 & val) >> 8;
        b = 0xff & val;

        r /= 255.0;
        g /= 255.0;
        b /= 255.0;
    }
    HEIGHTCOLOR(float hv, float rv, float gv, float bv)
    {
        h = hv;
        r = rv;
        g = gv;
        b = bv;

        int rd = r*255;
        int gd = g*255;
        int bd = b*255;

        hex = rd << 16 | gd << 8 | bd;
    }
    void toHex()
    {
        int rd = r*255;
        int gd = g*255;
        int bd = b*255;

        hex = rd << 16 | gd << 8 | bd;

    }
    void toRGB()
    {
        r = (0xff << 16 & hex) >> 16;
        g = (0xff << 8 & hex) >> 8;
        b = 0xff & hex;

        r /= 255.0;
        g /= 255.0;
        b /= 255.0;
    }
    int hex;
    float h;
    float r;
    float g;
    float b;
   
};

//////////////////////////////////////
//
#ifdef __linux__
typedef struct _TRIVERTEX
{
    long    x;
    long    y;
    unsigned short  Red;
    unsigned short Green;
    unsigned short Blue;
    unsigned short Alpha;
}TRIVERTEX,*PTRIVERTEX,*LPTRIVERTEX;

typedef struct _GRADIENT_RECT
{
    unsigned long UpperLeft;
    unsigned long LowerRight;
}GRADIENT_RECT,*PGRADIENT_RECT,*LPGRADIENT_RECT;
#endif

//目前只考虑了固定的12组高度-颜色值
const int	HCNUM = 12;
//覆盖调色板
const int	COLOR_NUM_PALETTE = 6;
//目前只考虑了10套高度-颜色方案
struct COLORLISTDATA
{
    HEIGHTCOLOR		data[HCNUM][10];
    TRIVERTEX       vert[2*HCNUM][10] ;
    GRADIENT_RECT   rect[HCNUM-1][10];
};
//高度-颜色方案，用户实时改变地球颜色
struct COLORTOSHADER
{
    glm::vec4 hcolor0[12];
    glm::vec4 hcolor1[12];
};

#ifdef __linux__
typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;
#endif
///////////////////////////////////////////////////////////////////

// TEMPLATE STRUCT greater
template<class _Ty = void>
struct greater
        : public binary_function<_Ty, _Ty, bool>
{	// functor for operator>
    bool operator()(const _Ty& _Left, const _Ty& _Right) const
    {	// apply operator> to operands
        return (_Left > _Right);
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//
typedef unsigned short VARTYPE;
typedef long long LONGLONG;
typedef short SHORT;
typedef long LONG;
typedef double DOUBLE;
typedef short VARIANT_BOOL;
typedef float FLOAT;

#define _VARIANT_BOOL    /##/

#ifdef __linux__
struct tagVARIANT
{
    union {
        LONGLONG llVal;
        LONG lVal;
        BYTE bVal;
        SHORT iVal;
        FLOAT fltVal;
        DOUBLE dblVal;
        VARIANT_BOOL boolVal;
    };
};
#endif

typedef /* [wire_marshal] */ struct tagVARIANT VARIANT;

#ifdef __linux__
enum VARENUM
{
    VT_EMPTY	= 0,
    VT_NULL	= 1,
    VT_I2	= 2,
    VT_I4	= 3,
    VT_R4	= 4,
    VT_R8	= 5,
    VT_CY	= 6,
    VT_DATE	= 7,
    VT_BSTR	= 8,
    VT_DISPATCH	= 9,
    VT_ERROR	= 10,
    VT_BOOL	= 11,
    VT_VARIANT	= 12,
    VT_UNKNOWN	= 13,
    VT_DECIMAL	= 14,
    VT_I1	= 16,
    VT_UI1	= 17,
    VT_UI2	= 18,
    VT_UI4	= 19,
    VT_I8	= 20,
    VT_UI8	= 21,
    VT_INT	= 22,
    VT_UINT	= 23,
    VT_VOID	= 24,
    VT_HRESULT	= 25,
    VT_PTR	= 26,
    VT_SAFEARRAY	= 27,
    VT_CARRAY	= 28,
    VT_USERDEFINED	= 29,
    VT_LPSTR	= 30,
    VT_LPWSTR	= 31,
    VT_RECORD	= 36,
    VT_INT_PTR	= 37,
    VT_UINT_PTR	= 38,
    VT_FILETIME	= 64,
    VT_BLOB	= 65,
    VT_STREAM	= 66,
    VT_STORAGE	= 67,
    VT_STREAMED_OBJECT	= 68,
    VT_STORED_OBJECT	= 69,
    VT_BLOB_OBJECT	= 70,
    VT_CF	= 71,
    VT_CLSID	= 72,
    VT_VERSIONED_STREAM	= 73,
    VT_BSTR_BLOB	= 0xfff,
    VT_VECTOR	= 0x1000,
    VT_ARRAY	= 0x2000,
    VT_BYREF	= 0x4000,
    VT_RESERVED	= 0x8000,
    VT_ILLEGAL	= 0xffff,
    VT_ILLEGALMASKED	= 0xfff,
    VT_TYPEMASK	= 0xfff
} ;
#endif

typedef signed long long INT64;

#ifdef __linux__
typedef struct tagRECT
{
    long    left;
    long    top;
    long    right;
    long    bottom;
} RECT, *PRECT;
#endif

typedef struct tagRECTPIC
{
    int type;
    GLuint textureId;
    RECT rc;
} RECT_PIC;

typedef struct tagBITPIC// 定义位图
{
    BYTE r;
    BYTE g;
    BYTE b;
} BITPIC;

typedef union tagRGB565
{
    unsigned short R:5;
    unsigned short G:6;
    unsigned short B:5;
}RGB565;

//标识结构
struct LabelDesc
{
    float  lat;
    float  lon;
    float  height;
    float  yaw;
    float  pitch;
    float  roll;
};

//标签信息
typedef float   TextVertex[4];	//字体顶点、纹理坐标
struct LabelInfo
{
	LabelInfo()
	{
        id = -1;
		textSize = 0;
        keyPos.level = -1;
        keyPos.row = -1;
        keyPos.col = -1;
        visible = 0;
	}
    LabelDesc lDesc;
    int id;
    int type;	//标签类型，0-中心点在图片中央；1-中心点在图片底部中间；2-中心点在图片左侧
	int texType;	//iconCollection的图标序号/标注类别:国家0、首都1等
    GLuint textureId;
    std::string name;
    std::string province;
    short alive;
    glm::dvec3 pos;	//可存放Camera状态
	int indexItem;	//QTreeWidgetItem 的索引
	int visible;	//0-隐藏；1-可见

	TextVertex  vert[FONT_LEN_MAX*4];	//每个字需要4个顶点
	int textSize;

    TILTPOS keyPos; //分块渲染
};

struct IconButtonStruct
{
	QPushButton* pBtn;
	QString iconPath;
	GLuint textureId;
};

////////////////////////////////////////////////////////////////////////////////
// 全局布尔变量结构
struct GlobalBool
{
    GlobalBool(void)
    {
        isMapSat = false;
        isMapVec = false;
        isMapTer = true;
        isMapGgl = false;
        isMapNone = false;
        isTerrUpdate = true;
        semaphore_signal = true;
    }

    void SetFalse()
    {
        isMapSat = false;
        isMapVec = false;
        isMapTer = false;
        isMapGgl = false;
        isMapNone = false;
    }


    bool isMapSat;	//卫星影像
    bool isMapVec;	//矢量地图
    bool isMapTer;	//自定义
    bool isMapGgl;	//谷歌地图
    bool isMapNone;	//无
    bool isTerrUpdate;//地块渲染更新标志
    bool semaphore_signal;

};

typedef struct VIEWPORT
{
	int TopLeftX;
	int TopLeftY;
	unsigned int Width;
	unsigned int Height;
	float MinDepth;
	float MaxDepth;
} 	VIEWPORT;

enum State { Waiting, Downloading, Finished, Stoped, Error };

typedef enum
{
    stop,
    up_dir,    //字符串类型
    down_dir,   //整型
    left_dir,    //浮点型
    right_dir    //未知类型

} STATE_CAMERA_MOVE;

///////////////////////////////////////////////////////////////////
//

GlobalScalar g_globalScalar;

int texType;
int label_mark;
int isSunLightOpen;
GlobalBool globalBool;
std::vector<LabelInfo> labelInfos;
std::vector<LabelInfo> iconInfos;
glm::vec3 _vLightDirection;
std::string g_worldDataPath;
std::string g_res_FilePath;

double e_radius; //pick point earth radius

int maxLevel;   //当前可以加载的最大层级

QPoint point_move;//左右键按下，鼠标当前屏幕坐标，按下时的坐标为mouseCurPos
glm::vec3 vect_pick;
double elevation_pick;






#include "QuadTile.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

int step_dem = sizeof(float)*NUM_PTS*NUM_PTS;

double e_radius = EARTH_MEANRADIUS;	//鼠标拾取点的球面半径，初始值为地球半径

extern map<TILTPOS, CQuadTile*>	ioQueueList;
extern std::map<TILTPOS, CQuadTile*> totalTileMap;
extern std::vector<TILTPOS> curTileVector;

////////////////////////////////////////////////////////////////////////////////////////////////////
//
/**
 * 从工程中读取数据，生成texture
 * @param fileName 文件名
 * @return GLuint
 */
GLuint LoadTexturePic ( const char *fileName )
{
    GLuint    textureId = 0;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // 设置纹理过滤参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 为当前绑定纹理对象设置
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //
    int width, height, nrChannels;

    unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, STBI_rgb_alpha);
	if (data)
    {
        std::string fName = fileName;
        fName = fName.substr(fName.length() - 3,3);
        if(strcmp(fName.c_str(), "jpg") == 0)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);

    }
    else
    {
        textureId = 0;
    }


	return  textureId;
}
/**
 * 从图片文件生成纹理
 * @param data 图片数据rgb/rgba
 * @param haveAlpha 是否有alpha通道
 * @return 返回纹理资源
 */
GLuint LoadTextureFromData(unsigned char *data, bool haveAlpha){
    GLuint    textureId = 0;

    // 加载并生成纹理
    int width=256;
    int height=256;

    if (data!=nullptr)
    {
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 2);
//         glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);

		if(haveAlpha)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);

//         glGenerateMipmap(GL_TEXTURE_2D);

//        glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, -1.5);

//        stbi_image_free(data);
//        data = NULL;
//         SAFE_DELETE_ARRAY(data);

    }
    return  textureId;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CQuadTile::CQuadTile()
{
    m_texture = 0;
    m_texturePic = 0;

    haschildren = false;
    isInitialized = false;
    bInUse = false;

    isDemLoaded = false;
    isNorLoaded = false;
    isSatLoaded = false;

    isNorTexLoaded = false;
    isSatTexLoaded = false;

    parent = nullptr;

	NW = nullptr;
	SW = nullptr;
	NE = nullptr;
	SE = nullptr;

	keyPos=TILTPOS(0,0,0);
	lives = 0;

    maxHeight = 8000.0;
    minHeight = -8000.0;

	vVertices = nullptr;
	vVertices2D = nullptr;

	g_TextureRectDataNor = nullptr;
	g_TextureRectDataHeight = nullptr;

	vertices = nullptr;
	vertices2D = nullptr;
	data_sat = nullptr;


}

CQuadTile::CQuadTile( TILTPOS kPos )
{
    bInUse = true;

    m_texture = 0;
    m_texturePic = 0;

	haschildren = false;
	isInitialized = false;

    isDemLoaded = false;
    isNorLoaded = false;
    isSatLoaded = false;

    isNorTexLoaded = false;
    isSatTexLoaded = false;

	parent = nullptr;

	NW = nullptr;
	SW = nullptr;
	NE = nullptr;
	SE = nullptr;

	keyPos = kPos;

	Level = kPos.level;
	Row = kPos.row;
	Col = kPos.col;

    maxHeight = 8000.0;
    minHeight = -8000.0;

	lives = 0;

	gridIndex = 0;
	shapeType = 0;

	//
	tileSize = 2.0 * EARTH_MEANRADIUS * ((double)PI) / pow(2.0, Level);//单位：米
	North = tiley2lat(Row, Level)*((double)PI) / 180.0;  //单位：弧度
	West = tilex2long(Col, Level)*((double)PI) / 180.0;

	double nY = LatToY(North);
	double sY = nY - tileSize;
	South = YToLat(sY);

	double wX = LonToX(West);
	double eX = wX + tileSize;
	East = XToLon(eX);

    CenterLatitude = 0.5F * (North + South);
    CenterLongitude = 0.5F * (West + East);
	LatitudeSpan = abs(North - South);
	LongitudeSpan = abs(East - West);

	m_BBox = CBoundingBox(South, North, West, East, EARTH_MEANRADIUS, EARTH_MEANRADIUS);

	//NW
	cornersPos[0].x = North;
	cornersPos[0].y = West;
	//SW
	cornersPos[1].x = South;
	cornersPos[1].y = West;
	//NE
	cornersPos[2].x = North;
	cornersPos[2].y = East;
	//SE
	cornersPos[3].x = North;
	cornersPos[3].y = South;

    ComputelocalOrigin(0.0);

    vVertices = nullptr;
    vVertices2D = nullptr;
	
	data_sat = nullptr;
	
	//渲染用
    g_TextureRectDataHeight = new float[NUM_PTS*NUM_PTS];
    g_TextureRectDataNor = new WORD[198147];//257*257*3

    isInitialized = true;
    
}

CQuadTile::~CQuadTile()
{
 	haschildren = false;
 	isInitialized = false;
 	bInUse = false;
    isDemLoaded = false;
    isNorLoaded = false;
    isSatLoaded = false;
    //
    isNorTexLoaded = false;
    isSatTexLoaded = false;
    isNorHaveLoaded = false;
    isSatHaveLoaded = false;

	if (parent)
	{
		if (parent->NW)
			if (parent->NW == this)
				parent->NW = nullptr;
		if (parent->NE)
			if (parent->NE == this)
				parent->NE = nullptr;
		if (parent->SW)
			if (parent->SW == this)
				parent->SW = nullptr;
		if (parent->SE)
			if (parent->SE == this)
				parent->SE = nullptr;

		parent = nullptr;
	
	}
	if (NW)
		NW->parent = nullptr;
	if (NE)
		NE->parent = nullptr;
	if (SW)
		SW->parent = nullptr;
	if (SE)
		SE->parent = nullptr;

	SAFE_DELETE_ARRAY(vVertices);
	SAFE_DELETE_ARRAY(vVertices2D);

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(vertices2D);

	SAFE_DELETE_ARRAY(g_TextureRectDataNor);
	SAFE_DELETE_ARRAY(g_TextureRectDataHeight);

	glDeleteTextures(1, &m_texture);
	glDeleteTextures(1, &m_texturePic);	

}

CQuadTile* CQuadTile::ComputeChild( int row, int col, int level )
{
	TILTPOS pos(level,col,row);

	auto iter = totalTileMap.find(pos);
	//如在缓冲队列里
    if(iter!=totalTileMap.end())
	{
		iter->second->bInUse = true;
		iter->second->lives = 0;
		iter->second->parent = this;
		return iter->second;
	}
	else
	{
        auto child = new CQuadTile(pos);
		assert(child);
		if (child != nullptr)
		{
			child->bInUse = true;
			child->lives = 0;
			child->parent = this;
			//
			totalTileMap.insert(make_pair(pos, child));

			g_globalScalar.tiltLoadedNum++;
		}
		return child;

	}


}
/**
 * 计算下一级，一分四tiles
 * 只要有一个不为空，则haschildren=true
 */
void CQuadTile::ComputeChildren()
{
    NW = ComputeChild(2*Row, 2*Col, Level+1);
    NE = ComputeChild(2*Row, 2*Col+1, Level+1);
    SW = ComputeChild(2*Row+1, 2*Col, Level+1);
    SE = ComputeChild(2*Row+1, 2*Col+1, Level+1);

	assert(NW);
	assert(NE);
	assert(SW);
	assert(SE);

   if(NW || NE || SW || SE)
        haschildren = true;

    if((texType==1 && !NW->isSatLoaded && !NW->isSatHaveLoaded) || (texType==3 && !NW->isNorLoaded && !NW->isNorHaveLoaded))
	{
		auto iter = ioQueueList.find(NW->keyPos);
		if(iter == ioQueueList.end())
		{
			ioQueueList.insert(std::make_pair(NW->keyPos, NW));
		}
	}
    if((texType==1 && !NE->isSatLoaded && !NE->isSatHaveLoaded) || (texType==3 && !NE->isNorLoaded && !NE->isNorHaveLoaded))
	{
		auto iter = ioQueueList.find(NE->keyPos);
		if(iter == ioQueueList.end())
		{
			ioQueueList.insert(std::make_pair(NE->keyPos, NE));
		}
	}
    if((texType==1 && !SW->isSatLoaded && !SW->isSatHaveLoaded) || (texType==3 && !SW->isNorLoaded && !SW->isNorHaveLoaded))
	{
		auto iter = ioQueueList.find(SW->keyPos);
		if(iter == ioQueueList.end())
		{
			ioQueueList.insert(std::make_pair(SW->keyPos, SW));
		}
	}
    if((texType==1 && !SE->isSatLoaded && !SE->isSatHaveLoaded) || (texType==3 && !SE->isNorLoaded && !SE->isNorHaveLoaded))
	{
		auto iter = ioQueueList.find(SE->keyPos);
		if(iter == ioQueueList.end())
		{
			ioQueueList.insert(std::make_pair(SE->keyPos, SE));
		}
	}


	//
	curTileVector.push_back(NW->keyPos);
	curTileVector.push_back(NE->keyPos);
	curTileVector.push_back(SW->keyPos);
	curTileVector.push_back(SE->keyPos);


}

bool CQuadTile::BuildEarthVertexBuffer( float* height )
{
    if (vertices == nullptr)
		vertices = new float[NUM_PTS_TILE * NUM_PTS_TILE * 3];
    if (vertices2D == nullptr)
		vertices2D = new float[NUM_PTS_TILE * NUM_PTS_TILE * 3];
    if (vVertices == nullptr)
		vVertices = new GLfloat[NUM_PTS_TILE * NUM_PTS_TILE * 3 + (9 + 9 + 7 + 7) * 3];
    if(vVertices2D == nullptr)
        vVertices2D =new GLfloat[NUM_PTS_TILE * NUM_PTS_TILE*3+(9+9+7+7)*3];

    maxHeight = 0.0;
    minHeight = 0.0;

    float f32[NUM_PTS_TILE*NUM_PTS_TILE];//0top	 1left	 2right	 3bottom	 4middle

    int coe = (NUM_PTS-1)/(NUM_PTS_TILE-1);
    for (int r=0;r<NUM_PTS_TILE;r++)
	{
        for (int c=0;c<NUM_PTS_TILE;c++)
		{
            f32[r*NUM_PTS_TILE+c] = height[(NUM_PTS_TILE-1-r)*NUM_PTS*coe+c*coe];

            if(maxHeight<f32[r*NUM_PTS_TILE+c])
                maxHeight = f32[r*NUM_PTS_TILE+c];
            if(minHeight>f32[r*NUM_PTS_TILE+c])
                minHeight = f32[r*NUM_PTS_TILE+c];

		}
	}
    int centNum = (NUM_PTS_TILE-1)/2;
    CenterHeight = f32[centNum*NUM_PTS_TILE+centNum];

    //NW
    cornersPos[0].z = f32[(NUM_PTS_TILE-1) * NUM_PTS_TILE];
    //SW
    cornersPos[1].z = f32[0];
    //NE
    cornersPos[2].z = f32[(NUM_PTS_TILE-1) * NUM_PTS_TILE + NUM_PTS_TILE-1];
    //SE
    cornersPos[3].z = f32[NUM_PTS_TILE-1];
	//////////////////////////////////////////////////////////////////////////
	//
    double nY = LatToY(North);
    double sY = nY - tileSize;
    double wX = LonToX(West);
	//
    for ( int y = 0; y < NUM_PTS_TILE; ++y )
	{
        for ( int x = 0; x < NUM_PTS_TILE; ++x )
		{
            double lat = YToLat(sY + (y*tileSize / (float)(NUM_PTS_TILE-1.0)));
            double lng = XToLon(wX + (x*tileSize / (float)(NUM_PTS_TILE-1.0)));
			glm::dvec3 point_basic = SphericalToCartesian(lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]);
            glm::dvec3 point = point_basic - SphericalToCartesian( CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

			vertices[((y *NUM_PTS_TILE) + x) * 3] = point_basic.x;
			vertices[((y *NUM_PTS_TILE) + x) * 3 + 1] = point_basic.y;
			vertices[((y *NUM_PTS_TILE) + x) * 3 + 2] = point_basic.z;

			vVertices[((y *NUM_PTS_TILE) + x) * 3] = point.x;
			vVertices[((y *NUM_PTS_TILE) + x) * 3 + 1] = point.y;
			vVertices[((y *NUM_PTS_TILE) + x) * 3 + 2] = point.z;

			point_basic = SphericalToMercator(lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]);

            point = point_basic - SphericalToMercator( CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

			vertices2D[((y *NUM_PTS_TILE) + x) * 3] = point_basic.x;
			vertices2D[((y *NUM_PTS_TILE) + x) * 3 + 1] = point_basic.y;
			vertices2D[((y *NUM_PTS_TILE) + x) * 3 + 2] = point_basic.z;

            vVertices2D[(( y *NUM_PTS_TILE ) + x)*3] = point.x;
            vVertices2D[(( y *NUM_PTS_TILE ) + x)*3 + 1] = point.y;
            vVertices2D[(( y *NUM_PTS_TILE ) + x)*3 + 2] = point.z;

		}
	}

	//
    int base = NUM_PTS_TILE*NUM_PTS_TILE*3;
	int y=0;
	float hb = 9000.0;
    for ( int x = 0; x < NUM_PTS_TILE; ++x )
    {
        double lat = YToLat(sY + (y*tileSize / (float)(NUM_PTS_TILE-1.0)));
        double lng = XToLon(wX + (x*tileSize / (float)(NUM_PTS_TILE-1.0)));
        glm::dvec3 point = SphericalToCartesian( lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]-hb ) - SphericalToCartesian( CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

        vVertices[base+x*3] = point.x;
        vVertices[base+x*3 + 1] = point.y;
        vVertices[base+x*3 + 2] = point.z;


        point = SphericalToMercator( lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]-hb ) - SphericalToMercator( CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

        vVertices2D[base+x*3] = point.x;
        vVertices2D[base+x*3 + 1] = point.y;
        vVertices2D[base+x*3 + 2] = point.z;
    }

    base += 9*3;
    y=NUM_PTS_TILE-1;
    for ( int x = 0; x < NUM_PTS_TILE; ++x )
    {
        double lat = YToLat(sY + (y*tileSize / (float)(NUM_PTS_TILE-1.0)));
        double lng = XToLon(wX + (x*tileSize / (float)(NUM_PTS_TILE-1.0)));
        glm::dvec3 point = SphericalToCartesian( lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]-hb ) - SphericalToCartesian( CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

        vVertices[base+x*3] = point.x;
        vVertices[base+x*3 + 1] = point.y;
        vVertices[base+x*3 + 2] = point.z;


        point = SphericalToMercator( lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]-hb ) - SphericalToMercator( CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

        vVertices2D[base+x*3] = point.x;
        vVertices2D[base+x*3 + 1] = point.y;
        vVertices2D[base+x*3 + 2] = point.z;
    }

    base += 9*3;
    int x = NUM_PTS_TILE-1;
    for ( int y = 1; y < NUM_PTS_TILE-1; ++y )
    {
        double lat = YToLat(sY + (y*tileSize / (float)(NUM_PTS_TILE-1.0)));
        double lng = XToLon(wX + (x*tileSize / (float)(NUM_PTS_TILE-1.0)));
        glm::dvec3 point = SphericalToCartesian( lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]-hb ) - SphericalToCartesian( CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

        vVertices[base+(y-1)*3] = point.x;
        vVertices[base+(y-1)*3 + 1] = point.y;
        vVertices[base+(y-1)*3 + 2] = point.z;


        point = SphericalToMercator( lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]-hb ) - SphericalToMercator( CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

        vVertices2D[base+(y-1)*3] = point.x;
        vVertices2D[base+(y-1)*3 + 1] = point.y;
        vVertices2D[base+(y-1)*3 + 2] = point.z;
    }

    base += 7*3;
    x = 0;
    for ( int y = 1; y < NUM_PTS_TILE-1; ++y )
    {
        double lat = YToLat(sY + (y*tileSize / (float)(NUM_PTS_TILE-1.0)));
        double lng = XToLon(wX + (x*tileSize / (float)(NUM_PTS_TILE-1.0)));
        glm::dvec3 point = SphericalToCartesian( lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]-hb ) - SphericalToCartesian( CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

        vVertices[base+(y-1)*3] = point.x;
        vVertices[base+(y-1)*3 + 1] = point.y;
        vVertices[base+(y-1)*3 + 2] = point.z;


        point = SphericalToMercator( lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]-hb ) - SphericalToMercator( CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

        vVertices2D[base+(y-1)*3] = point.x;
        vVertices2D[base+(y-1)*3 + 1] = point.y;
        vVertices2D[base+(y-1)*3 + 2] = point.z;
    }

	return true;
}

bool CQuadTile::BuildTileVertexBuffer(float* height)
{
    if (vertices == nullptr)
        vertices = new float[NUM_PTS_TILE * NUM_PTS_TILE * 3];
    if (vertices2D == nullptr)
        vertices2D = new float[NUM_PTS_TILE * NUM_PTS_TILE * 3];
    if (vVertices == nullptr)
        vVertices = new GLfloat[NUM_PTS_TILE * NUM_PTS_TILE * 3 + (9 + 9 + 7 + 7) * 3];
    if(vVertices2D == nullptr)
        vVertices2D =new GLfloat[NUM_PTS_TILE * NUM_PTS_TILE*3+(9+9+7+7)*3];

	maxHeight = 0.0;
	minHeight = 0.0;

	float f32[NUM_PTS_TILE*NUM_PTS_TILE];//0top	 1left	 2right	 3bottom	 4middle

	int coe = (NUM_PTS - 1) / (NUM_PTS_TILE - 1);
	for (int r = 0; r < NUM_PTS_TILE; r++)
	{
		for (int c = 0; c < NUM_PTS_TILE; c++)
		{
			f32[r*NUM_PTS_TILE + c] = height[(NUM_PTS_TILE - 1 - r)*NUM_PTS*coe + c*coe];

			if (maxHeight < f32[r*NUM_PTS_TILE + c])
				maxHeight = f32[r*NUM_PTS_TILE + c];
			if (minHeight > f32[r*NUM_PTS_TILE + c])
				minHeight = f32[r*NUM_PTS_TILE + c];

		}
	}
	int centNum = (NUM_PTS_TILE - 1) / 2;
	CenterHeight = f32[centNum*NUM_PTS_TILE + centNum];

	//NW
	cornersPos[0].z = f32[(NUM_PTS_TILE - 1) * NUM_PTS_TILE];
	//SW
	cornersPos[1].z = f32[0];
	//NE
	cornersPos[2].z = f32[(NUM_PTS_TILE - 1) * NUM_PTS_TILE + NUM_PTS_TILE - 1];
	//SE
	cornersPos[3].z = f32[NUM_PTS_TILE - 1];
	//////////////////////////////////////////////////////////////////////////
	//
	double nY = LatToY(North);
	double sY = nY - tileSize;
	double wX = LonToX(West);
	//
	for (int y = 0; y < NUM_PTS_TILE; ++y)
	{
		for (int x = 0; x < NUM_PTS_TILE; ++x)
		{
			double lat = YToLat(sY + (y*tileSize / (float)(NUM_PTS_TILE - 1.0)));
			double lng = XToLon(wX + (x*tileSize / (float)(NUM_PTS_TILE - 1.0)));
			glm::dvec3 point_basic = SphericalToCartesian(lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]);
			glm::dvec3 point = point_basic - SphericalToCartesian(CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

			vertices[((y *NUM_PTS_TILE) + x) * 3] = point_basic.x;
			vertices[((y *NUM_PTS_TILE) + x) * 3 + 1] = point_basic.y;
			vertices[((y *NUM_PTS_TILE) + x) * 3 + 2] = point_basic.z;

			vVertices[((y *NUM_PTS_TILE) + x) * 3] = point.x;
			vVertices[((y *NUM_PTS_TILE) + x) * 3 + 1] = point.y;
			vVertices[((y *NUM_PTS_TILE) + x) * 3 + 2] = point.z;

			point_basic = SphericalToMercator(lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]);

			point = point_basic - SphericalToMercator(CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

			vertices2D[((y *NUM_PTS_TILE) + x) * 3] = point_basic.x;
			vertices2D[((y *NUM_PTS_TILE) + x) * 3 + 1] = point_basic.y;
			vertices2D[((y *NUM_PTS_TILE) + x) * 3 + 2] = point_basic.z;

			vVertices2D[((y *NUM_PTS_TILE) + x) * 3] = point.x;
			vVertices2D[((y *NUM_PTS_TILE) + x) * 3 + 1] = point.y;
			vVertices2D[((y *NUM_PTS_TILE) + x) * 3 + 2] = point.z;

		}
	}

	//
	int base = NUM_PTS_TILE*NUM_PTS_TILE * 3;
	int y = 0;
	float hb = 9000.0;
	for (int x = 0; x < NUM_PTS_TILE; ++x)
	{
		double lat = YToLat(sY + (y*tileSize / (float)(NUM_PTS_TILE - 1.0)));
		double lng = XToLon(wX + (x*tileSize / (float)(NUM_PTS_TILE - 1.0)));
		glm::dvec3 point = SphericalToCartesian(lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x] - hb) - SphericalToCartesian(CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

		vVertices[base + x * 3] = point.x;
		vVertices[base + x * 3 + 1] = point.y;
		vVertices[base + x * 3 + 2] = point.z;


		point = SphericalToMercator(lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x] - hb) - SphericalToMercator(CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

		vVertices2D[base + x * 3] = point.x;
		vVertices2D[base + x * 3 + 1] = point.y;
		vVertices2D[base + x * 3 + 2] = point.z;
	}

	base += 9 * 3;
	y = NUM_PTS_TILE - 1;
	for (int x = 0; x < NUM_PTS_TILE; ++x)
	{
		double lat = YToLat(sY + (y*tileSize / (float)(NUM_PTS_TILE - 1.0)));
		double lng = XToLon(wX + (x*tileSize / (float)(NUM_PTS_TILE - 1.0)));
		glm::dvec3 point = SphericalToCartesian(lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x] - hb) - SphericalToCartesian(CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

		vVertices[base + x * 3] = point.x;
		vVertices[base + x * 3 + 1] = point.y;
		vVertices[base + x * 3 + 2] = point.z;


		point = SphericalToMercator(lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x] - hb) - SphericalToMercator(CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

		vVertices2D[base + x * 3] = point.x;
		vVertices2D[base + x * 3 + 1] = point.y;
		vVertices2D[base + x * 3 + 2] = point.z;
	}

	base += 9 * 3;
	int x = NUM_PTS_TILE - 1;
	for (int y = 1; y < NUM_PTS_TILE - 1; ++y)
	{
		double lat = YToLat(sY + (y*tileSize / (float)(NUM_PTS_TILE - 1.0)));
		double lng = XToLon(wX + (x*tileSize / (float)(NUM_PTS_TILE - 1.0)));
		glm::dvec3 point = SphericalToCartesian(lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x] - hb) - SphericalToCartesian(CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

		vVertices[base + (y - 1) * 3] = point.x;
		vVertices[base + (y - 1) * 3 + 1] = point.y;
		vVertices[base + (y - 1) * 3 + 2] = point.z;


		point = SphericalToMercator(lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x] - hb) - SphericalToMercator(CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

		vVertices2D[base + (y - 1) * 3] = point.x;
		vVertices2D[base + (y - 1) * 3 + 1] = point.y;
		vVertices2D[base + (y - 1) * 3 + 2] = point.z;
	}

	base += 7 * 3;
	x = 0;
	for (int y = 1; y < NUM_PTS_TILE - 1; ++y)
	{
		double lat = YToLat(sY + (y*tileSize / (float)(NUM_PTS_TILE - 1.0)));
		double lng = XToLon(wX + (x*tileSize / (float)(NUM_PTS_TILE - 1.0)));
		glm::dvec3 point = SphericalToCartesian(lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x] - hb) - SphericalToCartesian(CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

		vVertices[base + (y - 1) * 3] = point.x;
		vVertices[base + (y - 1) * 3 + 1] = point.y;
		vVertices[base + (y - 1) * 3 + 2] = point.z;


		point = SphericalToMercator(lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x] - hb) - SphericalToMercator(CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

		vVertices2D[base + (y - 1) * 3] = point.x;
		vVertices2D[base + (y - 1) * 3 + 1] = point.y;
		vVertices2D[base + (y - 1) * 3 + 2] = point.z;
	}

	return true;
}

bool CQuadTile::BuildEarthVertexBufferPole( float* height )
{
    if (vertices == nullptr)
		vertices = new float[NUM_PTS_TILE * NUM_PTS_TILE * 3];
    if (vertices2D == nullptr)
		vertices2D = new float[NUM_PTS_TILE * NUM_PTS_TILE * 3];
    if(vVertices == nullptr)
        vVertices =new GLfloat[NUM_PTS_TILE * NUM_PTS_TILE*3+(9+9+7+7)*3];
    if(vVertices2D == nullptr)
        vVertices2D =new GLfloat[NUM_PTS_TILE * NUM_PTS_TILE*3+(9+9+7+7)*3];

    maxHeight = 0.0;
    minHeight = 0.0;

    float f32[NUM_PTS_TILE*NUM_PTS_TILE];//0top	 1left	 2right	 3bottom	 4middle

    int coe = (NUM_PTS-1)/(NUM_PTS_TILE-1);
    for (int r=0;r<NUM_PTS_TILE;r++)
    {
        for (int c=0;c<NUM_PTS_TILE;c++)
        {
            f32[r*NUM_PTS_TILE+c] = height[(NUM_PTS_TILE-1-r)*NUM_PTS*coe+c*coe];
            if(maxHeight<f32[r*NUM_PTS_TILE+c])
                maxHeight = f32[r*NUM_PTS_TILE+c];
            if(minHeight>f32[r*NUM_PTS_TILE+c])
                minHeight = f32[r*NUM_PTS_TILE+c];
        }
    }
    int centNum = (NUM_PTS_TILE-1)/2;
    CenterHeight = f32[centNum*NUM_PTS_TILE+centNum];
	//////////////////////////////////////////////////////////////////////////
	//
    double wX = LonToX(West);
	//
    for ( int y = 0; y < NUM_PTS_TILE; ++y )
	{
        for ( int x = 0; x < NUM_PTS_TILE; ++x )
		{
            double lat = South + (y*(North-South) / (float)(NUM_PTS_TILE-1.0));
            double lng = XToLon(wX + (x*tileSize / (float)(NUM_PTS_TILE-1.0)));
			glm::dvec3 point_basic = SphericalToCartesian(lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]);
            glm::dvec3 point = point_basic - SphericalToCartesian( CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

			vertices[((y *NUM_PTS_TILE) + x) * 3] = point_basic.x;
			vertices[((y *NUM_PTS_TILE) + x) * 3 + 1] = point_basic.y;
			vertices[((y *NUM_PTS_TILE) + x) * 3 + 2] = point_basic.z;

            vVertices[(( y *NUM_PTS_TILE ) + x)*3] = point.x;
            vVertices[(( y *NUM_PTS_TILE ) + x)*3 + 1] = point.y;
            vVertices[(( y *NUM_PTS_TILE ) + x)*3 + 2] = point.z;

			point_basic = SphericalToMercator(lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]);

            point = point_basic - SphericalToMercator( CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

			vertices2D[((y *NUM_PTS_TILE) + x) * 3] = point_basic.x;
			vertices2D[((y *NUM_PTS_TILE) + x) * 3 + 1] = point_basic.y;
			vertices2D[((y *NUM_PTS_TILE) + x) * 3 + 2] = point_basic.z;

            vVertices2D[(( y *NUM_PTS_TILE ) + x)*3] = point.x;
            vVertices2D[(( y *NUM_PTS_TILE ) + x)*3 + 1] = point.y;
            vVertices2D[(( y *NUM_PTS_TILE ) + x)*3 + 2] = point.z;

		}
	}
    //
    int base = NUM_PTS_TILE*NUM_PTS_TILE*3;
    int y=0;
    float hb = 9000.0;
    for ( int x = 0; x < NUM_PTS_TILE; ++x )
    {
        double lat = South + (y*(North-South) / (float)(NUM_PTS_TILE-1.0));
        double lng = XToLon(wX + (x*tileSize / (float)(NUM_PTS_TILE-1.0)));
        glm::dvec3 point = SphericalToCartesian( lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]-hb ) - SphericalToCartesian( CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

        vVertices[base+x*3] = point.x;
        vVertices[base+x*3 + 1] = point.y;
        vVertices[base+x*3 + 2] = point.z;

        point = SphericalToMercator( lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]-hb ) - SphericalToMercator( CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

        vVertices2D[base+x*3] = point.x;
        vVertices2D[base+x*3 + 1] = point.y;
        vVertices2D[base+x*3 + 2] = point.z;
    }

    base += 9*3;
    y=NUM_PTS_TILE-1;
    for ( int x = 0; x < NUM_PTS_TILE; ++x )
    {
        double lat = South + (y*(North-South) / (float)(NUM_PTS_TILE-1.0));
        double lng = XToLon(wX + (x*tileSize / (float)(NUM_PTS_TILE-1.0)));
        glm::dvec3 point = SphericalToCartesian( lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]-hb ) - SphericalToCartesian( CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

        vVertices[base+x*3] = point.x;
        vVertices[base+x*3 + 1] = point.y;
        vVertices[base+x*3 + 2] = point.z;

        point = SphericalToMercator( lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]-hb ) - SphericalToMercator( CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

        vVertices2D[base+x*3] = point.x;
        vVertices2D[base+x*3 + 1] = point.y;
        vVertices2D[base+x*3 + 2] = point.z;
    }

    base += 9*3;
    int x = NUM_PTS_TILE-1;
    for ( int y = 1; y < NUM_PTS_TILE-1; ++y )
    {
        double lat = South + (y*(North-South) / (float)(NUM_PTS_TILE-1.0));
        double lng = XToLon(wX + (x*tileSize / (float)(NUM_PTS_TILE-1.0)));
        glm::dvec3 point = SphericalToCartesian( lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]-hb ) - SphericalToCartesian( CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

        vVertices[base+(y-1)*3] = point.x;
        vVertices[base+(y-1)*3 + 1] = point.y;
        vVertices[base+(y-1)*3 + 2] = point.z;

        point = SphericalToMercator( lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]-hb ) - SphericalToMercator( CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

        vVertices2D[base+(y-1)*3] = point.x;
        vVertices2D[base+(y-1)*3 + 1] = point.y;
        vVertices2D[base+(y-1)*3 + 2] = point.z;
    }

    base += 7*3;
    x = 0;
    for ( int y = 1; y < NUM_PTS_TILE-1; ++y )
    {
        double lat = South + (y*(North-South) / (float)(NUM_PTS_TILE-1.0));
        double lng = XToLon(wX + (x*tileSize / (float)(NUM_PTS_TILE-1.0)));
        glm::dvec3 point = SphericalToCartesian( lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]-hb ) - SphericalToCartesian( CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

        vVertices[base+(y-1)*3] = point.x;
        vVertices[base+(y-1)*3 + 1] = point.y;
        vVertices[base+(y-1)*3 + 2] = point.z;

        point = SphericalToMercator( lat, lng, EARTH_MEANRADIUS + f32[y * NUM_PTS_TILE + x]-hb ) - SphericalToMercator( CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);

        vVertices2D[base+(y-1)*3] = point.x;
        vVertices2D[base+(y-1)*3 + 1] = point.y;
        vVertices2D[base+(y-1)*3 + 2] = point.z;
    }
	return true;
}

void CQuadTile::ComputeWorldMatrix(glm::dvec3 viewPoint, bool is3D)
{
	glm::mat4 world = glm::mat4(1.0);
	glm::vec3 pos;
	if(is3D)
	{
		pos = SphericalToCartesian((float)CenterLatitude, (float)CenterLongitude, (float)EARTH_MEANRADIUS);
	}
	else
	{
		pos = SphericalToMercator((float)CenterLatitude, (float)CenterLongitude, (float)EARTH_MEANRADIUS);
	}

	glm::mat4 tranToOrg;
	tranToOrg = glm::mat4(1.0);
	MatrixTranslation(&tranToOrg,pos.x-viewPoint.x, pos.y-viewPoint.y, pos.z-viewPoint.z);
	world = tranToOrg * world;

	//////////////////////////////////////////////////////////////////////////
	worldMQT = world;

}

void CQuadTile::ComputelocalOrigin(double offsetA, bool is3D)
{
	if(is3D)
	{
		m_BBox.Get3DBox(South,North,West,East, EARTH_MEANRADIUS +minHeight, EARTH_MEANRADIUS +maxHeight);

        localOrigin = SphericalToCartesian(CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);
	}
	else
	{
        m_BBox.Get2DBox(South,North,West+offsetA,East+offsetA, EARTH_MEANRADIUS +minHeight, EARTH_MEANRADIUS +maxHeight);

        localOrigin = SphericalToMercator(CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);
	}
}

double CQuadTile::Getoffset(glm::dvec3 cameraPos, bool is3D)
{
    float offsetA = 0.0;
	
    if(!is3D)
    {
        glm::dvec3 pos_normal = SphericalToMercator(CenterLatitude, CenterLongitude, EARTH_MEANRADIUS);
        glm::dvec3 pos_add_2PI = SphericalToMercator(CenterLatitude, CenterLongitude+2.0*PI, EARTH_MEANRADIUS);
        glm::dvec3 pos_sub_2PI = SphericalToMercator(CenterLatitude, CenterLongitude-2.0*PI, EARTH_MEANRADIUS);

        double dist_normal = glm::length(cameraPos - pos_normal);
        double dist_add_2PI = glm::length(cameraPos - pos_add_2PI);
        double dist_sub_2PI = glm::length(cameraPos - pos_sub_2PI);

        if(dist_normal>dist_add_2PI)
        {
            offsetA = 2.0*PI;
        }
        else if(dist_normal>dist_sub_2PI)
        {
            offsetA = -2.0*PI;
        }

    }

    //
    ComputelocalOrigin(offsetA);

    return offsetA;
}

void CQuadTile::loadDemData(string fileURL)
{
    FILE *fp=nullptr;
    int err;
    int numread;

    err = fopen_s(&fp, fileURL.c_str(), "rb" );
    //如果高程文件存在
    if(err==0)
    {
        numread = fread ( g_TextureRectDataHeight, sizeof(float)*NUM_PTS*NUM_PTS, 1, fp );
        fclose ( fp );
        BuildTileVertexBuffer(g_TextureRectDataHeight);
        isDemLoaded = true;

    }
    else
    {
        memset(g_TextureRectDataHeight, 0, step_dem);
        if (parent!=nullptr && parent->isDemLoaded)
        {
            int row_r, col_r, rc_num = NUM_PTS_TILE/2+1;
            row_r = Row % 2;
            col_r = Col % 2;

            //NW
            if (row_r == 0 && col_r == 0)
            {
                for (int r = 0; r < rc_num; r++)
                {
                    for (int c = 0; c < rc_num; c++)
                    {
                        g_TextureRectDataHeight[2 * r*NUM_PTS + 2 * c] = parent->g_TextureRectDataHeight[r*NUM_PTS + c];
                    }
                }

            }
            //NE
            if (row_r == 0 && col_r == 1)
            {
                for (int r = 0; r < rc_num; r++)
                {
                    for (int c = 0; c < rc_num; c++)
                    {
                        g_TextureRectDataHeight[2 * r*NUM_PTS + 2 * c] = parent->g_TextureRectDataHeight[r*NUM_PTS + c + rc_num - 1];
                    }
                }
            }
            //SW
            if (row_r == 1 && col_r == 0)
            {
                for (int r = 0; r < rc_num; r++)
                {
                    for (int c = 0; c < rc_num; c++)
                    {
                        g_TextureRectDataHeight[2 * r*NUM_PTS + 2 * c] = parent->g_TextureRectDataHeight[(r + rc_num - 1)*NUM_PTS + c];
                    }
                }

            }
            //SE
            if (row_r == 1 && col_r == 1)
            {
                for (int r = 0; r < rc_num; r++)
                {
                    for (int c = 0; c < rc_num; c++)
                    {
                        g_TextureRectDataHeight[2 * r*NUM_PTS + 2 * c] = parent->g_TextureRectDataHeight[(r + rc_num - 1)*NUM_PTS + c + rc_num - 1];
                    }
                }

            }

            //
            for (int r = 0; r < rc_num; r++)
            {
                for (int c = 0; c < rc_num - 1; c++)
                {
                    g_TextureRectDataHeight[2 * r*NUM_PTS + 2 * c + 1] = 0.5*(g_TextureRectDataHeight[2 * r*NUM_PTS + 2 * c] + g_TextureRectDataHeight[2 * r*NUM_PTS + 2 * (c + 1)]);
                }
            }
            //
            for (int r = 0; r < rc_num - 1; r++)
            {
                for (int c = 0; c < NUM_PTS; c++)
                {
                    g_TextureRectDataHeight[(2 * r + 1)*NUM_PTS + c] = 0.5*(g_TextureRectDataHeight[2 * r*NUM_PTS + c] + g_TextureRectDataHeight[2 * (r + 1)*NUM_PTS + c]);
                }
            }
            BuildTileVertexBuffer(g_TextureRectDataHeight);
            isDemLoaded = true;
        }
        else
            isDemLoaded = false;
    }

}

void CQuadTile::loadNorData(string fileURL)
{
    FILE *fp=nullptr;
    int err;
    int numread;
    int numread1;
    int num=0;

    err = fopen_s(&fp, fileURL.c_str(), "rb" );

    if(err==0)
    {
        numread = fread ( &num, 4, 1, fp );
        numread1 = fread ( g_TextureRectDataNor, sizeof(WORD)*198147, 1, fp );
        fclose ( fp );

        assert(g_TextureRectDataNor);     
        isNorLoaded = g_TextureRectDataNor!=nullptr;

    }
    else
    {
        isNorLoaded = false;
    }

    isNorHaveLoaded = true;

}

void CQuadTile::loadSatData(string fileURL)
{
    data_sat = LoadTexDataSD(fileURL.c_str(),false);
//    assert(data_sat);
    isSatLoaded = data_sat!=nullptr;

    isSatHaveLoaded = true;

}

void CQuadTile::LoadTexture()
{
    //////////////////////////////////////////////////////////////////////////
    //加载dem数据, nor、sat纹理数据

    string texDemName;
    string texFileName;
    string texPicName;

    char s[64];

    sprintf(s, "/nor/%d/%d/%d.nor", Level, Col, Row);
    texFileName = g_worldDataPath+s;
    sprintf(s, "/dem/%d/%d/%d.dem", Level, Col, Row);
    texDemName = g_worldDataPath+s;
    sprintf(s, "/satellite/%d/%d/%d.jpg", Level, Col, Row);
    texPicName = g_worldDataPath+s;

    //卫星
    if (texType == 1)
    {
        if(!isDemLoaded)
            loadDemData(texDemName);
        if(!isSatLoaded)
        {
            loadSatData(texPicName);

        }

    }
    //标准地图
    if (texType == 2)
    {

    }

    //地势图
    if (texType == 3)
    {
        if(!isDemLoaded)
            loadDemData(texDemName);
        if(!isNorLoaded)
        {
            loadNorData(texFileName);

        }

    }

}


void CQuadTile::Initialize()
{
    tileSize = 2.0 * EARTH_MEANRADIUS * ((double)PI) / pow(2.0, Level);//单位：米
    North = tiley2lat(Row, Level)*((double)PI) / 180.0;  //单位：弧度
    West = tilex2long(Col, Level)*((double)PI) / 180.0;

    double nY = LatToY(North);
    double sY = nY - tileSize;
    South = YToLat(sY);

    double wX = LonToX(West);
    double eX = wX + tileSize;
    East = XToLon(eX);

    CenterLatitude = 0.5F * (North + South);
    CenterLongitude = 0.5F * (West + East);
    LatitudeSpan = abs(North - South);
    LongitudeSpan = abs(East - West);

    m_BBox = CBoundingBox(South, North, West, East, EARTH_MEANRADIUS, EARTH_MEANRADIUS);

    //NW
    cornersPos[0].x = North;
    cornersPos[0].y = West;
    //SW
    cornersPos[1].x = South;
    cornersPos[1].y = West;
    //NE
    cornersPos[2].x = North;
    cornersPos[2].y = East;
    //SE
    cornersPos[3].x = North;
    cornersPos[3].y = South;

    ComputelocalOrigin(0.0);
    
    keyPos=TILTPOS(Level,Col,Row);

	//
    g_TextureRectDataNor = new WORD[198147];//257*257*3
    g_TextureRectDataHeight=new float[NUM_PTS*NUM_PTS];

    //////////////////////////////////////////////////////////////////////////
    //生成自定义纹理
    //定义纹理属性

    FILE *fp=nullptr;
    int err;
    int numread;
    int numread1;
    int num=0;

	std::string texFileName;
	std::string texPicName;

	char s[64];

    //////////////////////////////////////////////////////////////////////////
    //读取dem、纹理
    sprintf(s, "/nor/%d/%d/%d.nor", Level, Col, Row);
    texFileName = g_worldDataPath+s;
	sprintf(s, "/satellite/%d/%d/%d.jpg", Level, Col, Row);
	texPicName = g_worldDataPath+s;

    //////////////////////////////////////////////////////////////////////////
    err = fopen_s(&fp, texFileName.c_str(), "rb" );

    if(err==0)
	{
		numread = fread ( &num, 4, 1, fp );
		numread1 = fread ( g_TextureRectDataNor, sizeof(WORD)*198147, 1, fp );
		fclose ( fp );

        sprintf(s, "/dem/%d/%d/%d.dem", Level, Col, Row);
        texFileName = g_worldDataPath+s;
        err = fopen_s(&fp, texFileName.c_str(), "rb" );
        numread1 = fread ( g_TextureRectDataHeight, sizeof(float)*NUM_PTS*NUM_PTS, 1, fp );
        fclose ( fp );

		BuildEarthVertexBuffer(g_TextureRectDataHeight);

		// Use tightly packed data
        glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
		// Generate a texture object
		glGenTextures ( 1, &m_texture );
		glBindTexture ( GL_TEXTURE_2D, m_texture );
//		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RG8, 257, 257, 0, GL_RG, GL_UNSIGNED_BYTE, g_TextureRectDataNor );
        glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB16F, 257, 257, 0, GL_RGB, GL_HALF_FLOAT, g_TextureRectDataNor );

		// Set the filtering mode
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);

        SAFE_DELETE_ARRAY(g_TextureRectDataNor);

		//
		m_texturePic = LoadTextureSD(texPicName.c_str());

        isDemLoaded = true;
		isNorLoaded = true;
        isSatLoaded = true;
        isNorTexLoaded = true;
        isSatTexLoaded = true;
        isInitialized = true;
        bInUse = true;
	}
	else
	{
		isInitialized = false;
	}

}

void CQuadTile::InitializeNPole()
{
    tileSize = 2.0 * EARTH_MEANRADIUS * ((double)PI) / pow(2.0, Level);//单位：米
    North = ((double)PI) / 2.0;  //单位：弧度
    West = tilex2long(Col, Level)*((double)PI) / 180.0;
    South = tiley2lat(Row, Level)*((double)PI) / 180.0;  //单位：弧度

    double wX = LonToX(West);
    double eX = wX + tileSize;
    East = XToLon(eX);

    CenterLatitude = 0.5F * (North + South);
    CenterLongitude = 0.5F * (West + East);
    LatitudeSpan = abs(North - South);
    LongitudeSpan = abs(East - West);

    m_BBox = CBoundingBox(South, North, West, East, EARTH_MEANRADIUS, EARTH_MEANRADIUS);

    ComputelocalOrigin(0.0);
    keyPos=TILTPOS(Level,Col+20,-1);

    //
    g_TextureRectDataNor = new WORD[198147];//257*257*3
    g_TextureRectDataHeight=new float[NUM_PTS*NUM_PTS];

    //////////////////////////////////////////////////////////////////////////
    //生成自定义纹理
    //定义纹理属性

    FILE *fp=nullptr;
    int err;
    int numread;
    int numread1;
    int num=0;

	std::string texFileName;
	std::string texPicName;
	std::string texMapName;
	std::string texRName;

	char s[64];

    //////////////////////////////////////////////////////////////////////////
    //读取dem、纹理
    sprintf(s, "/nor/NP/%d/%d/%d.nor", Level, Col, Row);
    texFileName = g_worldDataPath+s;
	sprintf(s, "/satellite/NP/%d/%d/%d.jpg", Level, Col, Row);
	texPicName = g_worldDataPath+s;
    sprintf(s, "/m/NP/%d/%d/%d.png", Level, Col, Row);
    texMapName = g_worldDataPath+s;
    sprintf(s, "/h/NP/%d/%d/%d.png", Level, Col, Row);
    texRName = g_worldDataPath+s;
    //////////////////////////////////////////////////////////////////////////
    err = fopen_s(&fp, texFileName.c_str(), "rb" );
    //////////////////////////////////////////////////////////////////////////
    //读入纹理数据
    if(err==0)
	{
		numread = fread ( &num, 4, 1, fp );
		numread1 = fread ( g_TextureRectDataNor, sizeof(WORD)*198147, 1, fp );
		fclose ( fp );

		sprintf(s, "/dem/NP/%d/%d/%d.dem", Level, Col, Row);
		texFileName = g_worldDataPath+s;
        err = fopen_s(&fp, texFileName.c_str(), "rb" );
		numread1 = fread ( g_TextureRectDataHeight, sizeof(float)*NUM_PTS*NUM_PTS, 1, fp );
		fclose ( fp );

		BuildEarthVertexBufferPole(g_TextureRectDataHeight);
		// Use tightly packed data
		glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
		// Generate a texture object
		glGenTextures ( 1, &m_texture );
		glBindTexture ( GL_TEXTURE_2D, m_texture );
        glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB16F, 257, 257, 0, GL_RGB, GL_HALF_FLOAT, g_TextureRectDataNor );

		// Set the filtering mode
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);
		//
		m_texturePic = LoadTextureSD(texPicName.c_str());

		SAFE_DELETE_ARRAY(g_TextureRectDataNor);

		isDemLoaded = true;
		isNorLoaded = true;
		isNorTexLoaded = true;
		isInitialized = true;
		bInUse = true;
	}
	else
	{
		isInitialized = false;
	}

}

void CQuadTile::InitializeSPole()
{
    tileSize = 2.0 * EARTH_MEANRADIUS * ((double)PI) / pow(2.0, Level);//单位：米
    North = -1.0*tiley2lat(Row, Level)*((double)PI) / 180.0;  //单位：弧度
    West = tilex2long(Col, Level)*((double)PI) / 180.0;

    South = -1.0*((double)PI) / 2.0;  //单位：弧度

    double wX = LonToX(West);
    double eX = wX + tileSize;
    East = XToLon(eX);

    CenterLatitude = 0.5F * (North + South);
    CenterLongitude = 0.5F * (West + East);
    LatitudeSpan = abs(North - South);
    LongitudeSpan = abs(East - West);

    m_BBox = CBoundingBox(South, North, West, East, EARTH_MEANRADIUS, EARTH_MEANRADIUS);

    ComputelocalOrigin(0.0);

    keyPos=TILTPOS(Level,Col+30,-2);

    //
    g_TextureRectDataNor = new WORD[198147];//257*257*3
    g_TextureRectDataHeight=new float[NUM_PTS*NUM_PTS];

    //////////////////////////////////////////////////////////////////////////
    //生成自定义纹理
    //定义纹理属性

    FILE *fp=nullptr;
    int err;
    int numread;
    int numread1;
    int num=0;

	std::string texFileName;
	std::string texPicName;
	std::string texMapName;
	std::string texRName;

	char s[64];

    //////////////////////////////////////////////////////////////////////////
    //读取dem、纹理
    sprintf(s, "/nor/SP/%d/%d/%d.nor", Level, Col, Row);
    texFileName = g_worldDataPath+s;
	sprintf(s, "/satellite/SP/%d/%d/%d.jpg", Level, Col, Row);
	texPicName = g_worldDataPath+s;
    sprintf(s, "/m/SP/%d/%d/%d.png", Level, Col, Row);
    texMapName = g_worldDataPath+s;
    sprintf(s, "/h/SP/%d/%d/%d.png", Level, Col, Row);
    texRName = g_worldDataPath+s;
    //////////////////////////////////////////////////////////////////////////
    err = fopen_s(&fp, texFileName.c_str(), "rb" );

    if(err==0)
    {
        numread = fread ( &num, 4, 1, fp );
        numread1 = fread ( g_TextureRectDataNor, sizeof(WORD)*198147, 1, fp );
        fclose ( fp );

        sprintf(s, "/dem/SP/%d/%d/%d.dem", Level, Col, Row);
        texFileName = g_worldDataPath+s;
        err = fopen_s(&fp, texFileName.c_str(), "rb" );
        numread1 = fread ( g_TextureRectDataHeight, sizeof(float)*NUM_PTS*NUM_PTS, 1, fp );
        fclose ( fp );

        BuildEarthVertexBufferPole(g_TextureRectDataHeight);

        // Use tightly packed data
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        // Generate a texture object
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        //		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RG8, 257, 257, 0, GL_RG, GL_UNSIGNED_BYTE, g_TextureRectDataNor );
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 257, 257, 0, GL_RGB, GL_HALF_FLOAT, g_TextureRectDataNor);

        // Set the filtering mode
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
        //
        m_texturePic = LoadTextureSD(texPicName.c_str());

        SAFE_DELETE_ARRAY(g_TextureRectDataNor);

        isDemLoaded = true;
        isNorLoaded = true;
        isNorTexLoaded = true;
        isInitialized = true;
        bInUse = true;
    }
    else
    {
        isInitialized = false;
    }

}

std::vector<std::string> CQuadTile::split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> res;
    if(str.empty()) return res;
	//先将要切割的字符串从string类型转换为char*类型
	char * strs = new char[str.length() + 1] ; //不要忘了
//    strcpy_s(strs, str.length()+1, str.c_str());
    strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
//    strcpy_s(d, delim.length() + 1, delim.c_str());
    strcpy(d, delim.c_str());
    char *buf;
    char *buftmp;

//    char *p = strtok_s(strs, d, &buf);
    char *p = strtok(strs, d);
	while(p) {
		std::string s = p; //分割得到的字符串转换为string类型
		res.push_back(s); //存入结果数组
//        p = strtok_s(NULL, d, &buftmp);
        p = strtok(nullptr, d);
    }

	return res;
}

void CQuadTile::Shutdown()
{
    haschildren = false;
    isInitialized = false;
    bInUse = false;

    if (parent)
    {
        if (parent)
        {
            if (parent->NW)
                if (parent->NW == this)
                    parent->NW = nullptr;
            if (parent->NE)
                if (parent->NE == this)
                    parent->NE = nullptr;
            if (parent->SW)
                if (parent->SW == this)
                    parent->SW = nullptr;
            if (parent->SE)
                if (parent->SE == this)
                    parent->SE = nullptr;
        }

    }
    if (NW)
    {
        NW->Shutdown();
        SAFE_DELETE(NW);
    }
    if (NE)
    {
        NE->Shutdown();
        SAFE_DELETE(NE);
    }
    if (SW)
    {
        SW->Shutdown();
        SAFE_DELETE(SW);
    }
    if (SE)
    {
        SE->Shutdown();
        SAFE_DELETE(SE);
    }

    SAFE_DELETE_ARRAY(vVertices);
    SAFE_DELETE_ARRAY(vVertices2D);

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(vertices2D);

    glDeleteTextures(1, &m_texture);
    glDeleteTextures(1, &m_texturePic);

    m_texture = 0;
    m_texturePic = 0;

    SAFE_DELETE_ARRAY(g_TextureRectDataNor);
    SAFE_DELETE_ARRAY(g_TextureRectDataHeight);

}

void CQuadTile::ComputeDistance( glm::dvec3 cameraPos, double offsetA, bool is3D)
{
	if(is3D)
	{
		glm::dvec3 v = cameraPos-localOrigin;
		distance = glm::length(v);
	}
	else
    {
		glm::dvec3 pot = localOrigin + glm::dvec3(0,offsetA*RADIAN_TO_METER,0);
		glm::dvec3 v = cameraPos-pot;
		distance = glm::length(v);

	}


}

void CQuadTile::UnuseQuadTile()
{
    if (NW != nullptr)
    {
        NW->UnuseQuadTile();
        NW->bInUse = false;
    }
    if (NE != nullptr)
    {
        NE->UnuseQuadTile();
        NE->bInUse = false;
    }
    if (SW != nullptr)
    {
        SW->UnuseQuadTile();
        SW->bInUse = false;
    }
    if (SE != nullptr)
    {
        SE->UnuseQuadTile();
        SE->bInUse = false;
    }
}

GLuint CQuadTile::loadTexture(const QString& path)
{
	GLuint textureID;
    glGenTextures(1, &textureID);

    QImage image = QImage(path).convertToFormat(QImage::Format_RGBA8888).mirrored(true, true);
    if (!image.isNull()) {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
 //       glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
    }

    return textureID;
}

// Load texture from disk
GLuint CQuadTile::LoadTextureSD(const char* fileName)
{
    GLuint    textureId = 0;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // 设置纹理过滤参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 为当前绑定纹理对象设置
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // 加载并生成纹理
    int width, height, nrChannels;

    unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, STBI_rgb_alpha);
    if (nrChannels==3 || nrChannels==4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);

    }
    else
    {
        textureId = 0;
    }


    return  textureId;
}

unsigned char* CQuadTile::LoadTexDataSD(QByteArray fileData)
{
    // 加载纹理数据
    int width, height, nrChannels;

    unsigned char *data = stbi_load_from_memory((unsigned char*)fileData.data(),fileData.size(), &width, &height, &nrChannels, STBI_rgb_alpha);

    return data;
}

unsigned char* CQuadTile::LoadTexDataSD(const char* fileName)
{
    // 加载纹理数据
    int width, height, nrChannels;

    unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, STBI_rgb_alpha);

    return data;
}

unsigned char* CQuadTile::LoadTexDataSD(const char* fileName, bool haveAlpha)
{
    // 加载纹理数据
    int width, height, nrChannels;
    unsigned char *data=nullptr;
    if(haveAlpha)
        data = stbi_load(fileName, &width, &height, &nrChannels, STBI_rgb_alpha);
    else
        data = stbi_load(fileName, &width, &height, &nrChannels, STBI_rgb);

    return data;
}

unsigned char* CQuadTile::LoadTexDataSD(unsigned char* fileData, int filesize, bool haveAlpha)
{
    // 加载纹理数据
    int width, height, nrChannels;
    unsigned char *data=nullptr;
    if(haveAlpha)
        data = stbi_load_from_memory(fileData,filesize, &width, &height, &nrChannels, STBI_rgb_alpha);
    else
        data = stbi_load_from_memory(fileData,filesize, &width, &height, &nrChannels, STBI_rgb);

    return data;
}

void CQuadTile::LoadVT()
{
    char str[64];
    sprintf(str, "%d%d%d.mvt", Level,Col,Row);
    std::string currentFile = g_worldDataPath+"/vt/"+str;

    FILE *fp = fopen(currentFile.c_str(), "rb");
    if (fp)
    {
        fread(&multiPts.num, sizeof(int), 1, fp);
        for (int i = 0; i < multiPts.num; i++)
        {
            V2Point v2pt;
            char strs[30];
            fread(&(v2pt.geoType), sizeof(int), 1, fp);
            fread(&(v2pt.pos.lat), sizeof(float), 1, fp);
            fread(&(v2pt.pos.lon), sizeof(float), 1, fp);
            fread(&(v2pt.len), sizeof(int), 1, fp);
            fread(strs, 1, v2pt.len, fp);
            strs[v2pt.len] = 0;
            v2pt.name = strs;
            multiPts.pts.push_back(v2pt);

        }
        fread(&multiLs.num, sizeof(int), 1, fp);
        for (int i = 0; i < multiLs.num; i++)
        {
            V2Line v2line;
            fread(&(v2line.geoType), sizeof(int), 1, fp);
            fread(&(v2line.num), sizeof(int), 1, fp);
            for (int j = 0; j < v2line.num; j++)
            {
                Point2F pt2f;
                fread(&(pt2f.lat), sizeof(float), 1, fp);
                fread(&(pt2f.lon), sizeof(float), 1, fp);
                v2line.pts.push_back(pt2f);
            }

            multiLs.lines.push_back(v2line);

        }


        //
        fclose(fp);
    }

}

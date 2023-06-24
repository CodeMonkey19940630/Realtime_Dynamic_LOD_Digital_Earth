//
// QuadTile.h
//

#pragma once

#include "../include/BoundingBox.hpp"


class CQuadTile
{
public:
    CQuadTile();
    CQuadTile( TILTPOS kPos );

    ~CQuadTile();

    double West;
    double East;
    double North;
    double South;
    double CenterLatitude;
    double CenterLongitude;
    double CenterHeight;
    double maxHeight;
    double minHeight;
    double LatitudeSpan;
    double LongitudeSpan;
    int Level;
    int Row;
    int Col;
    double distance;
    double tileSize;//单位：米

    //地块关键字
    TILTPOS keyPos;
    //地块生命值
    unsigned int lives;

    bool isInitialized;
    bool isDemLoaded = false;
    bool isNorLoaded = false;
    bool isSatLoaded = false;
    bool isNorTexLoaded = false;
    bool isSatTexLoaded = false;

    bool isNorHaveLoaded = false;
    bool isSatHaveLoaded = false;
    // Texture handle
    GLuint	m_texture;
    GLuint	m_texturePic;

    bool haschildren;
    bool bInUse;


    CQuadTile* parent=nullptr;

    CQuadTile* NW=nullptr;
    CQuadTile* SW=nullptr;
    CQuadTile* NE=nullptr;
    CQuadTile* SE=nullptr;

    int				maxlevel;
    byte			shapeType;
    int				gridIndex;

    glm::dvec3		cornersPos[4];

    glm::dvec3 localOrigin; // Add this offset to get world coordinates

    CBoundingBox	m_BBox;
    glm::mat4		worldMQT;//每块地块的model矩阵

    V2MultiPoint multiPts;	//点集合（含name）
    V2MultiLine multiLs;	//线集合

    struct VertexType
    {
        glm::vec3 position;
        glm::vec2 texture;
    };

    GLfloat *vVertices=nullptr;
    float *vertices=nullptr;  //精确计算交点
    float *vertices2D = nullptr;  //精确计算交点
    GLfloat *vVertices2D=nullptr;

    WORD* g_TextureRectDataNor=nullptr;
    float* g_TextureRectDataHeight=nullptr;
    unsigned char *data_sat=nullptr;


public:
    CQuadTile *ComputeChild( int row, int col, int level );
    void ComputeChildren();
    void ComputeDistance( glm::dvec3 cameraPos, double offsetA, bool is3D = true);
    std::vector<std::string> split(const std::string& str, const std::string& delim);

    void Initialize();
    void InitializeNPole();
    void InitializeSPole();
    bool BuildEarthVertexBuffer( float* height );
    bool BuildTileVertexBuffer(float* height);
    bool BuildEarthVertexBufferPole( float* height );

    void ComputeWorldMatrix(glm::dvec3 viewPoint, bool is3D = true);
    void ComputelocalOrigin(double offsetA, bool is3D = true);
    double Getoffset(glm::dvec3 cameraPos, bool is3D = true);

    void Shutdown();

    void UnuseQuadTile();
    void getPicData();

    void LoadTexture();
    GLuint loadTexture(const QString& path);
    GLuint LoadTextureSD(const char* fileName);
    unsigned char* LoadTexDataSD(QByteArray fileData);
    unsigned char* LoadTexDataSD(const char* fileName);
    unsigned char* LoadTexDataSD(const char* fileName, bool haveAlpha);
    unsigned char* LoadTexDataSD(unsigned char* fileData, int filesize, bool haveAlpha);

    void LoadVT();

private:
    void loadDemData(std::string fileURL);
    void loadNorData(std::string fileURL);
    void loadSatData(std::string fileURL);

};

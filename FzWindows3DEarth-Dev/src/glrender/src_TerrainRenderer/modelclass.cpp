
#include "modelclass.h"

ModelClass::ModelClass()
{
    m_Texture = 0;

    for (int i=0; i<k_totalDetailGridLevels; ++i)
    {
        for (int s=0; s<k_totalShapes; ++s)
        {
            s_detailLevel[i].tileBodies[s] = 0;
        }

    }
}

ModelClass::~ModelClass()
{

}

bool ModelClass::Initialize()
{
    generateDetailLevels();

    return true;
}

void ModelClass::Shutdown()
{
    // Release the vertex and index buffers.
    ShutdownBuffers();
}

void ModelClass::ShutdownBuffers()
{
    for (int i=0; i<k_totalDetailGridLevels; ++i)
    {
        for (int s=0; s<k_totalShapes; ++s)
        {
            s_detailLevel[i].tileBodies[s] = 0;
        }

    }

    return;
}

bool ModelClass::createIndexBuffer(detail_index_data** ppIndexBuffer, detail_index_data* source)
{
    *ppIndexBuffer = source;

    return true;
}

bool ModelClass::generateDetailLevels()
{
    for (int body=0; body<k_totalShapes; ++body)
    {
        if (!createIndexBuffer(&s_detailLevel[0].tileBodies[body], &grid_8M8[body]))	//8M8
        {
            return false;
        }
        if (!createIndexBuffer(&s_detailLevel[1].tileBodies[body], &grid_32M32[body]))	//32M32
        {
            return false;
        }
        if (!createIndexBuffer(&s_detailLevel[2].tileBodies[body], &grid_64M64[body]))	//64M64
        {
            return false;
        }
        if (!createIndexBuffer(&s_detailLevel[3].tileBodies[body], &grid_32M32To8M8[body]))	//32M32 connect
        {
            return false;
        }
        if (!createIndexBuffer(&s_detailLevel[4].tileBodies[body], &grid_64M64To32M32[body]))	//64M64 connect
        {
            return false;
        }
        if (!createIndexBuffer(&s_detailLevel[5].tileBodies[body], &grid_11M11[body]))	//11M11
        {
            return false;
        }
    }

    return true;

}

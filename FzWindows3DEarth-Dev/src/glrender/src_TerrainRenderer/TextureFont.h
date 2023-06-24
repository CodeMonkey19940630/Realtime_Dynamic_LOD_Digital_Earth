#pragma once

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftstroke.h>
#include <freetype/ftimage.h>

#include "../include/uniHead.h"

union Pixel32
{
    Pixel32()
        : integer(0) { }
    Pixel32(uint8 bi, uint8 gi, uint8 ri, uint8 ai = 255)
    {
        b = bi;
        g = gi;
        r = ri;
        a = ai;
    }

    unsigned int integer;

    struct
    {
        uint8 b, g, r, a;
    };
};

struct Character
{
public:
	Character()
	{
		x0 = 0;
		y0 = 0;
		x1 = 0;
		y1 = 0;
		offsetX = 0;
		offsetY = 0;
	}
	/**
	*   存储当前字符在纹理上的坐标位置
	*/

    unsigned int   x0 : 12;
    unsigned int   y0 : 12;
    unsigned int   x1 : 12;
    unsigned int   y1 : 12;
    unsigned int   offsetX : 12;
    unsigned int   offsetY : 12;
};

class CTextureFont
{
public:
	CTextureFont();
	~CTextureFont();

	CTextureFont(const char* fontFile, int fontSize);
	Character*  getCharacter(wchar_t ch);
	void drawText(float x, float y, const wchar_t* text);
	void genFontText(const wchar_t* text, LabelInfo &linfo);
    /**
     * @brief setFontColor
     * @param r 255
     * @param g 255
     * @param b 255
     */
    void setFontColor(uint8 r, uint8 g, uint8 b);
    /**
     * @brief setOutlineColor
     * @param r 255
     * @param g 255
     * @param b 255
     */
    void setOutlineColor(uint8 r, uint8 g, uint8 b);

	unsigned int _textureId;
    int outlineWidth = 2;
    Pixel32 fontCol=Pixel32(0, 0, 0);
    Pixel32 outlineCol=Pixel32(255, 255, 255);
//    Pixel32 fontCol=Pixel32(255, 255, 255);
//    Pixel32 outlineCol=Pixel32(0, 0, 0);
    unsigned int ch_arr_index = 0;//normal:0,blue:1

protected:

	Character   _character[2][1 << 16];
	FT_Library  _library;
	FT_Face     _face;
	int         _yStart;
	int         _xStart;
	int         _fontSize;
};


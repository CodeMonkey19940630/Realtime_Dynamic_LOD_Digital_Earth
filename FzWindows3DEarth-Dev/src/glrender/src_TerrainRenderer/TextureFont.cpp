#include "TextureFont.h"

struct Vec2
{
    Vec2() { }
    Vec2(float a, float b)
        : x(a), y(b) { }

    float x, y;
};


struct Rect
{
    Rect() { }
    Rect(float left, float top, float right, float bottom)
        : xmin(left), xmax(right), ymin(top), ymax(bottom) { }

    void Include(const Vec2 &r)
    {
        xmin = min(xmin, r.x);
        ymin = min(ymin, r.y);
        xmax = max(xmax, r.x);
        ymax = max(ymax, r.y);
    }

    float Width() const { return xmax - xmin + 1; }
    float Height() const { return ymax - ymin + 1; }

    float xmin, xmax, ymin, ymax;
};

struct Span
{
  Span() { }
  Span(int _x, int _y, int _width, int _coverage)
  : x(_x), y(_y), width(_width), coverage(_coverage) { }

  int x, y, width, coverage;
};

typedef std::vector<Span> Spans;

//
void RasterCallback(const int y,
                    const int count,
                    const FT_Span * const spans,
                    void * const user)
{
    Spans *sptr = (Spans *)user;
    for (int i = 0; i < count; ++i)
        sptr->push_back(Span(spans[i].x, y, spans[i].len, spans[i].coverage));
}

void RenderSpans(FT_Library &library,
                 FT_Outline * const outline,
                 Spans *spans)
{
    FT_Raster_Params params;
    memset(&params, 0, sizeof(params));
    params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
    params.gray_spans = RasterCallback;
    params.user = spans;

    FT_Outline_Render(library, outline, &params);
}

// TGA Header struct to make it simple to dump a TGA to disc.

#if defined(_MSC_VER) || defined(__GNUC__)
#pragma pack(push, 1)
#pragma pack(1)               // Dont pad the following struct.
#endif

struct TGAHeader
{
  uint8   idLength,           // Length of optional identification sequence.
          paletteType,        // Is a palette present? (1=yes)
          imageType;          // Image data type (0=none, 1=indexed, 2=rgb,
                              // 3=grey, +8=rle packed).
  uint16  firstPaletteEntry,  // First palette index, if present.
          numPaletteEntries;  // Number of palette entries, if present.
  uint8   paletteBits;        // Number of bits per palette entry.
  uint16  x,                  // Horiz. pixel coord. of lower left of image.
          y,                  // Vert. pixel coord. of lower left of image.
          width,              // Image width in pixels.
          height;             // Image height in pixels.
  uint8   depth,              // Image color depth (bits per pixel).
          descriptor;         // Image attribute flags.
};

#if defined(_MSC_VER) || defined(__GNUC__)
#pragma pack(pop)
#endif
bool
WriteTGA(const std::string &filename,
         const Pixel32 *pxl,
         uint16 width,
         uint16 height)
{
  std::ofstream file(filename.c_str(), std::ios::binary);
  if (file)
  {
    TGAHeader header;
    memset(&header, 0, sizeof(TGAHeader));
    header.imageType  = 2;
    header.width = width;
    header.height = height;
    header.depth = 32;
    header.descriptor = 0x20;

    file.write((const char *)&header, sizeof(TGAHeader));
    file.write((const char *)pxl, sizeof(Pixel32) * width * height);

    return true;
  }
  return false;
}

//
CTextureFont::CTextureFont()
{
}


CTextureFont::~CTextureFont()
{
	glDeleteTextures(1, &_textureId);
}


CTextureFont::CTextureFont(const char* fontFile, int fontSize)
{

	_fontSize = fontSize;
	_xStart = 0;
	_yStart = 0;

	memset(_character, 0, sizeof(_character));
	FT_Init_FreeType(&_library);
	assert(_library != 0);

	FT_New_Face(_library, fontFile, 0, &_face);
    FT_Set_Char_Size(_face, fontSize << 6, fontSize << 6, 72, 72);
    assert(_face != 0);

	glGenTextures(1, &_textureId);
	/**
	*   使用这个纹理id,或者叫绑定(关联)
	*/
	glBindTexture(GL_TEXTURE_2D, _textureId);
	/**
	*   指定纹理的放大,缩小滤波，使用线性方式，即当图片放大的时候插值方式
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);



	glTexImage2D(
		GL_TEXTURE_2D,      //! 指定是二维图片
		0,                  //! 指定为第一级别，纹理可以做mipmap,即lod,离近的就采用级别大的，远则使用较小的纹理
        GL_RGBA,           //! 纹理的使用的存储格式
		TEX_FONT_S,
		TEX_FONT_S,
		0,                  //! 是否的边
        GL_RGBA,           //! 数据的格式，bmp中，windows,操作系统中存储的数据是bgr格式
		GL_UNSIGNED_BYTE,   //! 数据是8bit数据
		0
	);

}


Character*  CTextureFont::getCharacter(wchar_t ch)
{
    if (_character[ch_arr_index][ch].x0 == 0 &&
        _character[ch_arr_index][ch].x0 == 0 &&
        _character[ch_arr_index][ch].x1 == 0 &&
        _character[ch_arr_index][ch].y1 == 0
		)
	{
		/**
		*   说明字符还没有绘制到纹理上，则进行绘制
		*/
        if (_xStart + _fontSize + 2*outlineWidth > TEX_FONT_S)
		{
			/**
			*   写满一行,从新开始
			*/
			_xStart = 0;
			/**
			*   y开始位置要增加
			*/
            _yStart += _fontSize + 2*outlineWidth;
		}
        FT_UInt cIndex = FT_Get_Char_Index(_face, ch);
        FT_Load_Glyph(_face, cIndex, FT_LOAD_DEFAULT);

        // Need an outline for this to work.
        if (_face->glyph->format == FT_GLYPH_FORMAT_OUTLINE)
        {
            // Render the basic glyph to a span list.
            Spans spans;
            RenderSpans(_library, &_face->glyph->outline, &spans);

            // Next we need the spans for the outline.
            Spans outlineSpans;

            // Set up a stroker.
            FT_Stroker stroker;
            FT_Stroker_New(_library, &stroker);
            FT_Stroker_Set(stroker,
                           (int)(outlineWidth * 64),
                           FT_STROKER_LINECAP_ROUND,
                           FT_STROKER_LINEJOIN_ROUND,
                           0);

            FT_Glyph glyph;
            if (FT_Get_Glyph(_face->glyph, &glyph) == 0)
            {
                FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1);
                // Again, this needs to be an outline to work.
                if (glyph->format == FT_GLYPH_FORMAT_OUTLINE)
                {
                    // Render the outline spans to the span list
                    FT_Outline *o =
                            &reinterpret_cast<FT_OutlineGlyph>(glyph)->outline;
                    RenderSpans(_library, o, &outlineSpans);
                }

                // Clean up afterwards.
                FT_Stroker_Done(stroker);
                FT_Done_Glyph(glyph);

                // Now we need to put it all together.
                if (!spans.empty())
                {
                    // Figure out what the bounding rect is for both the span lists.
                    Rect rect(spans.front().x,
                              spans.front().y,
                              spans.front().x,
                              spans.front().y);
                    for (Spans::iterator s = spans.begin();
                         s != spans.end(); ++s)
                    {
                        rect.Include(Vec2(s->x, s->y));
                        rect.Include(Vec2(s->x + s->width - 1, s->y));
                    }
                    for (Spans::iterator s = outlineSpans.begin();
                         s != outlineSpans.end(); ++s)
                    {
                        rect.Include(Vec2(s->x, s->y));
                        rect.Include(Vec2(s->x + s->width - 1, s->y));
                    }

#if 0
                    // This is unused in this test but you would need this to draw
                    // more than one glyph.
                    float bearingX = face->glyph->metrics.horiBearingX >> 6;
                    float bearingY = face->glyph->metrics.horiBearingY >> 6;
                    float advance = face->glyph->advance.x >> 6;
#endif

                    // Get some metrics of our image.
                    int imgWidth = rect.Width(),
                            imgHeight = rect.Height(),
                            imgSize = imgWidth * imgHeight;

                    // Allocate data for our image and clear it out to transparent.
                    Pixel32 *pxl = new Pixel32[imgSize];
                    memset(pxl, 0, sizeof(Pixel32) * imgSize);

                    // Loop over the outline spans and just draw them into the
                    // image.
                    for (Spans::iterator s = outlineSpans.begin();
                         s != outlineSpans.end(); ++s)
                        for (int w = 0; w < s->width; ++w)
                            pxl[(int)((imgHeight - 1 - (s->y - rect.ymin)) * imgWidth
                                      + s->x - rect.xmin + w)] =
                                    Pixel32(outlineCol.r, outlineCol.g, outlineCol.b,
                                            s->coverage);

                    // Then loop over the regular glyph spans and blend them into
                    // the image.
                    for (Spans::iterator s = spans.begin();
                         s != spans.end(); ++s)
                        for (int w = 0; w < s->width; ++w)
                        {
                            Pixel32 &dst =
                                    pxl[(int)((imgHeight - 1 - (s->y - rect.ymin)) * imgWidth
                                              + s->x - rect.xmin + w)];
                            Pixel32 src = Pixel32(fontCol.r, fontCol.g, fontCol.b,
                                                  s->coverage);
                            dst.r = (int)(dst.r + ((src.r - dst.r) * src.a) / 255.0f);
                            dst.g = (int)(dst.g + ((src.g - dst.g) * src.a) / 255.0f);
                            dst.b = (int)(dst.b + ((src.b - dst.b) * src.a) / 255.0f);
                            dst.a = min(255, dst.a + src.a);
                        }

                    // Dump the image to disk.
//                    std::string fileName = "/home/uk/t.tga";
//                    WriteTGA(fileName, pxl, imgWidth, imgHeight);

                    FT_Glyph glyph;
                    FT_Get_Glyph(_face->glyph, &glyph);

                    FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
                    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
                    FT_Bitmap& bitmap = bitmap_glyph->bitmap;
                    /**
                    *   如果没有数据，则不写，直接过去
                    */
                    if (bitmap.width == 0 || bitmap.rows == 0)
                    {
                        _xStart += _fontSize / 2;

                        _character[ch_arr_index][ch].x0 = _xStart;
                        _character[ch_arr_index][ch].y0 = _yStart;
                        _character[ch_arr_index][ch].x1 = _xStart + bitmap.width;
                        _character[ch_arr_index][ch].y1 = _yStart + bitmap.rows;
                        _character[ch_arr_index][ch].offsetY = 0;
                        _character[ch_arr_index][ch].offsetX = 0;

                    }
                    else
                    {
                        glBindTexture(GL_TEXTURE_2D, _textureId);

                        _character[ch_arr_index][ch].x0 = _xStart;
                        _character[ch_arr_index][ch].y0 = _yStart;
                        _character[ch_arr_index][ch].x1 = _xStart + imgWidth;
                        _character[ch_arr_index][ch].y1 = _yStart + imgHeight;

                        _character[ch_arr_index][ch].offsetY = bitmap_glyph->top;
                        _character[ch_arr_index][ch].offsetX = bitmap_glyph->left;

                        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                        glTexSubImage2D(
                            GL_TEXTURE_2D,
                            0,
                            _xStart,
                            _yStart,
                            imgWidth,
                            imgHeight,
                            GL_RGBA,
                            GL_UNSIGNED_BYTE,
                            (const char *)pxl
                        );
                        _xStart += imgWidth + 1;
                    }

                    delete [] pxl;
                }
            }
        }

	}
    return  &_character[ch_arr_index][ch];
}

void CTextureFont::genFontText(const wchar_t* text, LabelInfo &linfo)
{
	float       texWidth = TEX_FONT_S;
	float       texHeight = TEX_FONT_S;
	float       xStart = 0;
//	float       yStart = 0 + 16;
	float       yStart = 0;
	float       zStart = -1;
	unsigned    index = 0;
    unsigned    nSize = wcslen(text);
	float       fHeight = 0;

	if (nSize>FONT_LEN_MAX)
	{
		nSize = FONT_LEN_MAX;
	}

	linfo.textSize = nSize;

	for (unsigned i = 0; i < nSize; ++i)
	{
		Character*  ch = getCharacter(text[i]);

		int         h = ch->y1 - ch->y0;
		int         w = ch->x1 - ch->x0;
		float       offset = (float(h) - float(ch->offsetY));
		float       offsetX = float(ch->offsetX);

		//2  3
		//0  1
		/**
		*   第0个点
		*/
		int rows = i / NUMF_P_ROW;
		linfo.vert[index + 0][0] = xStart;
		linfo.vert[index + 0][1] = yStart + rows*FONT_SIZE;
		linfo.vert[index + 0][2] = ch->x0 / texWidth;
		linfo.vert[index + 0][3] = ch->y0 / texHeight;

		/**
		*   第1个点
		*/
		linfo.vert[index + 1][0] = w;
		linfo.vert[index + 1][1] = h;
// 		linfo.vert[index + 1][0] = xStart + w;
// 		linfo.vert[index + 1][1] = yStart - h + offset;
		linfo.vert[index + 1][2] = ch->x1 / texWidth;
		linfo.vert[index + 1][3] = ch->y0 / texHeight;
		/**
		*   第2个点
		*/
		linfo.vert[index + 2][0] = offsetX;
		linfo.vert[index + 2][1] = offset;
// 		linfo.vert[index + 2][0] = xStart;
// 		linfo.vert[index + 2][1] = yStart + offset;
		linfo.vert[index + 2][2] = ch->x0 / texWidth;
		linfo.vert[index + 2][3] = ch->y1 / texHeight;

		/**
		*   第3个点
		*/
		linfo.vert[index + 3][0] = xStart + w;
		linfo.vert[index + 3][1] = yStart + offset;
		linfo.vert[index + 3][2] = ch->x1 / texWidth;
		linfo.vert[index + 3][3] = ch->y1 / texHeight;


		index += 4;
		xStart += w + (ch->offsetX);

		if (i == (NUMF_P_ROW - 1))
		{
			xStart = 0;
		}
	}
    
}

void CTextureFont::drawText(float x, float y, const wchar_t* text)
{
	typedef float   TextVertex[5];
	TextVertex  vert[TEX_FONT_S];

	float       texWidth = TEX_FONT_S;
	float       texHeight = TEX_FONT_S;
	float       xStart = x;
	float       yStart = y + 16;
	float       zStart = -1;
	unsigned    index = 0;
	unsigned    nSize = wcslen(text);
	float       fHeight = 0;


	for (unsigned i = 0; i < nSize; ++i)
	{
		Character*  ch = getCharacter(text[i]);

		int         h = ch->y1 - ch->y0;
		int         w = ch->x1 - ch->x0;
		float       offset = (float(h) - float(ch->offsetY));
		float       offsetX = float(ch->offsetX);


		/**
		*   第一个点
		*/
		vert[index + 0][0] = xStart;
		vert[index + 0][1] = yStart - h + offset;
		vert[index + 0][2] = zStart;
		vert[index + 0][3] = ch->x0 / texWidth;
		vert[index + 0][4] = ch->y0 / texHeight;
		/**
		*   第二个点
		*/
		vert[index + 1][0] = xStart + w;
		vert[index + 1][1] = yStart - h + offset;
		vert[index + 1][2] = zStart;
		vert[index + 1][3] = ch->x1 / texWidth;
		vert[index + 1][4] = ch->y0 / texHeight;

		/**
		*   第三个点
		*/
		vert[index + 2][0] = xStart + w;
		vert[index + 2][1] = yStart + offset;
		vert[index + 2][2] = zStart;
		vert[index + 2][3] = ch->x1 / texWidth;
		vert[index + 2][4] = ch->y1 / texHeight;


		/**
		*   第四个点
		*/
		vert[index + 3][0] = xStart;
		vert[index + 3][1] = yStart + offset;
		vert[index + 3][2] = zStart;
		vert[index + 3][3] = ch->x0 / texWidth;
		vert[index + 3][4] = ch->y1 / texHeight;




		index += 4;
		xStart += w + (ch->offsetX);
	}

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);


    glVertexPointer(3, GL_FLOAT, sizeof(TextVertex), vert);
    glTexCoordPointer(2, GL_FLOAT, sizeof(TextVertex), &vert[0][3]);
    glDrawArrays(GL_QUADS, 0, index);
    
}

void CTextureFont::setFontColor(uint8 r, uint8 g, uint8 b)
{
    fontCol=Pixel32(b, g, r);

}
void CTextureFont::setOutlineColor(uint8 r, uint8 g, uint8 b)
{
    outlineCol=Pixel32(r, g, b);
}

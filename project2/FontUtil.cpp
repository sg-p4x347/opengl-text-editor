#include "pch.h"
#include "FontUtil.h"
#include "Font.h"
#include "Bitmap.h"
#include <GL/freeglut.h>
#include FT_CACHE_H

const path FontUtil::m_fontDirectory = "C:\\Windows\\Fonts";
const int FontUtil::m_tabSizeInSpaces = 5;
FontUtil& FontUtil::Get()
{
	static FontUtil utility;
	return utility;
}

FT_Error FontUtil::FaceRequester(FTC_FaceID faceId, FT_Library library, FT_Pointer requestData, FT_Face* fontFace)
{
	Font& font = *((Font*)faceId);
	string fontPath = (m_fontDirectory / (font.Name + ".ttf")).string();
	// Create the font face
	auto error = FT_New_Face(library,
		fontPath.c_str(),
		0,
		fontFace
	);
	LogError(error);
	//// Set the size in pixels
	//error = FT_Set_Pixel_Sizes(
	//	*fontFace,
	//	0,
	//	font.Size
	//);
	//LogError(error);
	return error;
}

FontUtil::FontUtil()
{
	// Initialize the freetype library
	FT_Error error = FT_Init_FreeType(&m_freetype);
	LogError(error);
	// Create the freetype cache manager
	FTC_Manager_New(
		m_freetype,
		16,
		128,
		100000000, // 100 MB
		FontUtil::FaceRequester,
		this,
		&m_cacheManager
	);
	FTC_ImageCache_New(
		m_cacheManager,
		&m_imageCache
	);
}

Font* FontUtil::GetFaceID(string font)
{
	shared_ptr<Font> faceId;
	if (!m_fonts.count(font)) {
		faceId = std::make_shared<Font>(font);
		m_fonts[font] = faceId;
	}
	else {
		faceId = m_fonts[font];
	}
	return faceId.get();
}

void FontUtil::LogError(FT_Error& error)
{
	if (error) {
		cout << "Freetype encountered an error: " << error << endl;
	}
}

void FontUtil::Render(Window& window, Vector2 position, string text, string font, int size, Color color)
{
	auto faceId = Get().GetFaceID(font);
	FT_Face face;
	FT_Error error = FTC_Manager_LookupFace(Get().m_cacheManager, faceId, &face);
	FT_GlyphSlot slot = face->glyph;

	for (char & ch : text)
	{
		if (ch != '\r') {
			bool tab = false;
			if (ch == '\t') {
				tab = true;
				ch = ' ';
			}
			//* retrieve glyph index from character code */
			FT_UInt glyph_index = FT_Get_Char_Index(face, ch);

			FT_Glyph glyph;
			FTC_Node node;
			FTC_ScalerRec scaler{
				faceId,
				0,
				size,
				1,
				0,
				0
			};

			error = FTC_ImageCache_LookupScaler(Get().m_imageCache, &scaler, FT_LOAD_DEFAULT, glyph_index, &glyph, &node);
			error = FT_Set_Pixel_Sizes(face, 0, size);
			LogError(error);
			error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
			LogError(error);
			
			/* convert to an anti-aliased bitmap */
			error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
			LogError(error);

			if (!tab) {
				Bitmap bmp(slot->bitmap.width, slot->bitmap.rows);
				for (int x = 0; x < bmp.GetWidth(); x++) {
					for (int y = 0; y < bmp.GetHeight(); y++) {
						uint8_t A = slot->bitmap.buffer[y * slot->bitmap.width + x];
						bmp.Set(x, (bmp.GetHeight() - 1) - y, Pixel(color.R * 255, color.G * 255, color.B * 255, ((A / 255.f) * color.A) * 255));
					}
				}
				Vector2 world = window.ScreenToWorld(Vector2(position.x + slot->bitmap_left, (position.y + slot->bitmap.rows) - slot->bitmap_top));
				glRasterPos2d(world.x, world.y);
				glDrawPixels(bmp.GetWidth(), bmp.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)bmp.GetPixels());

				position.x += slot->advance.x >> 6;
			}
			else {
				position.x += (slot->advance.x >> 6)* m_tabSizeInSpaces;
			}
		}
	}
}

uint32_t FontUtil::NearestCharacterIndex(string text, string font, int size, int offset)
{
	// The nearest index will always be the first index if the offset is negative
	if (offset <= 0)
		return 0;

	int cursorPos = 0;
	int cursorIndex = 0;
	auto faceId = Get().GetFaceID(font);
	FT_Face face;
	FT_Error error = FTC_Manager_LookupFace(Get().m_cacheManager, faceId, &face);
	FT_GlyphSlot slot = face->glyph;

	for (char& ch : text)
	{
		if (ch != '\r') {
			bool tab = false;
			if (ch == '\t') {
				tab = true;
				ch = ' ';
			}
			//* retrieve glyph index from character code */
			FT_UInt glyph_index = FT_Get_Char_Index(face, ch);

			FT_Glyph glyph;
			FTC_Node node;
			FTC_ScalerRec scaler{
				faceId,
				0,
				size,
				1,
				0,
				0
			};

			error = FTC_ImageCache_LookupScaler(Get().m_imageCache, &scaler, FT_LOAD_DEFAULT, glyph_index, &glyph, &node);
			error = FT_Set_Pixel_Sizes(face, 0, size);
			error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
			LogError(error);
			/* convert to an anti-aliased bitmap */
			error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
			LogError(error);

			int lastCursorPos = cursorPos;
			int lastCursorIndex = cursorIndex;

			if (!tab) {
				cursorPos += slot->advance.x >> 6;
			}
			else {
				cursorPos += (slot->advance.x >> 6)* m_tabSizeInSpaces;
			}
			cursorIndex++;

			if (lastCursorPos <= offset && cursorPos >= offset) {
				return (offset - lastCursorPos < cursorPos - offset) ? lastCursorIndex : cursorIndex;
			}
			
		}
	}

	// The offset does not lie between indices, therefore it must associate with the length of the text
	return cursorIndex;
}

int FontUtil::MeasureText(string text, string font, int size)
{
	int width = 0;
	auto faceId = Get().GetFaceID(font);
	FT_Face face;
	FT_Error error = FTC_Manager_LookupFace(Get().m_cacheManager, faceId, &face);
	FT_GlyphSlot slot = face->glyph;

	for (char& ch : text)
	{
		if (ch != '\r') {
			bool tab = false;
			if (ch == '\t') {
				tab = true;
				ch = ' ';
			}
			//* retrieve glyph index from character code */
			FT_UInt glyph_index = FT_Get_Char_Index(face, ch);

			FT_Glyph glyph;
			FTC_Node node;
			FTC_ScalerRec scaler{
				faceId,
				0,
				size,
				1,
				0,
				0
			};

			error = FTC_ImageCache_LookupScaler(Get().m_imageCache, &scaler, FT_LOAD_DEFAULT, glyph_index, &glyph, &node);
			error = FT_Set_Pixel_Sizes(face, 0, size);
			error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
			LogError(error);
			/* convert to an anti-aliased bitmap */
			error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
			LogError(error);
			if (!tab) {
				width += slot->advance.x >> 6;
			}
			else {
				width += (slot->advance.x >> 6)* m_tabSizeInSpaces;
			}
		}
	}
	return width;
}

vector<string> FontUtil::ListFonts()
{
	return vector<string>();
}

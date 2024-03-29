#pragma once
#include "pch.h"
#include "FontUtil.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_MANAGER_H
#include "Window.h"
#include "Color.h"
#include "Vector2.h"
#include "Font.h"
class FontUtil
{
public:
	static void Render(
		// Target window to render to
		Window& window, 
		// The screen position in pixels of the bottom left corner of the text
		Vector2 position, 
		// The text to render
		string text, 
		// The windows font to use (must be a valid .ttf file without the extension)
		string font,
		// Pixel height
		int size,
		// The color to use
		Color color
	);

	// Returns the index within the string that is closest to the pixel offset given
	static uint32_t NearestCharacterIndex(
		// The text to measure
		string text,
		// The font to use
		string font,
		// The font height in pixels
		int size,
		// The pixel offset from the start of the text
		int offset,
		// If true, the returned index will not extend beyond the offset
		bool noPass = false
	);
	// Returns the length of the given text rendered in the specified font and size
	static int MeasureText(
		// The text to measure
		string text,
		// The font to use
		string font,
		// The font height in pixels
		int size
	);
	static size_t GetMaxSize();
	static vector<vector<string>> ListFonts();
private:
	// Statics
	static FontUtil& Get();
	static const path m_fontDirectory;
	static const int m_tabSizeInSpaces;
	static const size_t m_maxSize;
	static FT_Error FaceRequester(FTC_FaceID faceId, FT_Library library, FT_Pointer requestData, FT_Face* fontFace);
	static void LogError(FT_Error& error);
	static FTC_SBit GetCharBitmap(FTC_FaceID faceId, int size, char ch, FTC_Node * node);
private:
	FontUtil();
	Font* GetFaceID(string font);

	FT_Library m_freetype;
	FTC_Manager m_cacheManager;
	FTC_ImageCache m_imageCache;
	FTC_CMapCache m_cMapCache;
	FTC_SBitCache m_sbitCache;

	map<string, shared_ptr<Font>> m_fonts;
};
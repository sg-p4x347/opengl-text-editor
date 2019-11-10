#include "pch.h"
#include "FontUtil.h"
#include "Font.h"
#include "Bitmap.h"
#include <GL/freeglut.h>
#include <filesystem>
#include FT_CACHE_H

const path FontUtil::m_fontDirectory = "C:\\Windows\\Fonts";
const int FontUtil::m_tabSizeInSpaces = 5;
const size_t FontUtil::m_maxSize = 128;
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
		m_maxSize,
		100000000, // 100 MB
		FontUtil::FaceRequester,
		this,
		&m_cacheManager
	);
	FTC_ImageCache_New(
		m_cacheManager,
		&m_imageCache
	);

	FTC_SBitCache_New(
		m_cacheManager,
		&m_sbitCache
	);
	FTC_CMapCache_New(
		m_cacheManager,
		&m_cMapCache
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

FTC_SBit FontUtil::GetCharBitmap(FTC_FaceID faceId, int size, char ch, FTC_Node * node)
{
	//* retrieve glyph index from character code */
	FT_UInt glyph_index = FTC_CMapCache_Lookup(Get().m_cMapCache, faceId, 0, ch);

	FTC_ScalerRec scaler;
	scaler.face_id = faceId;
	scaler.pixel = 1;
	scaler.height = size;
	scaler.width = 0;

	FTC_SBit bitmap;
	FTC_SBitCache_LookupScaler(Get().m_sbitCache, &scaler, FT_LOAD_DEFAULT | FT_LOAD_RENDER, glyph_index, &bitmap, node);

	return bitmap;
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
	for (char & ch : text)
	{
		FTC_Node node;
		auto bitmap = GetCharBitmap(faceId, size, ch, &node);
		switch (ch) {
			case '\r': break;
			case '\t': position.x += bitmap->xadvance * m_tabSizeInSpaces; break;
			default:
				Bitmap bmp(bitmap->width, bitmap->height);
				for (int x = 0; x < bmp.GetWidth(); x++) {
					for (int y = 0; y < bmp.GetHeight(); y++) {
						uint8_t A = bitmap->buffer[y * bitmap->width + x];
						bmp.Set(x, (bmp.GetHeight() - 1) - y, Pixel(color.R * 255, color.G * 255, color.B * 255, ((A / 255.f) * color.A) * 255));
					}
				}
				Vector2 world = window.ScreenToWorld(Vector2(position.x + bitmap->left, (position.y + bitmap->height) - bitmap->top));
				glRasterPos2d(world.x, world.y);
				glDrawPixels(bmp.GetWidth(), bmp.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)bmp.GetPixels());

				position.x += bitmap->xadvance;
				break;
		}
	}
}


uint32_t FontUtil::NearestCharacterIndex(string text, string font, int size, int offset, bool noPass)
{
	// The nearest index will always be the first index if the offset is negative
	if (offset <= 0)
		return 0;

	int lastCursorPos = 0;
	int cursorPos = 0;
	int cursorIndex = 0;
	auto faceId = Get().GetFaceID(font);
	for (char& ch : text)
	{
		FTC_Node node;
		auto bitmap = GetCharBitmap(faceId, size, ch, &node);
		switch (ch) {
		case '\r': break;
		case '\t': cursorPos += bitmap->xadvance * m_tabSizeInSpaces; break;
		default:
			cursorPos += bitmap->xadvance;

			if (lastCursorPos <= offset && cursorPos >= offset) {
				if (noPass)
					return std::max(0, cursorIndex);

				return (offset - lastCursorPos < cursorPos - offset) ? cursorIndex : cursorIndex + 1;
			}
			break;
		}
		cursorIndex++;
	}
	// The offset does not lie between indices, therefore it must associate with the length of the text
	return cursorIndex;
}

int FontUtil::MeasureText(string text, string font, int size)
{
	int width = 0;
	auto faceId = Get().GetFaceID(font);

	for (char& ch : text)
	{
		FTC_Node node;
		auto bitmap = GetCharBitmap(faceId, size, ch, &node);
		switch (ch) {
		case '\r': break;
		case '\t': width += bitmap->xadvance * m_tabSizeInSpaces; break;
		default: width += bitmap->xadvance; break;
		}
	}
	return width;
}

size_t FontUtil::GetMaxSize()
{
	return m_maxSize;
}

vector<vector<string>> FontUtil::ListFonts()
{
	vector<string> fontFamilyNames;
	vector<string> fileNames;
	for (const auto& dirEntry : std::filesystem::directory_iterator(m_fontDirectory))
		if (dirEntry.path().string()[dirEntry.path().string().size() - 1] == 'f') {
			string temp = dirEntry.path().string();
			temp.erase(0, m_fontDirectory.string().size() + 1);
			temp.erase(temp.end() - 4, temp.end());
			
			auto faceId = Get().GetFaceID(temp);
			FT_Face face;
			FTC_Manager_LookupFace(Get().m_cacheManager, faceId, &face);

			if (fontFamilyNames.size() == 0 || fontFamilyNames[fontFamilyNames.size() - 1] != face->family_name) {
				fontFamilyNames.push_back(face->family_name);
				fileNames.push_back(temp);
			}
		}
	return vector<vector<string>> { fontFamilyNames, fileNames };
}

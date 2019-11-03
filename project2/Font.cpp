#include "Font.h"

Font::Font(string fontname)
{
	font = fontname;
}

Font::~Font()
{
}

void Font::SetFont(string fontname)
{
	font = fontname;
}

string Font::GetFont(string font)
{
	return font;
}

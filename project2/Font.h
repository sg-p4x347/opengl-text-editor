#pragma once
#include <string>
#include "pch.h"

class Font {
	string font;
public:
	Font(
		string fontname
	);
	~Font();
	void SetFont(string font);
	string GetFont(string font);
};

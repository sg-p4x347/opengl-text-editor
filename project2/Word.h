#pragma once
#include "pch.h"
#include "Color.h"
struct Word
{
	Word(string text, string font, size_t size, Color color);
	string Text;
	string Font;
	size_t Size;
	Color Colour;
};


#pragma once
#include "pch.h"
struct Color
{
	// Default color is fully opaque black
	Color();
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	uint8_t R;
	uint8_t G;
	uint8_t B;
	uint8_t A;
};


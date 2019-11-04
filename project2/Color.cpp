#include "pch.h"
#include "Color.h"

Color::Color() : Color::Color(0, 0, 0, 255)
{
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : R(r), G(g), B(b), A(a)
{
}

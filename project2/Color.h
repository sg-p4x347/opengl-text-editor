#pragma once
#include "pch.h"

class Color {
public:
	Color(double r, double g, double b);
	void SetColor(double r, double g, double b);
	double GetRed();
	double GetGreen();
	double GetBlue();
	void SetRed(double r);
	void SetGreen(double g);
	void SetBlue(double b);

	double red;
	double green;
	double blue;
};
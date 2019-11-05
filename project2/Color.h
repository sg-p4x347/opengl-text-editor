#pragma once
#include "pch.h"

struct Color {
	Color(double r = 0.0, double g = 0.0, double b = 0.0, double a = 1.0);

	double R;
	double G;
	double B;
	double A;
};
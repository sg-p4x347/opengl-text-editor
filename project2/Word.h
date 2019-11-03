#pragma once
#include "Font.h"
#include "Color.h"
#include "pch.h"

class Word {
public:
	Word(
		string t, 
		string f, 
		double r,
		double g,
		double b
	);
	~Word();
	Font GetFont();
	string GetText();
	Color GetColor();
	void SetColor(double r, double g, double b);
	void SetFont(string f);
	void SetText(string text);
	void AddChar(char c);
	void DeleteChar();

	string text;
	Font font;
	Color color;



};

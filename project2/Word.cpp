#include "Word.h"

Word::Word(string t, string f, double r, double g, double b) : font(f),color(r,g,b)
	{
	text = t;
}

Word::~Word()
{
}

Font Word::GetFont()
{
	return font;
}

string Word::GetText()
{
	return text;
}

Color Word::GetColor()
{
	return color;
}

void Word::SetColor(double r, double g, double b)
{
	color.SetColor(r, g, b);
}

void Word::SetFont(string f)
{
	font.SetFont(f);
}

void Word::SetText(string t)
{
	text = t;
}

void Word::AddChar(char c)
{
	text += c;
}

void Word::DeleteChar()
{
	text = text.substr(0, text.size() - 1);
}

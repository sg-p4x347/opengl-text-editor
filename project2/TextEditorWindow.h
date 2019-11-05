#pragma once
#include "pch.h"
#include "Window.h"
#include "Vector2.h"
class TextEditorWindow : public Window
{
public:
	TextEditorWindow();
	static void ToggleCarat(int value);
private:
	int MeasureText(string text, void* font, int fontSize);

private:
	int m_lineSpacing;
	Vector2 m_editorPos;
	bool m_caratVisible;
	string m_text;
	int m_caratIndex;
	
};


#pragma once
#include "pch.h"
#include "Window.h"
class TextEditorWindow : public Window
{
public:
	TextEditorWindow();
private:
	int MeasureText(string text, void* font, int fontSize);

private:
	int m_lineSpacing;
};


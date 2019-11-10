#pragma once
#include "pch.h"
#include "Window.h"
#include "Vector2.h"
#include "TextEditor.h"
class TextEditorWindow : public Window
{
public:
	TextEditorWindow();
	static void ToggleCarat(int value);
private:
	int MeasureText(string text, void* font, int fontSize);
	void drawDocumentTabs();
private:
	int m_lineSpacing;
	Vector2 m_editorPos;
	bool m_caratVisible;
	string m_text;
	int m_caratIndex;
	int m_tabWidth;
	int m_tabHeight;
};


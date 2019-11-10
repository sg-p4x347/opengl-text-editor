#pragma once
#include "pch.h"
#include "Window.h"
#include "Vector2.h"
#include "TextEditor.h"
#include "Word.h"
class TextEditorWindow : public Window
{
public:
	TextEditorWindow();
	static void ToggleCarat(int value);
	virtual void DisplayFunc() override;
	virtual void MouseFunc(int button, int state, int x, int y) override;
	virtual void KeyboardFunc(unsigned char key, int x, int y) override;
private:
	int MeasureText(string text, void* font, int fontSize);
	uint32_t NearestCharacterIndex(Vector2 screenPos);
	void UpdateWords();
	void RenderWord(Word& word, Vector2 & pen, size_t & rowHeight, uint32_t & charIndex, uint32_t & carat, int& windowWidth);
	void InitMenu();
private:
	int m_lineSpacing;
	Vector2 m_editorPos;
	bool m_caratVisible;
	TextEditor m_textEditor;
	vector<Word> m_words;

	int m_mainMenuID;
	int m_fontMenuID;
	int m_sizeMenuID;
	int m_colorMenuID;
// Menu callbacks
public:
	static void MainMenuCallback(int entryID);
	static void FontMenuCallback(int entryID);
	static void SizeMenuCallback(int entryID);
	static void ColorMenuCallback(int entryID);
};


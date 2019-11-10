#pragma once
#include "pch.h"
#include "Window.h"
#include "Vector2.h"
#include "TextEditor.h"
#include "Word.h"
#include "Rectangle.h"
namespace ote {
	class TextEditorWindow : public Window
	{
	public:
		TextEditorWindow();
		// Callbacks
		virtual void DisplayFunc() override;
		virtual void MouseFunc(int button, int state, int x, int y) override;
		virtual void MotionFunc(int x, int y) override;
		virtual void KeyboardFunc(unsigned char key, int x, int y) override;
		virtual void SpecialFunc(int key, int x, int y) override;
		virtual void TimerFunc(int value) override;
	private:
		void InitMenu();
		int MeasureText(string text, void* font, int fontSize);
		uint32_t NearestCharacterIndex(Vector2 screenPos);
		uint32_t NearestCharacterIndex(vector<Word> row, int x);
		void UpdateWords();
		//void RenderWord(Word& word, Vector2& pen, size_t& rowHeight, uint32_t& charIndex, uint32_t& carat, int& windowWidth);
		vector<Word> GenerateRow(queue<Word>& words);
		size_t RowCharacterCount(vector<Word>& row);
		int CalculateRowHeight(vector<Word>& row);
		void RenderRow(vector<Word>& row, Vector2 pen, uint32_t& charIndex, Document& document);
		void MoveCarat(Document& document, uint32_t position, bool disableSelect = false, bool selectionOverride = true);
		void FillRect(Rectangle rect, Color color);
		void GlVertex(Vector2 point);
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
}


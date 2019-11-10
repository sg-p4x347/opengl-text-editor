#include "pch.h"
#include "TextEditorWindow.h"
#include <GL/glut.h>
#include "FontUtil.h"
#include "Font.h"

namespace ote {
	TextEditorWindow::TextEditorWindow() : 
		Window::Window("Text Editor", Vector2(), Vector2(400,400), 0.0, 1.0, 0.0, 1.0),
		m_lineSpacing(12),
		m_caratVisible(true),
		m_editorPos(0.f,0.f)

	{
		m_textEditor.NewDocument();
		InitMenu();
		glutTimerFunc(500,Window::TimerFuncDispatcher, m_id);
		UpdateWords();
	}

	int TextEditorWindow::MeasureText(string text, void* font, int fontSize)
	{
		return glutBitmapLength(font, (const unsigned char*)text.c_str());
	}

	uint32_t TextEditorWindow::NearestCharacterIndex(Vector2 screenPos)
	{

		queue<Word> words;
		for (auto& word : m_words) {
			words.push(word);
		}
		int rowBottom = 0;
		vector<Word> row;
		uint32_t charIndex = 0;
		while (screenPos.y > rowBottom) {
			row = GenerateRow(words);
			rowBottom += CalculateRowHeight(row);
			if (screenPos.y > rowBottom) {
				charIndex += RowCharacterCount(row);
			}
			if (row.size() == 0)
				return charIndex; // end of the file has been reached
		}
		return charIndex + NearestCharacterIndex(row, screenPos.x);
	}

	uint32_t TextEditorWindow::NearestCharacterIndex(vector<Word> row, int x)
	{
		int penX = m_editorPos.x;
		uint32_t charIndex = 0;
		for (int wordIndex = 0; wordIndex < row.size(); wordIndex++) {
			Word& word = row[wordIndex];
			uint32_t index = FontUtil::NearestCharacterIndex(word.Text, word.Font, word.Size, x - penX);
			charIndex += index;
			// If the index falls below the length or this is the last word in the row, this is the index
			if (index < word.Text.length() || wordIndex == row.size() - 1) {
				break;
			}

			penX += FontUtil::MeasureText(word.Text, word.Font, word.Size);
		}
		return charIndex;
	}

	void TextEditorWindow::UpdateWords()
	{
		m_words.clear();
		if (m_textEditor.GetActiveDocument()) {
			Document& document = *m_textEditor.GetActiveDocument();

			shared_ptr<Word> word = nullptr;
			string& text = document.GetText();
			for (int charIndex = 0; charIndex < document.GetLength(); charIndex++) {
				auto& fontStyle = document.GetFonts()[charIndex];
				auto& sizeStyle = document.GetSizes()[charIndex];
				auto& colorStyle = document.GetColors()[charIndex];

				// If no word exists, create one
				if (!word) {
					word = std::make_shared<Word>(string(1,text[charIndex]), fontStyle.Data, sizeStyle.Data, colorStyle.Data);
				}
				// If no styles have changed, add this character to the word
				else if (word->Font == fontStyle.Data && word->Size == sizeStyle.Data && word->Colour == colorStyle.Data) {
					word->Text += text[charIndex];
				}
				// Add this word to the m_words vector for later processing
				else {
					m_words.push_back(*word);
					// Clear the word and backtrack to start the next word with this character
					word = nullptr;
					charIndex--;
				}
			}
			if (word)
				m_words.push_back(*word);
		}
	}

	vector<Word> TextEditorWindow::GenerateRow(queue<Word>& words)
	{
		int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
		int x = m_editorPos.x;

		vector<Word> row;
		while (!words.empty()) {
			Word word = words.front();
			words.pop();
			
			size_t carriageReturnOffset = 0;
			if (word.Text[0] == '\r')
				carriageReturnOffset = 1;
			uint32_t index = std::min(FontUtil::NearestCharacterIndex(word.Text, word.Font, word.Size, windowWidth - x, true), (uint32_t)word.Text.find('\r', carriageReturnOffset));
			if (carriageReturnOffset <= index && index < word.Text.length() || x >= windowWidth) {
				if (index < word.Text.length()) {
					// Split this word and recursively render each part on separate lines
					Word left(word.Text.substr(0, index), word.Font, word.Size, word.Colour);
					word.Text = word.Text.substr(index);
					row.push_back(left);
				}
				// word wrap
				words.push(word);
				break;
			}
			else if (index == word.Text.length()) {
				row.push_back(word);
				x += FontUtil::MeasureText(word.Text, word.Font, word.Size);
			}
		}
		return row;
	}

	size_t TextEditorWindow::RowCharacterCount(vector<Word>& row)
	{
		size_t charCount = 0;
		for (auto& word : row)
			charCount += word.Text.length();
		return charCount;
	}

	int TextEditorWindow::CalculateRowHeight(vector<Word>& row)
	{
		int height = 0;
		for (auto& word : row)
			height = std::max(height, (int)word.Size);

		return height;
	}

	void TextEditorWindow::RenderRow(vector<Word>& row, Vector2 pen, uint32_t& charIndex, Document& document)
	{
		uint32_t carat = document.GetCaratPosition();
		uint32_t selectionStart = document.GetSelectionStart();
		for (Word& word : row) {
			FontUtil::Render(*this, pen, word.Text, word.Font, word.Size, word.Colour);

			uint32_t highlightStart = std::max(std::min(carat, selectionStart), charIndex);
			uint32_t highlightEnd = std::min(std::max(carat, selectionStart), charIndex + (uint32_t)word.Text.length());
			if (highlightStart < highlightEnd) {
				string highlightedText = word.Text.substr(highlightStart - charIndex, highlightEnd - highlightStart);
				FillRect(
					Rectangle(
						pen.x + FontUtil::MeasureText(word.Text.substr(0,highlightStart - charIndex),word.Font,word.Size), 
						pen.y - word.Size, 
						FontUtil::MeasureText(highlightedText, word.Font, word.Size), 
						word.Size
					), 
					Color(0.0,0.0,1.0,0.5)
				);
			}
			
			if (m_caratVisible) {
				// Render the carat here
				Vector2 caratPos = pen;
				if (carat == 0 && charIndex == 0 || carat > charIndex && carat <= charIndex + word.Text.length()) {

					//if (carat < word.Text.length()) {
						// Offset the carat within the word
						caratPos.x += FontUtil::MeasureText(word.Text.substr(0, carat - charIndex), word.Font, word.Size);
					//}

					glLineWidth(2);
					glColor3f(0, 0, 0);
					glBegin(GL_LINES);

					Vector2 start = ScreenToWorld(caratPos);
					glVertex2f(start.x, start.y);
					Vector2 end = ScreenToWorld(caratPos - Vector2(0, word.Size));
					glVertex2f(end.x, end.y);
					glEnd();
				}
			}
			pen.x += FontUtil::MeasureText(word.Text, word.Font, word.Size);
			charIndex += word.Text.length();
		}
	}

	void TextEditorWindow::MoveCarat(Document& document, uint32_t position, bool disableSelect, bool selectionOverride)
	{
		// Clamp the position to valid values
		position = std::max(0u, std::min((uint32_t)document.GetLength(), position));

		document.SetCaratPosition(position);
		if (disableSelect || selectionOverride && !(glutGetModifiers() & GLUT_ACTIVE_SHIFT)) {
			document.SetSelectionStart(position);
		}
	}

	void TextEditorWindow::FillRect(Rectangle rect, Color color)
	{
		glColor4d(color.R, color.G, color.B, color.A);
		glBegin(GL_POLYGON);
		GlVertex(Vector2(rect.X, rect.Y));
		GlVertex(Vector2(rect.X + rect.Width, rect.Y));
		GlVertex(Vector2(rect.X + rect.Width, rect.Y + rect.Height));
		GlVertex(Vector2(rect.X, rect.Y + rect.Height));
		glEnd();
	}

	void TextEditorWindow::GlVertex(Vector2 point)
	{
		point = ScreenToWorld(point);
		glVertex2f(point.x, point.y);
	}

void TextEditorWindow::InitMenu()
{
	m_fontMenuID = glutCreateMenu(TextEditorWindow::FontMenuDispatcher);

	m_sizeMenuID = glutCreateMenu(TextEditorWindow::SizeMenuDispatcher);
	for (int size = 8; size <= 14; ++size)
	{
		glutAddMenuEntry(std::to_string(size).c_str(), size);
	}

	m_colorMenuID = glutCreateMenu(TextEditorWindow::ColorMenuDispatcher);

	m_mainMenuID = glutCreateMenu(TextEditorWindow::MainMenuDispatcher);
	glutAddSubMenu("Change Active Font", m_fontMenuID);
	glutAddSubMenu("Change Active Size", m_sizeMenuID);
	glutAddSubMenu("Change Active Color", m_colorMenuID);
	glutAddMenuEntry("Save Current Text", 0);
	glutAddMenuEntry("Exit", -1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void TextEditorWindow::MainMenuFunc(int entryID)
{
	if (g_windows.count(glutGetWindow()))
	{
		TextEditorWindow* tw = (TextEditorWindow*)g_windows[glutGetWindow()].get();
		tw->MainMenuDispatcher(entryID);
	}
}

void TextEditorWindow::FontMenuFunc(int entryID)
{
	switch (entryID) {

	}
}

void TextEditorWindow::SizeMenuFunc(int entryID)
{
	switch (entryID)
	{
		
	}
}

void TextEditorWindow::ColorMenuFunc(int entryID)
{
	switch (entryID) {

	}
}

void TextEditorWindow::MainMenuDispatcher(int entryID)
{
	TextEditorWindow* tw = (TextEditorWindow*)g_windows[glutGetWindow()].get();
	TextEditor* te = tw->getTextEditor();
	Document* ad = te->GetActiveDocument().get();

	switch (entryID)
	{
	case 0:
		ad->Save();
	case -1:
		exit(0);
	}
}

TextEditor* TextEditorWindow::getTextEditor()
{
	return &m_textEditor;
}


	void TextEditorWindow::DisplayFunc()
	{
		int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
		if (m_textEditor.GetActiveDocument()) {
			Document& document = *m_textEditor.GetActiveDocument();
			// draw the background
			glClear(GL_COLOR_BUFFER_BIT);
			Vector2 pen = m_editorPos;
			uint32_t charIndex = 0;

			queue<Word> words;
			for (auto word : m_words) {
				words.push(word);
			}
			// Render one row at a time
			while (!words.empty()) {
				vector<Word> row = GenerateRow(words);
				pen.y += CalculateRowHeight(row);
				RenderRow(row,pen,charIndex,document);
				
			}

			// flush out the buffer contents
			glFlush();
		}
	}

	void TextEditorWindow::MouseFunc(int button, int state, int x, int y)
	{
		switch (button) {
		case GLUT_LEFT_BUTTON:
		{
			switch (state) {
			case GLUT_DOWN:
				if (m_textEditor.GetActiveDocument()) {
					Document& document = *m_textEditor.GetActiveDocument();
					MoveCarat(document, NearestCharacterIndex(Vector2(x, y)));
					DisplayFunc();
				}
				break;
			}
			break;
		}
		}
	}

	void TextEditorWindow::MotionFunc(int x, int y)
	{
		Window::MotionFunc(x, y);
		if (m_textEditor.GetActiveDocument()) {
			Document& document = *m_textEditor.GetActiveDocument();
			MoveCarat(document, NearestCharacterIndex(Vector2(x, y)), false, false);
			DisplayFunc();
		}
	}

	void TextEditorWindow::KeyboardFunc(unsigned char key, int x, int y)
	{
		Window::KeyboardFunc(key, x, y);
		if (m_textEditor.GetActiveDocument()) {
			Document& document = *m_textEditor.GetActiveDocument();
			uint32_t carat = document.GetCaratPosition();
			uint32_t selectionBegin = document.GetSelectionBegin();
			uint32_t selectionEnd = document.GetSelectionEnd();
			document.RemoveText(selectionBegin, selectionEnd - selectionBegin);
			switch (key) {
			case 8:
				if (selectionEnd - selectionBegin == 0 && carat >= 1) {
					document.RemoveText(carat - 1, 1);
					MoveCarat(document, carat - 1, true);
				}
				else {
					MoveCarat(document,selectionBegin, true);
				}
				UpdateWords();
				break;
			default:
				document.InsertText(string(1, (char)key), selectionBegin,document.GetFontAt(selectionBegin),document.GetSizeAt(selectionBegin),document.GetColorAt(selectionBegin));
				MoveCarat(document, selectionBegin + 1, true);
				

				UpdateWords();
				break;
			}

			DisplayFunc();
		}
	}

	void TextEditorWindow::SpecialFunc(int key, int x, int y)
	{
		Window::SpecialFunc(key, x, y);
		if (m_textEditor.GetActiveDocument()) {
			Document& document = *m_textEditor.GetActiveDocument();
			switch (key) {
			case GLUT_KEY_LEFT:
				MoveCarat(document, std::max(0u, document.GetCaratPosition() - 1));
				DisplayFunc();
				break;
			case GLUT_KEY_RIGHT:
				MoveCarat(document, std::min((uint32_t)document.GetLength(), (uint32_t)document.GetCaratPosition() + 1));
				DisplayFunc();
				break;
			case GLUT_KEY_UP:
				if (glutGetModifiers() & GLUT_ACTIVE_CTRL) {
					uint32_t carat = document.GetCaratPosition();
					uint32_t selectionBegin = std::min(document.GetSelectionStart(), carat);
					uint32_t selectionEnd = std::max(document.GetSelectionStart(), carat);
					document.SetSize(document.GetSizeAt(selectionBegin) + 1, selectionBegin, selectionEnd - selectionBegin);
					UpdateWords();
					DisplayFunc();
				}
				break;
			}
		}
	}
	void TextEditorWindow::TimerFunc(int value)
	{
		m_caratVisible = !m_caratVisible;
		DisplayFunc();
		glutTimerFunc(500, Window::TimerFuncDispatcher, value);
	}
}
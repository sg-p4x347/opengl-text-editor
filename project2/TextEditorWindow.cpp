#include "pch.h"
#include "TextEditorWindow.h"
#include <GL/glut.h>
#include "FontUtil.h"
#include "Font.h"

namespace ote {
	TextEditorWindow::TextEditorWindow() :
		Window::Window("Text Editor", Vector2(), Vector2(400, 400), 0.0, 1.0, 0.0, 1.0),
		m_lineSpacing(12),
		m_caratVisible(true),
		m_editorPos(0.f, 0.f)
	{
		m_textEditor.NewDocument();

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

			int fontIndex = 0;
			int sizeIndex = 0;
			int colorIndex = 0;

			uint32_t fontStart = 0;
			uint32_t sizeStart = 0;
			uint32_t colorStart = 0;

			uint32_t charIndex = 0;
			uint32_t styleStart = 0;
			uint32_t styleSize = 0;
			while (charIndex < document.GetLength()) {
				auto& fontStyle = document.GetFonts()[fontIndex];
				auto& sizeStyle = document.GetSizes()[sizeIndex];
				auto& colorStyle = document.GetColors()[colorIndex];

				uint32_t fontEnd = fontStart + fontStyle.Size - 1;
				uint32_t sizeEnd = sizeStart + sizeStyle.Size - 1;
				uint32_t colorEnd = colorStart + colorStyle.Size - 1;

				
				bool styleChanged = false;

				if (!(charIndex >= fontStart && charIndex <= fontEnd)) {
					fontIndex++;
					fontStart += fontStyle.Size;
					styleChanged = true;
				}
				if (!(charIndex >= sizeStart && charIndex <= sizeEnd)) {
					sizeIndex++;
					sizeStart += sizeStyle.Size;
					styleChanged = true;
				}
				if (!(charIndex >= fontStart && charIndex <= colorEnd)) {
					colorIndex++;
					colorStart += colorStyle.Size;
					styleChanged = true;
				}
				
				// A style has changed, render this section of the document
				if (styleChanged || charIndex == document.GetLength() - 1) {
					if (!styleChanged && charIndex == document.GetLength() - 1)
						styleSize++;
					string text = document.GetText().substr(styleStart, styleSize);
					m_words.push_back(Word(text, fontStyle.Data, sizeStyle.Data, colorStyle.Data));
					styleSize = 0;
					styleStart = charIndex;
				}
				charIndex++;
				styleSize++;
				
			}
		}
	}

	//void TextEditorWindow::RenderWord(Word& word, Vector2& pen, size_t& rowHeight, uint32_t& charIndex, uint32_t& carat, int& windowWidth)
	//{


	//	uint32_t index = FontUtil::NearestCharacterIndex(word.Text, word.Font, word.Size, windowWidth - pen.x);
	//	uint32_t carriageReturn = word.Text.find('\r');
	//	if (carriageReturn != string::npos && carriageReturn <= index) {
	//		index = carriageReturn;
	//		word.Text.erase(carriageReturn, 1);
	//		charIndex++;
	//	}
	//	if (index < word.Text.length() || pen.x >= windowWidth) {

	//		if (index < word.Text.length()) {
	//			// Split this word and recursively render each part on separate lines
	//			Word left(word.Text.substr(0, index), word.Font, word.Size, word.Colour);
	//			word.Text = word.Text.substr(index);
	//			RenderWord(left, pen, rowHeight, charIndex, carat, windowWidth);
	//		}
	//		// word wrap
	//		pen.x = m_editorPos.x;
	//		pen.y += rowHeight + m_lineSpacing;
	//		RenderWord(word, pen, rowHeight, charIndex, carat, windowWidth);
	//	}
	//	else if (index == word.Text.length()) {
	//		FontUtil::Render(*this, pen, word.Text, word.Font, word.Size, word.Colour);
	//		pen.x += FontUtil::MeasureText(word.Text, word.Font, word.Size);
	//		rowHeight = std::max(rowHeight, word.Size);

	//		if (m_caratVisible) {
	//			// Render the carat here
	//			Vector2 caratPos = pen;
	//			if (carat >= charIndex && carat <= charIndex + word.Text.length()) {

	//				if (carat < word.Text.length()) {
	//					// Offset the carat within the word
	//					caratPos.x += FontUtil::MeasureText(word.Text.substr(0, carat - charIndex), word.Font, word.Size);
	//				}

	//				glLineWidth(1);
	//				glColor3f(0, 0, 0);
	//				glBegin(GL_LINES);

	//				Vector2 start = ScreenToWorld(caratPos);
	//				glVertex2f(start.x, start.y);
	//				Vector2 end = ScreenToWorld(caratPos - Vector2(0, word.Size));
	//				glVertex2f(end.x, end.y);
	//				glEnd();
	//			}
	//		}
	//		charIndex += word.Text.length();
	//	}
	//}

	vector<Word> TextEditorWindow::GenerateRow(queue<Word>& words)
	{
		int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
		int x = m_editorPos.x;

		vector<Word> row;
		while (!words.empty()) {
			Word word = words.front();
			words.pop();
			uint32_t index = FontUtil::NearestCharacterIndex(word.Text, word.Font, word.Size, windowWidth - x, true);
			uint32_t carriageReturn = word.Text.find('\r');
			if (index < word.Text.length() || x >= windowWidth || carriageReturn != string::npos) {

				if (index < word.Text.length()) {
					// Split this word and recursively render each part on separate lines
					Word left(word.Text.substr(0, carriageReturn != string::npos ? index + 1 : index), word.Font, word.Size, word.Colour);
					word.Text = word.Text.substr(carriageReturn != string::npos ? index + 1 : index);
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
			uint32_t highlightEnd = std::min(std::max(carat, selectionStart), charIndex + word.Text.length());
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
				if (carat >= charIndex && carat <= charIndex + word.Text.length()) {

					//if (carat < word.Text.length()) {
						// Offset the carat within the word
						caratPos.x += FontUtil::MeasureText(word.Text.substr(0, carat - charIndex), word.Font, word.Size);
					//}

					glLineWidth(1);
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
		position = std::max(0u, std::min(document.GetLength(), position));

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

	void TextEditorWindow::ToggleCarat(int value)
	{
		/*if (g_windows.count(value)) {
			auto textEditorWindow = (TextEditorWindow*)(g_windows[value].get());
			textEditorWindow->m_caratVisible = !textEditorWindow->m_caratVisible;
			textEditorWindow->m_render();

		}
		glutTimerFunc(1000, TextEditorWindow::ToggleCarat, value);*/
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
			uint32_t selectionBegin = std::min(document.GetSelectionStart(), carat);
			uint32_t selectionEnd = std::max(document.GetSelectionStart(), carat);
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
				document.InsertText(string(1, (char)key), selectionBegin);
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
				MoveCarat(document, std::min(document.GetLength(), document.GetCaratPosition() + 1));
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
}
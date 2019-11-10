#include "pch.h"
#include "TextEditorWindow.h"
#include <GL/glut.h>
#include "FontUtil.h"
#include "Font.h"

TextEditorWindow::TextEditorWindow() : 
	Window::Window("Text Editor", Vector2(), Vector2(400,400), 0.0, 1.0, 0.0, 1.0),
	m_lineSpacing(12),
	m_caratVisible(true),
	m_editorPos(0.f,12.f)
	
{

	addToButtonVector(Button("untitled", Vector2(0.0f, 0.0f),0 ,0));
	
	m_textEditor.NewDocument();
}

int TextEditorWindow::MeasureText(string text, void* font, int fontSize)
{
	return glutBitmapLength(font, (const unsigned char*)text.c_str());
}

uint32_t TextEditorWindow::NearestCharacterIndex(Vector2 screenPos)
{
	return uint32_t();
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
				styleChanged = true;
			}
			if (!(charIndex >= sizeStart && charIndex <= sizeEnd)) {
				sizeIndex++;
				styleChanged = true;
			}
			if (!(charIndex >= fontStart && charIndex <= colorEnd)) {
				colorIndex++;
				styleChanged = true;
			}

			// A style has changed, render this section of the document
			if (styleChanged || charIndex == document.GetLength() - 1) {
				uint32_t styleStart = std::max(fontStart, std::max(sizeStart, colorStart));
				uint32_t styleSize = (std::min(fontEnd, std::min(sizeEnd, colorEnd)) - styleStart) + 1;
				string text = document.GetText().substr(styleStart, styleSize);
				m_words.push_back(Word(text, fontStyle.Data, sizeStyle.Data, colorStyle.Data));
			}
			charIndex++;
		}
	}
}

void TextEditorWindow::RenderWord(Word& word, Vector2& pen, size_t& rowHeight, uint32_t& charIndex, uint32_t& carat, int & windowWidth)
{
	

	uint32_t index = FontUtil::NearestCharacterIndex(word.Text, word.Font, word.Size, windowWidth - pen.x);
	uint32_t carriageReturn = word.Text.find('\r');
	if (carriageReturn != string::npos && carriageReturn <= index) {
		index = carriageReturn;
		word.Text.erase(carriageReturn, 1);
		charIndex++;
	}
	if (index < word.Text.length() || pen.x >= windowWidth) {

		if (index < word.Text.length()) {
			// Split this word and recursively render each part on separate lines
			Word left(word.Text.substr(0, index), word.Font, word.Size, word.Colour);
			word.Text = word.Text.substr(index);
			RenderWord(left, pen, rowHeight, charIndex, carat, windowWidth);
		}
		// word wrap
		pen.x = m_editorPos.x;
		pen.y += rowHeight + m_lineSpacing;
		RenderWord(word, pen, rowHeight, charIndex, carat, windowWidth);
	}
	else if (index == word.Text.length()) {
		FontUtil::DisplayFuncDispatcher(*this, pen, word.Text, word.Font, word.Size, word.Colour);
		pen.x += FontUtil::MeasureText(word.Text, word.Font, word.Size);
		rowHeight = std::max(rowHeight, word.Size);

		if (m_caratVisible) {
			// Render the carat here
			Vector2 caratPos = pen;
			if (carat >= charIndex && carat <= charIndex + word.Text.length()) {

				if (carat < word.Text.length()) {
					// Offset the carat within the word
					caratPos.x += FontUtil::MeasureText(word.Text.substr(0, carat - charIndex), word.Font, word.Size);
				}

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
		charIndex += word.Text.length();
	}
}

void TextEditorWindow::addToButtonVector(Button button)
{
	docTabs.push_back(button);
}

void TextEditorWindow::deleteFromButtonVector(Button button)
{
	// to do
}

void TextEditorWindow::createTabs()
{
	int i = 0;
	vector<shared_ptr<Document>> docs = m_textEditor.GetDocuments();
	for (shared_ptr<Document> doc : docs){
		string text = doc->GetName();
		double w = 50;
		double h = 12;
		double x = 4;
		double y = 13;
		glColor3f(0, 0, 1);
		glLineWidth(1);
		glBegin(GL_LINE_LOOP);
		Vector2 one((x + (w * i*1.1)),y);
		Vector2 two(((x + (w * i*1.1)) + w), y);
		Vector2 three(((x + (w * i*1.1)) + w), (y - h));
		Vector2 four((x + (w * i*1.1)), (y - h));
		one = Window::ScreenToWorld(one);
		two = Window::ScreenToWorld(two);
		three = Window::ScreenToWorld(three);
		four = Window::ScreenToWorld(four);
		glVertex2f((GLfloat)one.x, (GLfloat)one.y);
		glVertex2f((GLfloat)two.x, (GLfloat)two.y);
		glVertex2f((GLfloat)three.x, (GLfloat)three.y);
		glVertex2f((GLfloat)four.x, (GLfloat)four.y);
		glEnd();
		glRasterPos2f(one.x+.007,one.y+.01);
		if (text.size() > 7) {
			text = text.substr(0, 7) + "...";
		}
		
		for (int i = 0; i < text.size(); ++i)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, text.at(i));
		}



		i= i+1;
	}
	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex2f(0, 0.9675);
	glVertex2f(1, 0.9675);
	glEnd();
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
		size_t rowHeight = 0;
		uint32_t charIndex = 0;
		uint32_t carat = document.GetCaratPosition();
		for (auto word : m_words) {
			RenderWord(word, pen, rowHeight, charIndex, carat, windowWidth);

		}
		// Draw tabs
		createTabs();

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
			//m_textEditor.GetActiveDocument()->SetCaratPosition(FontUtil::NearestCharacterIndex(m_text, font, fontSize, x - m_editorPos.x));
			DisplayFunc();
			break;
		}
		case 1:
			cout << x << " " << y;
		break;
	}
	}
}

void TextEditorWindow::KeyboardFunc(unsigned char key, int x, int y)
{
	Window::KeyboardFunc(key, x, y);
	if (m_textEditor.GetActiveDocument()) {
		Document& document = *m_textEditor.GetActiveDocument();
		uint32_t carat = document.GetCaratPosition();
		switch (key) {
		case 8:
			document.RemoveText(carat - 1, 1);
			document.SetCaratPosition(std::max(0u, carat - 1));
			if (!(glutGetModifiers() & GLUT_ACTIVE_SHIFT)) {
				document.SetSelectionStart(document.GetCaratPosition());
			}
			UpdateWords();
			break;
		default:
			document.InsertText(string(1, (char)key), carat);
			document.SetCaratPosition(std::max(0u, carat + 1));
			if (!(glutGetModifiers() & GLUT_ACTIVE_SHIFT)) {
				document.SetSelectionStart(document.GetCaratPosition());
			}
			UpdateWords();
			break;
		}

		DisplayFunc();
	}
}

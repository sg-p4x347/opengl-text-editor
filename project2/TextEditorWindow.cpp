#include "pch.h"
#include "TextEditorWindow.h"
#include <GL/glut.h>
#include "FontUtil.h"
#include "Font.h"

TextEditorWindow::TextEditorWindow() : 
	Window::Window("Text Editor", Vector2(), Vector2(400,400), 0.0, 1.0, 0.0, 1.0),
	m_lineSpacing(8),
	m_caratVisible(true),
	m_text(""),
	m_caratIndex(0),
	m_tabWidth(50),
	m_tabHeight(50)
{	
	int fontSize = 36;
	string font = "times";
	SetRender([=]() {
		// draw the background
		glClear(GL_COLOR_BUFFER_BIT);
		Vector2 position = m_editorPos;
		
		position.y += fontSize;

		FontUtil::Render(*this, position, m_text, font, fontSize, Color());
		

		int caratPos = FontUtil::MeasureText(m_text.substr(0, m_caratIndex), font, fontSize);
		// Render the carat
		if (m_caratVisible) {
			glLineWidth(4);
			glColor3f(0, 0, 0);
			glBegin(GL_LINES);
			
			Vector2 start = ScreenToWorld(Vector2(caratPos + m_editorPos.x, position.y));
			glVertex2f(start.x, start.y);
			Vector2 end = ScreenToWorld(Vector2(caratPos + m_editorPos.x, position.y - fontSize));
			glVertex2f(end.x,end.y);
			glEnd();
		}
		// flush out the buffer contents
		glFlush();
	});
	SetMouseFunc([=](int button, int state, int x, int y) {
		switch (button) {
		case GLUT_LEFT_BUTTON:
		{
			switch (state) {
			case GLUT_DOWN:
				m_caratIndex = FontUtil::NearestCharacterIndex(m_text, font, fontSize, x - m_editorPos.x);
				m_render();
				break;
			}
			break;
		}
		}
	});
	SetKeyboardFunc([=](unsigned char key, int x, int y) {
		switch (key) {
		case 8:
			m_text = m_text.erase(m_caratIndex - 1, 1);
			m_caratIndex = std::max(0, m_caratIndex - 1);
			break;
		default:
			m_text.insert(m_caratIndex,1, (char)key);
			m_caratIndex++;
			break;
		}
		
		m_render();
	});
	
}

int TextEditorWindow::MeasureText(string text, void* font, int fontSize)
{
	return glutBitmapLength(font, (const unsigned char*)text.c_str());
}

void TextEditorWindow::drawDocumentTabs()
{
	vector<string> names = m_textEditor.getTabNames();
	int length = names.size();
	glBegin(GL_POLYGON);
	int i = 0;
	double y = 0;
	double x = 0;
	while( i < length){
		x = x - m_tabWidth * i;
		glVertex2f((GLfloat)x, (GLfloat)y);
		glVertex2f((GLfloat)x, (GLfloat)y-m_tabHeight);
		glVertex2f((GLfloat)x+m_tabWidth, (GLfloat)y-m_tabHeight);
		glVertex2f((GLfloat)x+m_tabWidth, (GLfloat)y);

		
		i++;
}

}


void TextEditorWindow::ToggleCarat(int value)
{
	if (g_windows.count(value)) {
		auto textEditorWindow = (TextEditorWindow*)(g_windows[value].get());
		textEditorWindow->m_caratVisible = !textEditorWindow->m_caratVisible;
		textEditorWindow->m_render();
		
	}
	glutTimerFunc(1000, TextEditorWindow::ToggleCarat, value);
}

#include "pch.h"
#include "TextEditorWindow.h"
#include <GL/glut.h>

TextEditorWindow::TextEditorWindow() : Window::Window("Text Editor", Vector2(), Vector2(400,400), 0.0, 1.0, 0.0, 1.0)
{
	m_render = []() {
		// draw the background
		glClear(GL_COLOR_BUFFER_BIT);
		// flush out the buffer contents
		glFlush();

	};
}

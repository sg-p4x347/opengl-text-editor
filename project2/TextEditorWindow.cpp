#include "pch.h"
#include "TextEditorWindow.h"
#include <GL/glut.h>


TextEditorWindow::TextEditorWindow() : Window::Window("Text Editor", Vector2(), Vector2(400,400), 0.0, 1.0, 0.0, 1.0),
m_lineSpacing(8)
{
	SetRender([=]() {
		// draw the background
		glClear(GL_COLOR_BUFFER_BIT);
		Vector2 world = ScreenToWorld(Vector2(0.5, 0.5));
		glPushMatrix();
		glScalef(1.f / 50.f, 1.f / 50.f, 1.f / 50.f);
		glRasterPos2d(world.x, world.y);
		glColor3d(0, 0, 0);
		glutStrokeCharacter(GLUT_STROKE_ROMAN, 'W');
		//glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'A');
		// flush out the buffer contents
		glFlush();
		glPopMatrix();

	});
}

int TextEditorWindow::MeasureText(string text, void* font, int fontSize)
{
	return glutBitmapLength(font, (const unsigned char*)text.c_str());
}

#include "pch.h"
#include "HelpWindow.h"
#include <GL/freeglut.h>

HelpWindow::HelpWindow() : Window::Window("Help",Vector2(600,0),Vector2(700,800),0,1,0,1),
m_splashScreen(Bitmap::FromFile("help_splash_screen.bmp"))
{
}

void HelpWindow::DisplayFunc()
{
	Select();
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawPixels(m_splashScreen.GetWidth(), m_splashScreen.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, m_splashScreen.GetPixels());
}

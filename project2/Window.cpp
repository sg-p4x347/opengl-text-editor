#include "pch.h"
#include "Window.h"
#include <GL/glut.h>				// include GLUT library


Window::Window(
	string title, 
	Vector2 position, 
	Vector2 size, 
	double left, 
	double right,
	double bottom, 
	double top
) :
	m_title(title), 
	m_left(left), 
	m_right(right), 
	m_bottom(bottom), 
	m_top(top)
{
	// specify a window size
	glutInitWindowSize(size.x, size.y);
	// specify a window position
	glutInitWindowPosition(position.x, position.y);
	m_id = glutCreateWindow(title.c_str());
	// specify a viewing area
	gluOrtho2D(left, right, bottom, top);
	// Register callbacks
	glutDisplayFunc(Window::Render);
	glutIdleFunc(Window::IdleFuncDispatcher);
	glutMouseFunc(Window::MouseFuncDispatcher);
	glutMotionFunc(Window::MotionFuncDispatcher);
	glutKeyboardFunc(Window::KeyboardFuncDispatcher);
	glutKeyboardUpFunc(Window::KeyboardUpFuncDispatcher);
	glutSpecialFunc(Window::SpecialFuncDispatcher);
	glutSpecialUpFunc(Window::SpecialUpFuncDispatcher);
	glutCreateMenu(Window::MainMenuDispatcher);
	glutCreateMenu(Window::FontMenuDispatcher);
	glutCreateMenu(Window::SizeMenuDispatcher);
	glutCreateMenu(Window::ColorMenuDispatcher);
	// Initialize rendering configurations
	glutInitDisplayMode(GLUT_ALPHA);
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Window::~Window()
{
	Delete(m_id);
}

void Window::Select()
{
	glutSetWindow(m_id);
}

int Window::GetID()
{
	return m_id;
}

void Window::Show()
{
	Select();
	glutShowWindow();
}

void Window::Hide()
{
	Select();
	glutHideWindow();
}


Vector2 Window::ScreenToWorld(Vector2 screen)
{
	Select();
	float xRatio = (screen.x / (float)glutGet(GLUT_WINDOW_WIDTH)) * (m_right - m_left);
	float yRatio = (1.f - (screen.y / (float)glutGet(GLUT_WINDOW_HEIGHT))) * (m_top - m_bottom);
	return Vector2(
		xRatio + m_left,
		yRatio + m_bottom
	);
}


void Window::Render()
{
	if (g_windows.count(glutGetWindow()))
		g_windows[glutGetWindow()]->DisplayFunc();
}

void Window::IdleFuncDispatcher()
{
	if (g_windows.count(glutGetWindow()))
		g_windows[glutGetWindow()]->IdleFunc();
}

void Window::MouseFuncDispatcher(int button, int state, int x, int y)
{
	if (g_windows.count(glutGetWindow()))
		g_windows[glutGetWindow()]->MouseFunc(button, state, x, y);
}

void Window::MotionFuncDispatcher(int x, int y)
{
	if (g_windows.count(glutGetWindow()))
		g_windows[glutGetWindow()]->MotionFunc(x, y);
}

void Window::KeyboardFuncDispatcher(unsigned char key, int x, int y)
{
	if (g_windows.count(glutGetWindow()))
		g_windows[glutGetWindow()]->KeyboardFunc(key, x, y);
}

void Window::KeyboardUpFuncDispatcher(unsigned char key, int x, int y)
{
	if (g_windows.count(glutGetWindow()))
		g_windows[glutGetWindow()]->KeyboardUpFunc(key, x, y);
}

void Window::SpecialFuncDispatcher(int key, int x, int y)
{
	if (g_windows.count(glutGetWindow()))
		g_windows[glutGetWindow()]->SpecialFunc(key, x, y);
}

void Window::SpecialUpFuncDispatcher(int key, int x, int y)
{
	if (g_windows.count(glutGetWindow()))
		g_windows[glutGetWindow()]->SpecialUpFunc(key, x, y);
}

void Window::TimerFuncDispatcher(int value)
{
	if (g_windows.count(glutGetWindow()))
		g_windows[glutGetWindow()]->TimerFunc(value);
}

void Window::MainMenuDispatcher(int entryID)
{
	if (g_windows.count(glutGetWindow()))
		g_windows[glutGetWindow()]->MainMenuFunc(entryID);
}

void Window::FontMenuDispatcher(int entryID)
{
	if (g_windows.count(glutGetWindow()))
		g_windows[glutGetWindow()]->FontMenuFunc(entryID);
}

void Window::SizeMenuDispatcher(int entryID)
{
	if (g_windows.count(glutGetWindow()))
		g_windows[glutGetWindow()]->SizeMenuFunc(entryID);
}

void Window::ColorMenuDispatcher(int entryID)
{
	if (g_windows.count(glutGetWindow()))
		g_windows[glutGetWindow()]->ColorMenuFunc(entryID);
}

void Window::DisplayFunc()
{
}

void Window::IdleFunc()
{
}

void Window::MouseFunc(int button, int state, int x, int y)
{
}

void Window::MotionFunc(int x, int y)
{
}

void Window::KeyboardFunc(unsigned char key, int x, int y)
{
	m_keys[key] = true;
}

void Window::KeyboardUpFunc(unsigned char key, int x, int y)
{
	m_keys[key] = false;
}

void Window::SpecialFunc(int key, int x, int y)
{
	m_specialKeys[key] = true;
}

void Window::SpecialUpFunc(int key, int x, int y)
{
	m_specialKeys[key] = false;
}

void Window::TimerFunc(int value)
{
}

void Window::MainMenuFunc(int entryID)
{
}

void Window::FontMenuFunc(int entryID)
{
}

void Window::SizeMenuFunc(int entryID)
{
}

void Window::ColorMenuFunc(int entryID)
{
}

shared_ptr<Window> Window::Create(shared_ptr<Window> window)
{
	g_windows.insert(std::make_pair(window->GetID(), window));
	return window;
}

void Window::Delete(int id)
{
	g_windows.erase(id);
}

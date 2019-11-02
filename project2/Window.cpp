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
	// specify a background clor: white
	glClearColor(1, 1, 1, 0);
	// specify a viewing area
	gluOrtho2D(left, right, bottom, top);
	glutDisplayFunc(Window::Render);
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

void Window::SetRender(std::function<void()>&& render)
{
	m_render = render;
}

void Window::Render()
{
	g_windows[glutGetWindow()]->m_render();
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

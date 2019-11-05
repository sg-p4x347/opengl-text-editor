#pragma once
#include "pch.h"
#include "Vector2.h"
//------------------------------------------------------
// Encapsulate window functionality

class Window {
public:
	Window(
		string title, 
		Vector2 position,
		Vector2 size, 
		double left, 
		double right, 
		double bottom, 
		double top
	);
	~Window();
	void Select();
	int GetID();
	void Show();
	void Hide();
	Vector2 ScreenToWorld(Vector2 screen);
	// Callbacks
	void SetRender(std::function<void()>&& render);
	void SetIdle(std::function<void()>&& idle);
	void SetMouseFunc(std::function<void(int, int, int, int)>&& mouseFunc);
	void SetKeyboardFunc(std::function<void(unsigned char, int, int)>&& keyboardFunc);
	static void Render();
	static void Idle();
	static void MouseFunc(int button, int state, int x, int y);
	static void KeyboardFunc(unsigned char key, int x, int y);

	static shared_ptr<Window> Create(shared_ptr<Window> window);
	static void Delete(int id);
protected:

	int m_id;
	string m_title;
	double m_left;
	double m_right;
	double m_bottom;
	double m_top;

	std::function<void()> m_render;
	std::function<void()> m_idle;
	std::function<void(int, int, int, int)> m_mouseFunc;
	std::function<void(unsigned char, int, int)> m_keyboardFunc;
};
static map<int, shared_ptr<Window>> g_windows;
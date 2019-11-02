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
		double top, 
		std::function<void()>&& render = []() {}
	);
	~Window();
	void Select();
	int GetID();
	void Show();
	void Hide();
	Vector2 ScreenToWorld(Vector2 screen);
	static void Render();
	static void Create(shared_ptr<Window> window);
	static void Delete(int id);
protected:

	int m_id;
	string m_title;
	double m_left;
	double m_right;
	double m_bottom;
	double m_top;
	std::function<void()> m_render;
};
static map<int, shared_ptr<Window>> g_windows;
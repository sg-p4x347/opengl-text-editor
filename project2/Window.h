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
	// Dispatchers
	static void Render();
	static void IdleFuncDispatcher();
	static void MouseFuncDispatcher(int button, int state, int x, int y);
	static void MotionFuncDispatcher(int x, int y);
	static void KeyboardFuncDispatcher(unsigned char key, int x, int y);
	static void KeyboardUpFuncDispatcher(unsigned char key, int x, int y);
	static void SpecialFuncDispatcher(int key, int x, int y);
	static void SpecialUpFuncDispatcher(int key, int x, int y);

	// Virtual callbacks
	virtual void DisplayFunc();
	virtual void IdleFunc();
	virtual void MouseFunc(int button, int state, int x, int y);
	virtual void MotionFunc(int x, int y);
	virtual void KeyboardFunc(unsigned char key, int x, int y);
	virtual void KeyboardUpFunc(unsigned char key, int x, int y);
	virtual void SpecialFunc(int key, int x, int y);
	virtual void SpecialUpFunc(int key, int x, int y);

	static shared_ptr<Window> Create(shared_ptr<Window> window);
	static void Delete(int id);
protected:

	int m_id;
	string m_title;
	double m_left;
	double m_right;
	double m_bottom;
	double m_top;

	map<char, bool> m_keys;
	map<int, bool> m_specialKeys;
};
static map<int, shared_ptr<Window>> g_windows;
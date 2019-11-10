#pragma once
#include "Window.h"
#include "Bitmap.h"
class HelpWindow :
	public Window
{
public:
	HelpWindow();

	virtual void DisplayFunc() override;

private:
	Bitmap m_splashScreen;
};


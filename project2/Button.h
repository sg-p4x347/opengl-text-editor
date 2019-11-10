#pragma once
#include <string>
#include "Vector2.h"

using std::string;

class Button
{
private:
	string m_text;
	Vector2 m_topLeftPosition;
	double m_height;
	double m_width;

public:
	Button();
	Button(
		string buttonText,
		Vector2 topLeftVertex,
		double height,
		double width
	);
	~Button();

	void setText(string text);
	string getText();
	void setPosition(Vector2 position);
	Vector2 getPosition();
	void setHeight(double height);
	double getHeight();
	void setWidth(double width);
	double getWidth();
};


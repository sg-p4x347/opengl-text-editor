#include "pch.h"
#include "Button.h"
#include "pch.h"
Button::Button()
{
	setText("button");
	setPosition(Vector2(0, 0));
	setHeight(10);
	setWidth(10);
}

Button::Button(string buttonText, Vector2 topLeftVertex, double height, double width)
{
	setText(buttonText);
	setPosition(topLeftVertex);
	setHeight(height);
	setWidth(width);
}

Button::~Button()
{
}

void Button::setText(string text)
{
	m_text = text;
}

string Button::getText()
{
	return m_text;
}

void Button::setPosition(Vector2 position)
{
	m_topLeftPosition = position;
}

Vector2 Button::getPosition()
{
	return m_topLeftPosition;
}

void Button::setHeight(double height)
{
	m_height = height;
}

double Button::getHeight()
{
	return m_height;
}

void Button::setWidth(double width)
{
	m_width = width;
}

double Button::getWidth()
{
	return m_width;
}
#include "pch.h"
#include "Document.h"

Document::Document()
{
	setName("document");
	setCaratPosition(0);
}

Document::Document(string text, vector<string> fonts, vector<Color> colors, vector<int> sizes, string name, int caratPosition)
{
	setText(text);
	setFont(fonts);
	setColor(colors);
	setSize(sizes);
	setName("CopyOf" + name);
	setCaratPosition(caratPosition);
}

Document::~Document()
{
}

void Document::setText(string text)
{
	m_text = text;
}

void Document::insertText(string text, int insertIndex)
{
	m_text.insert(insertIndex, text);
}

void Document::removeText(int startIndex, int endIndex)
{
	m_text.erase(startIndex, endIndex);
}

string Document::getText()
{
	return m_text;
}

void Document::setFont(vector<string> fonts)
{
	m_fonts = fonts;
}

void Document::changeFont(string font, int startIndex, int endIndex)
{
	for (int index = startIndex; index <= endIndex; ++index)
	{
		m_fonts[index] = font;
	}
}

void Document::insertFont(string font, int index)
{
	// Not implemented
	assert(false);
}

void Document::removeFont(int startIndex, int endIndex)
{
	m_fonts.erase(m_fonts.begin() + startIndex, m_fonts.begin() + endIndex);
}

string Document::getFont(int index)
{
	return m_fonts[index];
}

void Document::setColor(vector<Color> colors)
{
	m_colors = colors;
}

void Document::changeColor(Color color, int startIndex, int endIndex)
{
	for (int index = startIndex; index <= endIndex; ++index)
	{
		m_colors[index] = color;
	}
}

void Document::insertColor(Color color, int index)
{
	// Not implemented
	assert(false);
}

void Document::removeColor(int startIndex, int endIndex)
{
	m_colors.erase(m_colors.begin() + startIndex, m_colors.begin() + endIndex);
}

Color Document::getColor(int index)
{
	return m_colors[index];
}

void Document::setSize(vector<int> sizes)
{
	m_sizes = sizes;
}

void Document::changeSize(int size, int startIndex, int endIndex)
{
	for (int index = startIndex; index <= endIndex; ++index)
	{
		m_sizes[index] = size;
	}
}

void Document::insertSize(int size, int index)
{
	// Not implemented
	assert(false);
}

void Document::removeSize(int startIndex, int endIndex)
{
	m_sizes.erase(m_sizes.begin() + startIndex, m_sizes.begin() + endIndex);
}

int Document::getSize(int index)
{
	return m_sizes[index];
}

void Document::setName(string name)
{
	m_name = name;
}

string Document::getName()
{
	return m_name;
}

void Document::setCaratPosition(int position)
{
	m_caratPosition = position;
}

int Document::getCaratPosition()
{
	return m_caratPosition;
}


void Document::save()
{
	ofstream outFile;
	outFile.open(defaultFilePath + getName() + defaultFileExtension);
	outFile << getText();
	outFile.close();
}

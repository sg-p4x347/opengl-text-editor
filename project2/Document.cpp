#include "pch.h"
#include "Document.h"
#include "FontUtil.h"
Document::Document() : 
	m_name("document"),
	m_caratPosition(0),
	m_selectionStart(0),

	m_defaultFont("times"),
	m_defaultSize(24),
	m_defaultColor(0,0,0,1)

{
}

Document::~Document()
{
}
string& Document::GetText()
{
	return m_text;
}
size_t Document::GetLength()
{
	return m_text.length();
}
void Document::InsertText(string text, uint32_t index, string font, size_t size, Color color)
{
	// Extend the current styles at the insertion location
	InsertStyle(m_fonts, index, text.length(), font);
	InsertStyle(m_sizes, index, text.length(), size);
	InsertStyle(m_colors, index, text.length(), color);
	// Insert the text into the entire document string
	m_text.insert(index, text);
}
void Document::RemoveText(uint32_t index, size_t size)
{
	if (size > 0) {
		// Remove the current styles for this interval
		RemoveStyle(m_fonts, index, size);
		RemoveStyle(m_sizes, index, size);
		RemoveStyle(m_colors, index, size);
		// Remove the interval from the document string
		m_text.erase(index, size);
	}
}
void Document::SetFont(string font, uint32_t start, uint32_t size)
{
	SetStyle(m_fonts, start, Style<string>(size, font));
}
void Document::SetSize(size_t fontSize, uint32_t start, uint32_t size)
{
	SetStyle(m_sizes, start, Style<size_t>(size, std::min(FontUtil::GetMaxSize(), fontSize)));
}
void Document::SetColor(Color color, uint32_t start, uint32_t size)
{
	SetStyle(m_colors, start, Style<Color>(size, color));
}
void Document::SetName(string name)
{
	m_name = name;
}

string Document::GetName()
{
	return m_name;
}

void Document::SetCaratPosition(uint32_t position)
{
	m_caratPosition = position;
}

uint32_t Document::GetCaratPosition()
{
	return m_caratPosition;
}

void Document::SetSelectionStart(uint32_t position)
{
	m_selectionStart = position;
}

uint32_t Document::GetSelectionStart()
{
	return m_selectionStart;
}

uint32_t Document::GetSelectionBegin()
{
	return std::min(m_selectionStart, m_caratPosition);
}

uint32_t Document::GetSelectionEnd()
{
	return std::max(m_selectionStart, m_caratPosition);
}

string Document::GetFontAt(uint32_t index)
{
	return GetStyle(m_fonts, index, m_defaultFont);
}

uint32_t Document::GetSizeAt(uint32_t index)
{
	return GetStyle(m_sizes, index, m_defaultSize);
}

Color Document::GetColorAt(uint32_t index)
{
	return GetStyle(m_colors, index, m_defaultColor);
}

vector<Style<string>> & Document::GetFonts()
{
	return m_fonts;
}

vector<Style<Color>> & Document::GetColors()
{
	return m_colors;
}

vector<Style<size_t>> & Document::GetSizes()
{
	return m_sizes;
}

void Document::SetFont(string font)
{
	m_defaultFont = font;
	SetStyle(m_fonts, GetSelectionBegin(), Style<string>(GetSelectionEnd() - GetSelectionBegin(), font));
}

void Document::SetSize(size_t size)
{
	m_defaultSize = size;
	SetStyle(m_sizes, GetSelectionBegin(), Style<size_t>(GetSelectionEnd() - GetSelectionBegin(), size));
}

void Document::SetColor(Color color)
{
	m_defaultColor = color;
	SetStyle(m_colors, GetSelectionBegin(), Style<Color>(GetSelectionEnd() - GetSelectionBegin(), color));
}

void Document::Save()
{
	ofstream outFile;
	outFile.open(m_defaultFilePath + GetName() + m_defaultFileExtension);
	outFile << m_text;
	outFile.close();
}

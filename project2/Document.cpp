#include "pch.h"
#include "Document.h"

Document::Document() : 
	m_name("document"),
	m_caratPosition(0),
	m_selectionStart(0),

	m_defaultFont("times"),
	m_defaultSize(12),
	m_defaultColor(0,0,0,1),

	m_fonts {Style<string>(0, m_defaultFont)},
	m_sizes {Style<size_t>(0, m_defaultSize)},
	m_colors {Style<Color>(0, m_defaultColor)}
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
void Document::InsertText(string text, uint32_t index)
{
	// Extend the current styles at the insertion location
	ExtendStyle(m_fonts, index, text.length());
	ExtendStyle(m_sizes, index, text.length());
	ExtendStyle(m_colors, index, text.length());
	// Insert the text into the entire document string
	m_text.insert(index, text);
}
void Document::RemoveText(uint32_t index, size_t size)
{
	// Remove the current styles for this interval
	RemoveStyle(m_fonts, index, size);
	RemoveStyle(m_sizes, index, size);
	RemoveStyle(m_colors, index, size);
	// Remove the interval from the document string
	m_text.erase(index, size);
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

string Document::GetFontAt(uint32_t index)
{
	return GetStyle(m_fonts, index);
}

uint32_t Document::GetSizeAt(uint32_t index)
{
	return GetStyle(m_sizes, index);
}

Color Document::GetColorAt(uint32_t index)
{
	return GetStyle(m_colors, index);
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

void Document::Save()
{
	ofstream outFile;
	outFile.open(m_defaultFilePath + GetName() + m_defaultFileExtension);
	outFile << m_text;
	outFile.close();
}

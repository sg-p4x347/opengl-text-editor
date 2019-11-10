#pragma once
#include "pch.h"
#include "Color.h"
#include "Style.h"

using std::ofstream;

class Document
{
public:
	Document();
	~Document();
	
	string& GetText();
	size_t GetLength();

	void InsertText(string text, uint32_t index, string font, size_t size, Color color);
	void RemoveText(uint32_t index, size_t size);

	void SetFont(string font, uint32_t start, uint32_t size);
	void SetSize(size_t fontSize, uint32_t start, uint32_t size);
	void SetColor(Color color, uint32_t start, uint32_t size);

	void SetName(string name);
	string GetName();

	void SetCaratPosition(uint32_t position);
	uint32_t GetCaratPosition();

	void SetSelectionStart(uint32_t position);
	uint32_t GetSelectionStart();

	uint32_t GetSelectionBegin();
	uint32_t GetSelectionEnd();

	string GetFontAt(uint32_t index);
	uint32_t GetSizeAt(uint32_t index);
	Color GetColorAt(uint32_t index);

	vector<Style<string>> & GetFonts();
	vector<Style<Color>> & GetColors();
	vector<Style<size_t>> & GetSizes();

	void SetFont(string font);
	void SetSize(size_t size);
	void SetColor(Color color);

	void Save();
private:
	template<typename DataType>
	void SetStyle(vector<Style<DataType>>& styles, uint32_t start, Style<DataType>&& style);

	// Returns the starting style index where changes took place
	template<typename DataType>
	void RemoveStyle(vector<Style<DataType>>& styles, uint32_t start, size_t size);

	template<typename DataType>
	void InsertStyle(vector<Style<DataType>>& styles, uint32_t start, size_t size, DataType value);

	template<typename DataType>
	DataType GetStyle(vector<Style<DataType>>& styles, uint32_t index, DataType defaultValue);
private:
	string m_text;

	string m_defaultFont;
	Color m_defaultColor;
	size_t m_defaultSize;

	vector<Style<string>> m_fonts;
	vector<Style<Color>> m_colors;
	vector<Style<size_t>> m_sizes;
	
	string m_name;
	uint32_t m_caratPosition;
	uint32_t m_selectionStart;
	string m_defaultFilePath = "C:/Temp/";
	string m_defaultFileExtension = ".txt";
};

template<typename DataType>
inline void Document::SetStyle(vector<Style<DataType>>& styles, uint32_t start, Style<DataType>&& style)
{
	for (int i = start; i < start + style.Size; i++)
		styles[i] = style;
}

template<typename DataType>
inline void Document::RemoveStyle(vector<Style<DataType>>& styles, uint32_t start, size_t size)
{
	styles.erase(styles.begin() + start, styles.begin() + start + size);
}

template<typename DataType>
inline void Document::InsertStyle(vector<Style<DataType>>& styles, uint32_t start, size_t size, DataType value)
{
	// Push current styles back to accomodate the new style
	uint32_t currentStart = 0;
	int styleIndex = 0;
	for (int i = 0; i < size; i++) {
		styles.insert(styles.begin() + start, Style<DataType>(size, value));
	}
}

template<typename DataType>
inline DataType Document::GetStyle(vector<Style<DataType>>& styles, uint32_t index, DataType defaultValue)
{
	if (index < styles.size())
		return styles[index].Data;

	return defaultValue;
}

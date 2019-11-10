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

	void InsertText(string text, uint32_t index);
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

	string GetFontAt(uint32_t index);
	uint32_t GetSizeAt(uint32_t index);
	Color GetColorAt(uint32_t index);

	vector<Style<string>> & GetFonts();
	vector<Style<Color>> & GetColors();
	vector<Style<size_t>> & GetSizes();

	void Save();
private:
	template<typename DataType>
	void SetStyle(vector<Style<DataType>>& styles, uint32_t start, Style<DataType>&& style);

	// Returns the starting style index where changes took place
	template<typename DataType>
	uint32_t RemoveStyle(vector<Style<DataType>>& styles, uint32_t start, size_t size);

	template<typename DataType>
	void ExtendStyle(vector<Style<DataType>>& styles, uint32_t start, size_t size, DataType defaultValue);

	template<typename DataType>
	DataType GetStyle(vector<Style<DataType>>& styles, uint32_t index);
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
	uint32_t insertionIndex = RemoveStyle(styles, start, style.Size);
	// Insert the new style
	if (insertionIndex == styles.size() || styles.size() == 0) {
		styles.push_back(style);
	}
	else {
		styles.insert(styles.begin() + insertionIndex, style);
	}
}

template<typename DataType>
inline uint32_t Document::RemoveStyle(vector<Style<DataType>>& styles, uint32_t start, size_t size)
{
	uint32_t end = start + size - 1; // Minus 1 to be inclusive of the end
	uint32_t i = 0;
	uint32_t currentStart = 0;
	while (i < styles.size()) {
		Style<DataType> currentStyle = styles[i];
		uint32_t currentEnd = currentStart + currentStyle.Size - 1; // Minus 1 to be inclusive of the end
		// If an intersection exists, handle one of the 4 cases
		if (start <= currentEnd && currentStart <= end) {
			
			// The new style completely overlaps the current style
			if (start <= currentStart && end >= currentEnd) {
				styles.erase(styles.begin() + i);
				if (end == currentEnd) {
					break;
				}
			}
			// The new style is inserted into the middle of the current style
			else if (currentStart < start && currentEnd > end) {
				currentStyle.Size = start - currentStart;
				styles[i] = currentStyle;
				i++;
				// Split the current style into two, and insert the second one right after the first
				currentStyle.Size = currentEnd - end;
				styles.insert(styles.begin() + i, currentStyle);

				break;
			}
			// The new style overlaps the end of the current style
			else if (end >= currentEnd) {
				currentStyle.Size = start - currentStart;
				styles[i] = currentStyle;
				i++;
			}
			// The new style overlaps the start of the current style
			else if (start <= currentStart) {
				currentStyle.Size = currentEnd - end;
				styles[i] = currentStyle;
				break;
			}
		

		}
		else {
			i++;
		}
		currentStart += currentStyle.Size;
	}
	return i;
}

template<typename DataType>
inline void Document::ExtendStyle(vector<Style<DataType>>& styles, uint32_t start, size_t size, DataType defaultValue)
{
	// Push current styles back to accomodate the new style
	uint32_t currentStart = 0;
	int styleIndex = 0;
	for (auto& currentStyle : styles) {
		uint32_t currentEnd = currentStart + currentStyle.Size - 1;
		if (start >= currentStart && start <= currentEnd || styleIndex == styles.size() - 1) {
			currentStyle.Size += size;
			return;
		}
		styleIndex++;
	}
	// Add default style
	styles.push_back(Style<DataType>(size, defaultValue));
}

template<typename DataType>
inline DataType Document::GetStyle(vector<Style<DataType>>& styles, uint32_t index)
{
	uint32_t currentStart = 0;
	for (auto& style : styles) {
		uint32_t currentEnd = currentStart + style.Size - 1;
		if (index >= currentStart && index <= currentEnd) {
			return style.Data;
		}
		currentStart += style.Size;
	}
	// This should never be reached
	return DataType();
}

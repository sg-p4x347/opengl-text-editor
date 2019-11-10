#pragma once
#include "pch.h"
#include "Color.h"
#include <fstream>

using std::ofstream;

class Document
{
private:
	string m_text;
	vector<string> m_fonts;
	vector<Color> m_colors;
	vector<int> m_sizes;
	string m_name;
	int m_caratPosition;
	string defaultFilePath = "C:/Temp/";
	string defaultFileExtension = ".txt";
public:
	Document();
	Document(
		string text,
		vector<string> fonts,
		vector<Color> colors,
		vector<int> sizes,
		string name,
		int caratPosition
	);
	~Document();
	void setText(string text);
	void insertText(string text, int insertIndex);
	void removeText(int startIndex, int endIndex);
	string getText();
	void setFont(vector<string> fonts);
	void changeFont(string font, int startIndex, int endIndex);
	void insertFont(string font, int index);
	void removeFont(int startIndex, int endIndex);
	string getFont(int index);
	void setColor(vector<Color> colors);
	void changeColor(Color color, int startIndex, int endIndex);
	void insertColor(Color color, int index);
	void removeColor(int startIndex, int endIndex);
	Color getColor(int index);
	void setSize(vector<int> sizes);
	void changeSize(int size, int startIndex, int endIndex);
	void insertSize(int size, int index);
	void removeSize(int startIndex, int endIndex);
	int getSize(int index);
	void setName(string name);
	string getName();
	void setCaratPosition(int position);
	int getCaratPosition();
	void save();
};


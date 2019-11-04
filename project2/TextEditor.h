#pragma once
#include "pch.h"
#include "Document.h"
class TextEditor
{
private:
	vector<Document> documents;
	Document* m_active;
public:
	TextEditor();
};


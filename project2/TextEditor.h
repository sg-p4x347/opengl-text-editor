#pragma once
#include "pch.h"
#include "Document.h"
class TextEditor
{
private:
	vector<shared_ptr<Document>> m_documents;
	shared_ptr<Document> m_active;
public:
	TextEditor();
	shared_ptr<Document> GetActiveDocument();
	vector<shared_ptr<Document>> GetDocuments();
	void NewDocument();
};


#pragma once
#include "Document.h"
class TextEditor
{
private:
	vector<shared_ptr<Document>> m_documents;
	shared_ptr<Document> m_active;
public:
	TextEditor();
	void SetActiveDocument(shared_ptr<Document> doc);
	shared_ptr<Document> GetActiveDocument();
	vector<shared_ptr<Document>> GetDocuments();
	void NewDocument();
};


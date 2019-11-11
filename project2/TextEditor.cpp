#include "pch.h"
#include "TextEditor.h"

TextEditor::TextEditor()
{
}

void TextEditor::SetActiveDocument(shared_ptr<Document> doc)
{
	m_active = doc;
}

shared_ptr<Document> TextEditor::GetActiveDocument()
{
	return m_active;
}

vector<shared_ptr<Document>> TextEditor::GetDocuments()
{
	return m_documents;
}

void TextEditor::NewDocument()
{
	auto document = std::make_shared<Document>();
	m_active = document;
	m_documents.push_back(document);
}

#include "TextEditor.h"


vector<string> TextEditor::getTabNames()
{
	vector<string> res;
	for (Document item : documents) {
		res.push_back(item.getName);
	}
	return res;
}
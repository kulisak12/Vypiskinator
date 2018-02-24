#include "itemList.h"

void setTextProperty(std::string* source, std::string typeSign) {
	std::string textPropertyTag = (typeSign == "_") ? "sub" : "sup";
	int textEnd;
	// add the start sub or sup tag
	int textStart = source->find(typeSign);
	while (textStart != std::string::npos) {
		source->replace(textStart, 1, "<" + textPropertyTag + ">");
		textEnd = textStart + 6;
		// if the supscripted or superscripted text is longer than 1 character
		if ((*source)[textStart + 5] == '{') {
			source->erase(textStart + 5, 1);
			textEnd = source->find("}", textStart);
			if (textEnd == std::string::npos) {
				throw 1;
			}
			source->erase(textEnd, 1);
		}

		// add the closing tag
		source->insert(textEnd, "</" + textPropertyTag + ">");
		// look for another instance
		textStart = source->find(typeSign, textEnd + 4);
	}
}
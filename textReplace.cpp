#include "stdafx.h"
#include <string>

// replace all occurences of std::string by a different one
void textReplace(std::string* source, std::string toReplace, std::string replaceWith) {
	int element;
	element = source->find(toReplace);
	while (element != std::string::npos) {
		source->replace(element, toReplace.length(), replaceWith);
		element = source->find(toReplace, element + replaceWith.length());
	}
}
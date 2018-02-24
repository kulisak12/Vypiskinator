#include "itemList.h"

extern std::vector<std::string> code;

// lighter version of power function
int power(int base, int exponent) {
	if (exponent == 0) {
		return 1;
	}
	return base * power(base, exponent - 1);
}

void writeListData(std::vector<itemList> &setOfLists, std::string listType, std::string* lineStart, int indent) {
	void textReplace(std::string* source, std::string toReplace, std::string replaceWith);
	int searchForList(std::vector<itemList> &lists, int indent);
	std::vector<itemList>::iterator currentList;
	int space = -1;
	int firstIndex;
	std::string listSign = (listType == "-") ? "-" : ")";

	// remove tabs and dash in the code
	*lineStart = code[line].substr(0, code[line].find(listSign) + 2);
	textReplace(lineStart, "\t", "");

	if (listSign == ")") {
		space = lineStart->find_last_not_of("-)0123456789abcdefghijklmnopqrstuvwxyz", lineStart->length() - 2);
		if (space == std::string::npos) {
			space = -1;
		}
	}
	// write list data to object
	int current = searchForList(setOfLists, indent);
	if (current == -1) {
		setOfLists.push_back(0);
		currentList = setOfLists.end() - 1;
		currentList->indentLevel = indent;
		currentList->terminated = 0;
		currentList->itemLines.push_back(line);
		// set list start attribute
		if (listSign == ")") {
			currentList->firstIndex = lineStart->substr(space + 1, lineStart->length() - space - 3);
		}
		if (listType == "a") {
			// convert letters to numerical representation
			int startNumber = 0;
			for (int i = 0; i < currentList->firstIndex.length(); i++) {
				startNumber += power(26, currentList->firstIndex.length() - i - 1) * ((int) currentList->firstIndex[i] - 96);
			}
			currentList->firstIndex = std::to_string(startNumber);
		}
	} else {
		currentList = setOfLists.begin() + current;
		currentList->itemLines.push_back(line);

	}

	// remove list sign and space after it
	lineStart->erase(space + 1);
	code[line].replace(0, code[line].find(listSign) + 2, *lineStart);
}
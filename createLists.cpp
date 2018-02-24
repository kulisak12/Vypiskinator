#include "itemList.h"

extern std::vector<std::string> code;

void createLists(std::vector<itemList> &setOfLists, std::string listType) {
	for (int i = 0; i < setOfLists.size(); i++) {
		// for every list
		code[setOfLists[i].itemLines[0]].insert(0,
		(listType == "-") ? "<ul><li>" : ("<ol type=\"" + listType + "\" start=\"" + setOfLists[i].firstIndex + "\"><li>"));
		for (int j = 1; j < setOfLists[i].itemLines.size(); j++) {
			// for every list item
			code[setOfLists[i].itemLines[j] - 1].append("</li>");
			code[setOfLists[i].itemLines[j]].insert(0, "<li>");
		}
		code[setOfLists[i].lastLine].append((listType == "-") ? "</li></ul>" : "</li></ol>");
	}
}
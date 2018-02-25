#ifndef __ITEM_LIST_H__
#define __ITEM_LIST_H__
#include <vector>
#include <string>

class itemList {
	public:
		itemList(...) {} // for push_back
		std::vector<int> itemLines;
		int lastLine, indentLevel;
		std::string firstIndex;
		bool terminated = 1;
};

#endif
#include <iostream>
#include <fstream>
#include <vector>
#include "createHeading.cpp"
#include "createLists.cpp"
#include "entityToUtf8.cpp"
#include "searchForList.cpp"
#include "setTextProperty.cpp"
#include "textReplace.cpp"
#include "writeListData.cpp"
#include "itemList.h"

int line = 0;
std::vector<std::string> code;

int main() {
	std::string entityToUtf8(int entityCode);
	void textReplace(std::string* source, std::string toReplace, std::string replaceWith);
	void createHeading(std::string* source, std::string toReplace, std::string replaceWith);
	void writeListData(std::vector<itemList> &setOfLists, std::string listType, std::string* lineStart, int indent);
	void createLists(std::vector<itemList> &setOfLists, std::string listType);
	void setTextProperty(std::string* source, std::string typeSign);
	
	std::ifstream sourceFile;
	std::ofstream outputFile;
	
	try {
		sourceFile.open("note.html", std::ios::in);
		if (!sourceFile.is_open()) {
			std::cerr << "Couldn't open source file\n";
			throw 1;
		}
		outputFile.open("output.html", std::ios::out | std::ios::trunc);
		if (!outputFile.is_open()) {
			std::cerr << "Couldn't open output file\n";
			sourceFile.close();
			throw 2;
		}
	} catch (int x) {
		// keep the console open
		std::cout << "Press enter to exit";
		std::cin.clear();
		std::cin.sync();
		std::cin.get();
		return x;
	}
	
	std::string* thisLine = new std::string;
	int entity, entityStart, entityEnd;
	int indent, space;
	std::vector<itemList> bulletLists;
	std::vector<itemList> numberLists;
	std::vector<itemList> letterLists;

	// extract code from file and store it line by line
	while (std::getline(sourceFile, *thisLine)) {
		// replace HTML entities
		entityStart = thisLine->find("&#");
		while (entityStart != std::string::npos) {
			entityEnd = thisLine->find(';', entityStart);
			// extract the number from the entity syntax
			entity = std::stoi(thisLine->substr(entityStart + 2, entityEnd - entityStart - 2));
			thisLine->replace(entityStart, entityEnd - entityStart + 1, entityToUtf8(entity));
			entityStart = thisLine->find("&#", entityStart);
		}

		// change arrows
		textReplace(thisLine, "-&gt;", "&rarr;");
		// replace font-size with h2 and h3
		createHeading(thisLine, "<span style=\"font-size:16px\">", "<h2>");
		createHeading(thisLine, "<span style=\"font-size:13px\">", "<h3>");
		// delete all color changing spans
		textReplace(thisLine, "<span style=\"color:#000000\">", "");
		textReplace(thisLine, "</span>", "");
		// create subscripted and superscripted text
		try {
			setTextProperty(thisLine, "_");
			setTextProperty(thisLine, "^");
		} catch (int x) {
			std::cerr << "Found opening curly brace but no closing one\n";
			// keep the console open
			std::cout << "Press enter to exit";
			std::cin.clear();
			std::cin.sync();
			std::cin.get();
		}

		code.push_back(*thisLine);
		// mark the position of a list
		space = thisLine->find(" ");
		if (space == std::string::npos) {
			space = thisLine->length();
		}
		*thisLine = thisLine->substr(0, space);
		// remove formatting tags
		textReplace(thisLine, "<b>", "");
		textReplace(thisLine, "</b>", "");
		textReplace(thisLine, "<u>", "");
		textReplace(thisLine, "</u>", "");
		textReplace(thisLine, "<i>", "");
		textReplace(thisLine, "</i>", "");
		indent = thisLine->find_first_not_of("\t");
		thisLine->erase(0, indent);

		// check if line marks a list item
		if (*thisLine != "") {
			if (*thisLine == "-") {
				writeListData(bulletLists, "-", thisLine, indent);
			} else if (thisLine->substr(thisLine->length() - 1, 1) == ")") {
				if ((*thisLine)[thisLine->length() - 2] >= 48
				&& (*thisLine)[thisLine->length() - 2] < 58) { // 0 to 9
					writeListData(numberLists, "1", thisLine, indent);
				} else {
					writeListData(letterLists, "a", thisLine, indent);
				}
			} else {
				textReplace(&code[line], "\t", "&#8193;");
			}

			// terminate lists of higher indent
			searchForList(bulletLists, indent);
			searchForList(numberLists, indent);
			searchForList(letterLists, indent);
		}
		line++;
		if (code[line - 1].find("</p>") != std::string::npos) {
			// terminate all lists
			searchForList(bulletLists, -1);
			searchForList(numberLists, -1);
			searchForList(letterLists, -1);
		}
	}

	// change header
	code[0] = "<!--conversion by kulisak-->\n<!DOCTYPE html>\n";

	// create lists
	createLists(bulletLists, "-");
	createLists(numberLists, "1");
	createLists(letterLists, "a");

	// create output file
	for (int i = 0; i < line - 1; i++) {
		outputFile << code[i] << std::endl;
	}
	outputFile << code[line - 1] << "</html>";
	delete thisLine;
	thisLine = NULL;
	sourceFile.close();
	outputFile.close();
	return 0;
}
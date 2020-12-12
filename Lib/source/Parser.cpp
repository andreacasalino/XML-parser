/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <fstream>
#include <Parser.h>
#include "ErrorHandler.h"
#include "Tag.h"
#include <sstream>

namespace xmlPrs {
	std::list<std::string>  sliceFragments(const std::string& toSplit) {
		std::istringstream iss(toSplit);
		std::list<std::string> slices;
		while (!iss.eof()) {
			slices.emplace_back(std::string());
			iss >> slices.back();
			if(slices.back().empty()) slices.pop_back();
		}
		return slices;
	}

	Parser::Parser(const std::string& source_file) {
		auto tags = sliceTags(source_file);
		tags.pop_front();  // skpi preamble
		if(tags.size() >= 2) {
			auto end = tags.end();
			--end;
			this->root = Tag::Parse(tags.begin(), end, nullptr);
		}
		if(nullptr == this->root) {
			this->root = std::make_shared<Tag>("Root", nullptr);
			ErrorHandler::handle("invalid number of tags. An empty Parser will be built");
		}
	}

	Parser::Parser() {
		this->root = std::make_shared<Tag>("Root", nullptr);
	}

	Parser::~Parser(){
	}

	void Parser::Reprint(const std::string& file_name) {
		std::ofstream f(file_name);
		if (!f.is_open()) {
			ErrorHandler::handle("invalid file for printing the xml");
			return;
		}
		this->Reprint(f);
		f.close();
	}

	void Parser::Reprint(std::ostream& stream_to_use) {
		stream_to_use << "<?xml version=\"1.0\"?>\n";
		this->root->Reprint(stream_to_use, "");
	}

}

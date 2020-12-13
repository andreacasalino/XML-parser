/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <fstream>
#include <Parser.h>
#include "ErrorHandler.h"
#include "TagHandler.h"
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
			this->root = std::make_unique<Tag>("Root", nullptr);
			ErrorHandler::handle("invalid number of tags. An empty Parser will be built");
		}
	}

	Parser::Parser() {
		this->root = std::make_unique<Tag>("Root", nullptr);
	}

	Parser::~Parser(){
	}

	TagHandler Parser::GetRoot() {
		return TagHandler(*this);
	}

	void Parser::Reprint(const std::string& file_name) const {
		std::ofstream f(file_name);
		if (!f.is_open()) {
			ErrorHandler::handle("invalid file for printing the xml");
			return;
		}
		f << *this;
		f.close();
	}

	std::ostream& operator<<(std::ostream& s, const Parser& structure) {
		s << "<?xml version=\"1.0\"?>\n";
		structure.root->Reprint(s, "");
		return s;
	}

}

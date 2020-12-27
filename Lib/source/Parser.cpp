/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Parser.h>
#include <Error.h>
#include <fstream>
#include <sstream>

namespace xmlPrs {
	std::list<std::string> Parser::sliceFragments(const std::string& toSplit) {
		std::istringstream iss(toSplit);
		std::list<std::string> slices;
		while (!iss.eof()) {
			slices.emplace_back(std::string());
			iss >> slices.back();
			if(slices.back().empty()) slices.pop_back();
		}
		return slices;
	}
	
	std::list<std::size_t> findSymbolPositions(const std::string& content, const char& symbol) {
		std::list<std::size_t> pos;
		size_t s,S=content.size();
		for(s = 0; s<S; ++s) {
			if(content[s] == symbol) pos.emplace_back(s);
		}
		return pos;	
	}

	std::list<Parser::TagContent> Parser::sliceTags(const std::string& fileName) {
		auto areNotSpaces = [](const std::string& content, const std::size_t& startPos, const std::size_t& endPos){
			for(std::size_t s=startPos; s<endPos; ++s) {
				if(' ' != content[s]) return true;
			}
			return false;
		};

		std::list<TagContent> tags;
		std::ifstream t(fileName);
		if(!t.is_open()) {
			t.close();
			throw Error("impossible to read the file");
		}
		std::string content((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		// remove \n and \t
		auto removeSymbol = [&content](const char& symbol){
			auto it = content.begin();
			while (it != content.end()) {
				if(*it == symbol) it = content.erase(it);
				else ++it;
			}
		};
		removeSymbol('\n');
		removeSymbol('\t');

		t.close();
		std::list<std::size_t> openTagPositions = findSymbolPositions(content, '<');
		std::list<std::size_t> closeTagPositions = findSymbolPositions(content, '>');
		if(openTagPositions.empty()) {
			throw Error("no open tag symbols were found");
		}
		if(openTagPositions.size() != closeTagPositions.size()) {
			throw Error("number of open tag symbols don't match the close ones");
		}
		auto itOpen  = openTagPositions.begin();
		auto itClose = closeTagPositions.begin();
		if( (*itOpen != 0) && areNotSpaces(content, 0, *itOpen) ) {
			throw Error("found bad syntax for the xml");
		}
		if( (closeTagPositions.back() != (content.size()-1)) && areNotSpaces(content, closeTagPositions.back() + 1, content.size()) ) {
			throw Error("found bad syntax for the xml");
		}
		for(itClose; itClose!=closeTagPositions.end(); ++itClose){
			if(!((*itOpen+1) < *itClose)) {
				throw Error("found bad syntax for the xml");
			}
			tags.emplace_back(sliceFragments(std::string(content, *itOpen + 1, *itClose - *itOpen - 1)));
			if(tags.back().empty()) {
				throw Error("found empty tag");
			}
			++itOpen;
			if( (itOpen != openTagPositions.end()) && (*itOpen > (*itClose + 1)) && areNotSpaces(content, *itClose + 1, *itOpen) ) {
				throw Error("found bad syntax for the xml");
			}
		}
		return tags;
	}

	typedef std::pair<std::string, std::string> Field;
	std::unique_ptr<Field> ParseField(const std::string& word) {
		std::list<std::size_t> posEqual = findSymbolPositions(word, '=');
		if (posEqual.size() != 1) {
			return std::make_unique<Field>();
		}
		std::unique_ptr<Field> field = std::make_unique<Field>(std::string(word, 0, posEqual.front()) , std::string(word, posEqual.front() + 1));
		if ((field->second.front() != '\"') || (field->second.back() != '\"')) {
			return std::make_unique<Field>();
		}
		if (field->second.size() < 3) {
			return std::make_unique<Field>();
		}
		field->second = std::string(field->second, 1, field->second.size() - 2);
		return field;
	}

	Tag::TagPtr Parser::parse(std::list<Parser::TagContent>::const_iterator current, std::list<Parser::TagContent>::const_iterator end) {
		if(end->front().compare("/" + current->front()) !=  0) {
			throw Error("tag closing " , current->front() , " not found");
			return Tag::TagPtr();
		}
		if(current->empty()) {
			throw Error("found empty tag");
			return Tag::TagPtr();
		}
		auto itF = current->begin();

		Tag::TagPtr tag = std::make_unique<Tag>(*itF);
		// parse attributes
		++itF;
		for(itF; itF!=current->end(); ++itF) {
			std::unique_ptr<Field> field = std::move(ParseField(*itF));
			if(nullptr == field) return Tag::TagPtr();
			tag->getAttributes().emplace(field->first, field->second);
		}
		// parse nested
		std::list<Parser::TagContent>::const_iterator nested;
		nested = current;
		++nested;
		std::string closingName;
		std::list<Parser::TagContent>::const_iterator nestedEnd;
		while (nested != end) {
			closingName = "/" + nested->front();
			// find terminating tag
			for (nestedEnd = nested; nestedEnd != end; ++nestedEnd) {
				if(nestedEnd->front().compare(closingName) == 0) {
					break;
				}
			}
			Tag::TagPtr nestedTag = Parser::parse(nested, nestedEnd);
			if(nullptr == nestedTag){
				return Tag::TagPtr();
			}
			Tag::Emplacer::emplaceNested(*tag, std::move(nestedTag));
			nested = nestedEnd;
			++nested;
		}
		return tag;
	}

	Parser::Parser() {
		this->root = std::make_unique<Tag>("Root");
	}

	Parser::Parser(const std::string& source_file) 
		: Parser() {
		auto tags = sliceTags(source_file);
		tags.pop_front();  // skip preamble
		if(tags.size() >= 2) {
			try {
				auto end = tags.end();
				--end;
				auto parsedRoot = Parser::parse(tags.begin(), end);
				if(nullptr != parsedRoot) {
					this->root = std::move(parsedRoot);
				}
			}
			catch(...) {
				return;
			}
		}
	}

	Parser::Parser(const Tag& root)
		: Parser() {
		*this->root = root;
	}

	Parser::Parser(const Parser& o) {
		*this = o;
	}

	Parser& Parser::operator=(const Parser& o) {
		this->root = std::make_unique<Tag>(*o.root);
		return *this;
	}

	Parser::Parser(Parser&& o) {
		*this = std::move(o);
	}

	Parser& Parser::operator=(Parser&& o) {
		this->root = std::move(o.root);
		this->root = std::make_unique<Tag>("Root");
		return *this;
	}

	void Parser::reprint(const std::string& file_name) const {
		std::ofstream f(file_name);
		if (!f.is_open()) {
			f.close();
			throw Error("invalid file for printing the xml");
		}
		f << *this;
		f.close();
	}

	std::ostream& operator<<(std::ostream& s, const Parser& structure) {
		s << "<?xml version=\"1.0\"?>\n";
		s << *structure.root;
		return s;
	}

}

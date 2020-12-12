/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Parser.h>
#include "Tag.h"
#include "ErrorHandler.h"
#include <fstream>
#include <streambuf>
#include <algorithm>

namespace xmlPrs {

	std::list<std::size_t> findSymbolPositions(const std::string& content, const char& symbol) {
		std::list<std::size_t> pos;
		size_t s,S=content.size();
		for(s = 0; s<S; ++s) {
			if(content[s] == symbol) pos.emplace_back(s);
		}
		return pos;	
	}

	std::list<TagContent> sliceTags(const std::string& fileName) {
		auto areNotSpaces = [](const std::string& content, const std::size_t& startPos, const std::size_t& endPos){
			for(std::size_t s=startPos; s<endPos; ++s) {
				if(' ' != content[s]) return true;
			}
			return false;
		};

		std::list<TagContent> tags;
		std::ifstream t(fileName);
		if(!t.is_open()) {
			ErrorHandler::handle("impossible to read the file");
			t.close();
			return tags;
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
			ErrorHandler::handle("no open tag symbols were found");
			return tags;
		}
		if(openTagPositions.size() != closeTagPositions.size()) {
			ErrorHandler::handle("number of open tag symbols don't match the close ones");
			return tags;
		}
		auto itOpen  = openTagPositions.begin();
		auto itClose = closeTagPositions.begin();
		if( (*itOpen != 0) && areNotSpaces(content, 0, *itOpen) ) {
			ErrorHandler::handle("found bad syntax for the xml");
			return tags;
		}
		if( (closeTagPositions.back() != (content.size()-1)) && areNotSpaces(content, closeTagPositions.back() + 1, content.size()) ) {
			ErrorHandler::handle("found bad syntax for the xml");
			tags.clear();
			return tags;
		}
		for(itClose; itClose!=closeTagPositions.end(); ++itClose){
			if(!((*itOpen+1) < *itClose)) {
				ErrorHandler::handle("found bad syntax for the xml");
				tags.clear();
				return tags;
			}
			tags.emplace_back(sliceFragments(std::string(content, *itOpen + 1, *itClose - *itOpen - 1)));
			if(tags.back().empty()) {
				ErrorHandler::handle("found empty tag");
				tags.clear();
				return tags;
			}
			++itOpen;
			if( (itOpen != openTagPositions.end()) && (*itOpen > (*itClose + 1)) && areNotSpaces(content, *itClose + 1, *itOpen) ) {
				ErrorHandler::handle("found bad syntax for the xml");
				tags.clear();
				return tags;
			}
		}
		return tags;
	}

	Tag::Tag(const std::string& name, Tag* father) 
		: father(father) 
		, name(name) {
	}

	Tag::Tag()
		: Tag("", nullptr) {
	}

	typedef std::pair<std::string, std::string> Field;
	std::unique_ptr<Field> ParseField(const std::string& word) {
		std::list<std::size_t> posEqual = findSymbolPositions(word, '=');
		if (posEqual.size() != 1) {
			ErrorHandler::handle("Error in XML_reader::Tag::Extract_word, invalid word at line ");
			return std::make_unique<Field>();
		}
		std::unique_ptr<Field> field = std::make_unique<Field>(std::string(word, 0, posEqual.front()) , std::string(word, posEqual.front() + 1));
		if ((field->second.front() != '\"') || (field->second.back() != '\"')) {
			ErrorHandler::handle("Error in XML_reader::Tag::Extract_word, word not delimited by \" at line ");
			return std::make_unique<Field>();
		}
		if (field->second.size() < 3) {
			ErrorHandler::handle("Bad format for attribute");
			return std::make_unique<Field>();
		}
		field->second = std::string(field->second, 1, field->second.size() - 2);
		return field;
	}

	TagPtr Tag::Parse(std::list<TagContent>::const_iterator current, std::list<TagContent>::const_iterator end, Tag* father) {
		if(end->front().compare("/" + current->front()) !=  0) {
			ErrorHandler::handle("tag closing " + current->front() + " not found");
			return TagPtr();
		}
		TagPtr tag = std::make_shared<Tag>(current->front(), father);
		// parse attributes
		auto itF = current->begin();
		++itF;
		for(itF; itF!=current->end(); ++itF) {
			std::unique_ptr<Field> field = std::move(ParseField(*itF));
			if(nullptr == field) return TagPtr();
			tag->fields.emplace(field->first, field->second);
		}
		// parse nested
		std::list<TagContent>::const_iterator nested;
		nested = current;
		++nested;
		std::string closingName;
		std::list<TagContent>::const_iterator nestedEnd;
		while (nested != end) {
			closingName = "/" + nested->front();
			// find terminating tag
			for (nestedEnd = nested; nestedEnd != end; ++nestedEnd) {
				if(nestedEnd->front().compare(closingName) == 0) {
					break;
				}
			}
			TagPtr nestedTag = Tag::Parse(nested, nestedEnd, tag.get());
			if(nullptr == nestedTag){
				return TagPtr();
			}
			tag->nested.emplace(nestedTag->name, std::move(nestedTag));
			nested = nestedEnd;
			++nested;
		}
		return tag;
	}

	void Tag::Reprint(std::ostream& stream_to_use, const std::string& space_to_use) {
		stream_to_use << space_to_use << "<" << this->name << "";
		for (auto it = this->fields.begin(); it != this->fields.end(); ++it) {
			stream_to_use << " " << it->first << "=\"" << it->second << "\"";
		}
		stream_to_use << ">";

		if (!this->nested.empty()) {
			stream_to_use << std::endl;
			for (auto it = this->nested.begin(); it != this->nested.end(); ++it)
				it->second->Reprint(stream_to_use, space_to_use + "  ");
			stream_to_use << space_to_use;
		}

		stream_to_use << "</" << this->name << ">";
		if (nullptr != this->father)
			stream_to_use << std::endl;
	}
}
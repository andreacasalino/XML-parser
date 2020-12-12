/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#ifndef _XML_TAG_H_
#define _XML_TAG_H_

#include <Parser.h>
#include <memory>
#include <list>
#include <string>
#include <map>


namespace xmlPrs {
	typedef std::list<std::string> TagContent;
	std::list<TagContent> sliceTags(const std::string& fileName);

	class Tag {
		friend class TagHandler;
	public:
		Tag();
		Tag(const std::string& name, Tag* father);

		static TagPtr Parse(std::list<TagContent>::const_iterator current, std::list<TagContent>::const_iterator end, Tag* father);

		void Reprint(std::ostream& stream_to_use, const std::string& space_to_use);
	protected:
	// data
        Tag*									  father;
		std::string								  name;
		std::multimap<std::string, std::string>   fields;
		std::multimap<const std::string, TagPtr> nested;
	};
}

#endif
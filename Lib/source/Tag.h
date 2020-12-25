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
#include <map>
#include <set>

namespace xmlPrs {
	typedef std::list<std::string> TagContent;
	std::list<TagContent> sliceTags(const std::string& fileName);

	class Tag {
		friend class TagHandler;
	public:
		Tag(const std::string& name, Tag* father = nullptr);
		Tag(const Tag& o, Tag* father = nullptr);
		Tag(Tag&& o, const std::string& name);
		
		static TagPtr Parse(std::list<TagContent>::const_iterator current, std::list<TagContent>::const_iterator end, Tag* father);

		void Reprint(std::ostream& stream_to_use, const std::string& space_to_use);

		inline const std::string& getName() const { return this->name; };
	protected:
	// data
		std::string								  name;
        Tag*									  father;
		std::multimap<std::string, std::string>   fields;
		std::multiset<TagPtr> 	  				  nested;
	};
}

#endif
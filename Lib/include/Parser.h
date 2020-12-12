/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#ifndef _XML_PARSER_H_
#define _XML_PARSER_H_

#include <memory>
#include <ostream>
#include <list>
#include <string>

namespace xmlPrs {
	/** \brief split the line into some words
	 * /*/
	std::list<std::string>  sliceFragments(const std::string& toSplit);

	class Tag;
	typedef std::unique_ptr<Tag> TagPtr;

	class TagHandler;

	/** \brief when you create a define named __USE_THROW, errors are raised when inconsistent actions are taken, 
	 * as for example pass an inexistent file to the constrcutor of the reader. Otherwise, instructions are simply ignored.
	 * /*/
	class Parser {
		friend class TagHandler;
	public:
		/** @brief Import the xml specified in the passed location.
	     *  @param[in] source_file the location of the file to read, containing the xml structure to parse
		 */
		Parser(const std::string& source_file);

		/** \brief An empty structure is initialized using the default constructor.
		\details Only the root is created, i.e. a tag with a name 'Root' with no attributes. 
		You can use this  constructor to create an xml file from a process. In such a case, you have to use this
		constructor to initialize an empty structure. Then, you can get the root and add attributes ann nested tags using:
		Tag_readable::Add_field, Tag_readable::Add_Nested , Tag_readable::Add_Nested_and_return_created .
		* /*/
		Parser();

		~Parser();

		/** \brief Get the root of the structure.
		* /*/
		TagHandler GetRoot();

		/** \brief Print the actual structure in a textual file.
		@param[in] file_name the location of the file were to print the structure. It can be: a simle name, an absolute path or a relative path
		* /*/
		void		 Reprint(const std::string& file_name);

		/** \brief Similar to XML_reader::Reprint(const std::string& file_name)
		\details , but considering a stream to a file already opened: the structure is appended to the stream.
		@param[in] stream_to_use the stream to use
		* /*/
		void		 Reprint(std::ostream& stream_to_use);
	private:
	// data
		std::string rootName;
		TagPtr      root;
	};
}

#endif
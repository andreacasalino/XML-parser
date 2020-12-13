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
	/** @brief Usefull function consumed by Parser, which slice a string into many slices.
	 */
	std::list<std::string>  sliceFragments(const std::string& toSplit);

	class Tag;
	typedef std::unique_ptr<Tag> TagPtr;

	class TagHandler;

	/** @brief Interface for handling xml. When encountering errors, a ParserError exception can be raised,
	 * or informative cout can be printed. The behaviour can be regulated using xmlPrs::UseThrowError or xmlPrs::UseInformativeCout.
	 */
	class Parser {
		friend class TagHandler;
	public:
		/** @brief Import the xml specified in the passed location.
		 * In case the file is invalid, the same structure obtained using the default 
		 * c'tor is created.
	     *  @param[in] the location of the file to read and parse
		 */
		Parser(const std::string& source_file);

		/** @brief An empty structure containing a single tag named 'Root' is internally created. 
		 */
		Parser();

		~Parser();

		/** @return the root of the structure. 
		 * You can add nested tags and attributes using the methods of TagHandler.
		 */
		TagHandler GetRoot();

		/** @brief Create a file where to print the structure.
		 * In case the file already exists, is overwritten.
	     *  @param[in] the destination file
		 */
		void		 Reprint(const std::string& file_name) const;

		/** @brief Print the structure into an already opened stream
		 */
		friend std::ostream& operator<<(std::ostream&, const Parser&);
	private:
	// data
		std::string rootName;
		TagPtr      root;
	};

	std::ostream& operator<<(std::ostream&, const Parser&);
}

#endif
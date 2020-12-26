/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _XML_PARSER_H_
#define _XML_PARSER_H_

#include <Tag.h>
#include <list>

namespace xmlPrs {
	/** @brief Interface for handling xml. When encountering errors, a ParserError exception can be raised,
	 * or informative cout can be printed. The behaviour can be regulated using xmlPrs::UseThrowError or xmlPrs::UseInformativeCout.
	 */
	class Parser {
	public:
		/** @brief Slice a string into many pieces.
		 * todo METTERE esempio
		 */
		static std::list<std::string> sliceFragments(const std::string& toSplit);

		/** @brief Slices the content of a file into many TagContent data
		 * todo METTERE esempio
		 */
		typedef std::list<std::string> TagContent;
		static std::list<TagContent> sliceTags(const std::string& fileName);

		/** @brief Import the xml specified in the passed location.
		 * In case the file is invalid, the same structure obtained using the default 
		 * c'tor is created.
	     *  @param[in] the location of the file to read and parse
		 */
		Parser(const std::string& source_file);

		Parser(const Tag& root);
		Parser(Tag&& root);
		Parser();

		/** @return the root of the structure. 
		 * You can add nested tags and attributes using the methods of TagHandler.
		 */
		inline const Tag& GetRoot() const { return this->root; };
		inline Tag& GetRoot() { return this->root; };

		/** @brief Create a file where to print the structure.
		 * In case the file already exists, is overwritten.
	     *  @param[in] the destination file
		 */
		void		 reprint(const std::string& file_name) const;

		/** @brief Print the structure into an already opened stream
		 */
		friend std::ostream& operator<<(std::ostream&, const Parser&);
	private:
	// data
		Tag    					root;
	};

	std::ostream& operator<<(std::ostream& s, const Parser& p);
}

#endif
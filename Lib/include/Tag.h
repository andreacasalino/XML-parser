/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _XML_TAG_H_
#define _XML_TAG_H_

#include <map>
#include <memory>
#include <vector>
#include <ostream>
#include <string>
#include <functional>

namespace xmlPrs {
	class Tag {
	public:
		Tag(const std::string& name); 
		Tag(const Tag& o);
		Tag(Tag&& o);
		Tag& operator=(const Tag& o); 
		Tag& operator=(Tag&& o);

		friend std::ostream& operator<<(std::ostream&, const Tag&);

		typedef std::unique_ptr<Tag> TagPtr;
		typedef std::shared_ptr<std::string> TagName;

		bool hasFather() const;
		const Tag& getFather() const;
		Tag& getFather();

		/** @return the name of this tag
	 	 */
		inline const std::string& getName() const { return *this->name; };

		template<typename iterator_T>
		class Iterator_T;
		typedef Iterator_T<std::multimap<TagName, TagPtr>::iterator> Iterator;
		typedef Iterator_T<std::multimap<TagName, TagPtr>::const_iterator> ConstIterator;

		Iterator getNestedAll();
		ConstIterator getNestedAll() const;

		Iterator getNested(const std::string& name);
		ConstIterator getNested(const std::string& name) const;

		/** @brief Get a tag in a specified position, starting from the tag wrapped by this object.
		 * The term position refers to a chain of tag names. Essentially, the path is
		 * the series of tag to traverse to get a certain nested tag.
		 * Consider this example:
		 * <A><B><C><D></D></C></B></A>.
		 * The path of tag D from A is: {B,C,D}. At the same time, the path of tag D from
		 * B is: {C,D}. Therefore, If tou wanna get D using Tag_readable::Get_Nested(const std::list<std::string>& position) on Tag wrapping A,
		 * you should pass as input {B,C,D}. Instead, when you want to get D from a Tag_readable wrapping B, you should pass as input {C,D}.
		 * In case that it does not exists a tag in the specified position, an invalid Tag_readable is returned.
		 * @param[in] the sequence of tag to traverse to get the one of interest from the one wrapped
		 * @return the desired tag
		 */
		const Tag& getNested(const std::vector<std::string>& position) const; 
		Tag&       getNested(const std::vector<std::string>& position); 

		inline const std::multimap<std::string, std::string>& getAttributes() const { return this->fields; };
		inline std::multimap<std::string, std::string>& getAttributes() { return this->fields; };

		/** @brief set the name of the wrapped tag.
	 	 */
		void setName(const std::string& new_name);

		/** @brief change the names of all the attributes contained in the wrapped tag, 
		 * matching the passed name.
		 * @param[in] the name of the attributes to rename
		 * @param[in] the new name to set for the attributes
	 	 */
		void setAttributeName(const std::string& name_attribute, const std::string& new_name_attribute);

		/** @brief change the names of all the attributes contained in the wrapped tag, 
		 * matching the passed name and attribute value.
		 * @param[in] the name of the attributes to rename
		 * @param[in] the value of the attributes to rename
		 * @param[in] the new name to set for the attributes
	 	 */
		void setAttributeName(const std::string& name_attribute, const std::string& val_attribute, const std::string& new_name_attribute);

		void remove();

		void removeNestedAll();

		/** @brief adds an empty nested tag to the wrapped one.
		 * @param[in] the name of the tag to add
		 * @return the newly created tag.
	 	 */
		Tag& addNested(const std::string& tag_name); 

		/** @brief adds the content of structure as a substructure
		 * nested to this tag.
		 * @param[in] the substructure to add
		 * @return the newly created tag.
	 	 */
		Tag& addNested(const Tag& structure);
		Tag& addNested(Tag&& structure);

	private:
		void reprint(std::ostream& stream_to_use, const std::string& space_to_use) const;

	// data
		Tag*									  											father;
		TagName									  											name;
		std::multimap<std::string, std::string>   											fields;
		std::multimap<TagName, TagPtr, std::function<bool(const TagName&, const TagName&)>> nested;
	};

	std::ostream& operator<<(std::ostream& s, const Tag& t);
	
	template<typename iterator_T>
	class Tag::Iterator_T {
	public:
		Iterator_T(const iterator_T& begin, const iterator_T& end)
			: _begin(begin)
			, _end(end) {
		};

		inline std::size_t size() const { return std::distance(this->_begin , this->_end); };
        inline const iterator_T& begin() { return this->_begin; };
        inline const iterator_T& end() { return this->_end; };

	private:
		const iterator_T    _begin;
		const iterator_T    _end;
	};
}

#endif

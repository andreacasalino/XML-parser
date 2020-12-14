/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#ifndef _XML_TAG_HANDLER_H_
#define _XML_TAG_HANDLER_H_

#include <Parser.h>
#include <vector>

namespace xmlPrs {

	bool operator==(const TagHandler& , std::nullptr_t);
	bool operator==(std::nullptr_t, const TagHandler&);

	/** @brief This interface can be used to modify and manipulate a tag inside a Parser
	 */
	class TagHandler {
	public:
		friend bool operator==(const TagHandler&, std::nullptr_t);

		/** @brief The root tag of the passed structure is wrapped and can be manipulated 
		 * @param[in] the structure whose root should be wrapped
	 	 */
		TagHandler(Parser& structure); 

		TagHandler(const TagPtr& wrp);

		TagHandler(Tag* wrp);

		/** @return the name of the wrapped tag
	 	 */
		const std::string& GetTagName() const;

	//////////////////////////////////
	//    Queries on nested tags    //
	//////////////////////////////////
		/** @return true in case at least on tag with the passed name is nested in the
		 * one wrapped by this object.
	 	 */
		bool ExistNested(const std::string& name_nested);

		/** @return all the tags nested in the one wrapped by this object, having the passed name
	 	 */
        std::vector<TagHandler> GetNested(const std::string& name_nested); 

		/** @return the first tag nested in the one wrapped by this object, having the passed name
	 	 */
		TagHandler GetNestedFirst(const std::string& name_nested);

		/** @return all the tags nested in the one wrapped by this object
	 	 */
		std::vector<TagHandler> GetNestedAll();
        
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
		TagHandler GetNested(const std::vector<std::string>& position); 

	//////////////////////////////////
	//    Queries on attributes     //
	//////////////////////////////////
		/** @return true in case the wrapped tag contains at least one attribute
		 * with the passed name.
	 	 */
		bool ExistAttribute(const std::string& name_attribute);

		/** @return get the values of the attributes contained by the wrapped
		 * tag, matching the passed name.
	 	 */
		std::vector<std::string> GetAttributeValues(const std::string& name_attribute);

		/** @return get the first value of the attribute contained by the wrapped
		 * tag, matching the passed name.
	 	 */
		std::string GetAttributeValueFirst(const std::string& name_attribute);

		/** @return get all the attributes in the wrapped tag. Each element
		 * of the returned vector is a pair whose first element is the name
		 * of the attribute and the second is the value.
	 	 */
		std::vector< std::pair<std::string, std::string> > GetAttributeAll();

	///////////////
	// Modifiers //
	///////////////
		/** @brief set the name of the wrapped tag.
	 	 */
		void SetTagName(const std::string& new_name);

		/** @brief change the names of all the attributes contained in the wrapped tag, 
		 * matching the passed name.
		 * @param[in] the name of the attributes to rename
		 * @param[in] the new name to set for the attributes
	 	 */
		void SetAttributeName(const std::string& name_attribute, const std::string& new_name_attribute);

		/** @brief change the names of all the attributes contained in the wrapped tag, 
		 * matching the passed name and attribute value.
		 * @param[in] the name of the attributes to rename
		 * @param[in] the value of the attributes to rename
		 * @param[in] the new name to set for the attributes
	 	 */
		void SetAttributeName(const std::string& name_attribute, const std::string& val_attribute, const std::string& new_name_attribute);

		/** @brief change the values of all the attributes of the wrapped tag matching the passed name, 
		 * considering the passed set of values. The size of new_values should be equal ot the number
		 * of attributes matching name_attribute.
		 * @param[in] the name of the attributes to change
		 * @param[in] the new values to consider for the attributes
	 	 */
		void SetAttributeValue(const std::string& name_attribute, const std::vector<std::string>& new_values);

		/** @brief change the values of all the attributes of the wrapped tag matching the passed name and value, 
		 * considering the passed value.
		 * @param[in] the name of the attributes to change
		 * @param[in] the (old) value of the attributes to change
		 * @param[in] the new values to consider for the attributes
	 	 */
		void SetAttributeValue(const std::string& name_attribute, const std::string& val_attribute, const std::string& new_value);

		/** @brief remove the wrapped tag from the origin xml.
	 	 */
		void Remove();

		/** @brief removes all the attributes matching the passed name and value.
		 * @param[in] the name of the attributes to remove
		 * @param[in] the value of the attributes to remove
	 	 */
		void RemoveAttribute(const std::string& name_attribute, const std::string& value_attribute);

		/** @brief removes all the attributes matching the passed name.
		 * @param[in] the name of the attributes to remove
	 	 */
		void RemoveAttribute(const std::string& name_attribute);

		/** @brief removes all the attributes contained in the wrapped tag.
	 	 */
		void RemoveAttributeAll();

		/** @brief adds a new attribute to the wrapped tag.
		 * @param[in] the name of the attribute to add
		 * @param[in] the value of the attribute to add
	 	 */
		void AddAttribute(const std::string& name_attribute, const std::string& value_attribute);

		/** @brief adds an empty nested tag to the wrapped one.
		 * @param[in] the name of the tag to add
	 	 */
		void AddNested(const std::string& tag_name); 

		/** \brief Similar to Tag_readable::Add_Nested.
		\details Here the tag is created and returned in a Tag_readable.
		@param[in] tag_name the name of the nested tag to create
		* /*/
		/** @brief same to AddNested with the addition of returning
		 * the created tag in a TagHandler
		 * @param[in] the name of the tag to add
		 * @return an handler to newly created tag.
	 	 */
		TagHandler AddNestedReturnCreated(const std::string& tag_name);

	private:
        Tag* wrappedTag;
	};

}

#endif
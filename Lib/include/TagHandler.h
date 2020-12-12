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

	// bool operator==(const TagHandler& t, std::nullptr_t) noexcept;
	// bool operator==(std::nullptr_t, const TagHandler& t) noexcept;

	class TagHandler {
	public:
		// friend bool operator==(const TagHandler&, std::nullptr_t) noexcept;

		/** \brief The wrapped tag is the root of the passed structure.
		\details The copy of a Tag_readable is pretty fast, since only a pointer to the data is wrapped.
		@param[in] structure the structure whose root must be wrapped
		* /*/
		TagHandler(Parser& structure); 

		/** \brief Getter for the name of the tag
		@param[out] return the name of the wrapped tag
		* /*/
		const std::string& GetTagName() const;

	//////////////////////////////////
	//    Queries on nested tags    //
	//////////////////////////////////
		/** \brief Returns true in case it exists a tag whose name is the passed one.
		@param[in] name_nested the name of the tag whose existance must be checked
		* /*/
		bool ExistNested(const std::string& name_nested);

		/** \brief Get all the nested tags with a specific name.
		@param[in] name_nested the name of the tags to return
		@param[out] nested the list of tags whose name matches with name_nested
		* /*/
        std::vector<TagHandler> GetNested(const std::string& name_nested); 

		/** \brief Similar to Tag_readable::Get_Nested(const std::string& name_nested).
		\details Here only a single tag is returned: the first found with the passed name.
		In case that it does not exists a tag in the specified position, an invalid Tag_readable is returned.
		@param[in] name_nested the name of the tags to return
		@param[out] return the tag whose name matches with name_nested
		* /*/
		TagHandler GetNestedFirst(const std::string& name_nested);

		/** \brief Get all the nested tags, no matter their names.
		@param[out] nested_tags the list of nested tag
		* /*/
		std::vector<TagHandler> GetNestedAll();
        
		/** \brief Get a tag in a specified position in the structure.
		\details The term Position refers to a chain of tag names. Essentially, the path is
		the series of tag to traverse to get a certain nested tag.
		Consider this example:
		<A><B><C><D></D></C></B></A>.
		The path of tag D from A is: {B,C,D}. At the same time, the path of tag D from
		B is: {C,D}. Therefore, If tou wanna get D using Tag_readable::Get_Nested(const std::list<std::string>& position) on Tag wrapping A,
		you should pass as input {B,C,D}. Instead, when you want to get D from a Tag_readable wrapping B, you should pass as input {C,D}.
		In case that it does not exists a tag in the specified position, an invalid Tag_readable is returned.
		@param[in] position is the sequence of tag to traverse to get the one of interest from the one wrapped in the calling object
		@param[out] return the tag in the specified position
		* /*/
		TagHandler GetNested(const std::vector<std::string>& position); 

	//////////////////////////////////
	//    Queries on attributes     //
	//////////////////////////////////
		/** \brief Returns true in case it exists an attribute in this tag whose name is the passed one.
		@param[in] name_field the name of the attribute whose existance must be checked
		* /*/
		bool ExistAttribute(const std::string& name_attribute);

		std::vector<std::string> GetAttributeValues(const std::string& name_attribute);

		std::string GetAttributeValueFirst(const std::string& name_attribute);

		/** \brief Gets the names of the all the attributes in this tag
		@param[out] return the attributes names
		* /*/
		std::vector< std::pair<std::string, std::string> > GetAttributeAll();

	///////////////
	// Modifiers //
	///////////////

		/** \brief Change the name of this tag.
		@param[in] new_name the new name to consider
		* /*/
		void SetTagName(const std::string& new_name);

		/** \brief Change the name of the specified attribute.
		\details In case the name passed as input does not exists, this instruction is ignored.
		In case of multiple attributes with this name, they are all changed.
		@param[in] name_attribute the attribute whose name is to change
		@param[in] new_name_attribute the new name to consider for the specified attribute
		* /*/ 
		void SetAttributeName(const std::string& name_attribute, const std::string& new_name_attribute);

		/** \brief Similar to XML_Manager::Set_attribute_name(const std::string& name_attribute, const std::string& new_name_attribute).
		\details Here only a specific attribute is changed: the one with the passed name and value. In case such an attribute does not existst,
		this instruction is ignored.
		@param[in] name_attribute the name of the attributes whose name have to be changed
		@param[in] val_attribute the value of the attribute of interest
		@param[in] new_name_attribute the new name to impose for the attribute
		* /*/
		void SetAttributeName(const std::string& name_attribute, const std::string& val_attribute, const std::string& new_name_attribute);

		/** \brief Change the values of all the attributes with the passed name.
		\details In case the number of attributes with passed name is different from the size od the passed list,
		this instruction is ignored.
		@param[in] name_attribute the name of the attributes whose name have to be changed
		@param[in] new_values the new values to consider for all the attributes having the passed name
		* /*/ 
		void SetAttributeValue(const std::string& name_attribute, const std::vector<std::string>& new_values);

		/** \brief Similar to XML_Manager::Set_attribute_value(const std::string& name_attribute, const std::list<std::string>& new_values).
		\details Here only a specific attribute is changed: the one with the passed name and value. In case such an attribute does not existst,
		this instruction is ignored.
		@param[in] name_attribute the name of the attributes whose name have to be changed
		@param[in] val_attribute the value of the attribute of interest
		@param[in] new_value the new value to consider for all the attributes having the passed name
		* /*/
		void SetAttributeValue(const std::string& name_attribute, const std::string& val_attribute, const std::string& new_value);

		/** \brief Remove this tag from the structure
		* /*/
		void Remove();

		/** \brief Remove the attribute whose name and value matches with the passed ones.
		@param[in] name_attribute the name of the attribute to remove
		@param[in] value_attribute the new value of the attribute to remove
		* /*/
		void RemoveAttribute(const std::string& name_attribute, const std::string& value_attribute);

		/** \brief Remove all the attributes with this tag.
		@param[in] name_attribute the name of the attributes to remove
		* /*/
		void RemoveAttribute(const std::string& name_attribute);

		void RemoveAttributeAll();

		/** \brief Add an attribute with the passed name and value
		@param[in] name_attribute the name of the attribute to create
		@param[in] value_attribute the new value of the attribute to create
		* /*/
		void AddAttribute(const std::string& name_attribute, const std::string& value_attribute);

		/** \brief Add a nested tag with the passed name
		\details The tag created is empty, i.e. it has no attributes.
		@param[in] tag_name the name of the nested tag to create
		* /*/
		void AddNested(const std::string& tag_name); 

		/** \brief Similar to Tag_readable::Add_Nested.
		\details Here the tag is created and returned in a Tag_readable.
		@param[in] tag_name the name of the nested tag to create
		* /*/
		TagHandler AddNestedReturnCreated(const std::string& tag_name);

	private:
	// constructor
		TagHandler(const TagPtr& wrp);

		TagHandler(Tag* wrp);

        Tag* wrappedTag;
	};

}

#endif
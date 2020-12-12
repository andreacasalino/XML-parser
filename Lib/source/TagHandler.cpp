/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <TagHandler.h>
#include "Tag.h"
#include "ErrorHandler.h"

namespace xmlPrs {

    // bool operator==(const TagHandler& t, std::nullptr_t) noexcept{
    //     return (t.wrappedTag == nullptr);
    // }

	// bool operator==(std::nullptr_t, const TagHandler& t) noexcept{
    //     return (t == nullptr);
    // }

    TagHandler::TagHandler(Parser& structure)
        : TagHandler(structure.root) {
    }

    TagHandler::TagHandler(TagPtr wrp)
       : wrappedTag(wrp) {
    }

    const std::string& TagHandler::GetTagName() const { return this->wrappedTag->name; }

	bool TagHandler::ExistNested(const std::string& name_nested) { return (this->wrappedTag->nested.find(name_nested) != this->wrappedTag->nested.end()); }; 

    std::vector<TagHandler> TagHandler::GetNested(const std::string& name_nested) {
        auto range = this->wrappedTag->nested.equal_range(name_nested);
        std::list<TagPtr> tags;
        for(auto it = range.first; it!=range.second; ++it) {
            tags.emplace_back(it->second);          
        }
        std::vector<TagHandler> handlers;
        handlers.resize(tags.size());
        while (!tags.empty()) {
            handlers.emplace_back(tags.front());
            tags.pop_front();
        }
        return handlers;
    }

    TagHandler TagHandler::GetNestedFirst(const std::string& name_nested){
        auto it = this->wrappedTag->nested.find(name_nested);
        if(it == this->wrappedTag->nested.end()) {
            return TagHandler(std::make_shared<Tag>());
        }
        return TagHandler(it->second);
    }

    std::vector<TagHandler> TagHandler::GetNestedAll() {
        std::vector<TagHandler> handlers;
        handlers.reserve(this->wrappedTag->nested.size());
        for(auto it = this->wrappedTag->nested.begin(); it!=this->wrappedTag->nested.end(); ++it) {
            handlers.emplace_back(it->second);
        }
        return handlers;
    }

    TagHandler TagHandler::GetNested(const std::vector<std::string>& position) {
        if(position.empty()){
            return TagHandler(std::make_shared<Tag>());
        }
        TagPtr* cursor = &this->wrappedTag;
        for(auto it = position.begin(); it!=position.end(); ++it) {
            auto n = (*cursor)->nested.find(*it);
            if(n == this->wrappedTag->nested.end()) {
                cursor = nullptr;
                break;
            }
            cursor = &n->second;
        }
        if(nullptr == cursor) {
            return TagHandler(std::make_shared<Tag>());
        }
        return TagHandler(*cursor);
    }

    bool TagHandler::ExistAttribute(const std::string& name_attribute) { return (this->wrappedTag->fields.find(name_attribute) != this->wrappedTag->fields.end()); }; 

    std::vector<std::string> TagHandler::GetAttributeValues(const std::string& name_attribute) {
        std::vector<std::string> values;
        auto range = this->wrappedTag->fields.equal_range(name_attribute);
        for(auto it = range.first; it!=range.second; ++it) {
            values.push_back(it->second);
        }
        return values;
    }

    std::string TagHandler::GetAttributeValueFirst(const std::string& name_attribute){
        auto it = this->wrappedTag->fields.find(name_attribute);
        if(it == this->wrappedTag->fields.end()) return "";
        return it->second;
    }

    std::vector< std::pair<std::string, std::string> > TagHandler::GetAttributeAll(){
        std::vector< std::pair<std::string, std::string> > attributes;
        attributes.reserve(this->wrappedTag->fields.size());
        for(auto it = this->wrappedTag->fields.begin(); it!=this->wrappedTag->fields.end(); ++it){
            attributes.emplace_back(it->first , it->second);
        }
        return attributes;
    }

    void TagHandler::SetTagName(const std::string& new_name) { this->wrappedTag->name = new_name; };

    void TagHandler::SetAttributeName(const std::string& name_attribute, const std::string& new_name_attribute){
        auto range = this->wrappedTag->fields.equal_range(name_attribute);
        for(auto it=range.first; it!=range.second; ++it) {
            it->first = new_name_attribute;
        }
    }

	void TagHandler::SetAttributeName(const std::string& name_attribute, const std::string& val_attribute, const std::string& new_name_attribute){
        auto range = this->wrappedTag->fields.equal_range(name_attribute);
        for(auto it=range.first; it!=range.second; ++it) {
            if(it->second == val_attribute) it->first = new_name_attribute;
        }
    }

	void TagHandler::SetAttributeValue(const std::string& name_attribute, const std::vector<std::string>& new_values){
        if(new_values.empty()) return;
        auto range = this->wrappedTag->fields.equal_range(name_attribute);
        std::size_t v = 0, V = new_values.size();
        for(auto it=range.first; it!=range.second; ++it) {
            it->second = new_values[v];
            ++v;
            if(v == V) return;
        }
    }

	void TagHandler::SetAttributeValue(const std::string& name_attribute, const std::string& val_attribute, const std::string& new_value){
        auto range = this->wrappedTag->fields.equal_range(name_attribute);
        for(auto it=range.first; it!=range.second; ++it) {
            if(it->second == val_attribute) it->second = new_value;
        }
    }

    void TagHandler::Remove() {
        if (nullptr == this->wrappedTag->father) {
            ErrorHandler::handle("You cannot remove the root, ignored");
            return;
        }
        auto it = this->wrappedTag->father->nested.find(this->wrappedTag->name);
        this->wrappedTag->father->nested.erase(it);
        this->wrappedTag.reset();
    }

    void TagHandler::RemoveAttribute(const std::string& name_attribute, const std::string& value_attribute){
        auto range = this->wrappedTag->fields.equal_range(name_attribute);
        for(auto it=range.first; it!=range.second; ++it) {
            if(it->second == value_attribute) this->wrappedTag->fields.erase(it);
        }
    }

    void TagHandler::RemoveAttribute(const std::string& name_attribute) {
        auto range = this->wrappedTag->fields.equal_range(name_attribute);
        for(auto it=range.first; it!=range.second; ++it) {
            this->wrappedTag->fields.erase(it);
        }
    }

    void TagHandler::RemoveAttributeAll(){
        while (!this->wrappedTag->fields.empty()) {
            this->wrappedTag->fields.erase(this->wrappedTag->fields.begin());
        }
    }

    void TagHandler::AddAttribute(const std::string& name_attribute, const std::string& value_attribute){
        this->wrappedTag->fields.emplace(name_attribute, value_attribute);
    }

	void TagHandler::AddNested(const std::string& tag_name){
        TagPtr newTag = std::make_shared<Tag>(tag_name, *this->wrappedTag.get());
        this->wrappedTag->nested.emplace(newTag->name, newTag);
    } 

	TagHandler TagHandler::AddNestedReturnCreated(const std::string& tag_name){
        TagPtr newTag = std::make_shared<Tag>(tag_name, *this->wrappedTag.get());
        auto info = this->wrappedTag->nested.emplace(newTag->name, newTag);
        return TagHandler(info->second);
    }

}

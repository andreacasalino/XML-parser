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

    bool operator==(const TagHandler& t, std::nullptr_t){
       return (t.wrappedTag == nullptr);
    }

	bool operator==(std::nullptr_t, const TagHandler& t){
        return (t == nullptr);
    }

    TagHandler::TagHandler(Parser& structure)
        : TagHandler(structure.root) {
    }

    TagHandler::TagHandler(const TagPtr& wrp)
       : TagHandler(wrp.get()) {
    }

    TagHandler::TagHandler(Tag* wrp)
        : wrappedTag(wrp) {

    }

    const std::string& TagHandler::GetTagName() const { return this->wrappedTag->name; }

	bool TagHandler::ExistNested(const std::string& name_nested) const { 
        return (this->wrappedTag->nested.find(std::make_unique<Tag>(name_nested, nullptr)) != this->wrappedTag->nested.end()); 
    }; 

    std::vector<TagHandler> TagHandler::GetNested(const std::string& name_nested) const {
        auto range = this->wrappedTag->nested.equal_range(std::make_unique<Tag>(name_nested, nullptr));
        std::list<Tag*> tags;
        for(auto it = range.first; it!=range.second; ++it) {
            tags.emplace_back(it->get());          
        }
        std::vector<TagHandler> handlers;
        handlers.reserve(tags.size());
        while (!tags.empty()) {
            handlers.emplace_back(tags.front());
            tags.pop_front();
        }
        return handlers;
    }

    TagHandler TagHandler::GetNestedFirst(const std::string& name_nested) const {
        auto it = this->wrappedTag->nested.find(std::make_unique<Tag>(name_nested, nullptr));
        if(it == this->wrappedTag->nested.end()) {
            return TagHandler(nullptr);
        }
        return TagHandler(it->get());
    }

    std::vector<TagHandler> TagHandler::GetNestedAll() const {
        std::vector<TagHandler> handlers;
        handlers.reserve(this->wrappedTag->nested.size());
        for(auto it = this->wrappedTag->nested.begin(); it!=this->wrappedTag->nested.end(); ++it) {
            handlers.emplace_back(it->get());
        }
        return handlers;
    }

    TagHandler TagHandler::GetNested(const std::vector<std::string>& position) const {
        if(position.empty()){
            return TagHandler(nullptr);
        }
        Tag* cursor = this->wrappedTag;
        for(auto it = position.begin(); it!=position.end(); ++it) {
            auto n = cursor->nested.find(std::make_unique<Tag>(*it, nullptr));
            if(n == this->wrappedTag->nested.end()) {
                cursor = nullptr;
                break;
            }
            cursor = n->get();
        }
        if(nullptr == cursor) {
            return TagHandler(nullptr);
        }
        return TagHandler(cursor);
    }

    bool TagHandler::ExistAttribute(const std::string& name_attribute)  const { 
        return (this->wrappedTag->fields.find(name_attribute) != this->wrappedTag->fields.end()); 
    }; 

    std::vector<std::string> TagHandler::GetAttributeValues(const std::string& name_attribute) const {
        std::vector<std::string> values;
        auto range = this->wrappedTag->fields.equal_range(name_attribute);
        for(auto it = range.first; it!=range.second; ++it) {
            values.push_back(it->second);
        }
        return values;
    }

    std::string TagHandler::GetAttributeValueFirst(const std::string& name_attribute) const {
        auto it = this->wrappedTag->fields.find(name_attribute);
        if(it == this->wrappedTag->fields.end()) return "";
        return it->second;
    }

    std::vector< std::pair<std::string, std::string> > TagHandler::GetAttributeAll() const {
        std::vector< std::pair<std::string, std::string> > attributes;
        attributes.reserve(this->wrappedTag->fields.size());
        for(auto it = this->wrappedTag->fields.begin(); it!=this->wrappedTag->fields.end(); ++it){
            attributes.emplace_back(it->first , it->second);
        }
        return attributes;
    }

    void TagHandler::SetTagName(const std::string& new_name) { 
        if(nullptr == this->wrappedTag->father) this->wrappedTag->name = new_name; 
        else {
            auto range = this->wrappedTag->father->nested.equal_range(std::make_unique<Tag>(this->wrappedTag->name, nullptr));
            for(auto it = range.first; it!=range.second; ++it) {
                if(this->wrappedTag == it->get()) {
                    TagPtr modified = std::make_unique<Tag>(std::move(*it->get()), new_name);
                    this->wrappedTag = modified.get();
                    this->wrappedTag->father->nested.erase(it);
                    this->wrappedTag->father->nested.emplace(std::move(modified));
                    return;
                }
            }
        }
    };

    void TagHandler::SetAttributeName(const std::string& name_attribute, const std::string& new_name_attribute){
        auto range = this->wrappedTag->fields.equal_range(name_attribute);
        std::list<std::string> values;
        for(auto it=range.first; it!=range.second; ++it) {
            values.emplace_back(std::move(it->second));
            this->wrappedTag->fields.erase(it);
        }
        for(auto it = values.begin(); it!=values.end(); ++it) {
            this->wrappedTag->fields.emplace(new_name_attribute , std::move(*it));
        }
    }

	void TagHandler::SetAttributeName(const std::string& name_attribute, const std::string& val_attribute, const std::string& new_name_attribute){
        auto range = this->wrappedTag->fields.equal_range(name_attribute);
        std::size_t C = 0;
        for(auto it=range.first; it!=range.second; ++it) {
            if(it->second.compare(val_attribute) == 0) {
                this->wrappedTag->fields.erase(it);
                ++C;
            }
        }
        for(std::size_t c=0;c<C; ++c) {
            this->wrappedTag->fields.emplace(new_name_attribute, val_attribute);
        }
    }

	void TagHandler::SetAttributeValue(const std::string& name_attribute, const std::vector<std::string>& new_values){
        if(new_values.empty()) return;
        auto range = this->wrappedTag->fields.equal_range(name_attribute);
        std::size_t fieldsNumber = 0;
        for(auto it=range.first; it!=range.second; ++it) {
            ++fieldsNumber;
        }
        if(fieldsNumber != new_values.size()) {
            ErrorHandler::handle("invalid number of new values");
            return;
        }
        std::size_t v = 0;
        for(auto it=range.first; it!=range.second; ++it) {
            it->second = new_values[v];
            ++v;
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
        auto it = this->wrappedTag->father->nested.find(std::make_unique<Tag>(this->wrappedTag->name, nullptr));
        this->wrappedTag->father->nested.erase(it);
        this->wrappedTag = nullptr;
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
        this->wrappedTag->nested.emplace(std::make_unique<Tag>(tag_name, this->wrappedTag));
    } 

	TagHandler TagHandler::AddNestedReturnCreated(const std::string& tag_name){
        auto info = this->wrappedTag->nested.emplace(std::make_unique<Tag>(tag_name, this->wrappedTag));
        return TagHandler(info->get());
    }

}

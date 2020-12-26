/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Tag.h>
#include <list>
#include <Error.h>

namespace xmlPrs {

	Tag::Tag(const std::string& name) 
		: father(nullptr)
		, name(std::make_shared<std::string>(name))
		, nested([](const TagName& a, const TagName& b){ return (*a < *b); }) {
	}

	Tag::Tag(const Tag& o)
	    : Tag(*o.name) {
		*this = o;
	}

	Tag::Tag(Tag&& o)
	    : Tag(*o.name) {
		*this = std::move(o);
	}

	Tag& Tag::operator=(const Tag& o){
		*this->name = *o.name;
		this->fields = o.fields;
		this->nested.clear();
		for(auto it=o.nested.begin(); it!=o.nested.end(); ++it) {
			TagPtr clone = std::make_unique<Tag>(*it->second);
			clone->father = this;
			this->nested.emplace(clone->name, std::move(clone));
		}
		return *this;
	} 

	Tag& Tag::operator=(Tag&& o){
		*this->name = *o.name;
		this->fields = std::move(o.fields);
		this->nested = std::move(o.nested);
		for(auto it=this->nested.begin(); it!=this->nested.end(); ++it) {
			it->second->father = this;
		}
		return *this;
	}

	bool Tag::hasFather() const {
		return (nullptr != this->father);
	}

	const Tag& Tag::getFather() const {
		if(nullptr == this->father){
			throw Error("Tag has no father to return");
		}
		return *this->father;
	};

	Tag& Tag::getFather() {
		if(nullptr == this->father){
			throw Error("Tag has no father to return");
		}
		return *this->father;
	};

	Tag::Iterator Tag::getNestedAll() {
		return Iterator(this->nested.begin(), this->nested.end());
	};

	Tag::ConstIterator Tag::getNestedAll() const {
		return ConstIterator(this->nested.begin(), this->nested.end());
	};

	Tag::Iterator Tag::getNested(const std::string& name) {
		auto range = this->nested.equal_range(std::make_shared<std::string>(name));
		return Iterator(range.first, range.second);
	};

	Tag::ConstIterator Tag::getNested(const std::string& name) const {
		auto range = this->nested.equal_range(std::make_shared<std::string>(name));
		return ConstIterator(range.first, range.second);
	};

	const Tag& Tag::getNested(const std::vector<std::string>& position) const {
        if(position.empty()){
            return *this;
        }
        const Tag* cursor = this;
        for(auto it = position.begin(); it!=position.end(); ++it) {
            auto n = cursor->nested.find(std::make_shared<std::string>(*it));
            if(n == cursor->nested.end()) {
                cursor = nullptr;
                break;
            }
            cursor = n->second.get();
        }
        if(nullptr == cursor) {
			throw Error("inexistent nested tag");
        }
        return *cursor;
    }

	Tag& Tag::getNested(const std::vector<std::string>& position) {
        if(position.empty()){
            return *this;
        }
        Tag* cursor = this;
        for(auto it = position.begin(); it!=position.end(); ++it) {
            auto n = cursor->nested.find(std::make_shared<std::string>(*it));
            if(n == cursor->nested.end()) {
                cursor = nullptr;
                break;
            }
            cursor = n->second.get();
        }
        if(nullptr == cursor) {
			throw Error("inexistent nested tag");
        }
        return *cursor;
    }

	void Tag::setName(const std::string& new_name) {
		if(nullptr == this->father){
			*this->name = new_name;
			return;
		}
		auto range = this->father->getNested(*this->name);
		for(auto it = range.begin(); it!=range.end(); ++it) {
			if(this == it->second.get()) {
				TagPtr temp = std::move(it->second);
				this->father->nested.erase(it);
				*temp->name = new_name;
				this->father->nested.emplace(temp->name , std::move(temp));
				return;
			}
		}
	}

	void Tag::setAttributeName(const std::string& name_attribute, const std::string& new_name_attribute) {
		if(name_attribute.compare(new_name_attribute) == 0) return;
		auto it = this->fields.find(name_attribute);
		std::string value;
		while (it != this->fields.end()) {
			value = std::move(it->second);
			this->fields.erase(it);
			this->fields.emplace(new_name_attribute, value);
			it = this->fields.find(name_attribute);
		}
	}

	void Tag::setAttributeName(const std::string& name_attribute, const std::string& val_attribute, const std::string& new_name_attribute) {
		if(name_attribute.compare(new_name_attribute) == 0) return;
		auto range = this->fields.equal_range(name_attribute);
		std::list<std::multimap<std::string, std::string>::iterator> match;
		for(auto it = range.first; it!=range.second; ++it) {
			if(it->second.compare(val_attribute) == 0){
				match.emplace_back(it);
			}
		}
		std::string value;
		for(auto it=match.begin(); it!=match.end(); ++it){
			value = std::move((*it)->second);
			this->fields.erase(*it);
			this->fields.emplace(new_name_attribute, value);
		}
	}

	void Tag::remove() {
		if(nullptr == this->father){
			throw Error("can't remove root tag");
		}
		auto range = this->father->getNested(*this->name);
		for(auto it = range.begin(); it!=range.end(); ++it) {
			if(this == it->second.get()) {
				this->father->nested.erase(it);
				return;
			}
		}	
	}

	void Tag::removeNestedAll() {
		this->nested.clear();
	}

	Tag& Tag::addNested(const std::string& tag_name) {
		Tag newTag(tag_name);
		return this->addNested(std::move(newTag));
	}

	Tag& Tag::addNested(const Tag& structure) {
		TagPtr newTag = std::make_unique<Tag>(structure);
		newTag->father = this;
		auto info = this->nested.emplace(newTag->name, std::move(newTag));
		return *info->second.get();
	}

	Tag& Tag::addNested(Tag&& structure) {
		TagPtr newTag = std::make_unique<Tag>(std::move(structure));
		newTag->father = this;
		auto info = this->nested.emplace(newTag->name, std::move(newTag));
		return *info->second.get();
	}

	std::ostream& operator<<(std::ostream& s, const Tag& t) {
		t.reprint(s, "");
		return s;
	}

	void Tag::reprint(std::ostream& stream_to_use, const std::string& space_to_use) const {
		stream_to_use << space_to_use << "<" << *this->name << "";
		for (auto it = this->fields.begin(); it != this->fields.end(); ++it) {
			stream_to_use << " " << it->first << "=\"" << it->second << "\"";
		}
		stream_to_use << ">";

		if (!this->nested.empty()) {
			stream_to_use << std::endl;
			for (auto it = this->nested.begin(); it != this->nested.end(); ++it){
				it->second->reprint(stream_to_use, space_to_use + "  ");
			}
			stream_to_use << space_to_use;
		}

		stream_to_use << "</" << *this->name << ">";
		if (nullptr != this->father)
			stream_to_use << std::endl;
	}
}
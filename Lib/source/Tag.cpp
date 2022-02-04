/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Error.h>
#include <Tag.h>

namespace xmlPrs {
Tag::Tag(const Tag &o) { *this = o; }

Tag &Tag::operator=(const Tag &o) {
  this->attributes = o.attributes;
  for (const auto &[name, content] : o.getNested()) {
    auto it = this->nested.emplace(name, std::make_unique<Tag>());
    it->second->father = this;
    *it->second = *content;
  }
  return *this;
}

Tag::Tag(Tag &&o) { *this = std::move(o); }

Tag &Tag::operator=(Tag &&o) {
  this->attributes = std::move(o.attributes);
  for (auto &[name, content] : o.getNested()) {
    auto it = this->nested.emplace(name, std::move(content));
    it->second->father = this;
  }
  o.nested.clear();
  return *this;
}

bool Tag::hasFather() const { return (nullptr != this->father); }

const Tag &Tag::getFather() const {
  if (nullptr == this->father) {
    throw Error("Tag has no father to return");
  }
  return *this->father;
};

Tag &Tag::getFather() {
  if (nullptr == this->father) {
    throw Error("Tag has no father to return");
  }
  return *this->father;
};

void Tag::clear() {
  attributes.clear();
  nested.clear();
}

void Tag::clearNested() { nested.clear(); }

const Tag &Tag::getDescendant(const std::vector<std::string> &position) const {
  if (position.empty()) {
    return *this;
  }
  const Tag *cursor = this;
  for (const auto tag_name : position) {
    auto n = cursor->nested.find(tag_name);
    if (n == cursor->nested.end()) {
      cursor = nullptr;
      break;
    }
    cursor = n->second.get();
  }
  if (nullptr == cursor) {
    throw Error("inexistent nested tag");
  }
  return *cursor;
}

Tag &Tag::getDescendant(const std::vector<std::string> &position) {
  if (position.empty()) {
    return *this;
  }
  Tag *cursor = this;
  for (const auto tag_name : position) {
    auto n = cursor->nested.find(tag_name);
    if (n == cursor->nested.end()) {
      cursor = nullptr;
      break;
    }
    cursor = n->second.get();
  }
  if (nullptr == cursor) {
    throw Error("inexistent nested tag");
  }
  return *cursor;
}

std::unordered_multimap<std::string, TagPtr>::iterator Tag::findInFather() {
  for (auto it = this->father->nested.begin(); it != this->father->nested.end();
       ++it) {
    if (it->second.get() == this) {
      return it;
    }
  }
  return this->father->nested.end();
}

void Tag::remove() {
  if (nullptr == this->father) {
    throw Error("can't remove root tag");
  }
  this->father->nested.erase(findInFather());
}

void Tag::rename(const std::string &name) {
  if (nullptr == this->father) {
    throw Error("Tag has no father");
  }
  auto it = findInFather();
  if (it->first == name) {
    return;
  }
  TagPtr temp = std::move(it->second);
  this->father->nested.erase(it);
  this->father->nested.emplace(name, std::move(temp));
}

Tag &Tag::addNested(const std::string &tag_name) {
  auto info = this->nested.emplace(tag_name, std::make_unique<Tag>());
  info->second->father = this;
  return *info->second.get();
}

void Tag::print(std::ostream &stream_to_use, const std::string &space_to_skip,
                const std::string &name) const {
  stream_to_use << space_to_skip << '<' << name;
  for (const auto &[attr_name, attr_val] : this->attributes) {
    stream_to_use << ' ' << attr_name << "=\"" << attr_val << '\"';
  }
  stream_to_use << '>';

  if (!this->nested.empty()) {
    stream_to_use << std::endl;
    for (auto it = this->nested.begin(); it != this->nested.end(); ++it) {
      it->second->print(stream_to_use, space_to_skip + "  ", it->first);
    }
    stream_to_use << space_to_skip;
  }

  stream_to_use << "</" << name << ">";
  if (nullptr != this->father)
    stream_to_use << std::endl;
}

void Root::print(std::ostream &stream_to_use) const {
  this->Tag::print(stream_to_use, "", name);
}

std::ostream &operator<<(std::ostream &s, const Root &t) {
  t.print(s);
  return s;
}
} // namespace xmlPrs
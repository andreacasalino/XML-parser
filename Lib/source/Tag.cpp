/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <XML-Parser/Error.h>
#include <XML-Parser/Tag.h>

#include <sstream>

namespace xmlPrs {
Name::Name(std::string value) {
  if (value.empty()) {
    throw Error{"empty value"};
  }
  static_cast<std::string &>(*this) = std::move(value);
}

TagPtr::TagPtr(Tag &&o) { this->reset(new Tag{std::move(o)}); }

Tag::Tag(const Tag &o) { *this = o; }

Tag &Tag::operator=(const Tag &o) {
  this->attributes = o.attributes;
  for (const auto &[name, content] : o.getNested()) {
    auto it = this->nested.emplace(name, Tag{});
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
}

Tag &Tag::getFather() {
  if (nullptr == this->father) {
    throw Error("Tag has no father to return");
  }
  return *this->father;
}

Tag &Tag::operator[](const Name &tag_name) {
  auto it = this->nested.find(tag_name);
  if (it == this->nested.end()) {
    return this->addNested(tag_name);
  }
  return *it->second;
}

void Tag::clear() {
  attributes.clear();
  nested.clear();
}

void Tag::clearNested() { nested.clear(); }

const Tag &Tag::getDescendant(const std::vector<Name> &position) const {
  if (position.empty()) {
    return *this;
  }
  const Tag *cursor = this;
  for (const auto &tag_name : position) {
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

Tag &Tag::getDescendant(const std::vector<Name> &position) {
  if (position.empty()) {
    return *this;
  }
  Tag *cursor = this;
  for (const auto &tag_name : position) {
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

Tags::iterator Tag::findInFather() {
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

void Tag::rename(const Name &name) {
  auto *as_root = dynamic_cast<Root *>(this);
  if (nullptr != as_root) {
    as_root->setName(name);
    return;
  }
  auto it = findInFather();
  if (it->first == name) {
    return;
  }
  TagPtr temp = std::move(it->second);
  this->father->nested.erase(it);
  this->father->nested.emplace(name, std::move(temp));
}

Tag &Tag::addNested(const Name &tag_name) {
  auto info = this->nested.emplace(tag_name, Tag{});
  info->second->father = this;
  return *info->second;
}

std::size_t Tag::getTagDepth() const {
  std::size_t ancestors = 0;
  const Tag *cursor = this;
  while (cursor->father != nullptr) {
    cursor = cursor->father;
    ++ancestors;
  }
  return ancestors;
}

namespace {
static const std::string MARGIN_UNIT = std::string{"  "};

std::stringstream make_margin(const std::size_t size) {
  std::stringstream result;
  for (std::size_t k = 0; k < size; ++k) {
    result << MARGIN_UNIT;
  }
  return result;
}
} // namespace

void Tag::print(std::ostream &stream_to_use,
                const std::string &tag_name) const {
  const auto margin = make_margin(getTagDepth()).str();

  stream_to_use << margin << '<' << tag_name;
  for (const auto &[attr_name, attr_val] : this->attributes) {
    stream_to_use << ' ' << attr_name << "=\"" << attr_val << '\"';
  }
  stream_to_use << '>';

  if (!this->nested.empty()) {
    stream_to_use << std::endl;
    for (auto it = this->nested.begin(); it != this->nested.end(); ++it) {
      it->second->print(stream_to_use, it->first);
    }
    stream_to_use << margin;
  }

  stream_to_use << "</" << tag_name << ">";
  if (nullptr != this->father)
    stream_to_use << std::endl;
}

Root::Root(const Name &name) : name(name) {}

void Root::print(std::ostream &stream_to_use) const {
  this->Tag::print(stream_to_use, name);
}

std::ostream &operator<<(std::ostream &s, const Root &t) {
  t.print(s);
  return s;
}
} // namespace xmlPrs
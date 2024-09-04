/**
 * Author:    Andrea Casalino
 * Created:   04.09.2024
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <XML-Parser/Error.h>
#include <XML-Parser/Tag.h>

#include <algorithm>
#include <list>
#include <map>
#include <set>
#include <sstream>

namespace xmlPrs {
Tag::Tag(std::string n) : name{std::move(n)} {
  if (name.empty()) {
    throw Error{"Empty name for tag is not allowed"};
  }
}

Tag::Tag(const Tag &o) { *this = o; }

Tag &Tag::operator=(const Tag &o) {
  rename(o.name);
  nested.clear();
  this->attributes = o.attributes;
  for (const auto &[name, content] : o.getNested()) {
    auto clone = std::make_unique<Tag>(name.data());
    auto it = this->nested.emplace(clone->name.data(), std::move(clone));
    it->second->father = this;
    *it->second = *content;
  }
  return *this;
}

Tag &Tag::operator[](const std::string &tag_name) {
  auto it = this->nested.find(tag_name);
  if (it == this->nested.end()) {
    return this->emplaceNested(tag_name);
  }
  return *it->second;
}

Tags::iterator Tag::findInFather() {
  auto range = father->nested.equal_range(name.data());
  return std::find_if(range.first, range.second, [this](const auto &el) {
    return el.second.get() == this;
  });
}

void Tag::rename(const std::string &n) {
  if (n.empty()) {
    throw Error{"Empty name for tag is not allowed"};
  }
  if (father != nullptr) {
    auto it = findInFather();
    auto this_as_ptr = std::move(it->second);
    father->nested.erase(it);
    name = n;
    father->nested.emplace(name.data(), std::move(this_as_ptr));
  }
}

namespace {
const char *MARGIN_UNIT = "  ";

std::string make_margin(const std::size_t size) {
  std::string result;
  for (std::size_t k = 0; k < size; ++k) {
    result += MARGIN_UNIT;
  }
  return result;
}
} // namespace

void Tag::print(std::ostream &stream_to_use) const {
  const auto margin = make_margin(depth());

  stream_to_use << margin << '<' << name;
  for (const auto &[attr_name, attr_val] : this->attributes) {
    stream_to_use << ' ' << attr_name << "=\"" << attr_val << '\"';
  }
  stream_to_use << '>';

  if (!this->nested.empty()) {
    stream_to_use << '\n';
    for (auto it = this->nested.begin(); it != this->nested.end(); ++it) {
      it->second->print(stream_to_use);
    }
    stream_to_use << margin;
  }

  if(!this->content.empty()) {
    stream_to_use << content;
  }

  stream_to_use << "</" << name << ">";
  if (nullptr != this->father)
    stream_to_use << '\n';
}

const Tag &Tag::getDescendant_(const std::filesystem::path &path) const {
  const Tag *cursor = this;
  std::size_t valid = 0;
  for (const auto &n : path) {
    auto next_it = cursor->nested.find(n.string().data());
    if (next_it == cursor->nested.end()) {
      if (valid == 0) {
        throw Error("Invalid path");
      }
      std::filesystem::path validPortion;
      auto it = path.begin();
      for (std::size_t v = 0; v < valid; ++v, validPortion /= *it, ++it) {
      }
      throw Error{"Invalid path, however this was a valid subportion: {}",
                  validPortion.string()};
    }
    cursor = next_it->second.get();
    ++valid;
  }
  return *cursor;
}

std::ostream &operator<<(std::ostream &stream, const Tag &tag) {
  tag.print(stream);
  return stream;
}

namespace {
struct TagInfo {
  TagInfo(const Tag &node) : source{&node}, content{node.getContent().data()} {
    for (const auto &[k, v] : node.getAttributes()) {
      attributes[k.data()].emplace(v.data());
    }
  }

  const Tag *source;
  std::string_view content;
  std::map<std::string_view, std::multiset<std::string_view>> attributes;

  bool operator==(const TagInfo &o) const {
    return content == o.content && attributes == o.attributes;
  }
  bool operator!=(const TagInfo &o) const { return !(*this == o); }
};

using Level = std::map<std::filesystem::path, std::vector<TagInfo>>;

bool compare(const Level &a, const Level &b) {
  if (a.size() != b.size()) {
    return false;
  }
  auto it_a = a.begin();
  auto it_b = b.begin();
  for (; it_a != a.end(); ++it_a, ++it_b) {
    if (it_a->first == it_b->first &&
        std::is_permutation(it_a->second.begin(), it_a->second.end(),
                            it_b->second.begin())) {
      continue;
    }
    return false;
  }
  return true;
}
} // namespace

bool Tag::same(const Tag &o) const {
  auto propagate = [](Level &subject) {
    Level res;
    for (const auto &[path, nodes] : subject) {
      for (const auto &info : nodes) {
        for (const auto &[child_name, child] : info.source->getNested()) {
          auto next_path = path / child_name;
          res[next_path].emplace_back(*child);
        }
      }
    }
    subject = std::move(res);
  };

  Level from_this, from_o;
  from_this[std::filesystem::path{name}].emplace_back(*this);
  from_o[std::filesystem::path{o.name}].emplace_back(o);
  for (; !from_this.empty() || !from_o.empty(); propagate(from_this), propagate(from_o)) {
    if (!compare(from_this, from_o)) {
      return false;
    }
  }
  return true;
}

bool operator==(const Tag &a, const Tag &b) { return a.same(b); }

std::string toString(const Tag& subject) {
  std::stringstream stream;
  stream << subject;
  return stream.str();
}

} // namespace xmlPrs
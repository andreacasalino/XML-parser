/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Error.h>
#include <Parser.h>
#include <fstream>
#include <sstream>

namespace xmlPrs {
std::vector<std::string> slice_fragments(const std::string &toSplit) {
  std::istringstream iss(toSplit);
  std::vector<std::string> slices;
  while (!iss.eof()) {
    slices.emplace_back();
    iss >> slices.back();
    if (slices.back().empty())
      slices.pop_back();
  }
  return slices;
}

namespace {
void remove(std::string &subject, const char symbol) {
  auto it = subject.begin();
  while (it != subject.end()) {
    if (*it == symbol) {
      it = subject.erase(it);
    } else {
      ++it;
    }
  }
};

// puts the content into a unique string removing empty spaces
std::string readFile(const std::string &fileName) {
  std::ifstream t(fileName);
  if (!t.is_open()) {
    t.close();
    throw Error("impossible to read the file");
  }
  std::string content((std::istreambuf_iterator<char>(t)),
                      std::istreambuf_iterator<char>());
  remove(content, '\n');
  remove(content, '\t');
  return content;
}

std::vector<std::size_t> findSymbolPositions(const std::string &content,
                                             const char &symbol) {
  std::vector<std::size_t> pos;
  size_t s = 0;
  for (const auto &c : content) {
    if (content[s] == symbol) {
      pos.emplace_back(s);
    }
    ++s;
  }
  return pos;
}

using TagContent = std::vector<std::string>;
static std::vector<TagContent> sliceTags(const std::string &fileContent);

static TagPtr parse(TagContent::const_iterator current,
                    TagContent::const_iterator end);
} // namespace

std::list<Parser::TagContent> Parser::sliceTags(const std::string &fileName) {
  auto areNotSpaces = [](const std::string &content,
                         const std::size_t &startPos,
                         const std::size_t &endPos) {
    for (std::size_t s = startPos; s < endPos; ++s) {
      if (' ' != content[s])
        return true;
    }
    return false;
  };

  std::list<TagContent> tags;
  std::ifstream t(fileName);
  if (!t.is_open()) {
    t.close();
    throw Error("impossible to read the file");
  }
  std::string content((std::istreambuf_iterator<char>(t)),
                      std::istreambuf_iterator<char>());
  // remove \n and \t
  auto removeSymbol = [&content](const char &symbol) {
    auto it = content.begin();
    while (it != content.end()) {
      if (*it == symbol)
        it = content.erase(it);
      else
        ++it;
    }
  };
  removeSymbol('\n');
  removeSymbol('\t');

  t.close();
  std::list<std::size_t> openTagPositions = findSymbolPositions(content, '<');
  std::list<std::size_t> closeTagPositions = findSymbolPositions(content, '>');
  if (openTagPositions.empty()) {
    throw Error("no open tag symbols were found");
  }
  if (openTagPositions.size() != closeTagPositions.size()) {
    throw Error("number of open tag symbols don't match the close ones");
  }
  auto itOpen = openTagPositions.begin();
  auto itClose = closeTagPositions.begin();
  if ((*itOpen != 0) && areNotSpaces(content, 0, *itOpen)) {
    throw Error("found bad syntax for the xml");
  }
  if ((closeTagPositions.back() != (content.size() - 1)) &&
      areNotSpaces(content, closeTagPositions.back() + 1, content.size())) {
    throw Error("found bad syntax for the xml");
  }
  for (itClose; itClose != closeTagPositions.end(); ++itClose) {
    if (!((*itOpen + 1) < *itClose)) {
      throw Error("found bad syntax for the xml");
    }
    tags.emplace_back(sliceFragments(
        std::string(content, *itOpen + 1, *itClose - *itOpen - 1)));
    if (tags.back().empty()) {
      throw Error("found empty tag");
    }
    ++itOpen;
    if ((itOpen != openTagPositions.end()) && (*itOpen > (*itClose + 1)) &&
        areNotSpaces(content, *itClose + 1, *itOpen)) {
      throw Error("found bad syntax for the xml");
    }
  }
  return tags;
}

typedef std::pair<std::string, std::string> Field;
std::unique_ptr<Field> ParseField(const std::string &word) {
  std::list<std::size_t> posEqual = findSymbolPositions(word, '=');
  if (posEqual.size() != 1) {
    return std::make_unique<Field>();
  }
  std::unique_ptr<Field> field =
      std::make_unique<Field>(std::string(word, 0, posEqual.front()),
                              std::string(word, posEqual.front() + 1));
  if ((field->second.front() != '\"') || (field->second.back() != '\"')) {
    return std::make_unique<Field>();
  }
  if (field->second.size() < 3) {
    return std::make_unique<Field>();
  }
  field->second = std::string(field->second, 1, field->second.size() - 2);
  return field;
}

Tag::TagPtr Parser::parse(std::list<Parser::TagContent>::const_iterator current,
                          std::list<Parser::TagContent>::const_iterator end) {
  if (end->front().compare("/" + current->front()) != 0) {
    throw Error("tag closing ", current->front(), " not found");
    return Tag::TagPtr();
  }
  if (current->empty()) {
    throw Error("found empty tag");
    return Tag::TagPtr();
  }
  auto itF = current->begin();

  Tag::TagPtr tag = std::make_unique<Tag>(*itF);
  // parse attributes
  ++itF;
  for (itF; itF != current->end(); ++itF) {
    std::unique_ptr<Field> field = std::move(ParseField(*itF));
    if (nullptr == field)
      return Tag::TagPtr();
    tag->getAttributes().emplace(field->first, field->second);
  }
  // parse nested
  std::list<Parser::TagContent>::const_iterator nested;
  nested = current;
  ++nested;
  std::string closingName;
  std::list<Parser::TagContent>::const_iterator nestedEnd;
  while (nested != end) {
    closingName = "/" + nested->front();
    // find terminating tag
    for (nestedEnd = nested; nestedEnd != end; ++nestedEnd) {
      if (nestedEnd->front().compare(closingName) == 0) {
        break;
      }
    }
    Tag::TagPtr nestedTag = Parser::parse(nested, nestedEnd);
    if (nullptr == nestedTag) {
      return Tag::TagPtr();
    }
    Tag::Emplacer::emplaceNested(*tag, std::move(nestedTag));
    nested = nestedEnd;
    ++nested;
  }
  return tag;
}

Parser::Parser(const std::string &source_file) : Parser() {
  auto tags = sliceTags(source_file);
  tags.pop_front(); // skip preamble
  if (tags.size() >= 2) {
    try {
      auto end = tags.end();
      --end;
      auto parsedRoot = Parser::parse(tags.begin(), end);
      if (nullptr != parsedRoot) {
        this->root = std::move(parsedRoot);
      }
    } catch (...) {
      return;
    }
  }
}
} // namespace xmlPrs

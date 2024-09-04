/**
 * Author:    Andrea Casalino
 * Created:   04.09.2024
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <XML-Parser/Parser.h>
#include <XML-Parser/Tag.h>

#include <Element.h>
#include <Utils.h>

#include <fstream>
#include <sstream>
#include <stack>
#include <vector>

namespace xmlPrs {
namespace {
template <typename ElementT>
std::pair<std::optional<ElementT>, std::string_view>
findNext(std::string_view buffer) {
  std::pair<std::optional<ElementT>, std::string_view> res;
  bool keepOn = true;
  while (keepOn && !buffer.empty()) {
    stripL<' ', '\n', '\t'>(buffer);

    auto node = parse::parse_element<ElementT, parse::Comment>(buffer);
    std::visit(
        [&](auto &parsed) {
          if constexpr (std::is_same_v<decltype(parsed), parse::Null &>) {
            keepOn = false;
          }

          buffer = parsed.remaining;

          if constexpr (std::is_same_v<decltype(parsed), ElementT &>) {
            res.first.emplace(std::move(parsed));
            res.second = buffer;
            keepOn = false;
          }
        },
        node);
  }
  return res;
}

class ParsedXml {
public:
  ParsedXml(std::string_view bfr) : buffer{bfr} {
    stripR<' ', '\n', '\t'>(buffer);

    findPreamble();
    findRoot();
    mainLoop();

    if (!root) {
      throw Error{"Root not closed"};
    }
    stripL<' ', '\n', '\t'>(buffer);
    if (!buffer.empty()) {
      throw Error{"Found a valid root tag at the beginning, but "
                  "after there was something more ... not admitted"};
    }
  }

  TagPtr root;
  MetaData metaData;

  operator ParseResult() {
    return ParseResult{std::move(metaData), std::move(root)};
  }

private:
  void findPreamble() {
    auto &&[maybe_preamble, remaining] = findNext<parse::Preamble>(buffer);
    if (!maybe_preamble.has_value()) {
      return;
    }
    buffer = remaining;

    for (const auto &[k, v] : maybe_preamble->metaData) {
      std::string k_str{k.data(), k.size()};
      std::string v_str{v.data(), v.size()};
      metaData[k_str] = v_str;
    }
  }

  void findRoot() {
    auto &&[maybe_root, remaining] = findNext<parse::TagOpen>(buffer);
    if (!maybe_root.has_value()) {
      throw Error{"Root not found ... is that an empty document?"};
    }
    buffer = remaining;

    TagPtr n = maybe_root->make();
    if (maybe_root->withClosure) {
      root = std::move(n);
    } else {
      nodes.emplace().tag = std::move(n);
    }
  }

  void mainLoop() {
    while (!root && !buffer.empty()) {
      auto next_sep = buffer.find('<');
      if (next_sep == std::string::npos) {
        throw Error("Tag named: {} was not closed", nodes.top().tag->getName());
      }

      nodes.top().content += std::string_view{buffer.data(), next_sep};
      shift(buffer, next_sep);

      auto node =
          parse::parse_element<parse::TagClose, parse::TagOpen, parse::Comment>(
              buffer);
      std::visit(
          [&](auto &parsed) {
            if constexpr (std::is_same_v<decltype(parsed), parse::Null &>) {
              throw Error{"Unrecognized node"};
            }

            buffer = parsed.remaining;

            if constexpr (std::is_same_v<decltype(parsed), parse::TagClose &>) {
              if (nodes.top().tag->getName() == parsed.name) {
                auto node = std::move(nodes.top());
                nodes.pop();
                auto content = node.content.toString();
                if (!content.empty() && !node.tag->getNested().empty()) {
                  throw Error(
                      "Tag named: {} can't contain both text and children ",
                      node.tag->getName());
                }
                node.tag->setContent(std::move(content));

                addTag(std::move(node.tag));
              } else {
                nodes.top().tag->addNested(parsed.make());
              }
            }

            if constexpr (std::is_same_v<decltype(parsed), parse::TagOpen &>) {
              auto n = parsed.make();
              if (parsed.withClosure) {
                nodes.top().tag->addNested(std::move(n));
              } else {
                nodes.emplace().tag = std::move(n);
              }
            }
          },
          node);
    }
  }

  std::string_view buffer;

  struct NodeInfo {
    TagPtr tag;
    ViewsCollection content;
  };
  std::stack<NodeInfo> nodes;
  void addTag(TagPtr node) {
    if (nodes.empty()) {
      root = std::move(node);
    } else {
      nodes.top().tag->addNested(std::move(node));
    }
  }
};

} // namespace

ParseResult parseString(const std::string &content) {
  ParsedXml doc{content.data()};
  return doc;
}

ParseResult parseStream(std::istream &stream) {
  // TODO use sliding window approach across the document instead of reading
  // everything in one shot
  std::stringstream buffer;
  buffer << stream.rdbuf();
  return parseString(buffer.str());
}

ParseResult parseFile(const std::filesystem::path &path) {
  std::ifstream stream{path};
  if (!stream.is_open()) {
    throw Error{"{}: is an invalid path", path.string()};
  }
  return parseStream(stream);
}

} // namespace xmlPrs

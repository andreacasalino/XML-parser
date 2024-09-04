#include <gtest/gtest.h>

#include <map>
#include <set>
#include <sstream>

#include "Common.h"
#include <Element.h>
using namespace xmlPrs;

template <typename ElementT>
struct ElementParseTestFixture : ::testing::TestWithParam<std::string> {
  template <bool LeftOver> void body(std::string bfr) {
    buffer = std::move(bfr);
    if constexpr (LeftOver) {
      buffer += LEFT_OVER;
    }
    parsed = ElementT::parse(buffer.data());
    ASSERT_TRUE(parsed.has_value());
    if constexpr (LeftOver) {
      EXPECT_EQ(parsed->remaining, LEFT_OVER);
    }
  }

  static const inline std::string LEFT_OVER = R"(
 <a>  </a>
    < /b>
 <c> some text </c>

</root>  
)";

  std::optional<ElementT> parsed;

private:
  std::string buffer;
};

struct TagOpenTestFixture : ElementParseTestFixture<parse::TagOpen> {
  template <bool Closure, bool LeftOver> void body(std::string buffer) {
    if constexpr (Closure) {
      buffer = test::replace(buffer, '>', "/>");
    }
    this->ElementParseTestFixture<parse::TagOpen>::body<LeftOver>(buffer);
    EXPECT_EQ(parsed->name, "SomeName");
    EXPECT_EQ(parsed->withClosure, Closure);
  }
};

struct TagOpenNoAttributesTestFixture : TagOpenTestFixture {
  template <bool Closure, bool LeftOver> void body(std::string buffer) {
    this->TagOpenTestFixture::body<Closure, LeftOver>(buffer);
    EXPECT_TRUE(parsed->attributes.empty());
  }
};
TEST_P(TagOpenNoAttributesTestFixture, TagOpen_noClosure_parse) {
  this->body<false, false>(GetParam());
}
TEST_P(TagOpenNoAttributesTestFixture, TagOpen_withClosure_parse) {
  this->body<true, false>(GetParam());
}
TEST_P(TagOpenNoAttributesTestFixture, TagOpen_withLeftOver_parse) {
  this->body<false, true>(GetParam());
}
INSTANTIATE_TEST_CASE_P(ElementTests,
                        TagOpenNoAttributesTestFixture,
                        ::testing::Values("<SomeName>", "<SomeName  >",
                                          "<  SomeName>", "<SomeName\n\n>",
                                          "<\n\nSomeName>"));

struct TagOpenWithAttributesTestFixture : TagOpenTestFixture {
  template <bool Closure, bool LeftOver> void body(std::string buffer) {
    this->TagOpenTestFixture::body<Closure, LeftOver>(buffer);
    auto attributes = convert(parsed->attributes);
    EXPECT_EQ(attributes, attributes_expected);
  }

  using KeyValue = std::pair<std::string_view, std::string_view>;

  std::vector<KeyValue>
  convert(const std::unordered_multimap<std::string_view, std::string_view>
              &subject) {
    std::map<std::string_view, std::multiset<std::string_view>> tmp;
    for (const auto &[k, v] : subject) {
      tmp[k].emplace(v);
    }
    std::vector<KeyValue> res;
    for (const auto &[k, vals] : tmp) {
      for (const auto &v : vals) {
        res.emplace_back(std::make_pair(k, v));
      }
    }
    return res;
  }

  static inline const std::vector<KeyValue> attributes_expected{
      {"fieldA", "val0"}, {"fieldA", "val1"}, {"fieldB", "val0"}};
};
TEST_P(TagOpenWithAttributesTestFixture, TagOpen_noClosure_parse) {
  this->body<false, false>(GetParam());
}
TEST_P(TagOpenWithAttributesTestFixture, TagOpen_withClosure_parse) {
  this->body<true, false>(GetParam());
}
TEST_P(TagOpenWithAttributesTestFixture, TagOpen_withLeftOver_parse) {
  this->body<false, true>(GetParam());
}
INSTANTIATE_TEST_CASE_P(
    ElementTests, TagOpenWithAttributesTestFixture,
    ::testing::Values(
        R"(<SomeName fieldA="val0" fieldA="val1" fieldB="val0">)",
        R"(<SomeName fieldA="val0" fieldB="val0" fieldA="val1">)",
        R"(<SomeName fieldA="val0" fieldA="val1" fieldB="val0"  >)",
        R"(<SomeName   fieldA="val0" fieldA="val1" fieldB="val0"  >)",
        R"(<SomeName   fieldA="val0"    fieldA="val1" fieldB="val0"  >)",
        R"(<SomeName   fieldA="val0"    fieldA="val1" fieldB="val0"
        >)",
        R"(<  SomeName fieldA="val0" fieldA="val1" fieldB="val0">)",
        R"(<
        SomeName fieldA="val0" fieldA="val1" fieldB="val0">)",
        R"(<SomeName   
    fieldA="val0"    
    fieldA="val1"  
     fieldB="val0" 
        >)"));

struct TagCloseTestFixture : ElementParseTestFixture<parse::TagClose> {
  template <bool LeftOver> void body(std::string buffer) {
    this->ElementParseTestFixture<parse::TagClose>::body<LeftOver>(buffer);
    EXPECT_EQ(parsed->name, "SomeName");
  }
};
TEST_P(TagCloseTestFixture, TagClose_parse) { this->body<false>(GetParam()); }
TEST_P(TagCloseTestFixture, TagClose_withLeftOver_parse) {
  this->body<true>(GetParam());
}
INSTANTIATE_TEST_CASE_P(ElementTests, TagCloseTestFixture,
                        ::testing::Values("</SomeName>", "<  /SomeName>",
                                          "<\n\n/SomeName>", "</  SomeName>",
                                          "</\n\nSomeName>", "</SomeName  >",
                                          "</SomeName\n\n>"));

struct CommentTestFixture : ElementParseTestFixture<parse::Comment> {};
TEST_P(CommentTestFixture, Comment_parse) { this->body<false>(GetParam()); }
TEST_P(CommentTestFixture, Comment_withLeftOver_parse) {
  this->body<true>(GetParam());
}
INSTANTIATE_TEST_CASE_P(ElementTests, CommentTestFixture,
                        ::testing::Values("<!---->",
                                          "<!--dslgk sdlgk lsdkg ls-->",
                                          "<!--  -->",
                                          R"(<!--
                           -->)",
                                          R"(<!--
agsdglds usdgl ksdgù sldk gds g
lkgs ldk gldg ks
                           -->)"));

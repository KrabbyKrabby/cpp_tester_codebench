#include <gtest/gtest.h>
#include <stdexcept>
#include <memory>
#include <unordered_set>
#include "solution.cpp"

// Assuming your classes are defined as XMLNode and XMLToJSONConverter

// Positive Test Case: Simple XML to JSON conversion
TEST(XMLToJSONConverterTest, SimpleXMLToJsonConversion) {
    auto root = std::make_shared<XMLNode>("book", "Great Expectations");
    auto chapter1 = std::make_shared<XMLNode>("chapter", "1");
    auto chapter2 = std::make_shared<XMLNode>("chapter", "2");

    root->addChild(chapter1);
    root->addChild(chapter2);

    std::string expected_json = R"({
  "book": [
    {
      "chapter": "1"
    },
    {
      "chapter": "2"
    }
  ]
})";

    EXPECT_EQ(XMLToJSONConverter::convert(root.get()), expected_json);
}

// Negative Test Case: Add null child
TEST(XMLToJSONConverterTest, AddNullChild) {
    auto root = std::make_shared<XMLNode>("book", "Great Expectations");

    EXPECT_THROW({
        root->addChild(nullptr);
        }, std::invalid_argument);
}

// Negative Test Case: Null root node
TEST(XMLToJSONConverterTest, NullRootNode) {
    EXPECT_THROW({
        XMLToJSONConverter::convert(nullptr);
        }, std::invalid_argument);
}

// Negative Test Case: Circular Reference
TEST(XMLToJSONConverterTest, CircularReferenceDetection) {
    auto root = std::make_shared<XMLNode>("book", "Great Expectations");
    auto chapter1 = std::make_shared<XMLNode>("chapter", "1");

    // Creating circular reference
    root->addChild(chapter1);
    chapter1->addChild(root);  // Circular reference

    EXPECT_THROW({
        XMLToJSONConverter::convert(root.get());
        }, std::runtime_error);
}

// Edge Case: Empty node (root with no children)
TEST(XMLToJSONConverterTest, EmptyNode) {
    auto root = std::make_shared<XMLNode>("book");

    std::string expected_json = R"({
  "book": ""
})";

    EXPECT_EQ(XMLToJSONConverter::convert(root.get()), expected_json);
}

// Edge Case: Single node with value but no children
TEST(XMLToJSONConverterTest, SingleNodeWithValue) {
    auto root = std::make_shared<XMLNode>("book", "Great Expectations");

    std::string expected_json = R"({
  "book": "Great Expectations"
})";

    EXPECT_EQ(XMLToJSONConverter::convert(root.get()), expected_json);
}

// Corner Case: Node with special characters in name and value
TEST(XMLToJSONConverterTest, SpecialCharactersInNode) {
    auto root = std::make_shared<XMLNode>("book\"special", "Great Expectations\\");
    auto chapter1 = std::make_shared<XMLNode>("chapter\tnew", "1\n");

    root->addChild(chapter1);

    std::string expected_json = R"({
  "book\"special": [
    {
      "chapter\tnew": "1\n"
    }
  ]
})";

    EXPECT_EQ(XMLToJSONConverter::convert(root.get()), expected_json);
}

// Edge Case: Large tree with depth
TEST(XMLToJSONConverterTest, LargeTree) {
    auto root = std::make_shared<XMLNode>("root", "Root Value");
    auto current_node = root;

    // Create a deep tree of 100 nodes
    for (int i = 0; i < 100; ++i) {
        auto child = std::make_shared<XMLNode>("node" + std::to_string(i), "Node Value " + std::to_string(i));
        current_node->addChild(child);
        current_node = child; // Move to next child
    }

    // Ensure no exceptions are thrown for large depth, just a rough check for the output
    EXPECT_NO_THROW({
        std::string json = XMLToJSONConverter::convert(root.get());
        EXPECT_FALSE(json.empty());
        });
}

// Positive Test Case: Nested XML with values and children
TEST(XMLToJSONConverterTest, NestedXMLWithChildren) {
    auto root = std::make_shared<XMLNode>("book", "Great Expectations");
    auto chapter1 = std::make_shared<XMLNode>("chapter", "1");
    auto chapter2 = std::make_shared<XMLNode>("chapter", "2");
    auto subChapter1 = std::make_shared<XMLNode>("subChapter", "1.1");

    chapter1->addChild(subChapter1);
    root->addChild(chapter1);
    root->addChild(chapter2);

    std::string expected_json = R"({
  "book": [
    {
      "chapter": [
        {
          "subChapter": "1.1"
        }
      ]
    },
    {
      "chapter": "2"
    }
  ]
})";

    EXPECT_EQ(XMLToJSONConverter::convert(root.get()), expected_json);
}

// Edge Case: Node with empty name and value
TEST(XMLToJSONConverterTest, NodeWithEmptyNameAndValue) {
    auto root = std::make_shared<XMLNode>("", "");

    std::string expected_json = R"({
  "": ""
})";

    EXPECT_EQ(XMLToJSONConverter::convert(root.get()), expected_json);
}

// Edge Case: Single child node with special characters
TEST(XMLToJSONConverterTest, SpecialCharacterInValue) {
    auto root = std::make_shared<XMLNode>("book", "Value with special chars: \"\\\b\f\n\r\t");

    std::string expected_json = R"({
  "book": "Value with special chars: \"\\\b\f\n\r\t"
})";

    EXPECT_EQ(XMLToJSONConverter::convert(root.get()), expected_json);
}
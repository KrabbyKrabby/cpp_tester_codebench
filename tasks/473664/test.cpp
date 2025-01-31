#include <gtest/gtest.h>
#include <stdexcept>

#include "solution.cpp"

// Positive Case: Valid XML structure
TEST(XMLNodeTest, ValidXMLStructure) {
    XMLNode* root = new XMLNode("root");
    XMLNode* validChild1 = new XMLNode("child1", "value1");
    XMLNode* validChild2 = new XMLNode("child2", "value2");

    root->addChild(validChild1);
    root->addChild(validChild2);
    EXPECT_TRUE(XMLValidator::isValid(root));

    delete root; // Deletes all children
}

// Negative Case: Invalid child with empty name
TEST(XMLNodeTest, InvalidChildName) {
    XMLNode* root = new XMLNode("root");

    EXPECT_THROW({
        XMLNode* invalidChild = new XMLNode("");
        root->addChild(invalidChild);
    }, std::invalid_argument);

    delete root;
}

// Negative Case: Duplicate child nodes
TEST(XMLNodeTest, DuplicateChildNodes) {
    XMLNode* root = new XMLNode("root");
    XMLNode* validChild1 = new XMLNode("child1", "value1");

    root->addChild(validChild1);
    EXPECT_THROW(root->addChild(new XMLNode("child1", "value1")), std::runtime_error);

    delete root; // Deletes all children
}

// Edge Case: Deeply nested structure
TEST(XMLNodeTest, DeeplyNestedStructure) {
    XMLNode* root = new XMLNode("root");
    XMLNode* current = root;

    for (int i = 0; i < 100; ++i) {
        XMLNode* child = new XMLNode("level" + std::to_string(i));
        current->addChild(child);
        current = child;
    }
    EXPECT_TRUE(XMLValidator::isValid(root));

    delete root; // Deletes all children recursively
}

// Negative Case: Invalid characters in node name
TEST(XMLNodeTest, InvalidCharactersInName) {
    XMLNode* root = new XMLNode("root");

    EXPECT_THROW(root->addChild(new XMLNode("inv@lid!")), std::invalid_argument);

    delete root;
}

// Positive Case: Node without children or value
TEST(XMLNodeTest, NodeWithoutChildrenOrValue) {
    XMLNode* emptyNode = new XMLNode("emptyNode");
    EXPECT_TRUE(XMLValidator::isValid(emptyNode));
    delete emptyNode;
}

// Positive Case: Valid symbols in name
TEST(XMLNodeTest, ValidSymbolsInName) {
    XMLNode* root = new XMLNode("root");
    XMLNode* nodeWithSymbols = new XMLNode("_valid-name.test");

    EXPECT_NO_THROW(root->addChild(nodeWithSymbols));
    EXPECT_TRUE(XMLValidator::isValid(root));

    delete root; // Deletes all children
}

// Negative Case: Invalid starting character
TEST(XMLNodeTest, InvalidStartingCharacter) {
    XMLNode* root = new XMLNode("root");

    EXPECT_THROW(root->addChild(new XMLNode("1child", "someValue")), std::invalid_argument);

    delete root;
}

// Positive Case: Same name with different value
TEST(XMLNodeTest, SameNameDifferentValue) {
    XMLNode* root = new XMLNode("root");
    XMLNode* validChild1 = new XMLNode("child1", "value1");

    root->addChild(validChild1);
    EXPECT_NO_THROW(root->addChild(new XMLNode("child1", "value2")));

    delete root;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
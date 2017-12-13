#ifndef SRC_UTIL_TRIE_H_
#define SRC_UTIL_TRIE_H_

#include <memory>
#include <string>
#include <vector>

namespace util {

using namespace std;

/**
 * @brief Trie class that supports adding strings and giving the best auto
 *        complete on those string for a given sequence.
 */
class Trie {
public:
  Trie()
    : m_root{ std::make_unique<TrieNode>('-', false) } {
  }

  /**
   * Adds a string that can be used to auto completed on.
   *
   * @param value The string that will be added
   */
  void addString(std::string value) {

    if (value.empty())
      return;

    TrieNode* node = m_root.get();
    for (unsigned i = 0; i < value.size(); ++i) {
      auto c = value[i];
      TrieNode* child = node->getChild(c);
      if (child) {
        node = child;
        continue;
      }
      auto newNode = std::make_unique<TrieNode>(c, i == value.size() - 1);
      auto* tmp = newNode.get();
      node->addChild(std::move(newNode));
      node = tmp;
    }
  }

  /**
   * @brief Auto completes a string from the given value.
   *
   * If there is no string added that starts with the value, an empty
   * string will be returned. If there are more than one string that starts
   * with the value, the longest common substring will be returned.
   *
   * @param value The value that will be auto completed on.
   * @return The best auto complete match, empty if there is no string that
   *         starts with the value. The longest common substring if there are
   *         more than one string that contains the value.
   */
  std::string getFirstWordWithSequence(const std::string& value) {

    TrieNode* node = m_root.get();
    for (auto c : value) {
      TrieNode* child = node->getChild(c);
      if (!child)
        return string();

      node = child;
    }

    std::string result = value;
    while (auto child = node->getSingleChild()) {
      result += child->getValue();
      node = child;

      if (child->isEndChild())
        break;
    }

    return result;
  }

private:
  class TrieNode {
  public:
    TrieNode(char ch, bool isEnd)
      : m_ch(ch)
      , m_isEnd(isEnd) {
    }

    char getValue() {
      return m_ch;
    }

    void addChild(std::unique_ptr<TrieNode> node) {
      m_children.push_back(std::move(node));
    }

    TrieNode* getChild(char c) {
      for (auto& child : m_children) {
        if (child->m_ch == c)
          return child.get();
      }
      return nullptr;
    }

    TrieNode* getSingleChild() {
      if (m_children.size() == 1)
        return m_children[0].get();

      return nullptr;
    }

    unsigned getNumberOfChildren() {
      return m_children.size();
    }

    bool isEndChild() {
      return m_isEnd;
    }

  private:
    const char m_ch{};
    const bool m_isEnd{};
    std::vector<std::unique_ptr<TrieNode>> m_children{};
  };

  std::unique_ptr<TrieNode> m_root;
};

} /* namespace util */
#endif /* SRC_UTIL_TRIE_H_ */

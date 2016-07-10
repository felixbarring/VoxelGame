
#ifndef SRC_UTIL_TRIE_H_
#define SRC_UTIL_TRIE_H_

#include <vector>
#include <string>

using namespace std;

/**
 * Trie class that supports adding strings and giving the best auto complete
 * on those string for a given sequence
 */
class Trie {
public:

	virtual ~Trie() {};

	/**
	 * Adds a string that can be used to auto completed on
	 *
	 * @param value The string that will be added
	 */
	void addString(std::string value) {
		TrieNode *node = &m_root;
		for (int i = 0; i < value.size(); ++i) {
			auto c = value[i];
			if (node->getChild(c)) {
				node = node->getChild(c);
				continue;
			}

			auto newNode = new TrieNode(c, i == value.size() - 1, node);
			node->addChild(newNode);
			node = newNode;
		}
	}

	/**
	 * Checks if the string exists
	 *
	 * @param value The string that will be check if it exists
	 * @return True if the string exists, else false
	 */
	bool stringExists(const std::string &value) {
		TrieNode *node = &m_root;
		for (auto c : value) {
			if (node->getChild(c))
				continue;
			return false;
		}
		return true;
	}

	/**
	 * Auto completes a string from the given value.
	 *
	 * If there is no string added that starts with the value, an empty
	 * string will be returned. If there are more than one string that starts
	 * with the value, the longest common substring will be returned.
	 *
 	 * @param value The value that will be auto completed on
	 * @return The best auto complete match, empty if there is no string that
	 *         starts with the value. The longest common substring if there are
	 *         more than one string that contains the value.
	 */
	std::string getFirstWordWithSequence(const std::string &value) {
		TrieNode *node = &m_root;
		for (auto c : value) {
			if (!node->getChild(c))
				return string();

			node = node->getChild(c);
		}

		while (auto child = node->getSingleChild())
			node = child;

		auto result = node->buildString();

		return string(result.rbegin(), result.rend());
	}

private:

	class TrieNode {
	public:

		TrieNode(char ch, bool isEnd, const TrieNode *previous)
			: m_ch(ch),
			  m_isEnd(isEnd),
			  m_previous(previous)
		{
		}

		void addChild(TrieNode *node) {
			m_children.push_back(node);
		}

		TrieNode* getChild(char c) {
			for (auto child : m_children) {
				if (child->m_ch == c)
					return child;
			}
			return nullptr;
		}



		TrieNode* getSingleChild() {
			if (m_children.size() == 1)
				return m_children[0];

			return nullptr;
		}

		std::string buildString() {
			std::string str;
			return buildStringHelper(this, str);
		}

	private:

		std::string buildStringHelper(const TrieNode *node, std::string &str) {
			if (node->m_previous) {
				str.push_back(node->m_ch);
				return buildStringHelper(node->m_previous, str);
			}
			return str;
		}

		const char m_ch{};
		const bool m_isEnd{};
		const TrieNode *m_previous{};
		std::vector<TrieNode*> m_children{};
	};

	TrieNode m_root{'-', false, nullptr};
};

#endif /* SRC_UTIL_TRIE_H_ */

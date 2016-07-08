
#ifndef SRC_UTIL_TRIE_H_
#define SRC_UTIL_TRIE_H_

#include <vector>
#include <string>

using namespace std;

class Trie {
public:

	Trie() {}

	virtual ~Trie() {};

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

	bool stringExists(const std::string &value) {
		TrieNode *node = &m_root;
		for (auto c : value) {
			if (node->getChild(c))
				continue;
			return false;
		}
		return true;
	}

	std::string getFirstWordWithSequence(const std::string &value) {
		TrieNode *node = &m_root;
		for (auto c : value) {
			if (node->getChild(c))
				continue;

			return string();
		}

		while (auto child = node->getSingleChild())
			node = child;

		return node->buildString();
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
			auto func = [](TrieNode *node, std::string &str) -> std::string
						{
							str.push_back(node->m_ch);
							if (node->m_previous)
								return func(node->m_previous, str);

							return str;
						};

			std::string str;
			return func(this, str);
		}

	private:

		const char m_ch{};
		const bool m_isEnd{};
		const TrieNode *m_previous{};
		std::vector<TrieNode*> m_children{};
	};

	TrieNode m_root{'-', false, nullptr};

};

#endif /* SRC_UTIL_TRIE_H_ */

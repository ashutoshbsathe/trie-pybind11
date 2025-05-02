#ifndef _TRIE_HPP
#define _TRIE_HPP

#include "utf8.hpp"
#include <cstdint>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

struct TrieNode;

typedef struct TrieNode {
  // utf-8
  std::map<char32_t, TrieNode *> children;
  bool terminal; // is this a terminal node?
  size_t count;  // sub tree node count, includes this node

  TrieNode();
  ~TrieNode();
} TrieNode;

class Trie {
private:
  TrieNode *root;

public:
  Trie();
  ~Trie();

  void add_string(const std::string &string);
  void add_string_batch(const std::vector<std::string> &string_batch);
  TrieNode *traverse_along_string(const std::string &prefix) const;
  size_t get_total_node_count() const;
  bool contains_string(const std::string &string) const;
};

#endif // _TRIE_HPP

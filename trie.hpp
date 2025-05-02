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

  // Apparently this prevents unnecessary copying
  TrieNode(const TrieNode &) = delete;
  TrieNode &operator=(const TrieNode &) = delete;
} TrieNode;

class PyTrieNode {
private:
  TrieNode *node_ptr; // This is just copy of the pointer, PyTrieNode will not
                      // own the memory

public:
  PyTrieNode(TrieNode *ptr); // constructor

  // Methods exposed to Python
  PyTrieNode get_child(char32_t c) const;
  bool is_terminal() const;
  size_t get_subtree_node_count() const;
  bool is_valid() const; // Check if `node_ptr` is valid
};

class Trie {
private:
  TrieNode *root;
  TrieNode *traverse_along_string(const std::string &prefix) const;

public:
  Trie();
  ~Trie();
  // Apparently this prevents unnecessary copying
  Trie(const Trie &) = delete;
  Trie &operator=(const Trie &) = delete;

  void add_string(const std::string &string);
  void add_string_batch(const std::vector<std::string> &string_batch);
  size_t get_total_node_count() const;
  bool contains_string(const std::string &string) const;

  PyTrieNode get_root_node() const;
  PyTrieNode traverse_along_prefix(const std::string &prefix) const;
};

#endif // _TRIE_HPP

#include "trie.hpp"

TrieNode::TrieNode() : terminal(false), count(1) {}

TrieNode::~TrieNode() {
  for (auto const &[key, val] : children) {
    delete val;
  }
}

Trie::Trie() { root = new TrieNode(); }

Trie::~Trie() { delete root; }

PyTrieNode::PyTrieNode(TrieNode *ptr) : node_ptr(ptr) {}

PyTrieNode PyTrieNode::get_child(char32_t c) const {
  if (!node_ptr) {
    return PyTrieNode(nullptr);
  }
  auto it = node_ptr->children.find(c);
  if (it != node_ptr->children.end()) {
    return PyTrieNode(it->second);
  } else {
    return PyTrieNode(nullptr);
  }
}

bool PyTrieNode::is_terminal() const {
  return node_ptr ? node_ptr->terminal : false;
}

size_t PyTrieNode::get_subtree_node_count() const {
  return node_ptr ? node_ptr->count : 0;
}

bool PyTrieNode::is_valid() const { return node_ptr != nullptr; }

void Trie::add_string(const std::string &string) {
  // UTF-8 adds a bit of complexity, but it's alright
  const char *curr_char_ptr = string.c_str();
  const char *end_char_ptr = curr_char_ptr + string.length();
  TrieNode *current_node = root;
  std::vector<TrieNode *> path; // Maintain the path to update counts
  path.push_back(current_node);

  while (curr_char_ptr < end_char_ptr) {
    char32_t code_point = decode_utf8(curr_char_ptr, end_char_ptr);

    auto it = current_node->children.find(code_point);
    bool node_created = false;
    if (it == current_node->children.end()) {
      // add a new TrieNode
      TrieNode *new_node = new TrieNode();
      current_node->children[code_point] = new_node;
      current_node = new_node;
      node_created = true;
    } else {
      current_node = it->second;
    }

    path.push_back(current_node);
    if (node_created) {
      // add 1 to all before this
      for (size_t i = 0; i < path.size() - 1; i++) {
        path[i]->count += 1;
      }
    }
  }
  current_node->terminal = true;
  return;
}

void Trie::add_string_batch(const std::vector<std::string> &string_batch) {
  for (const auto &string : string_batch) {
    try {
      add_string(string);
    } catch (const std ::exception &e) {
      std::cerr << "Error processing string '" << string << "': " << e.what()
                << std::endl;
      continue;
    }
  }
  return;
}

TrieNode *Trie::traverse_along_string(const std::string &prefix) const {
  TrieNode *current_node = root;
  const char *curr_char_ptr = prefix.c_str();
  const char *end_char_ptr = curr_char_ptr + prefix.length();
  try {
    while (curr_char_ptr < end_char_ptr && current_node != nullptr) {
      char32_t code_point = decode_utf8(curr_char_ptr, end_char_ptr);
      auto it = current_node->children.find(code_point);
      if (it == current_node->children.end()) {
        current_node = nullptr;
      } else {
        current_node = it->second;
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "Error decoding prefix '" << prefix << "': " << e.what()
              << std::endl;
    return nullptr;
  }
  return current_node;
}

size_t Trie::get_total_node_count() const { return root ? root->count : 0; }

bool Trie::contains_string(const std::string &string) const {
  TrieNode *node = traverse_along_string(string);
  return (node != nullptr) && node->terminal;
}

PyTrieNode Trie::get_root_node() const { return PyTrieNode(root); }

PyTrieNode Trie::traverse_along_prefix(const std::string &prefix) const {
  return PyTrieNode(traverse_along_string(prefix));
}

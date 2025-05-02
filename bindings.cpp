#include "trie.hpp"
#include "utf8.hpp"
#include <pybind11/operators.h> // For __bool__
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Automatic conversions for vector, string, map etc.
#include <sstream>

namespace py = pybind11;

// Helper function to convert python single character string to char32_t
char32_t get_char32_t_from_py_str(const std::string &py_char_str) {
  if (py_char_str.length() == 0) {
    throw py ::value_error(
        "get_child requires a character, received empty string");
  }
  const char *curr_char_ptr = py_char_str.c_str();
  const char *end_char_ptr = curr_char_ptr + py_char_str.length();
  try {
    return decode_utf8(curr_char_ptr, end_char_ptr);
  } catch (const std::exception &e) {
    std::ostringstream oss;
    oss << "UTF-8 decoding error: " << e.what() << std::endl;
    throw py::value_error(oss.str());
  }
}

// `trie_py` is what is defined in setup.py
PYBIND11_MODULE(trie_py, m) {
  m.doc() = "PyBind11 Trie Implementation";
  py::class_<PyTrieNode>(m, "PyTrieNode")
      .def(
          "get_child",
          [](const PyTrieNode &self, const std::string &py_char) -> py::object {
            char32_t c = get_char32_t_from_py_str(py_char);
            PyTrieNode child_node = self.get_child(c);
            if (child_node.is_valid()) {
              return py::cast(child_node);
            } else {
              return py::none();
            }
          },
          py ::arg("char"), "Gets the child node for a given single character")
      .def_property_readonly("is_terminal", &PyTrieNode::is_terminal,
                             "Checks if the string ends at this node")
      .def_property_readonly("is_valid", &PyTrieNode::is_valid,
                             "Checks if the node points to the valid C++ node.")
      .def("__len__", &PyTrieNode::get_subtree_node_count,
           "Returns the number of nodes in the subtree rooted here (including "
           "this node).");

  py::class_<Trie>(m, "Trie")
      .def(py::init<>(), "Creates a new empty Trie.")
      .def("add_string", &Trie::add_string, py::arg("string"),
           "Adds a string to the Trie.")
      .def("add_string_batch", &Trie::add_string_batch, py::arg("string_batch"),
           "Adds a list of strings to the Trie.")
      .def(
          "get_root_node",
          [](const Trie &self) -> py::object {
            PyTrieNode root_node = self.get_root_node();
            if (root_node.is_valid()) {
              return py::cast(root_node);
            } else {
              return py::none();
            }
          },
          "Returns a PyTrieNode corresponding to the root of the Trie.")
      .def(
          "traverse_along_prefix",
          [](const Trie &self, const std::string &prefix) -> py::object {
            PyTrieNode node = self.traverse_along_prefix(prefix);
            if (node.is_valid()) {
              return py::cast(node);
            } else {
              return py::none();
            }
          },
          py::arg("prefix"),
          "Traverses the Trie along the prefix. Returns PyTrieNode or None.")
      .def("__len__", &Trie::get_total_node_count,
           "Returns the total number of nodes in the entire Trie.")
      .def("__contains__", &Trie::contains_string, py::arg("string"),
           "Checks if a complete string exists in the Trie.")
      .def_property_readonly("size", &Trie::get_total_node_count,
                             "Total number of nodes in the entire Trie.");
}

load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")
load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("@rules_python//python:defs.bzl", "py_binary")

cc_library(
    name = "utf8_cc_impl",
    srcs = ["utf8.cpp"],
    hdrs = ["utf8.hpp"],
)

cc_library(
    name = "trie_cc_impl",
    srcs = ["trie.cpp"],
    hdrs = [
        "trie.hpp",
        "utf8.hpp",
    ],
    deps = [":utf8_cc_impl"],
)

pybind_extension(
    name = "trie_py",
    srcs = ["bindings.cpp"],
    deps = [":trie_cc_impl"],
)

py_binary(
    name = "try_trie_py",
    srcs = ["try_trie.py"],
    main = "try_trie.py",
    deps = [":trie_py"],
    data = [":trie_py"],
)

import sys

# --- How to build ---
# 1. Ensure pybind11 is installed: pip install pybind11
# 2. Run build: python setup.py build_ext --inplace
#    This creates the .so/.pyd file in the current directory.

try:
    # Import the classes directly from the compiled pybind11 module
    from trie_py import Trie, PyTrieNode
except ImportError as e:
    print("ERROR: Failed to import the compiled module 'trie_py'.")
    print("Have you built it? Run: python setup.py build_ext --inplace")
    print(f"Original error: {e}")
    sys.exit(1)


if __name__ == "__main__":
    print("Creating Trie...")
    trie = Trie()
    print(f"Initial Trie size: {len(trie)}") # Should be 1 (root node)

    words_to_add = ["hello", "hell", "hi", "你好", "世界", "你好世界", "héllö"]
    print(f"\nAdding batch: {words_to_add}")
    try:
        trie.add_string_batch(words_to_add)
    except Exception as e:
        print(f"An error occurred during add_batch: {e}")
        # Depending on C++ error handling, exceptions might propagate here
        sys.exit(1)

    print(f"Trie size after add: {len(trie)}")

    print("\nChecking word existence:")
    print(f"'hello' in trie: {'hello' in trie}")
    print(f"'hell' in trie: {'hell' in trie}")
    print(f"'hel' in trie: {'hel' in trie}") # Should be False (prefix, not word)
    print(f"'你好' in trie: {'你好' in trie}")
    print(f"'世界' in trie: {'世界' in trie}")
    print(f"'你好世界' in trie: {'你好世界' in trie}")
    print(f"'héllö' in trie: {'héllö' in trie}")
    print(f"'goodbye' in trie: {'goodbye' in trie}")

    print("\nTraversing:")
    prefix = "he"
    node = trie.traverse_along_prefix(prefix)
    print(f"Traversed to '{prefix}': {node}") # Uses the __repr__ we defined
    if node: # Uses the __bool__ we defined
        print(f"  Subtree size at '{prefix}': {len(node)}") # Uses __len__
        # Try traversing further
        child_l1 = node.get_child('l') # Calls the bound get_child
        print(f"  Child 'l': {child_l1}")
        if child_l1:
             child_l2 = child_l1.get_child('l')
             print(f"  Child 'l': {child_l2}")
             if child_l2:
                 child_o_umlaut = child_l2.get_child('ö') # Unicode character
                 print(f"  Child 'ö': {child_o_umlaut}")
                 if child_o_umlaut:
                      # Use property access defined via def_property_readonly
                      print(f"    Is end of word: {child_o_umlaut.is_terminal}")


    prefix_cn = "你好"
    node_cn = trie.traverse_along_prefix(prefix_cn)
    print(f"\nTraversed to '{prefix_cn}': {node_cn}")
    if node_cn:
        print(f"  Is end of word: {node_cn.is_terminal}")
        print(f"  Subtree size: {len(node_cn)}")
        child_sj = node_cn.get_child("世")
        print(f"  Child '世': {child_sj}")


    print("\nDeleting Trie (will happen automatically on exit)...")
    # del trie # Explicit deletion also triggers C++ destructor via Pybind11

    print("Finished.")

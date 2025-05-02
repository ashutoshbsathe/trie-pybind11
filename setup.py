from setuptools import setup
import sys

# Use pybind11 helper (install pybind11 first: pip install pybind11)
try:
    from pybind11.setup_helpers import Pybind11Extension, build_ext
except ImportError:
    print("Error: pybind11 not installed. Please install it: pip install pybind11")
    sys.exit(1)


# Define C++ standard (adjust if needed, C++11 required for map/vector)
cpp_std = 17

ext_modules = [
    Pybind11Extension(
        "trie_py", # Module name (must match PYBIND11_MODULE name)
        [
            "trie.cpp",    # C++ source file(s)
            "utf8.cpp",    # UTF-8 helper
            "bindings.cpp" # Pybind11 binding code file
        ],
        # Example of passing include directories or compile args if needed
        # include_dirs=['/path/to/includes'],
        # extra_compile_args=['-O3'],
        extra_compile_args=['-Wall'],
        cxx_std=cpp_std # Request C++ standard
    ),
]

setup(
    name="pybind11_trie",
    version="0.0.1",
    description="Trie implementation with Pybind11",
    ext_modules=ext_modules,
    # Tell setuptools to use the custom build_ext command
    cmdclass={"build_ext": build_ext},
    zip_safe=False, # Recommended for C extensions
    python_requires=">=3.7", # Specify your minimum Python version
)

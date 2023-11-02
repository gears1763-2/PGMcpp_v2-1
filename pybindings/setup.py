"""
    [...]
    
    $   python setup.py build_ext --inplace
    
    ref: https://pybind11.readthedocs.io/en/stable/compiling.html
    ref: https://github.com/pybind/python_example/blob/master/setup.py
    ref: https://stackoverflow.com/questions/64261546/how-to-solve-error-microsoft-visual-c-14-0-or-greater-is-required-when-inst
"""


import os
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension


# remove any existing extensions
for root, dirs, files in os.walk(os.getcwd()):
    for name in files:
        if "PGMcpp" in name:
            os.remove(name)


# generate list of source files
source_list = ["PYBIND11_PGM.cpp"]
for root, dirs, files in os.walk(".." + os.sep + "source"):
    for name in files:
        path_str = os.path.join(root, name)
        if "control" not in path_str:
            source_list.append(path_str)


# generate list of pybind11 extensions
ext_modules = [
    Pybind11Extension(
        "PGMcpp",
        sources=source_list
    )
]


# set up extensions
setup(
    name="PGMcpp",
    ext_modules=ext_modules,
    zip_safe=False,
    python_requires=">=3.7",
    install_requires=["setuptools>=42", "pybind11>=2.6.1"]
)

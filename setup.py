from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
import sys
import setuptools

class get_pybind_include(object):
    """Helper class to determine the pybind11 include path
    The purpose of this class is to postpone importing pybind11
    until it is actually installed, so that the `get_include()`
    method can be invoked."""

    def __str__(self):
        import pybind11
        return pybind11.get_include()

ext_modules = [
    Extension(
        'your_module',
        ['include/pybind-c.cpp'],
        include_dirs=[
            get_pybind_include(),
            'include'  # Path to header files
        ],
        language='c++'
    ),
]

setup(
    name='TDD',
    version='0.0.1',
    author='Hong xing',
    author_email='xin.hong@student.uts.edu.au',
    url='https://github.com/Veriqc/TDD_C',
    # todo: add describtion
    description='',
    long_description='',
    ext_modules=ext_modules,
    extras_require={'test': ['pytest']},
    install_requires=[],
    setup_requires=['pybind11>=2.5.0'],
    cmdclass={'build_ext': build_ext},
    packages=find_packages(),
)

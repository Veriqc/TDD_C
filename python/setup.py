from setuptools import setup, Extension, find_packages
import numpy,os

include_dir = os.path.abspath('../include')
# @windows: conda_lib_include_dir = os.path.join(os.path.dirname(sys.executable), 'Library', 'include')
project_name = "tensordd"
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
        project_name,
        sources = ['python-c.cpp'],
        include_dirs=[
            get_pybind_include(),
            include_dir, 
            numpy.get_include()
            # @windows: ,conda_lib_include_dir
        ],
        language='c++',
        extra_compile_args=['-std=c++20']
        # @windows: extra_compile_args=['/std:c++20']
    ),
]

with open("../README.md", "r") as fh:
    long_description = fh.read()

setup(
    name=project_name,
    version='0.0.1',
    author='Hong xing',
    author_email='xin.hong@student.uts.edu.au',
    url='https://github.com/Veriqc/TDD_C',
    description='',
    long_description=long_description,
    long_description_content_type="text/markdown",
    license='MIT',
    ext_modules=ext_modules,
    extras_require={'test': ['pytest']},
    install_requires=['pybind11>=2.5.0', 'numpy>=1.20.0'],
    setup_requires=['pybind11>=2.5.0', 'numpy>=1.20.0'],
    packages=find_packages(),
)


import os
from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

ext_modules=[ 
    Extension("pyQA400",

              sources = ["pyQA400.pyx"],
              language="c++",                # this causes Pyrex/Cython to create C++ source
              # include_dirs = [os.getcwd(), ],  # path to .h file(s)
              # library_dirs = [os.getcwd(),],  # path to library
              extra_compile_args = ["/D", "WIN32", "/EHsc"],
              libraries = ['QA400API'],
              ),
]

setup(
  name = 'pyQA400',
  cmdclass = {'build_ext': build_ext},
  ext_modules = ext_modules,
)
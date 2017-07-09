from distutils.core import setup, Extension
import sys

if sys.platform == 'win32':
    extra_compile_args = []
else:
    extra_compile_args = ['-std=c11']

ext = Extension(name='_ndtypes',
                sources=['ndtypes.c'],
                include_dirs=['../'],
                extra_compile_args=extra_compile_args,
                library_dirs=['../'],
                libraries=['ndtypes'])

setup(name='ndtypes',
      ext_modules=[ext],
      )

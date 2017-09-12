#
# BSD 3-Clause License
#
# Copyright (c) 2017, plures
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

from distutils.core import setup, Extension
from distutils.cmd import Command
from sys import platform
from glob import glob


DESCRIPTION = \
    """Dynamic types for data description and in-memory computations"""


class TestCommand(Command):
    user_options = []

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def get_module_path(self):
        pathlist = glob("build/lib.*/")
        if pathlist:
            return pathlist[0]
        raise RuntimeError("cannot find ndtypes module in build directory")

    def run(self):
        import sys, os, subprocess
        python_path = os.getenv('PYTHONPATH')
        module_path = self.get_module_path()
        path = python_path + ':' + module_path if python_path else module_path
        env = os.environ.copy()
        env['PYTHONPATH'] = path

        ret = subprocess.call([sys.executable, "python/test_ndtypes.py"], env=env)
        raise SystemExit(ret)


def ndtypes_ext():
    depends = [
      "attr.h",
      "grammar.h",
      "lexer.h",
      "ndtypes.h",
      "parsefuncs.h",
      "seq.h",
      "symtable.h"
    ]

    sources = [
      "alloc.c",
      "attr.c",
      "display.c",
      "display_meta.c",
      "equal.c",
      "grammar.c",
      "lexer.c",
      "match.c",
      "ndtypes.c",
      "parsefuncs.c",
      "parser.c",
      "seq.c",
      "symtable.c",
      "python/_ndtypes.c",
    ]

    if platform == "win32":
        extra_compile_args = [
          "-I.", "/wd4200", "/wd4201", "/wd4244", "/wd4267", "/wd4702",
          "/wd4127", "/nologo", "/DYY_NO_UNISTD_H=1", "/D__STDC_VERSION__=199901L"
        ]
    else:
        extra_compile_args = [
           "-I.", "-Wextra", "-Wno-missing-field-initializers", "-std=c11"
        ]

    return Extension (
      "_ndtypes",
      depends = depends,
      sources = sources,
      extra_compile_args = extra_compile_args
    )

setup (
    name = "_ndtypes",
    version = "0.1",
    description = DESCRIPTION,
    url = "https://github.com/plures/ndtypes",
    license = "BSD License",
    keywords = ["ndtypes", "array computing", "data description"],
    platforms = ["Many"],
    classifiers = [
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
    ],
    ext_modules = [ndtypes_ext()],
    cmdclass = {
        'test': TestCommand
    }
)

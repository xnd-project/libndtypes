/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2017-2018, plures
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <stdio.h>
#include "test.h"


const char *buffer_tests[] = {
  "?",
  "c", "b", "B",
  "h", "i", "l", "q",
  "H", "I", "L", "Q",
  "n", "N",
  "e", "f", "d",
  "E", "F", "D",
  "s", "100s",
  "T{b:a:100s:b:}",


  "(10,2)?",
  "(10)c",
  "(2,3)b",
  "(0,2,0)B",
  "(1,2,3,4)h",
  "(2,3,10)F",
  "(20,2,100)D",
  "(10,2)s",
  "(10,2)100s",
  "(10,2)T{b:a:100s:b:}",
  "(10,2)T{b:a:100s:b:}",

  "T{T{F:foo:(2,3)L:bar:}:a:100s:b:}",

  "f -> f",
  "ff -> f",
  "fc -> f",
  "F -> Q",

  "T{b:a:100s:b:} -> T{T{F:foo:(2,3)L:bar:}:a:100s:b:}",

   NULL
};

const char *buffer_roundtrip_tests[] = {
  "=b", "<b", ">b",
  "=B", "<B", ">B",
  "=h", "<H", ">B",
  "=i", "<I", ">I",
  "=q", "<q", ">q",
  "=H", "<H", ">H",
  "=I", "<I", ">I",
  "=Q", "<Q", ">Q",
  "=e", "<e", ">e",
  "=f", "<f", ">f",
  "=d", "<d", ">d",
  "=E", "<E", ">E",
  "=F", "<F", ">F",
  "=D", "<D", ">D",

  "(2)=Q",
  "(2)<Q",
  "(2)>Q",
  "(2,3)=Q",
  "(2,3)<Q",
  "(2,3)>Q",
  "(2,3,4)=Q",
  "(2,3,4)<Q",
  "(2,3,4)>Q",

  "T{=b:a:100s:b:}",
  "T{T{=F:foo:(2,3)=I:bar:}:a:100s:b:}",

   NULL
};

const char *buffer_error_tests[] = {
  "&",
  "Ze",
  "Zf",
  "Zd",
  "O", "O",
  "T{O:a:100O:b:}",

  "(2,3)T{f:a:=0s:b:}",

  "{10,2)f",
  "({,2)f",
  "(10{2)f",
  "(10,{)f",
  "(10,2}f",
  "(10,2}}",

  "(10,2)T{{:a:100s:b:}",
  "(10,2)T{b{a:100s:b:}",
  "(10,2)T{b:{:100s:b:}",
  "(10,2)T{b:a:{:b:}",
  "(10,2)T{b:a:100{:b:}",
  "(10,2)T{b:a:100s{b:}",
  "(10,2)T{b:a:100s:{:}",
  "(10,2)T{b:a:100s:b{}",
  "(10,2)T{b:a:100s:b:{",
  "(10,2)T{b:a:100s:b:}}",

  "T{T{F:foo:(2,3)%:bar:}:a:100s:b:}",

   NULL
};

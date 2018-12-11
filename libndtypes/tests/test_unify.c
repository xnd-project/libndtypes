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


const unify_testcase_t unify_tests[] = {

  { "bool", "bool", "bool" },
  { "bool", "?bool", "?bool" },
  { "?bool", "bool", "?bool" },

  { "bool", "int8", NULL },
  { "bool", "?int8", NULL },
  { "?bool", "int8", NULL },
  { "?bool", "?int8", NULL },

  { "int8", "int8", "int8" },
  { "int8", "int16", "int16" },
  { "int8", "int32", "int32" },
  { "int8", "int64", "int64" },

  { "int8", "?int8", "?int8" },
  { "?int8", "int8", "?int8" },
  { "?int8", "int16", "?int16" },
  { "int8", "?int16", "?int16" },

  { "bfloat16", "?bfloat16", "?bfloat16" },
  { "float16", "?float16", "?float16" },

  { "complex32", "?complex64", "?complex64" },
  { "?complex64", "complex128", "?complex128" },

  { "complex32", "?string", NULL },

  { "2 * 3 * int64",
    "2 * 4 * int64",
    NULL },

  { "2 * 3 * int64",
    "2 * 3 * int64",
    "2 * 3 * int64" },

  { "2 * 3 * int64",
    "2 * 3 * ?int64",
    "2 * 3 * ?int64" },

  { "2 * 3 * ?int64",
    "2 * 3 * int64",
    "2 * 3 * ?int64" },

  { "2 * 3 * int32",
    "2 * 3 * int64",
    "2 * 3 * int64" },

  { "2 * 3 * int8",
    "2 * 3 * int64",
    "2 * 3 * int64" },

  { "2 * 3 * (int8, float32)",
    "2 * 3 * (int32, float64)",
    "2 * 3 * (int32, float64)" },

  { "2 * 3 * (int8, {foo: complex64, bar: string})",
    "2 * 3 * (int8, {foo: complex64, x: string})",
    NULL },

  { "2 * 3 * (int8, {foo: complex64, bar: string})",
    "2 * 3 * (int8, {foo: complex128, bar: string})",
    "2 * 3 * (int8, {foo: complex128, bar: string})" },

  { "2 * 3 * (int8, {foo: ?Any, bar: string})",
    "2 * 3 * (int8, {foo: complex128, bar: ?Any})",
    "2 * 3 * (int8, {foo: ?complex128, bar: ?string})" },

  { "var(offsets=[0,2]) * (int8, {foo: complex64, bar: string})",
    "var(offsets=[0,3]) * (int8, {foo: complex128, bar: string})",
    NULL },

  { "var(offsets=[0,2]) * (int8, {foo: complex64, bar: string})",
    "var(offsets=[0,2]) * (int8, {foo: complex128, bar: int32})",
    NULL },

  { "var(offsets=[0,2]) * (int8, {foo: complex64, bar: string})",
    "var(offsets=[0,2]) * (int8, {foo: complex128, bar: string})",
    "var(offsets=[0,2]) * (int8, {foo: complex128, bar: string})" },

  { "var(offsets=[0,2]) * var(offsets=[0,3,10]) * (int8, {foo: complex64, bar: string})",
    "var(offsets=[0,2]) * var(offsets=[0,3,10]) * (int8, {foo: complex128, bar: string})",
    "var(offsets=[0,2]) * var(offsets=[0,3,10]) * (int8, {foo: complex128, bar: string})" },

  { NULL, NULL, 0 }
};

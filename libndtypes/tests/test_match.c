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


const match_testcase_t match_tests[] = {
  { "2395344366 * Any",
    "2395344366 * Any",
    1 },

  { "10 * 2395344366 * Any",
    "10 * ... * Any",
    0 },

  { "N * 10 * 2395344366 * Any",
    "10 * 10 * 2395344366 * Any",
    1 },

  { "N * N * 2395344366 * Any",
    "10 * 10 * 2395344366 * Any",
    1 },

  { "N * M * 2395344366 * Any",
    "10 * 10 * 2395344366 * Any",
    1 },

  { "N * M * N * Any",
    "10 * 10 * 2395344366 * Any",
    0 },

  { "N * M * P * Any",
    "... * 10 * 2395344366 * Any",
    0 },

  { "N * M * P * Any",
    "10 * ... * 2395344366 * Any",
    0 },

  { "N * M * P * Any",
    "10 * ... * 2395344366 * Any",
    0 },

  { "L * Any",
    "L * Any",
    1 },

  { "10 * L * Any",
    "100 * L * Any",
    0 },

  { "var * Any",
    "var * Any",
    1 },

  { "var * Any",
    "10 * Any",
    0 },

  { "10 * Any",
    "var * Any",
    0 },

  { "?var * Any",
    "?var * Any",
    1 },

  { "... * Any",
    "... * Any",
    1 },

  { "... * Any",
    "10 * 20 * 30 * Any",
    1 },

  { "... * Any",
    "10 * ... *  30 * Any",
    1 },

  { "10 * ... * Any",
    "10 * ... * Any",
    1 },

  { "10 * N * ... * Any",
    "10 * 10 * ... * Any",
    1 },

  { "N * 10 * ... * Any",
    "10 * 10 * ... * Any",
    1 },

  { "N * 20 * ... * Any",
    "10 * 10 * ... * Any",
    0 },

  { "N * N * ... * Any",
    "10 * 10 * ... * Any",
    1 },

  { "N * N * ... * Any",
    "10 * 10 * ... * 20 * Z * Any",
    1 },

  { "Any",
    "10 * Any",
    1 },

  { "Any",
    "10 * (...) -> int64",
    1 },

  { "?Any",
    "?Any",
    1 },

  { "10 * Scalar",
    "10 * Scalar",
    1 },

  { "10 * Scalar",
    "10 * (...)",
    0 },

  { "Scalar",
    "Scalar",
    1 },

  { "Scalar",
    "string",
    1 },

  { "?Scalar",
    "?uint64",
    1 },

  { "10 * bool",
    "10 * bool",
    1 },

  { "bool",
    "bool",
    1 },

  { "complex128",
    "bool",
    0 },

  { "?bool",
    "?bool",
    1 },

  { "10 * Signed",
    "10 * Signed",
    1 },

  { "Signed",
    "Signed",
    1 },

  { "Signed",
    "int64",
    1 },

  { "?Signed",
    "?int16",
    1 },

  { "10 * int8",
    "10 * int8",
    1 },

  { "int8",
    "int8",
    1 },

  { "?int8",
    "?Signed",
    0 },

  { "10 * int16",
    "10 * int16",
    1 },

  { "int16",
    "{...}",
    0 },

  { "?int16",
    "?int16",
    1 },

  { "10 * int32",
    "10 * int32",
    1 },

  { "int32",
    "int32",
    1 },

  { "?int32",
    "?int32",
    1 },

  { "10 * int64",
    "10 * float64",
    0 },

  { "int64",
    "int64",
    1 },

  { "?int64",
    "?int64",
    1 },

  { "10 * Unsigned",
    "10 * Unsigned",
    1 },

  { "10 * Unsigned",
    "10 * uint8",
    1 },

  { "Unsigned",
    "Unsigned",
    1 },

  { "?Unsigned",
    "?Unsigned",
    1 },

  { "10 * uint8",
    "10 * uint8",
    1 },

  { "uint32",
    "uint8",
    0 },

  { "?uint8",
    "?uint8",
    1 },

  { "10 * uint16",
    "10 * uint16",
    1 },

  { "uint16",
    "uint16",
    1 },

  { "?uint16",
    "?int16",
    0 },

  { "10 * uint32",
    "10 * uint32",
    1 },

  { "uint32",
    "uint32",
    1 },

  { "?uint32",
    "?uint32",
    1 },

  { "10 * uint64",
    "10 * uint64",
    1 },

  { "uint64",
    "uint64",
    1 },

  { "?uint64",
    "?uint64",
    1 },

  { "10 * Float",
    "10 * Float",
    1 },

  { "float64",
    "float64",
    1 },

  { "?float64",
    "?float64",
    1 },

  { "10 * Complex",
    "10 * complex128",
    1 },

  { "10 * Complex",
    "10 * complex64",
    1 },

  { "Complex",
    "Complex",
    1 },

  { "?Complex",
    "?X",
    0 },

  { "?X",
    "?Complex",
    1 },

  { "10 * complex64",
    "10 * complex64",
    1 },

  { "complex64",
    "complex64",
    1 },

  { "?complex64",
    "?complex64",
    1 },

  { "10 * complex128",
    "10 * complex128",
    1 },

  { "complex128",
    "complex64",
    0 },

  { "?complex128",
    "?float64",
    0 },

  { "10 * char('ascii')",
    "10 * char('ascii')",
    1 },

  { "char('ascii')",
    "char('ascii')",
    1 },

  { "?char('ascii')",
    "?char('ascii')",
    1 },

  { "10 * char('utf8')",
    "10 * char('utf16')",
    0 },

  { "char('utf8')",
    "char('utf8')",
    1 },

  { "?char('utf8')",
    "?char('utf16')",
    0 },

  { "10 * char('utf16')",
    "10 * char('utf16')",
    1 },

  { "char('utf16')",
    "char('utf16')",
    1 },

  { "?char('utf16')",
    "?char('utf16')",
    1 },

  { "10 * char('utf32')",
    "10 * char('utf32')",
    1 },

  { "char('utf32')",
    "10 * char('utf32')",
    0 },

  { "?char('utf32')",
    "?char('utf32')",
    1 },

  { "10 * char('ucs2')",
    "10 * char('ucs2')",
    1 },

  { "char('ucs2')",
    "char('ucs2')",
    1 },

  { "?char('ucs2')",
    "char('ucs2')",
    0 },

  { "10 * string",
    "10 * string",
    1 },

  { "string",
    "string",
    1 },

  { "?string",
    "?string",
    1 },

  { "10 * FixedString",
    "10 * FixedString",
    1 },

  { "10 * FixedString",
    "10 * fixed_string(100)",
    1 },

  { "FixedString",
    "fixed_bytes(size=200, align=2)",
    0 },

  { "?FixedString",
    "?string",
    0 },

  { "10 * fixed_string(729742655, 'ascii')",
    "10 * fixed_string(729742655, 'ascii')",
    1 },

  { "fixed_string(729742655, 'ascii')",
    "fixed_string(729742655, 'utf32')",
    0 },

  { "?fixed_string(729742655, 'ascii')",
    "?fixed_string(729742655, 'ascii')",
    1 },

  { "10 * fixed_string(3952068488, 'utf8')",
    "10 * fixed_bytes(size=3952068488, align=2)",
    0 },

  { "fixed_string(3952068488, 'utf8')",
    "fixed_string(3952068488, 'utf8')",
    1 },

  { "?fixed_string(3952068488, 'utf8')",
    "?fixed_string(3952068488, 'utf8')",
    1 },

  { "10 * fixed_string(2949183030, 'utf16')",
    "10 * string",
    0 },

  { "fixed_string(2949183030, 'utf16')",
    "fixed_string(2949183030, 'utf16')",
    1 },

  { "?fixed_string(2949183030, 'utf16')",
    "?fixed_string(2949183030, 'ascii')",
    0 },

  { "10 * fixed_string(375133997, 'utf32')",
    "10 * fixed_string(375133997, 'utf32')",
    1 },

  { "fixed_string(375133997, 'utf32')",
    "fixed_string(375133997, 'utf32')",
    1 },

  { "?fixed_string(375133997, 'utf32')",
    "?fixed_string(375133997, 'utf32')",
    1 },

  { "10 * fixed_string(969064578, 'ucs2')",
    "10 * fixed_string(969064578, 'ucs2')",
    1 },

  { "fixed_string(969064578, 'ucs2')",
    "fixed_string(969064578, 'ucs2')",
    1 },

  { "?fixed_string(969064578, 'ucs2')",
    "?fixed_string(969064578, 'ucs2')",
    1 },

  { "10 * bytes(align=16)",
    "10 * bytes(align=16)",
    1 },

  { "bytes(align=16)",
    "bytes(align=16)",
    1 },

  { "?bytes(align=16)",
    "?bytes(align=16)",
    1 },

  { "10 * FixedBytes",
    "10 * FixedBytes",
    1 },

  { "FixedBytes",
    "bytes(align=16)",
    0 },

  { "?FixedBytes",
    "?fixed_bytes(size=100, align=2)",
    1 },

  { "10 * fixed_bytes(size=1904128700, align=4)",
    "10 * fixed_bytes(size=1904128700, align=4)",
    1 },

  { "fixed_bytes(size=1904128700, align=4)",
    "fixed_bytes(size=1904128700, align=2)",
    0 },

  { "?fixed_bytes(size=1904128700, align=4)",
    "?fixed_bytes(size=1904128000, align=4)",
    0 },

  { "10 * categorical(63)",
    "10 * categorical(63)",
    1 },

  { "categorical(63)",
    "categorical(63)",
    1 },

  { "?categorical(63)",
    "?categorical(63)",
    1 },

  { "categorical(8)",
    "categorical(10)",
    0 },

  { "10 * categorical(10, 63)",
    "10 * categorical(10, 63)",
    1 },

  { "10 * categorical(10, 63)",
    "10 * categorical(63, 10)",
    0 },

  { "categorical(10, 63)",
    "categorical(10, 63)",
    1 },

  { "?categorical(10, 63)",
    "?categorical(10, 63)",
    1 },

  { "10 * categorical(10, 20, 63)",
    "10 * categorical(10, 20, 63)",
    1 },

  { "categorical(10, 20, 63)",
    "categorical(10, 20, 63)",
    1 },

  { "?categorical(10, 20, 63)",
    "?categorical(10, 20, 63)",
    1 },

  { "10 * categorical(18514)",
    "10 * categorical(18514)",
    1 },

  { "categorical(18514)",
    "categorical(18514)",
    1 },

  { "?categorical(18514)",
    "?categorical(18514)",
    1 },

  { "10 * categorical(10, 18514)",
    "10 * categorical(10, 18514)",
    1 },

  { "categorical(10, 18514)",
    "categorical(10, 18514)",
    1 },

  { "?categorical(10, 18514)",
    "?categorical(10, 18514)",
    1 },

  { "10 * categorical(10, 20, 18514)",
    "10 * categorical(10, 20, 18514)",
    1 },

  { "categorical(10, 20, 18514)",
    "categorical(10, 20, 18514)",
    1 },

  { "?categorical(10, 20, 18514)",
    "?categorical(10, 20, 18514)",
    1 },

  { "10 * categorical(-176354404)",
    "10 * categorical(-176354404)",
    1 },

  { "categorical(-176354404)",
    "categorical(-176354404)",
    1 },

  { "?categorical(-176354404)",
    "?categorical(-176354404)",
    1 },

  { "10 * categorical(10, -176354404)",
    "10 * categorical(10, -176354404)",
    1 },

  { "categorical(10, -176354404)",
    "categorical(10, -176354404)",
    1 },

  { "?categorical(10, -176354404)",
    "?categorical(10, -176354404)",
    1 },

  { "10 * categorical(10, 20, -176354404)",
    "10 * categorical(10, 20, -176354404)",
    1 },

  { "categorical(10, 20, -176354404)",
    "categorical(10, 20, -176354404)",
    1 },

  { "?categorical(10, 20, -176354404)",
    "?categorical(10, 20, -176354404)",
    1 },

  { "10 * categorical(500601201)",
    "10 * categorical(500601201)",
    1 },

  { "categorical(500601201)",
    "categorical(500601201)",
    1 },

  { "?categorical(500601201)",
    "?categorical(500601201)",
    1 },

  { "10 * categorical(10, 500601201)",
    "10 * categorical(10, 500601201)",
    1 },

  { "categorical(10, 500601201)",
    "categorical(10, 500601201)",
    1 },

  { "?categorical(10, 500601201)",
    "?categorical(10, 500601201)",
    1 },

  { "10 * categorical(10, 20, 500601201)",
    "10 * categorical(10, 20, 500601201)",
    1 },

  { "categorical(10, 20, 500601201)",
    "categorical(10, 20, 500601201)",
    1 },

  { "?categorical(10, 20, 500601201)",
    "?categorical(10, 20, 500601201)",
    1 },

  { "10 * categorical(135)",
    "10 * categorical(135)",
    1 },

  { "categorical(135)",
    "categorical(135)",
    1 },

  { "?categorical(135)",
    "?categorical(135)",
    1 },

  { "10 * categorical(10, 135)",
    "10 * categorical(10, 135)",
    1 },

  { "categorical(10, 135)",
    "categorical(10, 135)",
    1 },

  { "?categorical(10, 135)",
    "?categorical(10, 135)",
    1 },

  { "10 * categorical(10, 20, 135)",
    "10 * categorical(10, 20, 135)",
    1 },

  { "categorical(10, 20, 135)",
    "categorical(10, 20, 135)",
    1 },

  { "?categorical(10, 20, 135)",
    "?categorical(10, 20, 135)",
    1 },

  { "10 * categorical(22234)",
    "10 * categorical(22234)",
    1 },

  { "categorical(22234)",
    "categorical(22234)",
    1 },

  { "?categorical(22234)",
    "?categorical(22234)",
    1 },

  { "10 * categorical(10, 22234)",
    "10 * categorical(10, 22234)",
    1 },

  { "categorical(10, 22234)",
    "categorical(10, 22234)",
    1 },

  { "?categorical(10, 22234)",
    "?categorical(10, 22234)",
    1 },

  { "10 * categorical(10, 20, 22234)",
    "10 * categorical(10, 20, 22234)",
    1 },

  { "categorical(10, 20, 22234)",
    "categorical(10, 20, 22234)",
    1 },

  { "?categorical(10, 20, 22234)",
    "?categorical(10, 20, 22234)",
    1 },

  { "10 * categorical(3761672407)",
    "10 * categorical(3761672407)",
    1 },

  { "categorical(3761672407)",
    "categorical(3761672407)",
    1 },

  { "?categorical(3761672407)",
    "?categorical(3761672407)",
    1 },

  { "10 * categorical(10, 3761672407)",
    "10 * categorical(10, 3761672407)",
    1 },

  { "categorical(10, 3761672407)",
    "categorical(10, 3761672407)",
    1 },

  { "?categorical(10, 3761672407)",
    "?categorical(10, 3761672407)",
    1 },

  { "10 * categorical(10, 20, 3761672407)",
    "10 * categorical(10, 20, 3761672407)",
    1 },

  { "categorical(10, 20, 3761672407)",
    "categorical(10, 20, 3761672407)",
    1 },

  { "?categorical(10, 20, 3761672407)",
    "?categorical(10, 20, 3761672407)",
    1 },

  { "10 * categorical(2339269771)",
    "10 * categorical(2339269771)",
    1 },

  { "categorical(2339269771)",
    "categorical(2339269771)",
    1 },

  { "?categorical(2339269771)",
    "?categorical(2339269771)",
    1 },

  { "10 * categorical(10, 2339269771)",
    "10 * categorical(10, 2339269771)",
    1 },

  { "categorical(10, 2339269771)",
    "categorical(10, 2339269771)",
    1 },

  { "?categorical(10, 2339269771)",
    "?categorical(10, 2339269771)",
    1 },

  { "10 * categorical(10, 20, 2339269771)",
    "10 * categorical(10, 20, 2339269771)",
    1 },

  { "categorical(10, 20, 2339269771)",
    "categorical(10, 20, 2339269771)",
    1 },

  { "?categorical(10, 20, 2339269771)",
    "?categorical(10, 20, 2339269771)",
    1 },

  { "10 * categorical(1.2)",
    "10 * categorical(1.2)",
    1 },

  { "categorical(-1.2e+33)",
    "categorical(-1.2e+33)",
    1 },

  { "?categorical(-1.2e-33)",
    "?categorical(-1.2e-33)",
    1 },

  { "10 * categorical(10, 1.05e+10)",
    "10 * categorical(10, 1.05e+10)",
    1 },

  { "categorical(10, 1.05e+10)",
    "categorical(10, 1.05e+10)",
    1 },

  { "?categorical(10, 1.05e+10)",
    "?categorical(10, 1.05e+10)",
    1 },

  { "10 * categorical(10, 20, 1.05e+10)",
    "10 * categorical(10, 20, 1.05e+10)",
    1 },

  { "categorical(10, 20, 1.05e+10)",
    "categorical(10, 20, 1.05e+10)",
    1 },

  { "?categorical(10, 20, 1.05e+10)",
    "?categorical(10, 20, 1.05e+10)",
    1 },

  { "10 * categorical(1.05e+300)",
    "10 * categorical(1.05e+300)",
    1 },

  { "categorical(-1.05e+300)",
    "categorical(-1.05e+300)",
    1 },

  { "?categorical(-1.05e+300)",
    "?categorical(-1.05e+300)",
    1 },

  { "10 * categorical(10, 1.05e+300)",
    "10 * categorical(10, 1.05e+300)",
    1 },

  { "categorical(10, 1.05e+300)",
    "categorical(10, 1.05e+300)",
    1 },

  { "?categorical(10, 1.05e+300)",
    "?categorical(10, 1.05e+300)",
    1 },

  { "10 * categorical(10, 20, 1.05e+300)",
    "10 * categorical(10, 20, 1.05e+300)",
    1 },

  { "categorical(10, 20, 1.05e+300)",
    "categorical(10, 20, 1.05e+300)",
    1 },

  { "?categorical(10, 20, 1.05e+300)",
    "?categorical(10, 20, 1.05e+300)",
    1 },

  { "10 * categorical('jRAMoBPQ')",
    "10 * categorical('jRAMoBPQ')",
    1 },

  { "categorical('jRAMoBPQ')",
    "categorical('jRAMoBPQ')",
    1 },

  { "?categorical('jRAMoBPQ')",
    "?categorical('jRAMoBPQ')",
    1 },

  { "10 * categorical(10, 'jRAMoBPQ')",
    "10 * categorical(10, 'jRAMoBPQ')",
    1 },

  { "categorical(10, 'jRAMoBPQ')",
    "categorical(10, 'jRAMoBPQ')",
    1 },

  { "?categorical(10, 'jRAMoBPQ')",
    "?categorical(10, 'jRAMoBPQ')",
    1 },

  { "10 * categorical(10, 20, 'jRAMoBPQ')",
    "10 * categorical(10, 20, 'jRAMoBPQ')",
    1 },

  { "categorical(10, 20, 'jRAMoBPQ')",
    "categorical(10, 20, 'jRAMoBPQ')",
    1 },

  { "?categorical(10, 20, 'jRAMoBPQ')",
    "?categorical(10, 20, 'jRAMoBPQ')",
    1 },

  { "10 * categorical(127)",
    "10 * categorical(127)",
    1 },

  { "categorical(127)",
    "categorical(127)",
    1 },

  { "?categorical(127)",
    "?categorical(127)",
    1 },

  { "10 * categorical(10, 127)",
    "10 * categorical(10, 127)",
    1 },

  { "categorical(10, 127)",
    "categorical(10, 127)",
    1 },

  { "?categorical(10, 127)",
    "?categorical(10, 127)",
    1 },

  { "10 * categorical(10, 20, 127)",
    "10 * categorical(10, 20, 127)",
    1 },

  { "categorical(10, 20, 127)",
    "categorical(10, 20, 127)",
    1 },

  { "?categorical(10, 20, 127)",
    "?categorical(10, 20, 127)",
    1 },

  { "10 * categorical(-128)",
    "10 * categorical(-128)",
    1 },

  { "categorical(-128)",
    "categorical(-128)",
    1 },

  { "?categorical(-128)",
    "?categorical(-128)",
    1 },

  { "10 * categorical(10, -128)",
    "10 * categorical(10, -128)",
    1 },

  { "categorical(10, -128)",
    "categorical(10, -128)",
    1 },

  { "?categorical(10, -128)",
    "?categorical(10, -128)",
    1 },

  { "10 * categorical(10, 20, -128)",
    "10 * categorical(10, 20, -128)",
    1 },

  { "categorical(10, 20, -128)",
    "categorical(10, 20, -128)",
    1 },

  { "?categorical(10, 20, -128)",
    "?categorical(10, 20, -128)",
    1 },

  { "10 * categorical(32767)",
    "10 * categorical(32767)",
    1 },

  { "categorical(32767)",
    "categorical(32767)",
    1 },

  { "?categorical(32767)",
    "?categorical(32767)",
    1 },

  { "10 * categorical(10, 32767)",
    "10 * categorical(10, 32767)",
    1 },

  { "categorical(10, 32767)",
    "categorical(10, 32767)",
    1 },

  { "?categorical(10, 32767)",
    "?categorical(10, 32767)",
    1 },

  { "10 * categorical(10, 20, 32767)",
    "10 * categorical(10, 20, 32767)",
    1 },

  { "categorical(10, 20, 32767)",
    "categorical(10, 20, 32767)",
    1 },

  { "?categorical(10, 20, 32767)",
    "?categorical(10, 20, 32767)",
    1 },

  { "10 * categorical(-32768)",
    "10 * categorical(-32768)",
    1 },

  { "categorical(-32768)",
    "categorical(-32768)",
    1 },

  { "?categorical(-32768)",
    "?categorical(-32768)",
    1 },

  { "10 * categorical(10, -32768)",
    "10 * categorical(10, -32768)",
    1 },

  { "categorical(10, -32768)",
    "categorical(10, -32768)",
    1 },

  { "?categorical(10, -32768)",
    "?categorical(10, -32768)",
    1 },

  { "10 * categorical(10, 20, -32768)",
    "10 * categorical(10, 20, -32768)",
    1 },

  { "categorical(10, 20, -32768)",
    "categorical(10, 20, -32768)",
    1 },

  { "?categorical(10, 20, -32768)",
    "?categorical(10, 20, -32768)",
    1 },

  { "10 * categorical(2147483647)",
    "10 * categorical(2147483647)",
    1 },

  { "categorical(2147483647)",
    "categorical(2147483647)",
    1 },

  { "?categorical(2147483647)",
    "?categorical(2147483647)",
    1 },

  { "10 * categorical(10, 2147483647)",
    "10 * categorical(10, 2147483647)",
    1 },

  { "categorical(10, 2147483647)",
    "categorical(10, 2147483647)",
    1 },

  { "?categorical(10, 2147483647)",
    "?categorical(10, 2147483647)",
    1 },

  { "10 * categorical(10, 20, 2147483647)",
    "10 * categorical(10, 20, 2147483647)",
    1 },

  { "categorical(10, 20, 2147483647)",
    "categorical(10, 20, 2147483647)",
    1 },

  { "?categorical(10, 20, 2147483647)",
    "?categorical(10, 20, 2147483647)",
    1 },

  { "10 * categorical(-2147483648)",
    "10 * categorical(-2147483648)",
    1 },

  { "categorical(-2147483648)",
    "categorical(-2147483648)",
    1 },

  { "?categorical(-2147483648)",
    "?categorical(-2147483648)",
    1 },

  { "10 * categorical(10, -2147483648)",
    "10 * categorical(10, -2147483648)",
    1 },

  { "categorical(10, -2147483648)",
    "categorical(10, -2147483648)",
    1 },

  { "?categorical(10, -2147483648)",
    "?categorical(10, -2147483648)",
    1 },

  { "10 * categorical(10, 20, -2147483648)",
    "10 * categorical(10, 20, -2147483648)",
    1 },

  { "categorical(10, 20, -2147483648)",
    "categorical(10, 20, -2147483648)",
    1 },

  { "?categorical(10, 20, -2147483648)",
    "?categorical(10, 20, -2147483648)",
    1 },

  { "10 * categorical(9223372036854775807)",
    "10 * categorical(9223372036854775807)",
    1 },

  { "categorical(9223372036854775807)",
    "categorical(9223372036854775807)",
    1 },

  { "?categorical(9223372036854775807)",
    "?categorical(9223372036854775807)",
    1 },

  { "10 * categorical(10, 9223372036854775807)",
    "10 * categorical(10, 9223372036854775807)",
    1 },

  { "categorical(10, 9223372036854775807)",
    "categorical(10, 9223372036854775807)",
    1 },

  { "?categorical(10, 9223372036854775807)",
    "?categorical(10, 9223372036854775807)",
    1 },

  { "10 * categorical(10, 20, 9223372036854775807)",
    "10 * categorical(10, 20, 9223372036854775807)",
    1 },

  { "categorical(10, 20, 9223372036854775807)",
    "categorical(10, 20, 9223372036854775807)",
    1 },

  { "?categorical(10, 20, 9223372036854775807)",
    "?categorical(10, 20, 9223372036854775807)",
    1 },

  { "10 * categorical(-9223372036854775808)",
    "10 * categorical(-9223372036854775808)",
    1 },

  { "categorical(-9223372036854775808)",
    "categorical(-9223372036854775808)",
    1 },

  { "?categorical(-9223372036854775808)",
    "?categorical(-9223372036854775808)",
    1 },

  { "10 * categorical(10, -9223372036854775808)",
    "10 * categorical(10, -9223372036854775808)",
    1 },

  { "categorical(10, -9223372036854775808)",
    "categorical(10, -9223372036854775808)",
    1 },

  { "?categorical(10, -9223372036854775808)",
    "?categorical(10, -9223372036854775808)",
    1 },

  { "10 * categorical(10, 20, -9223372036854775808)",
    "10 * categorical(10, 20, -9223372036854775808)",
    1 },

  { "categorical(10, 20, -9223372036854775808)",
    "categorical(10, 20, -9223372036854775808)",
    1 },

  { "?categorical(10, 20, -9223372036854775808)",
    "?categorical(10, 20, -9223372036854775808)",
    1 },

  { "10 * categorical(255)",
    "10 * categorical(255)",
    1 },

  { "categorical(255)",
    "categorical(255)",
    1 },

  { "?categorical(255)",
    "?categorical(255)",
    1 },

  { "10 * categorical(10, 255)",
    "10 * categorical(10, 255)",
    1 },

  { "categorical(10, 255)",
    "categorical(10, 255)",
    1 },

  { "?categorical(10, 255)",
    "?categorical(10, 255)",
    1 },

  { "10 * categorical(10, 20, 255)",
    "10 * categorical(10, 20, 255)",
    1 },

  { "categorical(10, 20, 255)",
    "categorical(10, 20, 255)",
    1 },

  { "?categorical(10, 20, 255)",
    "?categorical(10, 20, 255)",
    1 },

  { "10 * categorical(65535)",
    "10 * categorical(65535)",
    1 },

  { "categorical(65535)",
    "categorical(65535)",
    1 },

  { "?categorical(65535)",
    "?categorical(65535)",
    1 },

  { "10 * categorical(10, 65535)",
    "10 * categorical(10, 65535)",
    1 },

  { "categorical(10, 65535)",
    "categorical(10, 65535)",
    1 },

  { "?categorical(10, 65535)",
    "?categorical(10, 65535)",
    1 },

  { "10 * categorical(10, 20, 65535)",
    "10 * categorical(10, 20, 65535)",
    1 },

  { "categorical(10, 20, 65535)",
    "categorical(10, 20, 65535)",
    1 },

  { "?categorical(10, 20, 65535)",
    "?categorical(10, 20, 65535)",
    1 },

  { "10 * categorical(4294967295)",
    "10 * categorical(4294967295)",
    1 },

  { "categorical(4294967295)",
    "categorical(4294967295)",
    1 },

  { "?categorical(4294967295)",
    "?categorical(4294967295)",
    1 },

  { "10 * categorical(10, 4294967295)",
    "10 * categorical(10, 4294967295)",
    1 },

  { "categorical(10, 4294967295)",
    "categorical(10, 4294967295)",
    1 },

  { "?categorical(10, 4294967295)",
    "?categorical(10, 4294967295)",
    1 },

  { "10 * categorical(10, 20, 4294967295)",
    "10 * categorical(10, 20, 4294967295)",
    1 },

  { "categorical(10, 20, 4294967295)",
    "categorical(10, 20, 4294967295)",
    1 },

  { "?categorical(10, 20, 4294967295)",
    "?categorical(10, 20, 4294967295)",
    1 },

  { "10 * categorical(1.1755e-38)",
    "10 * categorical(1.1755e-38)",
    1 },

  { "categorical(1.1755e-38)",
    "categorical(1.1755e-38)",
    1 },

  { "?categorical(1.1755e-38)",
    "?categorical(1.1755e-38)",
    1 },

  { "10 * categorical(10, 1.1755e-38)",
    "10 * categorical(10, 1.1755e-38)",
    1 },

  { "categorical(10, 1.1755e-38)",
    "categorical(10, 1.1755e-38)",
    1 },

  { "?categorical(10, 1.1755e-38)",
    "?categorical(10, 1.1755e-38)",
    1 },

  { "10 * categorical(10, 20, 1.1755e-38)",
    "10 * categorical(10, 20, 1.1755e-38)",
    1 },

  { "categorical(10, 20, 1.1755e-38)",
    "categorical(10, 20, 1.1755e-38)",
    1 },

  { "?categorical(10, 20, 1.1755e-38)",
    "?categorical(10, 20, 1.1755e-38)",
    1 },

  { "10 * categorical(2.22508e-308)",
    "10 * categorical(2.22508e-308)",
    1 },

  { "categorical(2.22508e-308)",
    "categorical(2.22508e-308)",
    1 },

  { "?categorical(2.22508e-308)",
    "?categorical(2.22508e-308)",
    1 },

  { "10 * categorical(10, 2.22508e-308)",
    "10 * categorical(10, 2.22508e-308)",
    1 },

  { "categorical(10, 2.22508e-308)",
    "categorical(10, 2.22508e-308)",
    1 },

  { "?categorical(10, 2.22508e-308)",
    "?categorical(10, 2.22508e-308)",
    1 },

  { "10 * categorical(10, 20, 2.22508e-308)",
    "10 * categorical(10, 20, 2.22508e-308)",
    1 },

  { "categorical(10, 20, 2.22508e-308)",
    "categorical(10, 20, 2.22508e-308)",
    1 },

  { "?categorical(10, 20, 2.22508e-308)",
    "?categorical(10, 20, 2.22508e-308)",
    1 },

  { "10 * categorical(3.40282e+38)",
    "10 * categorical(3.40282e+38)",
    1 },

  { "categorical(3.40282e+38)",
    "categorical(3.40282e+38)",
    1 },

  { "?categorical(3.40282e+38)",
    "?categorical(3.40282e+38)",
    1 },

  { "10 * categorical(10, 3.40282e+38)",
    "10 * categorical(10, 3.40282e+38)",
    1 },

  { "categorical(10, 3.40282e+38)",
    "categorical(10, 3.40282e+38)",
    1 },

  { "?categorical(10, 3.40282e+38)",
    "?categorical(10, 3.40282e+38)",
    1 },

  { "10 * categorical(10, 20, 3.40282e+38)",
    "10 * categorical(10, 20, 3.40282e+38)",
    1 },

  { "categorical(10, 20, 3.40282e+38)",
    "categorical(10, 20, 3.40282e+38)",
    1 },

  { "?categorical(10, 20, 3.40282e+38)",
    "?categorical(10, 20, 3.40282e+38)",
    1 },

  { "10 * categorical(1.79769e+308)",
    "10 * categorical(1.79769e+308)",
    1 },

  { "categorical(1.79769e+308)",
    "categorical(1.79769e+308)",
    1 },

  { "?categorical(1.79769e+308)",
    "?categorical(1.79769e+308)",
    1 },

  { "10 * categorical(10, 1.79769e+308)",
    "10 * categorical(10, 1.79769e+308)",
    1 },

  { "categorical(10, 1.79769e+308)",
    "categorical(10, 1.79769e+308)",
    1 },

  { "?categorical(10, 1.79769e+308)",
    "?categorical(10, 1.79769e+308)",
    1 },

  { "10 * categorical(10, 20, 1.79769e+308)",
    "10 * categorical(10, 20, 1.79769e+308)",
    1 },

  { "categorical(10, 20, 1.79769e+308)",
    "categorical(10, 20, 1.79769e+308)",
    1 },

  { "?categorical(10, 20, 1.79769e+308)",
    "?categorical(10, 20, 1.79769e+308)",
    1 },

  { "10 * categorical('')",
    "10 * categorical('')",
    1 },

  { "categorical('')",
    "categorical('')",
    1 },

  { "?categorical('')",
    "?categorical('')",
    1 },

  { "10 * categorical(10, '')",
    "10 * categorical(10, '')",
    1 },

  { "categorical(10, '')",
    "categorical(10, '')",
    1 },

  { "?categorical(10, '')",
    "?categorical(10, '')",
    1 },

  { "10 * categorical(10, 20, '')",
    "10 * categorical(10, 20, '')",
    1 },

  { "categorical(10, 20, '')",
    "categorical(10, 20, '')",
    1 },

  { "?categorical(10, 20, '')",
    "?categorical(10, 20, '')",
    1 },

  { "10 * categorical('')",
    "10 * categorical('')",
    1 },

  { "categorical('')",
    "categorical('')",
    1 },

  { "?categorical('')",
    "?categorical('')",
    1 },

  { "10 * categorical(10, '')",
    "10 * categorical(10, '')",
    1 },

  { "categorical(10, '')",
    "categorical(10, '')",
    1 },

  { "?categorical(10, '')",
    "?categorical(10, '')",
    1 },

  { "10 * categorical(10, 20, '')",
    "10 * categorical(10, 20, '')",
    1 },

  { "categorical(10, 20, '')",
    "categorical(10, 20, '')",
    1 },

  { "?categorical(10, 20, '')",
    "?categorical(10, 20, '')",
    1 },

  { "10 * categorical('xyz')",
    "10 * categorical('xyz')",
    1 },

  { "categorical('xyz')",
    "categorical('xyz')",
    1 },

  { "?categorical('xyz')",
    "?categorical('xyz')",
    1 },

  { "10 * categorical(10, 'xyz')",
    "10 * categorical(10, 'xyz')",
    1 },

  { "categorical(10, 'xyz')",
    "categorical(10, 'xyz')",
    1 },

  { "?categorical(10, 'xyz')",
    "?categorical(10, 'xyz')",
    1 },

  { "10 * categorical(10, 20, 'xyz')",
    "10 * categorical(10, 20, 'xyz')",
    1 },

  { "categorical(10, 20, 'xyz')",
    "categorical(10, 20, 'xyz')",
    1 },

  { "?categorical(10, 20, 'xyz')",
    "?categorical(10, 20, 'xyz')",
    1 },

  { "10 * categorical('xyz')",
    "10 * categorical('xyz')",
    1 },

  { "categorical('xyz')",
    "categorical('xyz')",
    1 },

  { "?categorical('xyz')",
    "?categorical('xyz')",
    1 },

  { "10 * categorical(10, 'xyz')",
    "10 * categorical(10, 'xyz')",
    1 },

  { "categorical(10, 'xyz')",
    "categorical(10, 'xyz')",
    1 },

  { "?categorical(10, 'xyz')",
    "?categorical(10, 'xyz')",
    1 },

  { "10 * categorical(10, 20, 'xyz')",
    "10 * categorical(10, 20, 'xyz')",
    1 },

  { "categorical(10, 20, 'xyz')",
    "categorical(10, 20, 'xyz')",
    1 },

  { "?categorical(10, 20, 'xyz')",
    "?categorical(10, 20, 'xyz')",
    1 },

  { "10 * ref(Any)",
    "10 * ref(Any)",
    1 },

  { "ref(Any)",
    "ref(Any)",
    1 },

  { "?ref(Any)",
    "?ref(Any)",
    1 },

  { "10 * (...)",
    "10 * (...)",
    1 },

  { "(...)",
    "(...)",
    1 },

  { "?(...)",
    "?(...)",
    1 },

  { "10 * (Any)",
    "10 * (Any)",
    1 },

  { "(Any)",
    "(Any)",
    1 },

  { "?(Any)",
    "?(Any)",
    1 },

  { "10 * (int64, Any)",
    "10 * (int64, Any)",
    1 },

  { "(int64, Any)",
    "(int64, Any)",
    1 },

  { "?(int64, Any)",
    "?(int64, Any)",
    1 },

  { "10 * (int64, int64, Any)",
    "10 * (int64, int64, Any)",
    1 },

  { "(int64, int64, Any)",
    "(int64, int64, Any)",
    1 },

  { "?(int64, int64, Any)",
    "?(int64, int64, Any)",
    1 },

  { "10 * (Any)",
    "10 * (Any)",
    1 },

  { "(Any)",
    "(Any)",
    1 },

  { "?(Any)",
    "?(Any)",
    1 },

  { "10 * (Any)",
    "10 * (Any)",
    1 },

  { "(Any)",
    "(Any)",
    1 },

  { "?(Any)",
    "?(Any)",
    1 },

  { "10 * (Any, ...)",
    "10 * (Any, ...)",
    1 },

  { "(Any, ...)",
    "(Any, ...)",
    1 },

  { "?(Any, ...)",
    "?(Any, ...)",
    1 },

  { "10 * {...}",
    "10 * {...}",
    1 },

  { "{...}",
    "{...}",
    1 },

  { "?{...}",
    "?{...}",
    1 },

  { "10 * {a : Any}",
    "10 * {a : Any}",
    1 },

  { "{a : Any}",
    "{a : Any}",
    1 },

  { "?{a : Any}",
    "?{a : Any}",
    1 },

  { "10 * {x : int64, a : Any}",
    "10 * {x : int64, a : Any}",
    1 },

  { "{x : int64, a : Any}",
    "{x : int64, a : Any}",
    1 },

  { "?{x : int64, a : Any}",
    "?{x : int64, a : Any}",
    1 },

  { "10 * {x : int64, x : int64, a : Any}",
    "10 * {x : int64, x : int64, a : Any}",
    1 },

  { "{x : int64, x : int64, a : Any}",
    "{x : int64, x : int64, a : Any}",
    1 },

  { "?{x : int64, x : int64, a : Any}",
    "?{x : int64, x : int64, a : Any}",
    1 },

  { "10 * {a : int64}",
    "10 * {a : int64}",
    1 },

  { "{a : int64}",
    "{a : int64}",
    1 },

  { "?{a : int64}",
    "?{a : int64}",
    1 },

  { "10 * {a : int64}",
    "10 * {a : int64}",
    1 },

  { "{a : int64}",
    "{a : int64}",
    1 },

  { "?{a : int64}",
    "?{a : int64}",
    1 },

  { "10 * {a : int64, ...}",
    "10 * {a : int64, ...}",
    1 },

  { "{a : int64, ...}",
    "{a : int64, ...}",
    1 },

  { "?{a : int64, ...}",
    "?{a : int64, ...}",
    1 },

  { "10 * (...) -> Any",
    "10 * (...) -> Any",
    1 },

  { "(...) -> Any",
    "(...) -> Any",
    1 },

  { "?(...) -> Any",
    "?(...) -> Any",
    1 },

  { "10 * () -> (...)",
    "10 * () -> (...)",
    1 },

  { "() -> (...)",
    "() -> (...)",
    1 },

  { "?() -> (...)",
    "?() -> (...)",
    1 },

  { "10 * (Any) -> Any",
    "10 * (Any) -> Any",
    1 },

  { "(Any) -> Any",
    "(Any) -> Any",
    1 },

  { "?(Any) -> Any",
    "?(Any) -> Any",
    1 },

  { "10 * () -> (Any)",
    "10 * () -> (Any)",
    1 },

  { "() -> (Any)",
    "() -> (Any)",
    1 },

  { "?() -> (Any)",
    "?() -> (Any)",
    1 },

  { "10 * (int64, Any) -> Any",
    "10 * (int64, Any) -> Any",
    1 },

  { "(int64, Any) -> Any",
    "(int64, Any) -> Any",
    1 },

  { "?(int64, Any) -> Any",
    "?(int64, Any) -> Any",
    1 },

  { "10 * () -> (int64, Any)",
    "10 * () -> (int64, Any)",
    1 },

  { "() -> (int64, Any)",
    "() -> (int64, Any)",
    1 },

  { "?() -> (int64, Any)",
    "?() -> (int64, Any)",
    1 },

  { "10 * (int64, int64, Any) -> Any",
    "10 * (int64, int64, Any) -> Any",
    1 },

  { "(int64, int64, Any) -> Any",
    "(int64, int64, Any) -> Any",
    1 },

  { "?(int64, int64, Any) -> Any",
    "?(int64, int64, Any) -> Any",
    1 },

  { "10 * () -> (int64, int64, Any)",
    "10 * () -> (int64, int64, Any)",
    1 },

  { "() -> (int64, int64, Any)",
    "() -> (int64, int64, Any)",
    1 },

  { "?() -> (int64, int64, Any)",
    "?() -> (int64, int64, Any)",
    1 },

  { "10 * (Any) -> Any",
    "10 * (Any) -> Any",
    1 },

  { "(Any) -> Any",
    "(Any) -> Any",
    1 },

  { "?(Any) -> Any",
    "?(Any) -> Any",
    1 },

  { "10 * () -> (Any)",
    "10 * () -> (Any)",
    1 },

  { "() -> (Any)",
    "() -> (Any)",
    1 },

  { "?() -> (Any)",
    "?() -> (Any)",
    1 },

  { "10 * (Any) -> Any",
    "10 * (Any) -> Any",
    1 },

  { "(Any) -> Any",
    "(Any) -> Any",
    1 },

  { "?(Any) -> Any",
    "?(Any) -> Any",
    1 },

  { "10 * () -> (Any)",
    "10 * () -> (Any)",
    1 },

  { "() -> (Any)",
    "() -> (Any)",
    1 },

  { "?() -> (Any)",
    "?() -> (Any)",
    1 },

  { "10 * (Any, ...) -> Any",
    "10 * (Any, ...) -> Any",
    1 },

  { "(Any, ...) -> Any",
    "(Any, ...) -> Any",
    1 },

  { "?(Any, ...) -> Any",
    "?(Any, ...) -> Any",
    1 },

  { "10 * () -> (Any, ...)",
    "10 * () -> (Any, ...)",
    1 },

  { "() -> (Any, ...)",
    "() -> (Any, ...)",
    1 },

  { "?() -> (Any, ...)",
    "?() -> (Any, ...)",
    1 },

  { "10 * (a : Any, ...) -> Any",
    "10 * (a : Any, ...) -> Any",
    1 },

  { "(a : Any) -> int64",
    "(a : Any) -> int64",
    1 },

  { "10 * (a : Any, ...) -> Any",
    "10 * (a : Any, ...) -> Any",
    1 },

  { "(a : Any, ...) -> Any",
    "(a : Any, ...) -> Any",
    1 },

  { "?(a : Any, ...) -> Any",
    "?(a : Any, ...) -> Any",
    1 },

  { "10 * (x : int64, a : Any, ...) -> Any",
    "10 * (x : int64, a : Any, ...) -> Any",
    1 },

  { "(x : int64, a : Any, ...) -> Any",
    "(x : int64, a : Any, ...) -> Any",
    1 },

  { "?(x : int64, a : Any, ...) -> Any",
    "?(x : int64, a : Any, ...) -> Any",
    1 },

  { "10 * (x : int64, x : int64, a : Any, ...) -> Any",
    "10 * (x : int64, x : int64, a : Any, ...) -> Any",
    1 },

  { "(x : int64, x : int64, a : Any, ...) -> Any",
    "(x : int64, x : int64, a : Any, ...) -> Any",
    1 },

  { "?(x : int64, x : int64, a : Any, ...) -> Any",
    "?(x : int64, x : int64, a : Any, ...) -> Any",
    1 },

  { "10 * (Any) -> Any",
    "10 * (Any) -> Any",
    1 },

  { "(Any) -> Any",
    "(Any) -> Any",
    1 },

  { "?(Any) -> Any",
    "?(Any) -> Any",
    1 },

  { "10 * (Any) -> Any",
    "10 * (Any) -> Any",
    1 },

  { "(Any) -> Any",
    "(Any) -> Any",
    1 },

  { "?(Any) -> Any",
    "?(Any) -> Any",

    1 },

  { "10 * (Any, ...) -> Any",
    "10 * (Any, ...) -> Any",
    1 },

  { "(Any, ...) -> Any",
    "(Any, ...) -> Any",
    1 },

  { "?(Any, ...) -> Any",
    "?(Any, ...) -> Any",
    1 },

  { "10 * (Any, a : Any) -> Any",
    "10 * (Any, a : Any) -> Any",
    1 },

  { "(Any, a : Any) -> Any",
    "(Any, a : Any) -> Any",
    1 },

  { "?(Any, a : Any) -> Any",
    "?(Any, a : Any) -> Any",
    1 },

  { "10 * (Any, ..., a : Any) -> Any",
    "10 * (Any, ..., a : Any) -> Any",
    1 },

  { "(Any, ..., a : Any) -> Any",
    "(Any, ..., a : Any) -> Any",
    1 },

  { "?(Any, ..., a : Any) -> Any",
    "?(Any, ..., a : Any) -> Any",
    1 },

  { "10 * (int64, Any, a : Any) -> Any",
    "10 * (int64, Any, a : Any) -> Any",
    1 },

  { "(int64, Any, a : Any) -> Any",
    "(int64, Any, a : Any) -> Any",
    1 },

  { "?(int64, Any, a : Any) -> Any",
    "?(int64, Any, a : Any) -> Any",
    1 },

  { "10 * (int64, Any, ..., a : Any) -> Any",
    "10 * (int64, Any, ..., a : Any) -> Any",
    1 },

  { "(int64, Any, ..., a : Any) -> Any",
    "(int64, Any, ..., a : Any) -> Any",
    1 },

  { "?(int64, Any, ..., a : Any) -> Any",
    "?(int64, Any, ..., a : Any) -> Any",
    1 },

  { "10 * (int64, int64, Any, a : Any) -> Any",
    "10 * (int64, int64, Any, a : Any) -> Any",
    1 },

  { "(int64, int64, Any, a : Any) -> Any",
    "(int64, int64, Any, a : Any) -> Any",
    1 },

  { "?(int64, int64, Any, a : Any) -> Any",
    "?(int64, int64, Any, a : Any) -> Any",
    1 },

  { "10 * (int64, int64, Any, ..., a : Any) -> Any",
    "10 * (int64, int64, Any, ..., a : Any) -> Any",
    1 },

  { "(int64, int64, Any, ..., a : Any) -> Any",
    "(int64, int64, Any, ..., a : Any) -> Any",
    1 },

  { "?(int64, int64, Any, ..., a : Any) -> Any",
    "?(int64, int64, Any, ..., a : Any) -> Any",
    1 },

  { "10 * (Any, a : Any) -> Any",
    "10 * (Any, a : Any) -> Any",
    1 },

  { "(Any, a : Any) -> Any",
    "(Any, a : Any) -> Any",
    1 },

  { "?(Any, a : Any) -> Any",
    "?(Any, a : Any) -> Any",
    1 },

  { "10 * (Any, x : int64, a : Any) -> Any",
    "10 * (Any, x : int64, a : Any) -> Any",
    1 },

  { "(Any, x : int64, a : Any) -> Any",
    "(Any, x : int64, a : Any) -> Any",
    1 },

  { "?(Any, x : int64, a : Any) -> Any",
    "?(Any, x : int64, a : Any) -> Any",
    1 },

  { "10 * (Any, x : int64, x : int64, a : Any) -> Any",
    "10 * (Any, x : int64, x : int64, a : Any) -> Any",
    1 },

  { "(Any, x : int64, x : int64, a : Any) -> Any",
    "(Any, x : int64, x : int64, a : Any) -> Any",
    1 },

  { "?(Any, x : int64, x : int64, a : Any) -> Any",
    "?(Any, x : int64, x : int64, a : Any) -> Any",
    1 },

  { "10 * (Any, a : Any) -> Any",
    "10 * (Any, a : Any) -> Any",
    1 },

  { "(Any, a : Any) -> Any",
    "(Any, a : Any) -> Any",
    1 },

  { "?(Any, a : Any) -> Any",
    "?(Any, a : Any) -> Any",
    1 },

  { "10 * (Any, ..., a : Any) -> Any",
    "10 * (Any, ..., a : Any) -> Any",
    1 },

  { "(Any, ..., a : Any) -> Any",
    "(Any, ..., a : Any) -> Any",
    1 },

  { "?(Any, ..., a : Any) -> Any",
    "?(Any, ..., a : Any) -> Any",
    1 },

  { "10 * (Any, a : Any) -> Any",
    "10 * (Any, a : Any) -> Any",
    1 },

  { "(Any, a : Any) -> Any",
    "(Any, a : Any) -> Any",
    1 },

  { "?(Any, a : Any) -> Any",
    "?(Any, a : Any) -> Any",
    1 },

  { "10 * (Any, ..., a : Any) -> Any",
    "10 * (Any, ..., a : Any) -> Any",
    1 },

  { "(Any, ..., a : Any) -> Any",
    "(Any, ..., a : Any) -> Any",
    1 },

  { "?(Any, ..., a : Any) -> Any",
    "?(Any, ..., a : Any) -> Any",
    1 },

  { "10 * (Any, a : Any, ...) -> Any",
    "10 * (Any, a : Any, ...) -> Any",
    1 },

  { "(Any, a : Any, ...) -> Any",
    "(Any, a : Any, ...) -> Any",
    1 },

  { "?(Any, a : Any, ...) -> Any",
    "?(Any, a : Any, ...) -> Any",
    1 },

  { "10 * (Any, ..., a : Any, ...) -> Any",
    "10 * (Any, ..., a : Any, ...) -> Any",
    1 },

  { "(Any, ..., a : Any, ...) -> Any",
    "(Any, ..., a : Any, ...) -> Any",
    1 },

  { "?(Any, ..., a : Any, ...) -> Any",
    "?(Any, ..., a : Any, ...) -> Any",
    1 },

  { "10 * Up",
    "10 * Up",
    1 },

  { "10 * X(Any)",
    "10 * X(Any)",
    1 },

  { "X(Any)",
    "X(Any)",
    1 },

  { "?X(Any)",
    "?X(Any)",
    1 },

  /* BEGIN RANDOM */
  { "fixed_bytes(size=1342281000, align=8)",
    "fixed_bytes(size=1342281000, align=8)",
    1 },

  { "?ref(fixed_string(3865363101, 'ucs2'))",
    "?ref(fixed_string(3865363101, 'ucs2'))",
    1 },

  { "var * var * var * (...)",
    "var * var * var * (...)",
    1 },

  { "categorical('a', 1619552300, 49062, 18772, -84, 'l')",
    "categorical('a', 1619552300, 49062, 18772, -84, 'l')",
    1 },

  { "?categorical(30987, -16536, 1.2e-32, 189, -823800099)",
    "?categorical(30987, -16536, 1.2e-32, 189, -823800099)",
    1 },

  { "categorical(1.2e-32, 2840608325, 44038, 1763886348, 'jOt', 24)",
    "categorical(1.2e-32, 2840608325, 44038, 1763886348, 'jOt', 24)",
    1 },

  { "categorical(-2049848438, 94, 3599739756, 3577056037, 1.2e-32)",
    "categorical(-2049848438, 94, 3599739756, 3577056037, 1.2e-32)",
    1 },

  { "categorical(2753441703, 1.2e-32)",
    "categorical(2753441703, 1.2e-32)",
    1 },

  { "var * var * ... * string",
    "var * var * ... * string",
    1 },

  { "1286044356 * LbR6JPFI * 2096958782 * uint8",
    "1286044356 * LbR6JPFI * 2096958782 * uint8",
    1 },

  { "categorical(1.2e-32, -25910, 'xM3Mys0XqH', 4265882500)",
    "categorical(1.2e-32, -25910, 'xM3Mys0XqH', 4265882500)",
    1 },

  { "var * var * FixedBytes",
    "var * var * FixedBytes",
    1 },

  { "var * ... * Nf",
    "var * ... * Nf",
    1 },

  { "var * var * var * fixed_bytes(size=2816010000, align=16)",
    "var * var * var * fixed_bytes(size=2816010000, align=16)",
    1 },

  { "36 * 16 * fixed_bytes(size=912328236, align=2)",
    "36 * 16 * fixed_bytes(size=912328236, align=2)",
    1 },

  { "var * var * var * ZcTmBXnKPi",
    "var * var * var * ZcTmBXnKPi",
    1 },

  { "?categorical(31275, 69, 1.2e-32)",
    "?categorical(31275, 69, 1.2e-32)",
    1 },

  { "categorical('omhwkoWVWw', 43, 946986991)",
    "categorical('omhwkoWVWw', 43, 946986991)",
    1 },

  { "var * float32",
    "var * float32",
    1 },

  { "var * var * var * uint16",
    "var * var * var * uint16",
    1 },

  { "var * ... * int16",
    "var * ... * int16",
    1 },

  { "var * var * var * Scalar",
    "var * var * var * Scalar",
    1 },

  { "?var * ... * int16",
    "?var * ... * int16",
    1 },

  { "fixed_bytes(size=280180000, align=8)",
    "fixed_bytes(size=280180000, align=8)",
    1 },

  { "fixed_string(1901614748, 'utf32')",
    "fixed_string(1901614748, 'utf32')",
    1 },

  { "var * M7",
    "var * M7",
    1 },

  { "var * bytes(align=2)",
    "var * bytes(align=2)",
    1 },

  { "var * ... * E",
    "var * ... * E",
    1 },

  { "2948091211 * ... * categorical(-1869840431, 2052908559)",
    "2948091211 * ... * categorical(-1869840431, 2052908559)",
    1 },

  { "fixed_bytes(size=1882250000, align=16)",
    "fixed_bytes(size=1882250000, align=16)",
    1 },

  { "categorical(1.2e-32, -58, -83)",
    "categorical(1.2e-32, -58, -83)",
    1 },

  { "3739637665 * 1476113530 * 1701021063 * IKd * 450514382 * WXn",
    "3739637665 * 1476113530 * 1701021063 * IKd * 450514382 * WXn",
    1 },

  { "ref(AilcKv4su1(FixedBytes))",
    "ref(AilcKv4su1(FixedBytes))",
    1 },

  { "var * ... * complex128",
    "var * ... * complex128",
    1 },

  { "?fixed_string(2371793992, 'ucs2')",
    "?fixed_string(2371793992, 'ucs2')",
    1 },

  { "var * R8KFFEabJ",
    "var * R8KFFEabJ",
    1 },

  { "?fixed_bytes(size=3129686000, align=8)",
    "?fixed_bytes(size=3129686000, align=8)",
    1 },

  { "var * QoFb",
    "var * QoFb",
    1 },

  { "var * ... * fixed_string(1464831555, 'ucs2')",
    "var * ... * fixed_string(1464831555, 'ucs2')",
    1 },

  { "var * fixed_string(1233383142, 'utf32')",
    "var * fixed_string(1233383142, 'utf32')",
    1 },

  { "fixed_bytes(size=2882790000, align=16)",
    "fixed_bytes(size=2882790000, align=16)",
    1 },

  { "fixed_bytes(size=1233209000, align=8)",
    "fixed_bytes(size=1233209000, align=8)",
    1 },

  { "WhRsMHHXYp(categorical(145))",
    "WhRsMHHXYp(categorical(145))",
    1 },

  { "?categorical(4079164787, 23814)",
    "?categorical(4079164787, 23814)",
    1 },

  { "?categorical(123, 1.2e-32, 'piLDeH2', 214836863)",
    "?categorical(123, 1.2e-32, 'piLDeH2', 214836863)",
    1 },

  { "var * var * bytes(align=16)",
    "var * var * bytes(align=16)",
    1 },

  { "categorical(-49, -26791, 1780, -85, 'pX', 8094, 1.2e-32)",
    "categorical(-49, -26791, 1780, -85, 'pX', 8094, 1.2e-32)",
    1 },

  { "D6nfBg_(categorical(147170982, 16278, 1.2e-32, 'ylR7', 26651))",
    "D6nfBg_(categorical(147170982, 16278, 1.2e-32, 'ylR7', 26651))",
    1 },

  { "categorical(-1.2e-32, 3229733591, 482331467, 1.2e-32)",
    "categorical(-1.2e-32, 3229733591, 482331467, 1.2e-32)",
    1 },

  /* END RANDOM */

  /* BEGIN MANUALLY GENERATED */

  { "()",
    "()",
    1 },

  { "(...)",
    "(...)",
    1 },

  { "{}",
    "{}",
    1 },

  { "{...}",
    "{...}",
    1 },

  { "() -> ()",
    "() -> ()",
    1 },

  { "(...) -> ()",
    "(...) -> ()",
    1 },

  { "(...) -> (...)",
    "(...) -> (...)",
    1 },

  { "(..., a : int64, ...) -> complex128",
    "(..., a : int64, ...) -> complex128",
    1 },

  { "() -> int32",
    "() -> int32",
    1 },

  { "(int32) -> int32",
    "(int32) -> int32",
    1 },

  { "(int32, float64) -> int32",
    "(int32, float64) -> int32",
    1 },

  { "(scale : float64) -> int32",
    "(scale : float64) -> int32",
    1 },

  { "(scale : float64, color : float64) -> int32",
    "(scale : float64, color : float64) -> int32",
    1 },

  { "(int32, scale : float64) -> int32",
    "(int32, scale : float64) -> int32",
    1 },

  { "(int32, scale : float64, color : float64) -> int32",
    "(int32, scale : float64, color : float64) -> int32",
    1 },

  { "(int32, float32, scale : float64, color : float64) -> int32",
    "(int32, float32, scale : float64, color : float64) -> int32",
    1 },

  { "(...) -> int32",
    "(...) -> int32",
    1 },

  { "(int32, ...) -> int32",
    "(int32, ...) -> int32",
    1 },

  { "(int32, float32, ...) -> int32",
    "(int32, float32, ...) -> int32",
    1 },

  { "(scale : float64, ...) -> int32",
    "(scale : float64, ...) -> int32",
    1 },

  { "(scale : float64, color : float64, ...) -> int32",
    "(scale : float64, color : float64, ...) -> int32",
    1 },

  { "(..., scale : float64) -> int32",
    "(..., scale : float64) -> int32",
    1 },

  { "(int32, ..., scale : float64) -> int32",
    "(int32, ..., scale : float64) -> int32",
    1 },

  { "(int32, float32, ..., scale : float64) -> int32",
    "(int32, float32, ..., scale : float64) -> int32",
    1 },

  { "(int32, scale : float64, ...) -> int32",
    "(int32, scale : float64, ...) -> int32",
    1 },

  { "(int32, float32, scale : float64, color : float64, ...) -> int32",
    "(int32, float32, scale : float64, color : float64, ...) -> int32",
    1 },

  { "(..., scale : float64, ...) -> int32",
    "(..., scale : float64, ...) -> int32",
    1 },

  { "(int32, ..., scale : float64, color : float64, ...) -> int32",
    "(int32, ..., scale : float64, color : float64, ...) -> int32",
    1 },

  { "(int32, float32, ..., scale : float64, color : float64, ...) -> int32",
    "(int32, float32, ..., scale : float64, color : float64, ...) -> int32",
    1 },


  { "10 * defined_t",
    "10 * defined_t",
    1 },

  { "defined_t",
    "defined_t",
    1 },

  { "?defined_t",
    "?defined_t",
    1 },

  { "defined_t",
    "defined_t",
    1 },

  { "(int32, float32, ..., scale : defined_t, color : float64, ...) -> defined_t",
    "(int32, float32, ..., scale : defined_t, color : float64, ...) -> defined_t",
    1 },

  { "(a : ref({b : defined_t, c : (defined_t, int16) -> float64})) -> defined_t",
    "(a : ref({b : defined_t, c : (defined_t, int16) -> float64})) -> defined_t",
    1 },


  { "10 * foo_t",
    "10 * foo_t",
    1 },

  { "foo_t",
    "foo_t",
    1 },

  { "?foo_t",
    "?foo_t",
    1 },

  { "foo_t",
    "foo_t",
    1 },

  { "(int32, float32, ..., scale : defined_t, color : float64, ...) -> foo_t",
    "(int32, float32, ..., scale : defined_t, color : float64, ...) -> foo_t",
    1 },

  { "(a : ref({b : defined_t, c : (foo_t, int16) -> float64})) -> foo_t",
    "(a : ref({b : defined_t, c : (foo_t, int16) -> float64})) -> foo_t",
    1 },

  { "10 * foo_t",
    "10 * foo_t", 1 },

  { "10 * foo_t",
    "11 * foo_t", 0 },

  { "N * foo_t",
    "10 * foo_t", 1 },

  { "N * foo_t",
    "10 * int64", 0 },

  { "... * foo_t",
    "10 * foo_t", 1 },

  { "... * foo_t",
    "... * foo_t", 1 },

  { "... * foo_t",
    "... * 10 * foo_t", 1 },

  { "... * 10 * foo_t",
    "... * foo_t", 0 },

  { "var * ... * 10 * 30 * foo_t",
    "... * foo_t", 0 },

  { "... * foo_t",
    "... * 10 * foo_t", 1 },

  { "N * N * foo_t",
    "10 * 10 * foo_t", 1 },

  { "N * Z * foo_t",
    "10 * 20 * foo_t", 1 },

  { "N * Z * ... * foo_t",
    "10 * 20 * 30 * foo_t", 1 },

  { "N * Z * ... * foo_t",
    "10 * 20 * 30 * 10 * ... * foo_t", 1 },

  { "N * Z * ... * foo_t",
    "10 * 20 * 30 * 10 * ... * foo_t", 1 },

  { "N * Z * ... * N * foo_t",
    "10 * 20 * 30 * 10 * ... * 10 * foo_t", 1 },

  { "N * Z * ... * N * foo_t",
    "10 * 20 * 30 * 10 * ... * 10 * foo_t", 1 },

  { "N * Z * ... * N * foo_t",
    "10 * 20 * 30 * 10 * ... * 20 * foo_t", 0 },

  { "N * Z * ... * N * foo_t",
    "A * 20 * 30 * 10 * ... * A * foo_t", 1 },

  { "N * Z * ... * N * Z * foo_t",
    "10 * 20 * 30 * 10 * ... * 10 * 20 * foo_t", 1 },

  { "N * Z * ... * 10 * N * Z * foo_t",
    "10 * 20 * 30 * 10 * ... * 10 * 20 * foo_t", 0 },

  /* Half-float */
  { "3 * float16",
    "3 * float16", 1 },

  { "3 * float32",
    "3 * float16", 0 },

  { "3 * complex32",
    "3 * complex32", 1 },

  { "3 * complex64",
    "3 * complex32", 0 },

  /* Named dtypes using the constructor syntax */
  { "3 * Matrix(3 * 3 * float64)",
    "3 * Matrix(3 * 3 * float64)", 1 },

  { "3 * Matrix(3 * 3 * float64)",
    "3 * 3 * 3 * float64", 0 },

  { "3 * Matrix(3 * 3 * float64)",
    "3 * OtherMatrix(3 * 3 * float64)", 0 },

  /* Named ellipses */
  { "Dims... * 2 * 3 * int64",
    "2 * 3 * int64", 1 },

  { "... * 2 * 3 * int64",
    "2 * 3 * int64", 1 },

  { "(Dims... * 2 * 3 * int64, Dims... * 2 * 3 * int64)",
    "(2 * 3 * int64, 2 * 3 * int64)", 1 },

  { "A... * float64",
    "A... * float64", 1 },

  { "(A... * float64, A... * float64)",
    "(A... * float64, A... * float64)", 1 },

  { "A... * float64",
    "3 * 5 * float64", 1 },

  { "A... * float64",
    "B... * float64", 1 },

  { "(A... * float64, A... * float64) -> A... * float64",
    "(3 * 5 * float64, 3 * 5 * float64) -> 3 * 5 * float64", 1 },

  { "(A... * float64, A... * float32, B... * float32)",
    "(B... * float64, 2 * 30 * float32, 3 * 30 * float32)", 0 },

  { "(A... * float64, A... * float64)",
    "(3 * 5 * float64, 3 * 5 * float64)", 1 },

  { "(3 * A... * float64, 3 * A... * float64)",
    "(3 * 5 * 10 *  float64, 3 * 5 * 10 * float64)", 1 },

  { "(A... * float64, A... * float64)",
    "(3 * 5 * float64, 3 * 4 * float64)", 0 },

  { "(A... * float64, A... * float64)",
    "(3 * 5 * float64, 3 * 5 * 6 * float64)", 0 },

  { "(A... * float64, A... * float32, B... * float32)",
    "(B... * float64, 3 * float32, 3 * float32)", 0 },

  { "(A... * float64, A... * float32, B... * float32)",
    "(B... * float64, 3 * ... * float32, 3 * ... * float32)", 0 },

  { "(3 * A... * float64, 3 * A... * float64)",
    "(3 * 5 * 10 *  float64, 3 * 5 * 2 * 10 * float64)", 0 },

  { "(A... * float64, A... * float64)",
    "(B... * float64, A... * float64)", 0 },

  { "(A... * float64, A... * float64)",
    "(A... * float64, B... * float64)", 0 },

  { "(A... * float64,  B... * float64,  C... * float64,    A... * float64)",
    "(B... * float64,  C... * float64,  3 * 10 * float64,  3 * 11 * float64)", 0 },

  { "(A... * float64,  B... * float64,  C... * float64,    A... * float64)",
    "(B... * float64,  C... * float64,  3 * 10 * float64,  3 * 10 * float64)", 0 },

  { "(A * float64,  B * float64,  C * float64,  A * float64)",
    "(B * float64,  C * float64,  3 * float64,  3 * float64)", 0 },

  { "(M * N * T, N * P * T)",
    "(2 * 3 * int64, 3 * 10 * int64)", 1 },

  { "... * M * N * T",
    "2 * 3 * int64", 1 },

  { "(... * M * N * T, ... * N * P * T)",
    "(2 * 3 * int64, 3 * 10 * int64)", 1 },

  { "(Dims... * M * N * T, Dims... * N * P * T)",
    "(2 * 3 * int64, 3 * 10 * int64)", 1 },

  { "(Dims... * M * N * T, Dims... * N * P * T)",
    "(2 * 3 * int64, 2 * 10 * int64)", 0 },

#if 0
  /* ndarray */
  { "[10 * 2 * int64, style='ndarray']",
    "[10 * 2 * int64, style='ndarray']", 1 },

  { "[10 * 2 * int32, style='ndarray']",
    "[10 * 2 * int64, style='ndarray']", 0 },

  { "[10 * N * int64, style='ndarray']",
    "[10 * 2 * int64, style='ndarray']", 1 },

  { "[10 * 2 * int64, style='ndarray']",
    "[10 * N * int64, style='ndarray']", 0 },

  { "[N * M * int64, style='ndarray']",
    "[10 * 2 * int64, style='ndarray']", 1 },

  { "[10 * 2 * int64, order='F', style='ndarray']",
    "[10 * 2 * int64, order='F', style='ndarray']", 1 },

  { "[10 * 2 * int64, order='C', style='ndarray']",
    "[10 * 2 * int64, order='F', style='ndarray']", 0 },

  { "[10 * 2 * int64, order='F', style='ndarray']",
    "[10 * 2 * int64, order='C', style='ndarray']", 0 },
#endif

  /* END MANUALLY GENERATED */

  { NULL, NULL, 0 }
};

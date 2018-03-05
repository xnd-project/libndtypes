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


const typecheck_testcase_t typecheck_tests[] = {

  { .signature = "void -> float32",
    .in        = {NULL},
    .out       = {"float32"},
    .nin       = 0,
    .nout      = 1,
    .outer     = 0,
    .success   = true },

  { .signature = "void -> T",
    .in        = {NULL},
    .nin       = 0,
    .out       = {NULL},
    .nout      = 0,
    .outer     = 0,
    .success   = false },

  { .signature = "T -> T",
    .in        = {"T"},
    .out       = {NULL},
    .nin       = 1,
    .nout      = 0,
    .outer     = 0,
    .success   = false },

  { .signature = "T -> T",
    .in        = {"int64"},
    .out       = {"int64"},
    .nin       = 1,
    .nout      = 1,
    .outer     = 0,
    .success   = true },

  { .signature = "M*N*T, N*P*T -> M*P*T",
    .in        = {"2 * 3 * int64", "3 * 10 * int64"},
    .out       = {"2 * 10 * int64"},
    .nin       = 2,
    .nout      = 1,
    .outer     = 0,
    .success   = true },

  { .signature = "Dims... *M*N*T, Dims... *N*P*T -> Dims... *M*P*T",
    .in        = {"2 * 3 * int64", "3 * 10 * int64"},
    .out       = {"2 * 10 * int64"},
    .nin       = 2,
    .nout      = 0,
    .outer     = 0,
    .success   = false },

  { .signature = "Dims... *M*N*T, Dims... *N*P*T -> Dims... *M*P*T",
    .in        = {"100 * 2 * 3 * int64", "100 * 3 * 10 * int64"},
    .out       = {"100 * 2 * 10 * int64"},
    .nin       = 2,
    .nout      = 1,
    .outer     = 1,
    .success   = true },

  { .signature = "Dims... *M*N*T, Dims... *N*P*T -> Dims... *M*P*T",
    .in        = {"2 * 3 * int64", "3 * 10 * int32"},
    .out       = {NULL},
    .nin       = 2,
    .nout      = 0,
    .outer     = 0,
    .success   = false },

  { .signature = "Dims... *M*N*T, Dims... *N*P*T -> Dims... *M*P*T",
    .in        = {"3 * int64", "3 * 10 * int64"},
    .out       = {NULL},
    .nin       = 2,
    .nout      = 0,
    .outer     = 0,
    .success   = false },

  { .signature = "Dims... * M*N*T, Dims... * N*P*T -> Dims... *M*T",
    .in        = {"10 * 2 * 3 * int64", "10 * 3 * 10 * int64"},
    .out       = {"10 * 2 * int64"},
    .nin       = 2,
    .nout      = 1,
    .outer     = 1,
    .success   = true },

  { .signature = "Dims... * M*T, Dims... * N*T -> Dims... * M*T",
    .in        = {"2 * 3 * int64", "3 * 3 * int64"},
    .out       = {NULL},
    .nin       = 2,
    .nout      = 0,
    .outer     = 0,
    .success   = false },

  { .signature = "Dims... * M*T, Dims... * N*T -> Dims... * M*T",
    .in        = {"2 * 3 * int64", "3 * 3 * int64"},
    .out       = {NULL},
    .nin       = 2,
    .nout      = 0,
    .outer     = 0,
    .success   = false },

  { .signature = "M*N*T, N*P*T -> M*P*T",
    .in        = {"2 * 3 * int64", "2 * 10 * int64"},
    .out       = {NULL},
    .nin       = 2,
    .nout      = 0,
    .outer     = 0,
    .success   = false },

  { .signature = "M*N*T, N*P*T -> M*P*T",
    .in        = {"2 * 3 * int64", "3 * 10 * int32"},
    .out       = {NULL},
    .nin       = 2,
    .nout      = 0,
    .outer     = 0,
    .success   = false },

  { .signature = "Dims... * M*N*T, Dims... * N*P*T -> Dims... *M*P*T",
    .in        = {"3 * int64", "3 * 10 * int64"},
    .out       = {NULL},
    .nin       = 2,
    .nout      = 0,
    .outer     = 0,
    .success   = false },

  { .signature = "Dims... * M*N*T, Dims... *N*P*T -> Dims... *M*P*T",
    .in        = {"100 * 2 * 3 * int64", "100 * 3 * 10 * int64"},
    .out       = {"100 * 2 * 10 * int64"},
    .nin       = 2,
    .nout      = 1,
    .outer     = 1,
    .success   = true },

  { .signature = "Dims... * M*N*T, Dims... *N*P*T -> Dims... *M*P*T",
    .in        = {"400 * 2 * 3 * int64", "400 * 3 * 10 * int64"},
    .out       = {"400 * 2 * 10 * int64"},
    .nin       = 2,
    .nout      = 1,
    .outer     = 1,
    .success   = true },

  { .signature = "Dims... * M*N*T, Dims... * N*P*T -> Dims... * M*P*T",
    .in        = {"1000 * 400 * 2 * 3 * int64", "1000 * 400 * 3 * 10 * int64"},
    .out       = {"1000 * 400 * 2 * 10 * int64"},
    .nin       = 2,
    .nout      = 1,
    .outer     = 2,
    .success   = true },

  { .signature = "Dims... * M*N*T, Dims... *N*P*T -> Dims... * M*P*T",
    .in        = {"2 * 3 * 1000 * 400 * 2 * 3 * int64", "2 * 3 * 1000 * 400 * 3 * 10 * int64"},
    .out       = {"2 * 3 * 1000 * 400 * 2 * 10 * int64"},
    .nin       = 2,
    .nout      = 1,
    .outer     = 4,
    .success   = true },

  { .signature = "D... * int64 -> D... * int64",
    .in        = {"2 * 3 * int64"},
    .out       = {"2 * 3 * int64"},
    .nin       = 1,
    .nout      = 1,
    .outer     = 2,
    .success   = true },

  { NULL, {NULL}, {NULL}, 0, 0, 0, false }
};

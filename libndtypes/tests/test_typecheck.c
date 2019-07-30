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

  { .loc = loc(),
    .success=true,

    .signature="void -> float32",
    .args={NULL},
    .kwargs={NULL},

    .outer_dims=0,
    .nin=0,
    .nout=1,
    .nargs=1,
    .types={"float32"} },

  { .loc = loc(),
    .success=true,

    .signature="void -> float32",
    .args={NULL},
    .kwargs={"float32"},

    .outer_dims=0,
    .nin=0,
    .nout=1,
    .nargs=1,
    .types={"float32"} },

  { .loc = loc(),
    .success=false,

    .signature="T -> T",
    .args={"T"},
    .kwargs={NULL} },

  { .loc = loc(),
    .success=false,

    .signature="T -> T",
    .args={"T"},
    .kwargs={"T"} },

  { .loc = loc(),
    .success=true,

    .signature="T -> T",
    .args={"int64"},
    .kwargs={NULL},

    .outer_dims=0,
    .nin=1,
    .nout=1,
    .nargs=2,
    .types={"int64", "int64"} },


  { .loc = loc(),
    .success=true,

    .signature="T -> T",
    .args={"int64"},
    .kwargs={"int64"},

    .outer_dims=0,
    .nin=1,
    .nout=1,
    .nargs=2,
    .types={"int64", "int64"} },


  { .loc = loc(),
    .success=false,

    .signature="T -> T",
    .args={"int64"},
    .kwargs={"int32"} },

  { .loc = loc(),
    .success=true,

    .signature="M*N*T, N*P*T -> M*P*T",
    .args={"2 * 3 * int64", "3 * 10 * int64"},
    .kwargs={NULL},

    .outer_dims=0,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={"2 * 3 * int64", "3 * 10 * int64", "2 * 10 * int64"} },

  { .loc = loc(),
    .success=true,

    .signature="M*N*T, N*P*T -> M*P*T",
    .args={"2 * 3 * int64", "3 * 10 * int64"},
    .kwargs={"2 * 10 * int64"},

    .outer_dims=0,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={"2 * 3 * int64", "3 * 10 * int64", "2 * 10 * int64"} },

  { .loc = loc(),
    .success=false,

    .signature="M*N*T, N*P*T -> M*P*T",
    .args={"2 * 3 * int64", "3 * 10 * int64", "3 * 10 * int64"},
    .kwargs={NULL} },

  { .loc = loc(),
    .success=true,

    .signature="Dims... *M*N*T, Dims... *N*P*T -> Dims... *M*P*T",
    .args={"2 * 3 * int64", "3 * 10 * int64"},
    .kwargs={NULL},

    .outer_dims=0,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={"2 * 3 * int64", "3 * 10 * int64", "2 * 10 * int64"} },

  { .loc = loc(),
    .success=true,

    .signature="Dims... *M*N*T, Dims... *N*P*T -> Dims... *M*P*T",
    .args={"2 * 3 * int64", "3 * 10 * int64"},
    .kwargs={"2 * 10 * int64"},

    .outer_dims=0,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={"2 * 3 * int64", "3 * 10 * int64", "2 * 10 * int64"} },

  { .loc = loc(),
    .success=true,

    .signature="Dims... *M*N*T, Dims... *N*P*T -> Dims... *M*P*T",
    .args={"100 * 2 * 3 * int64", "100 * 3 * 10 * int64"},
    .kwargs={NULL},

    .outer_dims=1,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={"100 * 2 * 3 * int64", "100 * 3 * 10 * int64", "100 * 2 * 10 * int64"} },

  { .loc = loc(),
    .success=true,

    .signature="Dims... *M*N*T, Dims... *N*P*T -> Dims... *M*P*T",
    .args={"100 * 2 * 3 * int64", "100 * 3 * 10 * int64"},
    .kwargs={"100 * 2 * 10 * int64"},

    .outer_dims=1,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={"100 * 2 * 3 * int64", "100 * 3 * 10 * int64", "100 * 2 * 10 * int64"} },

  { .loc = loc(),
    .success=false,

    .signature="Dims... *M*N*T, Dims... *N*P*T -> Dims... *M*P*T",
    .args={"100 * 2 * 3 * int64", "100 * 3 * 10 * int64"},
    .kwargs={"100 * 2 * 20 * int64"} },

  { .loc = loc(),
    .success=false,

    .signature="Dims... *M*N*T, Dims... *N*P*T -> Dims... *M*P*T",
    .args={"2 * 3 * int64", "3 * 10 * int32"},
    .kwargs={NULL} },

  { .loc = loc(), 
    .success=false,

    .signature="Dims... *M*N*T, Dims... *N*P*T -> Dims... *M*P*T",
    .args={"3 * int64", "3 * 10 * int64"},
    .kwargs={NULL} },

  { .loc = loc(),
    .success=true,

    .signature="Dims... * M*N*T, Dims... * N*P*T -> Dims... *M*T",
    .args={"10 * 2 * 3 * int64", "10 * 3 * 10 * int64"},
    .kwargs={NULL},

    .outer_dims=1,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={"10 * 2 * 3 * int64", "10 * 3 * 10 * int64", "10 * 2 * int64"} },

  { .loc = loc(),
    .success=true,

    .signature="Dims... * M*N*T, Dims... * N*P*T -> Dims... *M*T",
    .args={"10 * 2 * 3 * int64", "10 * 3 * 10 * int64"},
    .kwargs={"10 * 2 * int64"},

    .outer_dims=1,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={"10 * 2 * 3 * int64", "10 * 3 * 10 * int64", "10 * 2 * int64"} },

  { .loc = loc(),
    .success=false,

    .signature="Dims... * M*T, Dims... * N*T -> Dims... * M*T",
    .args={"2 * 3 * int64", "3 * 3 * int64"},
    .kwargs={NULL} },

  { .loc = loc(),
    .success=false,

    .signature="Dims... * M*T, Dims... * N*T -> Dims... * M*T",
    .args={"2 * 3 * int64", "3 * 3 * int64"},
    .kwargs={"3 * 3 * int64"} },

  { .loc = loc(),
    .success=false,

    .signature="Dims... * M*T, Dims... * N*T -> Dims... * M*T",
    .args={"2 * 3 * int64", "3 * 3 * int64"},
    .kwargs={NULL} },

  { .loc = loc(),
    .success=false,

    .signature="M*N*T, N*P*T -> M*P*T",
    .args={"2 * 3 * int64", "2 * 10 * int64"},
    .kwargs={NULL} },

  { .loc = loc(),
    .success=false,

    .signature="M*N*T, N*P*T -> M*P*T",
    .args={"2 * 3 * int64", "3 * 10 * int32"},
    .kwargs={NULL} },

  { .loc = loc(),
    .success=false,

    .signature="Dims... * M*N*T, Dims... * N*P*T -> Dims... *M*P*T",
    .args={"3 * int64", "3 * 10 * int64"},
    .kwargs={NULL} },

  { .loc = loc(),
    .success=true,

    .signature="Dims... * M*N*T, Dims... * N*P*T -> Dims... * M*P*T",
    .args={"1000 * 400 * 2 * 3 * int64", "1000 * 400 * 3 * 10 * int64"},
    .kwargs={NULL},

    .outer_dims=2,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types = { "1000 * 400 * 2 * 3 * int64",
               "1000 * 400 * 3 * 10 * int64",
               "1000 * 400 * 2 * 10 * int64"} },

  { .loc = loc(),
    .success=true,

    .signature="Dims... * M*N*T, Dims... * N*P*T -> Dims... * M*P*T",
    .args={"1000 * 400 * 2 * 3 * int64", "1000 * 400 * 3 * 10 * int64"},
    .kwargs={"1000 * 400 * 2 * 10 * int64"},

    .outer_dims=2,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types = { "1000 * 400 * 2 * 3 * int64",
               "1000 * 400 * 3 * 10 * int64",
               "1000 * 400 * 2 * 10 * int64"} },

  { .loc = loc(),
    .success=true,

    .signature="Dims... * M*N*T, Dims... *N*P*T -> Dims... * M*P*T",
    .args={"2 * 3 * 1000 * 400 * 2 * 3 * int64", "2 * 3 * 1000 * 400 * 3 * 10 * int64"},
    .kwargs={"2 * 3 * 1000 * 400 * 2 * 10 * int64"},

    .outer_dims=4,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types = { "2 * 3 * 1000 * 400 * 2 * 3 * int64",
               "2 * 3 * 1000 * 400 * 3 * 10 * int64",
               "2 * 3 * 1000 * 400 * 2 * 10 * int64"} },

  { .loc = loc(),
    .success=false,

    .signature="Dims... * M*N*T, Dims... *N*P*T -> Dims... * M*P*T",
    .args={"2 * 3 * 1000 * 400 * 2 * 3 * int64", "2 * 3 * 1000 * 400 * 3 * 10 * int64"},
    .kwargs={"2 * 3 * 100 * 400 * 2 * 10 * int64"} },

  { .loc = loc(),
    .success=true,

    .signature="D... * int64 -> D... * int64",
    .args={"2 * 3 * int64"},
    .kwargs={NULL},

    .outer_dims=2,
    .nin=1,
    .nout=1,
    .nargs=2,
    .types = { "2 * 3 * int64", "2 * 3 * int64"} },

  { .loc = loc(),
    .success=true,

    .signature="D... * int64 -> D... * int64",
    .args={"2 * 3 * int64"},
    .kwargs={"2 * 3 * int64"},

    .outer_dims=2,
    .nin=1,
    .nout=1,
    .nargs=2,
    .types = { "2 * 3 * int64", "2 * 3 * int64"} },

  { .loc = loc(),
    .success=false,

    .signature="D... * int64 -> D... * int64",
    .args={"2 * 3 * int64"},
    .kwargs={"2 * 3 * float32"} },

  { .loc = loc(),
    .success=true,

    .signature="... * int64 -> ... * int64",
    .args={"2 * 3 * int64"},
    .kwargs={NULL},

    .outer_dims=2,
    .nin=1,
    .nout=1,
    .nargs=2,
    .types = { "2 * 3 * int64", "2 * 3 * int64"} },

  { .loc = loc(),
    .success=true,

    .signature="... * int64 -> ... * int64",
    .args={"2 * 3 * int64"},
    .kwargs={"2 * 3 * int64"},

    .outer_dims=2,
    .nin=1,
    .nout=1,
    .nargs=2,
    .types = { "2 * 3 * int64", "2 * 3 * int64"} },

  { .loc = loc(),
    .success=false,

    .signature="... * int64 -> ... * int64",
    .args={"2 * 3 * int64"},
    .kwargs={"2 * 4 * int64"} },


  { .loc = loc(),
    .success=false,

    .signature="... * int64 -> ... * int64",
    .args={"2 * 3 * int64"},
    .kwargs={"2 * 3 * int64", "2 * 3 * int64"} },

  { .loc = loc(),
    .success=true,

    .signature="... * int64, ... * int64 -> ... * int64",
    .args={"2 * 3 * int64", "10 * 2 * 3 * int64"},
    .kwargs = {NULL},

    .outer_dims=3,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types = { "fixed(shape=10, step=0) * 2 * 3 * int64",
               "10 * 2 * 3 * int64",
               "10 * 2 * 3 * int64" } },

  { .loc = loc(),
    .success=true,

    .signature="... * int64, ... * int64 -> ... * int64",
    .args={"2 * 3 * int64", "10 * 2 * 3 * int64"},
    .kwargs = {"10 * 2 * 3 * int64"},

    .outer_dims=3,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types = { "fixed(shape=10, step=0) * 2 * 3 * int64",
               "10 * 2 * 3 * int64",
               "10 * 2 * 3 * int64" } },

  { .loc = loc(),
    .success=true,

    .signature="... * N * int64,      ... * int64 -> ... * int64",
    .args={"2 * 3 * int64", "10 * 2 * int64"},
    .kwargs={NULL},

    .outer_dims=2,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types = { "fixed(shape=10, step=0) * 2 * 3 * int64",
               "10 * 2 * int64",
               "10 * 2 * int64" } },

  { .loc = loc(),
    .success=true,

    .signature="... * N * int64, ... * int64 -> ... * int64",
    .args={"2 * 3 * int64", "20 * 10 * 2 * int64"},
    .kwargs={NULL},

    .outer_dims=3,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types = { "fixed(shape=20, step=0) * fixed(shape=10, step=0) * 2 * 3 * int64",
               "20 * 10 * 2 * int64",
               "20 * 10 * 2 * int64" } },

  { .loc = loc(),
    .success=true,

    .signature="... * N * int64, ... * int64 -> ... * N * int64",
    .args={"2 * 3 * int64", "20 * 10 * 2 * int64"},
    .kwargs={NULL},

    .outer_dims=3,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types = { "fixed(shape=20, step=0) * fixed(shape=10, step=0) * 2 * 3 * int64",
               "20 * 10 * 2 * int64",
               "20 * 10 * 2 * 3 * int64" } },

  { .loc = loc(),
    .success=false,

    .signature="var... * int64 -> var... * int64",
    .args={"2 * 3 * int64"},
    .kwargs={NULL} },

  { .loc = loc(),
    .success=false,

    .signature="A... * int64 -> A... * int64",
    .args={"var(offsets=[0,2]) * int64"},
    .kwargs={NULL} },

  { .loc = loc(),
    .success=true,

    .signature="var... * int64 -> var... * int64",
    .args={"var(offsets=[0,2]) * int64"},
    .kwargs={NULL},

    .outer_dims=1,
    .nin=1,
    .nout=1,
    .nargs=2,
    .types = { "var(offsets=[0,2]) * int64", "var(offsets=[0,2]) * int64" } },


  { .loc = loc(),
    .success=true,

    .signature="var... * int64 -> var... * int64",
    .args={"var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64"},
    .kwargs={"var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64"},

    .outer_dims=2,
    .nin=1,
    .nout=1,
    .nargs=2,
    .types = { "var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64",
               "var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64" } },

  { .loc = loc(),
    .success=true,

    .signature="var... * int64 -> var... * int64",
    .args={"var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64"},
    .kwargs={NULL},

    .outer_dims=2,
    .nin=1,
    .nout=1,
    .nargs=2,
    .types={ "var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64",
             "var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64"} },

  { .loc = loc(),
    .success=true,

    .signature="var... * int64, var... * int64 -> var... * int64",
    .args={ "var(offsets=[0,2]) * int64",
            "var(offsets=[0,2]) * int64" },
    .kwargs={NULL},

    .outer_dims=1,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={ "var(offsets=[0,2]) * int64",
             "var(offsets=[0,2]) * int64",
             "var(offsets=[0,2]) * int64"} },

  { .loc = loc(),
    .success=false,

    .signature="var... * int64, var... * int64 -> var... * int64",
    .args={ "var(offsets=[0,3]) * int64",
            "var(offsets=[0,2]) * int64" },
    .kwargs={NULL} },

  { .loc = loc(),
    .success=true,

    .signature="var... * int64, var... * int64 -> var... * int64",
    .args={ "var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64",
            "var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64" },
    .kwargs={NULL},

    .outer_dims=2,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={ "var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64",
             "var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64",
             "var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64" } },

  { .loc = loc(),
    .success=false,

    .signature="var... * int64, var... * int64 -> var... * int64",
    .args={ "var(offsets=[0,2]) * var(offsets=[0,4,11]) * int64",
            "var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64" },
    .kwargs={NULL} },

  { .loc = loc(),
    .success=false,

    .signature="var... * int64, var... * int64 -> var... * int64",
    .args={ "var(offsets=[0,3]) * var(offsets=[0,4,10,20]) * int64",
            "var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64" },
    .kwargs={NULL} },

  { .loc = loc(),
    .success=true,

    .signature="var * var * (int32, float64), int32 -> var * var * int32",
    .args={ "var(offsets=[0,3]) * var(offsets=[0,4,10,20]) * (int32, float64)",
            "int32" },
    .kwargs={NULL},

    .outer_dims=0,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={ "var(offsets=[0,3]) * var(offsets=[0,4,10,20]) * (int32, float64)",
             "int32",
             "var * var * int32"} },

  { .loc = loc(),
    .success=true,

    .signature="... * C[2 * 3 * float64], ... * F[3 * 4 * float32] -> ... * F[10 * 2 * float64]",
    .args={"2 * 3 * float64", "!3 * 4 * float32"},
    .kwargs={NULL},

    .outer_dims=0,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={ "2 * 3 * float64", "!3 * 4 * float32", "!10 * 2 * float64" } },

  { .loc = loc(),
    .success=true,

    .signature="... * C[2 * 3 * float64], ... * F[3 * 4 * float32] -> ... * F[10 * 2 * float64]",
    .args={"2 * 3 * float64", "!3 * 4 * float32"},
    .kwargs={"!10 * 2 * float64"},

    .outer_dims=0,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={ "2 * 3 * float64", "!3 * 4 * float32", "!10 * 2 * float64" } },

  { .loc = loc(),
    .success=false,

    .signature="... * C[2 * 3 * float64], ... * F[3 * 4 * float32] -> ... * F[10 * 2 * float64]",
    .args={"2 * 3 * float64", "!3 * 4 * float32"},
    .kwargs={"10 * 2 * float64"} },

  { .loc = loc(),
    .success=true,

    .signature="... * C[2 * 3 * float64], ... * F[3 * 4 * float32] -> ... * F[10 * 2 * float64]",
    .args={ "20 * 2 * 3 * float64", "20 * !3 * 4 * float32" },
    .kwargs={NULL},

    .outer_dims=1,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={ "20 * 2 * 3 * float64", "20 * !3 * 4 * float32", "20 * !10 * 2 * float64"} },

  { .loc = loc(),
    .success=true,

    .signature="... * C[2 * 3 * float64], ... * F[3 * 4 * float32] -> ... * F[10 * 2 * float64]",
    .args={ "20 * 2 * 3 * float64", "!3 * 4 * float32" },
    .kwargs={NULL},

    .outer_dims=1,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={ "20 * 2 * 3 * float64",
             "fixed(shape=20, step=0) * !3 * 4 * float32",
             "20 * !10 * 2 * float64" } },

  { .loc = loc(),
    .success=true,

    .signature="... * C[2 * 3 * float64], ... * F[3 * 4 * float32] -> ... * F[10 * 2 * float64]",
    .args={ "2 * 3 * float64", "20 * 30 * !3 * 4 * float32" },
    .kwargs={NULL},

    .outer_dims=2,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={ "fixed(shape=20, step=0) * fixed(shape=30, step=0) * 2 * 3 * float64",
             "20 * 30 * !3 * 4 * float32",
             "20 * 30 * !10 * 2 * float64"} },

  { .loc = loc(),
    .success=true,

    .signature="... * C[2 * 3 * float64], ... * F[3 * 4 * float32] -> ... * C[10 * 2 * float64]",
    .args={"2 * 3 * float64", "20 * 30 * !3 * 4 * float32"},
    .kwargs={NULL},

    .outer_dims=2,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={ "fixed(shape=20, step=0) * fixed(shape=30, step=0) * 2 * 3 * float64",
             "20 * 30 * !3 * 4 * float32", 
             "20 * 30 * 10 * 2 * float64" } },

  { .loc = loc(),
    .success=true,

    .signature="... * C[2 * 3 * float64], ... * F[3 * 4 * float32] -> C[... * 10 * 2 * float64]",
    .args={"2 * 3 * float64", "20 * 30 * !3 * 4 * float32"},
    .kwargs={NULL},

    .outer_dims=2,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={ "fixed(shape=20, step=0) * fixed(shape=30, step=0) * 2 * 3 * float64",
             "20 * 30 * !3 * 4 * float32",
             "20 * 30 * 10 * 2 * float64" } },

  { .loc = loc(),
    .success=true,

    .signature="... * C[2 * 3 * float64], ... * F[3 * 4 * float32] -> F[... * 10 * 2 * float64]",
    .args={"2 * 3 * float64", "20 * 30 * !3 * 4 * float32"},
    .kwargs={NULL},

    .outer_dims=2,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={ "fixed(shape=20, step=0) * fixed(shape=30, step=0) * 2 * 3 * float64",
             "20 * 30 * !3 * 4 * float32",
             "!20 * 30 * 10 * 2 * float64" } },

  { .loc = loc(),
    .success=true,

    .signature="... * C[2 * 3 * float64], ... * F[3 * 4 * float32] -> F[... * 20 * 30 * 10 * 2 * float64]",
    .args={"2 * 3 * float64", "!3 * 4 * float32"},
    .kwargs={NULL},

    .outer_dims=0,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={ "2 * 3 * float64", "!3 * 4 * float32", "!20 * 30 * 10 * 2 * float64" } },

  { .loc = loc(),
    .success=true,

    .signature="... * C[2 * 3 * float64], ... * F[3 * 4 * float32] -> F[... * 20 * 30 * 10 * 2 * float64]",
    .args={"2 * 3 * float64", "!3 * 4 * float32"},
    .kwargs={"!20 * 30 * 10 * 2 * float64"},

    .outer_dims=0,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={ "2 * 3 * float64", "!3 * 4 * float32", "!20 * 30 * 10 * 2 * float64" } },

  { .loc = loc(),
    .success=false,

    .signature="... * C[2 * 3 * float64], ... * F[3 * 4 * float32] -> F[... * 20 * 30 * 10 * 2 * float64]",
    .args={"2 * 3 * float64", "!3 * 4 * float32"},
    .kwargs={"20 * 30 * 10 * 2 * float64"} },

  { .loc = loc(),
    .success=true,

    .signature="C[... * 2 * 3 * int64], ... * F[3 * 4 * float32] -> F[... * 20 * 30 * 10 * 2 * float64]",
    .args={"2 * 3 * int64", "!3 * 4 * float32"},
    .kwargs={NULL},

    .outer_dims=0,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={ "2 * 3 * int64", "!3 * 4 * float32", "!20 * 30 * 10 * 2 * float64" } },

  /* First argument is not C after broadcasting. */
  { .loc = loc(),
    .success=false,
    
    .signature="C[... * 2 * 3 * int64], ... * F[2 * 3 * 4 * float32] -> F[... * 20 * 30 * 10 * 2 * float64]",
    .args={"2 * 3 * float64", "!2 * 3 * 4 * float32"},
    .kwargs={NULL} },

  /* Second argument is not F after broadcasting. */
  { .loc = loc(),
    .success=false,

    .signature="... * C[2 * 3 * 4 * int64], F[... * 3 * 4 * float32] -> F[... * 20 * 30 * 10 * 2 * float64]",
    .args={"3 * 4 * float64", "!3 * 4 * float32"},
    .kwargs={NULL} },

  { .loc = loc(),
    .success=true,

    .signature="array... * float64 -> array... * int64",
    .args={"array * float64"},
    .kwargs={NULL},

    .outer_dims=1,
    .nin=1,
    .nout=1,
    .nargs=2,
    .types={ "array * float64", "array * int64" } },

  { .loc = loc(),
    .success=true,

    .signature="array... * array * float64, array... * uint8 -> array... * array * int64",
    .args={"array * array * float64", "array * uint8"},
    .kwargs={NULL},

    .outer_dims=1,
    .nin=2,
    .nout=1,
    .nargs=3,
    .types={ "array * array * float64", "array * uint8", "array * array * int64" } },

  { .loc=NULL, .success=false, .signature= NULL, .args={NULL}, .kwargs={NULL} }
};

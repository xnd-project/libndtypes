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

  { .signature="void -> float32",
    .in={NULL},
    .out={"float32"},
    .broadcast={NULL},
    .outer_dims=0,
    .success=true },

  { .signature="T -> T",
    .in={"T"},
    .out={NULL},
    .broadcast={NULL},
    .outer_dims=0,
    .success=false },

  { .signature="T -> T",
    .in={"int64"},
    .out={"int64"},
    .broadcast={NULL},
    .outer_dims=0,
    .success=true },

  { .signature="M*N*T, N*P*T -> M*P*T",
    .in={"2 * 3 * int64", "3 * 10 * int64"},
    .out={"2 * 10 * int64"},
    .broadcast={NULL},
    .outer_dims=0,
    .success=true },

  { .signature="Dims... *M*N*T, Dims... *N*P*T -> Dims... *M*P*T",
    .in={"2 * 3 * int64", "3 * 10 * int64"},
    .out={"2 * 10 * int64"},
    .broadcast={NULL},
    .outer_dims=0,
    .success=true },

  { .signature="Dims... *M*N*T, Dims... *N*P*T -> Dims... *M*P*T",
    .in={"100 * 2 * 3 * int64", "100 * 3 * 10 * int64"},
    .out={"100 * 2 * 10 * int64"},
    .broadcast={NULL},
    .outer_dims=1,
    .success=true },

  { .signature="Dims... *M*N*T, Dims... *N*P*T -> Dims... *M*P*T",
    .in={"2 * 3 * int64", "3 * 10 * int32"},
    .out={NULL},
    .broadcast={NULL},
    .outer_dims=0,
    .success=false },

  { .signature="Dims... *M*N*T, Dims... *N*P*T -> Dims... *M*P*T",
    .in={"3 * int64", "3 * 10 * int64"},
    .out={NULL},
    .broadcast={NULL},
    .outer_dims=0,
    .success=false },

  { .signature="Dims... * M*N*T, Dims... * N*P*T -> Dims... *M*T",
    .in={"10 * 2 * 3 * int64", "10 * 3 * 10 * int64"},
    .out={"10 * 2 * int64"},
    .broadcast={NULL},
    .outer_dims=1,
    .success=true },

  { .signature="Dims... * M*T, Dims... * N*T -> Dims... * M*T",
    .in={"2 * 3 * int64", "3 * 3 * int64"},
    .out={NULL},
    .broadcast={NULL},
    .outer_dims=0,
    .success=false },

  { .signature="Dims... * M*T, Dims... * N*T -> Dims... * M*T",
    .in={"2 * 3 * int64", "3 * 3 * int64"},
    .out={NULL},
    .broadcast={NULL},
    .outer_dims=0,
    .success=false },

  { .signature="M*N*T, N*P*T -> M*P*T",
    .in={"2 * 3 * int64", "2 * 10 * int64"},
    .out={NULL},
    .broadcast={NULL},
    .outer_dims=0,
    .success=false },

  { .signature="M*N*T, N*P*T -> M*P*T",
    .in={"2 * 3 * int64", "3 * 10 * int32"},
    .out={NULL},
    .broadcast={NULL},
    .outer_dims=0,
    .success=false },

  { .signature="Dims... * M*N*T, Dims... * N*P*T -> Dims... *M*P*T",
    .in={"3 * int64", "3 * 10 * int64"},
    .out={NULL},
    .broadcast={NULL},
    .outer_dims=0,
    .success=false },

  { .signature="Dims... * M*N*T, Dims... *N*P*T -> Dims... *M*P*T",
    .in={"100 * 2 * 3 * int64", "100 * 3 * 10 * int64"},
    .out={"100 * 2 * 10 * int64"},
    .broadcast={NULL},
    .outer_dims=1,
    .success=true },

  { .signature="Dims... * M*N*T, Dims... *N*P*T -> Dims... *M*P*T",
    .in={"400 * 2 * 3 * int64", "400 * 3 * 10 * int64"},
    .out={"400 * 2 * 10 * int64"},
    .broadcast={NULL},
    .outer_dims=1,
    .success=true },

  { .signature="Dims... * M*N*T, Dims... * N*P*T -> Dims... * M*P*T",
    .in={"1000 * 400 * 2 * 3 * int64", "1000 * 400 * 3 * 10 * int64"},
    .out={"1000 * 400 * 2 * 10 * int64"},
    .broadcast={NULL},
    .outer_dims=2,
    .success=true },

  { .signature="Dims... * M*N*T, Dims... *N*P*T -> Dims... * M*P*T",
    .in={"2 * 3 * 1000 * 400 * 2 * 3 * int64", "2 * 3 * 1000 * 400 * 3 * 10 * int64"},
    .out={"2 * 3 * 1000 * 400 * 2 * 10 * int64"},
    .broadcast={NULL},
    .outer_dims=4,
    .success=true },

  { .signature="D... * int64 -> D... * int64",
    .in={"2 * 3 * int64"},
    .out={"2 * 3 * int64"},
    .broadcast={NULL},
    .outer_dims=2,
    .success=true },

  { .signature="... * int64 -> ... * int64",
    .in={"2 * 3 * int64"},
    .out={"2 * 3 * int64"},
    .broadcast={"2 * 3 * int64"},
    .outer_dims=2,
    .success=true },

  { .signature="... * int64, ... * int64 -> ... * int64",
    .in={"2 * 3 * int64", "10 * 2 * 3 * int64"},
    .out={"10 * 2 * 3 * int64"},
    .broadcast={"fixed(shape=10, step=0) * 2 * 3 * int64", "10 * 2 * 3 * int64"},
    .outer_dims=3,
    .success=true },

  { .signature="... * N * int64,      ... * int64 -> ... * int64",
    .in={"2 * 3 * int64", "10 * 2 * int64"},
    .out={"10 * 2 * int64"},
    .broadcast={"fixed(shape=10, step=0) * 2 * 3 * int64", "10 * 2 * int64"},
    .outer_dims=2,
    .success=true },

  { .signature="... * N * int64, ... * int64 -> ... * int64",
    .in={"2 * 3 * int64", "20 * 10 * 2 * int64"},
    .out={"20 * 10 * 2 * int64"},
    .broadcast={"fixed(shape=20, step=0) * fixed(shape=10, step=0) * 2 * 3 * int64", "20 * 10 * 2 * int64"},
    .outer_dims=3,
    .success=true },

  { .signature="... * N * int64, ... * int64 -> ... * N * int64",
    .in={"2 * 3 * int64", "20 * 10 * 2 * int64"},
    .out={"20 * 10 * 2 * 3 * int64"},
    .broadcast={"fixed(shape=20, step=0) * fixed(shape=10, step=0) * 2 * 3 * int64", "20 * 10 * 2 * int64"},
    .outer_dims=3,
    .success=true },

  { .signature="var... * int64 -> var... * int64",
    .in={"2 * 3 * int64"},
    .out={NULL},
    .broadcast={NULL},
    .outer_dims=0,
    .success=false },

  { .signature="A... * int64 -> A... * int64",
    .in={"var(offsets=[0,2]) * int64"},
    .out={NULL},
    .broadcast={NULL},
    .outer_dims=0,
    .success=false },

  { .signature="var... * int64 -> var... * int64",
    .in={"var(offsets=[0,2]) * int64"},
    .out={"var(offsets=[0,2]) * int64"},
    .broadcast={NULL},
    .outer_dims=1,
    .success=true },

  { .signature="var... * int64 -> var... * int64",
    .in={"var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64"},
    .out={"var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64"},
    .broadcast={NULL},
    .outer_dims=2,
    .success=true },

  { .signature="var... * int64 -> var... * int64",
    .in={"var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64"},
    .out={"var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64"},
    .broadcast={NULL},
    .outer_dims=2,
    .success=true },

  { .signature="var... * int64, var... * int64 -> var... * int64",
    .in={"var(offsets=[0,2]) * int64",
         "var(offsets=[0,2]) * int64"},
    .out={"var(offsets=[0,2]) * int64"},
    .broadcast={NULL},
    .outer_dims=1,
    .success=true },

  { .signature="var... * int64, var... * int64 -> var... * int64",
    .in={"var(offsets=[0,3]) * int64",
         "var(offsets=[0,2]) * int64"},
    .out={NULL},
    .broadcast={NULL},
    .outer_dims=0,
    .success=false },

  { .signature="var... * int64, var... * int64 -> var... * int64",
    .in={"var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64",
         "var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64"},
    .out={"var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64"},
    .broadcast={NULL},
    .outer_dims=2,
    .success=true },

  { .signature="var... * int64, var... * int64 -> var... * int64",
    .in={"var(offsets=[0,2]) * var(offsets=[0,4,11]) * int64",
         "var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64"},
    .out={NULL},
    .broadcast={NULL},
    .outer_dims=0,
    .success=false },

  { .signature="var... * int64, var... * int64 -> var... * int64",
    .in={"var(offsets=[0,3]) * var(offsets=[0,4,10,20]) * int64",
         "var(offsets=[0,2]) * var(offsets=[0,4,10]) * int64"},
    .out={NULL},
    .broadcast={NULL},
    .outer_dims=0,
    .success=false },

  { .signature="var * var * (int32, float64), int32 -> var * var * int32",
    .in={"var(offsets=[0,3]) * var(offsets=[0,4,10,20]) * (int32, float64)",
         "int32"},
    .out={"var * var * int32"},
    .broadcast={NULL},
    .outer_dims=0,
    .success=true },

  { .signature="... * C[2 * 3 * float64], ... * F[3 * 4 * float32] -> ... * F[10 * 2 * float64]",
    .in={"2 * 3 * float64", "!3 * 4 * float32"},
    .out={"!10 * 2 * float64"},
    .broadcast={"2 * 3 * float64", "!3 * 4 * float32"},
    .outer_dims=0,
    .success=true },

  { .signature="... * C[2 * 3 * float64], ... * F[3 * 4 * float32] -> ... * F[10 * 2 * float64]",
    .in={"20 * 2 * 3 * float64", "20 * !3 * 4 * float32"},
    .out={"20 * !10 * 2 * float64"},
    .broadcast={"20 * 2 * 3 * float64", "20 * !3 * 4 * float32"},
    .outer_dims=1,
    .success=true },

  { .signature="... * C[2 * 3 * float64], ... * F[3 * 4 * float32] -> ... * F[10 * 2 * float64]",
    .in={"20 * 2 * 3 * float64", "!3 * 4 * float32"},
    .out={"20 * !10 * 2 * float64"},
    .broadcast={"20 * 2 * 3 * float64", "fixed(shape=20, step=0) * !3 * 4 * float32"},
    .outer_dims=1,
    .success=true },

  { .signature="... * C[2 * 3 * float64], ... * F[3 * 4 * float32] -> ... * F[10 * 2 * float64]",
    .in={"2 * 3 * float64", "20 * 30 * !3 * 4 * float32"},
    .out={"20 * 30 * !10 * 2 * float64"},
    .broadcast={"fixed(shape=20, step=0) * fixed(shape=30, step=0) * 2 * 3 * float64",
                "20 * 30 * !3 * 4 * float32"},
    .outer_dims=2,
    .success=true },

  { .signature="... * C[2 * 3 * float64], ... * F[3 * 4 * float32] -> ... * C[10 * 2 * float64]",
    .in={"2 * 3 * float64", "20 * 30 * !3 * 4 * float32"},
    .out={"20 * 30 * 10 * 2 * float64"},
    .broadcast={"fixed(shape=20, step=0) * fixed(shape=30, step=0) * 2 * 3 * float64",
                "20 * 30 * !3 * 4 * float32"},
    .outer_dims=2,
    .success=true },

  { .signature="... * C[2 * 3 * float64], ... * F[3 * 4 * float32] -> C[... * 10 * 2 * float64]",
    .in={"2 * 3 * float64", "20 * 30 * !3 * 4 * float32"},
    .out={"20 * 30 * 10 * 2 * float64"},
    .broadcast={"fixed(shape=20, step=0) * fixed(shape=30, step=0) * 2 * 3 * float64",
                "20 * 30 * !3 * 4 * float32"},
    .outer_dims=2,
    .success=true },

  { .signature="... * C[2 * 3 * float64], ... * F[3 * 4 * float32] -> F[... * 10 * 2 * float64]",
    .in={"2 * 3 * float64", "20 * 30 * !3 * 4 * float32"},
    .out={"!20 * 30 * 10 * 2 * float64"},
    .broadcast={"fixed(shape=20, step=0) * fixed(shape=30, step=0) * 2 * 3 * float64",
                "20 * 30 * !3 * 4 * float32"},
    .outer_dims=2,
    .success=true },

  { .signature="... * C[2 * 3 * float64], ... * F[3 * 4 * float32] -> F[... * 20 * 30 * 10 * 2 * float64]",
    .in={"2 * 3 * float64", "!3 * 4 * float32"},
    .out={"!20 * 30 * 10 * 2 * float64"},
    .broadcast={"2 * 3 * float64", "!3 * 4 * float32"},
    .outer_dims=0,
    .success=true },

  { .signature="C[... * 2 * 3 * int64], ... * F[3 * 4 * float32] -> F[... * 20 * 30 * 10 * 2 * float64]",
    .in={"2 * 3 * int64", "!3 * 4 * float32"},
    .out={"!20 * 30 * 10 * 2 * float64"},
    .broadcast={"2 * 3 * int64", "!3 * 4 * float32"},
    .outer_dims=0,
    .success=true },

  /* First argument is not C after broadcasting. */
  { .signature="C[... * 2 * 3 * int64], ... * F[2 * 3 * 4 * float32] -> F[... * 20 * 30 * 10 * 2 * float64]",
    .in={"2 * 3 * float64", "!2 * 3 * 4 * float32"},
    .out={NULL},
    .broadcast={NULL},
    .outer_dims=0,
    .success=false },

  /* Second argument is not F after broadcasting. */
  { .signature="... * C[2 * 3 * 4 * int64], F[... * 3 * 4 * float32] -> F[... * 20 * 30 * 10 * 2 * float64]",
    .in={"3 * 4 * float64", "!3 * 4 * float32"},
    .out={NULL},
    .broadcast={NULL},
    .outer_dims=0,
    .success=false },

  { NULL, {NULL}, {NULL}, {NULL}, 0, false }
};

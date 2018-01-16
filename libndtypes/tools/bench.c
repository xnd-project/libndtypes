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
#include "ndtypes.h"


const char *bar = "{parent : { id: int64, count: uint16, prefix: char, length: int }, time : int, ratio : float32, size : uint16 }";

const char *s = "{battingpost: var * {yearID: ?int32, round: ?string, playerID: ?string, teamID: ?string, lgID: (?string, int64, 5 * 10 * {a: complex128, b: ?int32}), G: ?int32, AB: ?int32, R: ?int32, H: (int32, ... * int32) -> int32, B: (int32, ...) -> int32, HR: {a: 10 * float64, b: var * int32, ...}, RBI: ?int32, SB: ?int32, CS: ?int32, BB: ?int32, SO: ?int32, IBB: ?int32, HBP: ?int32, SH: ?int32, SF: ?int32, GIDP: ?int32, AString: fixed_string(100,'utf32'), BString: fixed_string(100), CBytes: bytes(align=16), DBytes: fixed_bytes(size=1600, align=16)}, awardsmanagers: var * {managerID: ?string, awardID: ?string, yearID: ?int32, lgID: ?string, tie: ?string, notes: ?string}, hofold: var * {hofID: ?string, yearid: ?int32, votedBy: ?string, ballots: ?int32, votes: ?int32, inducted: ?string, category: ?string}, salaries: var * {yearID: ?int32, teamID: ?string, lgID: ?string, playerID: ?string, salary: ?float64}, pitchingpost: var * {playerID: ?string, yearID: ?int32, round: ?string, teamID: ?string, lgID: ?string, W: ?int32, L: ?int32, G: ?int32, GS: ?int32, CG: ?int32, SHO: ?int32, SV: ?int32, IPouts: ?int32, H: ?int32, ER: ?int32, HR: ?int32, BB: ?int32, SO: ?int32, BAOpp: ?float64, ERA: ?float64, IBB: ?int32, WP: ?int32, HBP: ?int32, BK: ?int32, BFP: ?int32, GF: ?int32, R: ?int32, SH: ?int32, SF: ?int32, GIDP: ?int32}, managers: var * {managerID: ?string, yearID: ?int32, teamID: ?string, lgID: ?string, inseason: ?int32, G: ?int32, W: ?int32, L: ?int32, rank: ?int32, plyrMgr: ?string}, teams: var * {yearID: ?int32, lgID: ?string, teamID: ?string, franchID: ?string, divID: ?string, Rank: ?int32, G: ?int32, Ghome: ?int32, W: ?int32, L: ?int32, DivWin: ?string, WCWin: ?string, LgWin: ?string, WSWin: ?string, R: ?int32, AB: ?int32, H: ?int32, B: ?int32, B: ?int32, HR: ?int32, BB: ?int32, SO: ?int32, SB: ?int32, CS: ?int32, HBP: ?int32, SF: ?int32, RA: ?int32, ER: ?int32, ERA: ?float64, CG: ?int32, SHO: ?int32, SV: ?int32, IPouts: ?int32, HA: ?int32, HRA: ?int32, BBA: ?int32, SOA: ?int32, E: ?int32, DP: ?int32, FP: ?float64, name: ?string, park: ?string, attendance: ?int32, BPF: ?int32, PPF: ?int32, teamIDBR: ?string, teamIDlahman45: ?string, teamIDretro: ?string}}";


int
main(void)
{
    ndt_context_t *ctx;
    ndt_t *t;
    int i;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }

    if (ndt_init(ctx) < 0) {
        ndt_err_fprint(stderr, ctx);
        ndt_context_del(ctx);
        return 1;
    }

    for (i = 0; i < 100000; i++) {
        t = ndt_from_string(s, ctx);
        if (t == NULL) {
            ndt_err_fprint(stderr, ctx);
            break;
        }
        ndt_del(t);
    }

    ndt_context_del(ctx);
    ndt_finalize();

    return 0;
}
        




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


const indent_testcase_t indent_tests[] = {
  {
"\
{battingpost: 100 * {yearID: ?int32, round: ?string, playerID: ?string, teamID: ?string, lgID: (?string, int64, 5 * 10 * {a: complex128, b: ?int32}), G: ?int32, AB: ?int32, R: ?int32, H: (int32, 10 * int32), HR: {a: 10 * float64, b: 10 * int32}, RBI: ?int32, SB: ?int32, CS: ?int32, BB: ?int32, SO: ?int32, IBB: ?int32, HBP: ?int32, SH: ?int32, SF: ?int32, GIDP: ?int32, AString: fixed_string(100,'utf32'), BString: fixed_string(100), CBytes: bytes(align=16), DBytes: fixed_bytes(size=1600, align=16)}, awardsmanagers: 10 * {managerID: ?string, awardID: ?string, yearID: ?int32, lgID: ?string, tie: ?string, notes: ?string}, hofold: 10 * {hofID: ?string, yearid: ?int32, votedBy: ?string, ballots: ?int32, votes: ?int32, inducted: ?string, category: ?string}, salaries: 10 * {yearID: ?int32, teamID: ?string, lgID: ?string, playerID: ?string, salary: ?float64}, pitchingpost: 10 * {playerID: ?string, yearID: ?int32, round: ?string, teamID: ?string, lgID: ?string, W: ?int32, L: ?int32, G: ?int32, GS: ?int32, CG: ?int32, SHO: ?int32, SV: ?int32, IPouts: ?int32, H: ?int32, ER: ?int32, HR: ?int32, BB: ?int32, SO: ?int32, BAOpp: ?float64, ERA: ?float64, IBB: ?int32, WP: ?int32, HBP: ?int32, BK: ?int32, BFP: ?int32, GF: ?int32, R: ?int32, SH: ?int32, SF: ?int32, GIDP: ?int32}, managers: 10 * {managerID: ?string, yearID: ?int32, teamID: ?string, lgID: ?string, inseason: ?int32, G: ?int32, W: ?int32, L: ?int32, rank: ?int32, plyrMgr: ?string}, teams: 10 * {yearID: ?int32, lgID: ?string, teamID: ?string, franchID: ?string, divID: ?string, Rank: ?int32, G: ?int32, Ghome: ?int32, W: ?int32, L: ?int32, DivWin: ?string, WCWin: ?string, LgWin: ?string, WSWin: ?string, R: ?int32, AB: ?int32, H: ?int32, B: ?int32, B: ?int32, HR: ?int32, BB: ?int32, SO: ?int32, SB: ?int32, CS: ?int32, HBP: ?int32, SF: ?int32, RA: ?int32, ER: ?int32, ERA: ?float64, CG: ?int32, SHO: ?int32, SV: ?int32, IPouts: ?int32, HA: ?int32, HRA: ?int32, BBA: ?int32, SOA: ?int32, E: ?int32, DP: ?int32, FP: ?float64, name: ?string, park: ?string, attendance: ?int32, BPF: ?int32, PPF: ?int32, teamIDBR: ?string, teamIDlahman45: ?string, teamIDretro: ?string}}\
",

"\
{\n\
  battingpost : 100 * {\n\
    yearID : ?int32,\n\
    round : ?string,\n\
    playerID : ?string,\n\
    teamID : ?string,\n\
    lgID : (?string, int64, 5 * 10 * {\n\
      a : complex128,\n\
      b : ?int32\n\
    }),\n\
    G : ?int32,\n\
    AB : ?int32,\n\
    R : ?int32,\n\
    H : (int32, 10 * int32),\n\
    HR : {\n\
      a : 10 * float64,\n\
      b : 10 * int32\n\
    },\n\
    RBI : ?int32,\n\
    SB : ?int32,\n\
    CS : ?int32,\n\
    BB : ?int32,\n\
    SO : ?int32,\n\
    IBB : ?int32,\n\
    HBP : ?int32,\n\
    SH : ?int32,\n\
    SF : ?int32,\n\
    GIDP : ?int32,\n\
    AString : fixed_string(100, 'utf32'),\n\
    BString : fixed_string(100),\n\
    CBytes : bytes(align=16),\n\
    DBytes : fixed_bytes(size=1600, align=16)\n\
  },\n\
  awardsmanagers : 10 * {\n\
    managerID : ?string,\n\
    awardID : ?string,\n\
    yearID : ?int32,\n\
    lgID : ?string,\n\
    tie : ?string,\n\
    notes : ?string\n\
  },\n\
  hofold : 10 * {\n\
    hofID : ?string,\n\
    yearid : ?int32,\n\
    votedBy : ?string,\n\
    ballots : ?int32,\n\
    votes : ?int32,\n\
    inducted : ?string,\n\
    category : ?string\n\
  },\n\
  salaries : 10 * {\n\
    yearID : ?int32,\n\
    teamID : ?string,\n\
    lgID : ?string,\n\
    playerID : ?string,\n\
    salary : ?float64\n\
  },\n\
  pitchingpost : 10 * {\n\
    playerID : ?string,\n\
    yearID : ?int32,\n\
    round : ?string,\n\
    teamID : ?string,\n\
    lgID : ?string,\n\
    W : ?int32,\n\
    L : ?int32,\n\
    G : ?int32,\n\
    GS : ?int32,\n\
    CG : ?int32,\n\
    SHO : ?int32,\n\
    SV : ?int32,\n\
    IPouts : ?int32,\n\
    H : ?int32,\n\
    ER : ?int32,\n\
    HR : ?int32,\n\
    BB : ?int32,\n\
    SO : ?int32,\n\
    BAOpp : ?float64,\n\
    ERA : ?float64,\n\
    IBB : ?int32,\n\
    WP : ?int32,\n\
    HBP : ?int32,\n\
    BK : ?int32,\n\
    BFP : ?int32,\n\
    GF : ?int32,\n\
    R : ?int32,\n\
    SH : ?int32,\n\
    SF : ?int32,\n\
    GIDP : ?int32\n\
  },\n\
  managers : 10 * {\n\
    managerID : ?string,\n\
    yearID : ?int32,\n\
    teamID : ?string,\n\
    lgID : ?string,\n\
    inseason : ?int32,\n\
    G : ?int32,\n\
    W : ?int32,\n\
    L : ?int32,\n\
    rank : ?int32,\n\
    plyrMgr : ?string\n\
  },\n\
  teams : 10 * {\n\
    yearID : ?int32,\n\
    lgID : ?string,\n\
    teamID : ?string,\n\
    franchID : ?string,\n\
    divID : ?string,\n\
    Rank : ?int32,\n\
    G : ?int32,\n\
    Ghome : ?int32,\n\
    W : ?int32,\n\
    L : ?int32,\n\
    DivWin : ?string,\n\
    WCWin : ?string,\n\
    LgWin : ?string,\n\
    WSWin : ?string,\n\
    R : ?int32,\n\
    AB : ?int32,\n\
    H : ?int32,\n\
    B : ?int32,\n\
    B : ?int32,\n\
    HR : ?int32,\n\
    BB : ?int32,\n\
    SO : ?int32,\n\
    SB : ?int32,\n\
    CS : ?int32,\n\
    HBP : ?int32,\n\
    SF : ?int32,\n\
    RA : ?int32,\n\
    ER : ?int32,\n\
    ERA : ?float64,\n\
    CG : ?int32,\n\
    SHO : ?int32,\n\
    SV : ?int32,\n\
    IPouts : ?int32,\n\
    HA : ?int32,\n\
    HRA : ?int32,\n\
    BBA : ?int32,\n\
    SOA : ?int32,\n\
    E : ?int32,\n\
    DP : ?int32,\n\
    FP : ?float64,\n\
    name : ?string,\n\
    park : ?string,\n\
    attendance : ?int32,\n\
    BPF : ?int32,\n\
    PPF : ?int32,\n\
    teamIDBR : ?string,\n\
    teamIDlahman45 : ?string,\n\
    teamIDretro : ?string\n\
  }\n\
}\
" },

  { NULL, NULL }
};


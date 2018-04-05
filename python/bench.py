from ndtypes import *
import time

# =============================================================================
#                       Type with huge number of offsets
# =============================================================================

s = "var(offsets=[0,10000000]) * var(offsets=%s) * int64" % list(range(10000001))
print("Parse 10_000_000 var offsets:")
start = time.time()
t = ndt(s)
end = time.time()
print(end-start)


b = t.serialize()
print("\nDeserialize 10_000_000 var offsets:")
start = time.time()
u = ndt.deserialize(b)
end = time.time()
print(end-start)

assert t == u


# =============================================================================
#                                  Large type
# =============================================================================

s = """
{battingpost: 100 * {yearID: ?int32, round: ?string, playerID: ?string, teamID: ?string, lgID: (?string, int64, 5 * 10 * {a: complex128, b: ?int32}), G: ?int32, AB: ?int32, R: ?int32, H: (int32, 10 * int32), HR: {a: 10 * float64, b: 10 * int32}, RBI: ?int32, SB: ?int32, CS: ?int32, BB: ?int32, SO: ?int32, IBB: ?int32, HBP: ?int32, SH: ?int32, SF: ?int32, GIDP: ?int32, AString: fixed_string(100,'utf32'), BString: fixed_string(100), CBytes: bytes(align=16), DBytes: fixed_bytes(size=1600, align=16)}, awardsmanagers: 10 * {managerID: ?string, awardID: ?string, yearID: ?int32, lgID: ?string, tie: ?string, notes: ?string}, hofold: 10 * {hofID: ?string, yearid: ?int32, votedBy: ?string, ballots: ?int32, votes: ?int32, inducted: ?string, category: ?string}, salaries: 10 * {yearID: ?int32, teamID: ?string, lgID: ?string, playerID: ?string, salary: ?float64}, pitchingpost: 10 * {playerID: ?string, yearID: ?int32, round: ?string, teamID: ?string, lgID: ?string, W: ?int32, L: ?int32, G: ?int32, GS: ?int32, CG: ?int32, SHO: ?int32, SV: ?int32, IPouts: ?int32, H: ?int32, ER: ?int32, HR: ?int32, BB: ?int32, SO: ?int32, BAOpp: ?float64, ERA: ?float64, IBB: ?int32, WP: ?int32, HBP: ?int32, BK: ?int32, BFP: ?int32, GF: ?int32, R: ?int32, SH: ?int32, SF: ?int32, GIDP: ?int32}, managers: 10 * {managerID: ?string, yearID: ?int32, teamID: ?string, lgID: ?string, inseason: ?int32, G: ?int32, W: ?int32, L: ?int32, rank: ?int32, plyrMgr: ?string}, teams: 10 * {yearID: ?int32, lgID: ?string, teamID: ?string, franchID: ?string, divID: ?string, Rank: ?int32, G: ?int32, Ghome: ?int32, W: ?int32, L: ?int32, DivWin: ?string, WCWin: ?string, LgWin: ?string, WSWin: ?string, R: ?int32, AB: ?int32, H: ?int32, B: ?int32, B: ?int32, HR: ?int32, BB: ?int32, SO: ?int32, SB: ?int32, CS: ?int32, HBP: ?int32, SF: ?int32, RA: ?int32, ER: ?int32, ERA: ?float64, CG: ?int32, SHO: ?int32, SV: ?int32, IPouts: ?int32, HA: ?int32, HRA: ?int32, BBA: ?int32, SOA: ?int32, E: ?int32, DP: ?int32, FP: ?float64, name: ?string, park: ?string, attendance: ?int32, BPF: ?int32, PPF: ?int32, teamIDBR: ?string, teamIDlahman45: ?string, teamIDretro: ?string}}
"""

print("\nParse large type (100_000 repetitions):")
start = time.time()
for i in range(100000):
    t = ndt(s)
end = time.time()
print(end-start)

b = t.serialize()
print("\nDeserialize large type (100_000 repetitions):")
start = time.time()
for i in range(100000):
    u = ndt.deserialize(b)
end = time.time()
print(end-start)

assert t == u



##########
New syntax
##########

========================================================
  Trailing metadata after dimensions and record fields
========================================================

   10 [stride=64] * 2 [stride=1280] * {a: int32 [pad=4], b: int64}


==================================================================
   Reserve brackets for metadata and use the constructor syntax
==================================================================

Old
___

  fixed_string[size=300]
  bytes[size=256, align=16]


New
---

  FixedString(size=300)
  Bytes(size=256, align=16)


========================================================
    Uppercase or lowercase type constructors better?
========================================================

  1) All type constructors uppercase (including Int64 etc.)

  2) Primitive nullary constructors lowercase (int64 etc.)

  3) Primitive and builtin constructors lowercase (int64,
     fixed_string(size=300), etc.)





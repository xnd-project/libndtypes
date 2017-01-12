

###############################################
Three syntax variants for low-level information
###############################################

Problem
-------

Specify low-level data on the grammar level. This is *optional*, the regular
syntax will remain.


Example
-------

Array of structs with funny alignment and padding.


==============================================================================
1) Trailing metadata after the regular datashape string (previously suggested)
==============================================================================

The metadata is specified after the "|" symbol.  The structure of the metadata
mirrors the regular string, metadata is specified in brackets using attribute
notation:

   10 * 2 * {a: int32, b: int64} | [stride=64] * [] * {[align=16, pad=12], [align=16]}


Advantages
----------

  - Relatively concise for short strings.

  - Does not look like having to write the AST manually.

  - Visual separation between the abstract type and the concrete layout.


Disadvantages
-------------

  - Can get *very* unreadable for larger strings -- which metadata belongs
    to which element?  Easy to make mistakes when specifying the metadata
    manually. (See: https://www.continuum.io/blog/developer/blaze-datasets)

  - Users have to remember a new syntax.

  - Harder to add new types like FortranArray.


=====================
2) Constructor syntax
=====================

This would use the familiar object constructor syntax with any number of
attributes per object:

  FixedDim(shape=10, stride=64) * FixedDim(shape=2) * Record(Field('a' : int32, align=16, pad=12),
                                                             Field('b' : int64, align=16))

Advantages
----------

  - Easy to integrate new types on the grammar level (we have discussed
    FortranArray last week):

      FortranArray(
        shape=[10, 20],
        dtype=Record(Field('a' : int32, align=16, pad=12),
                     Field('b' : int64, align=16)))

  - Syntax could be directly used on the Python level as well. Users would
    have to learn less.


Disadvantages
-------------

  - Looks like writing the AST directly and is less shiny.

  - Is switching between the syntax sugar version and this one annoying?


=====================
3) Fashionable syntax
=====================

Whitespace syntax and similar (yaml, Google protocol buffers) seem to be
trendy:

  Array
    FixedDim
      shape=10
      stride=64
    FixedDim
      shape=2
    Dtype
      Record
        Field
          name=a
          type=int32
          align=16
        Field
          name=b
          type=int64
          align=16
    order=Fortran


Advantages
----------

  - Probably looks "modern".

  - Is readable and extensible.


Disadvantages
-------------

  - Hard (and probably annoying) to use in datashape strings. This kind
    of syntax might be better for a config file.

  - Hard to parse (would require an extra indent/dedent parse step to
    stay sane).

  - May be harder to write than to read (remember exact syntax, parser error
    messages will probably be worse).




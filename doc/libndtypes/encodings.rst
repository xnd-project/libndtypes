.. meta::
   :robots: index,follow
   :description: libndtypes documentation

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>



Encodings
=========

Some types support encoding parameters.

.. c:type:: enum ndt_encoding

  Encoding for characters and strings

  .. c:var:: enum ndt_encoding Ascii
  .. c:var:: enum ndt_encoding Utf8
  .. c:var:: enum ndt_encoding Utf16
  .. c:var:: enum ndt_encoding Utf32
  .. c:var:: enum ndt_encoding Ucs2


Functions
---------

.. c:function:: enum ndt_encoding ndt_encoding_from_string(const char *s, ndt_context_t *ctx)

  Convert a string to the corresponding enum value.  The caller must use
  :c:func:`ndt_err_occurred` to check for errors.


.. c:function:: const char *ndt_encoding_as_string(enum ndt_encoding encoding)

  Convert an encoding to its string representation.


.. c:function:: size_t ndt_sizeof_encoding(enum ndt_encoding encoding)

  Return the memory size of a single code point.


.. c:function:: uint16_t ndt_alignof_encoding(enum ndt_encoding encoding)

  Return the alignment of a single code point.




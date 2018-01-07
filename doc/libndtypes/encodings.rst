.. meta::
   :robots: index,follow
   :description: libndtypes documentation

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>



Encodings
=========

Some types support encoding parameters.

.. code-block:: c

   #include <ndtypes.h>

   /* Encoding for characters and strings */
   enum ndt_encoding {
     Ascii,
     Utf8,
     Utf16,
     Utf32,
     Ucs2,
   };


Functions
---------

.. topic:: ndt_encoding_from_string

.. code-block:: c

   enum ndt_encoding ndt_encoding_from_string(const char *s, ndt_context_t *ctx);

Convert a string to the corresponding enum value.  The caller must use
:func:`ndt_err_occurred` to check for errors.


.. topic:: ndt_encoding_as_string

.. code-block:: c

   const char *ndt_encoding_as_string(enum ndt_encoding encoding);

Convert an encoding to its string representation.


.. topic:: ndt_sizeof_encoding

.. code-block:: c

   size_t ndt_sizeof_encoding(enum ndt_encoding encoding);

Return the memory size of a single code point.


.. topic:: ndt_alignof_encoding

.. code-block:: c

   uint16_t ndt_alignof_encoding(enum ndt_encoding encoding);

Return the alignment of a single code point.




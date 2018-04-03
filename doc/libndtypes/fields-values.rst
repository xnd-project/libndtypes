.. meta::
   :robots: index,follow
   :description: libndtypes documentation

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


Fields and values
=================

Some API functions expect fields for creating tuple or record types or values
for creating categorical types.


Fields
------

.. c:type:: struct uint16_opt_t

  .. c:member:: enum ndt_option tag
  .. c:member:: uint16_t Some

  Due to the multitude of options in creating fields a number of functions take
  a *uint16_opt_t* struct.  If *tag* is *None*, no value has been specified
  and the *Some* field is undefined.

  If *tag* is *Some*, the value in the *Some* field has been explicitly given.


Functions
---------

.. topic:: ndt_field

.. c:function:: ndt_field_t *ndt_field(char *name, ndt_t *type, uint16_opt_t align, \
                          uint16_opt_t pack, uint16_opt_t pad, ndt_context_t *ctx)


  Create a new field.  For tuples, *name* is :c:macro:`NULL`.  The `align`
  and `pack` options are mutually exclusive and have exactly the same
  function as *gcc's* `aligned` and `packed` attributes when applied to
  individual fields.

  The `pad` field has no influence on the field layout. It is present to
  enable sanity checks when an explicit number of padding bytes has been
  specified (Example: PEP-3118).


.. c:function:: void ndt_field_del(ndt_field_t *field)

  Deallocate a field.


.. c:function:: void ndt_field_array_del(ndt_field_t *fields, int64_t shape)

  Deallocate an array of fields.


Values
------

.. c:type:: enum ndt_value

  Selected values for the categorical type.

  .. c:var:: enum ndt_value ValBool
  .. c:var:: enum ndt_value ValInt64
  .. c:var:: enum ndt_value ValFloat64
  .. c:var:: enum ndt_value ValString
  .. c:var:: enum ndt_value ValNA

.. code-block:: c

   typedef struct {
     enum ndt_value tag;
       union {
         bool ValBool;
         int64_t ValInt64;
         double ValFloat64;
         char *ValString;
     };
   } ndt_value_t;


The categorical type contains values.  Currently a small number of primitive
types are supported.  It would be possible to use memory typed by *ndt_t* itself
either by introducing a circular relationship between libndtypes and container
libraries or by duplicating parts of a container library.

It remains to be seen if such an added complexity is useful.


.. c:function:: ndt_value_t *ndt_value_from_number(enum ndt_value tag, char *v, ndt_context_t *ctx)

  Construct a number or boolean value from a string.  *tag* must be one of
  :c:data:`ValBool`, :c:data:`ValInt64`, or :c:data:`ValFloat64`.


.. c:function:: ndt_value_t *ndt_value_from_string(char *v, ndt_context_t *ctx)

  Construct a :c:data:`ValString` value from a string.

.. c:function::  ndt_value_t *ndt_value_na(ndt_context_t *ctx)

  Construct the :c:data:`NA` value.


.. c:function:: int ndt_value_equal(const ndt_value_t *x, const ndt_value_t *y)

  Determine if two values are equal. :c:data:`NA` compares not equal to
  itself.


.. c:function:: ndt_value_mem_equal(const ndt_value_t *x, const ndt_value_t *y)

  Determine if two values are structurally equal. :c:data:`NA` compares
  equal to itself.


.. c:function:: int ndt_value_compare(const ndt_value_t *x, const ndt_value_t *y)

  Compare values according to a sorting order. :c:data:`NA` compares equal
  to itself.

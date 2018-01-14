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

.. topic:: uint16_opt

.. code-block:: c

   enum ndt_option {
     None,
     Some
   };

   typedef struct {
     enum ndt_option tag;
     uint16_t Some;
   } uint16_opt_t;


Due to the multitude of options in creating fields a number of functions take
a *uint16_opt_t* struct.  If *tag* is *None*, no value has been specified
and the *Some* field is undefined.

If *tag* is *Some*, the value in the *Some* field has been explicitly given.


Functions
---------

.. topic:: ndt_field

.. code-block:: c

   ndt_field_t *ndt_field(char *name, ndt_t *type, uint16_opt_t align,
                          uint16_opt_t pack, uint16_opt_t pad, ndt_context_t *ctx);


Create a new field.  For tuples, *name* is :c:macro:`NULL`.  The `align`
and `pack` options are mutually exclusive and have exactly the same
function as *gcc's* `aligned` and `packed` attributes when applied to
individual fields.

The `pad` field has no influence on the field layout. It is present to
enable sanity checks when an explicit number of padding bytes has been
specified (Example: PEP-3118).


.. topic:: ndt_field_del

.. code-block:: c

   void ndt_field_del(ndt_field_t *field);

Deallocate a field.


.. topic:: ndt_field_array_del

.. code-block:: c

   void ndt_field_array_del(ndt_field_t *fields, int64_t shape);


Deallocate an array of fields.


Values
------

.. topic:: values

.. code-block:: c

   /* Selected values for the categorical type. */
   enum ndt_value {
     ValBool,
     ValInt64,
     ValFloat64,
     ValString,
     ValNA,
   };

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



.. topic:: ndt_value_from_number

.. code-block:: c

   ndt_value_t *ndt_value_from_number(enum ndt_value tag, char *v, ndt_context_t *ctx);

Construct a number or boolean value from a string.  *tag* must be one of
:c:macro:`ValBool`, :c:macro:`ValInt64`, or :c:macro:`ValFloat64`.


.. topic:: ndt_value_from_string

.. code-block:: c

   ndt_value_t *ndt_value_from_string(char *v, ndt_context_t *ctx);

Construct a :c:macro:`ValString` value from a string.


.. topic:: *ndt_value_na

.. code-block:: c

   ndt_value_t *ndt_value_na(ndt_context_t *ctx);

Construct the :c:macro:`NA` value.


.. topic:: ndt_value_equal

.. code-block:: c

   int ndt_value_equal(const ndt_value_t *x, const ndt_value_t *y);

Determine if two values are equal. :c:macro:`NA` compares not equal to
itself.


.. topic:: ndt_value_mem_equal

.. code-block:: c

   ndt_value_mem_equal(const ndt_value_t *x, const ndt_value_t *y);

Determine if two values are structurally equal. :c:macro:`NA` compares
equal to itself.


.. topic:: ndt_value_compare

.. code-block:: c

   int ndt_value_compare(const ndt_value_t *x, const ndt_value_t *y);

Compare values according to a sorting order. :c:macro:`NA` compares equal
to itself.





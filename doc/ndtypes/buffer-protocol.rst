.. meta::
   :robots: index,follow
   :description: ndtypes
   :keywords: ndtypes, buffer protocol

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


Buffer protocol
===============

ndtypes supports conversion from PEP-3118 format strings to datashape:

.. doctest::

   >>> from ndtypes import ndt
   >>> ndt.from_format("T{<b:a:Q:b:}")
   ndt("{a : <int8, b : uint64}")


Note that there are a couple of open issues around the buffer protocol,
e.g. https://bugs.python.org/issue26746 .






import _ndtypes as _ndt
from _ndtypes import *
from enum import Enum

class endian(Enum):
    little = b'L'
    big = b'B'

class encoding(Enum):
    ascii = _ndt._ascii
    utf8 = _ndt._utf8
    utf16 = _ndt._utf16
    utf32 = _ndt._utf32
    ucs2 = _ndt._ucs2

class dimension(Enum):
    none = _ndt._dim_none
    uint8 = _ndt._dim_uint8
    uint16 = _ndt._dim_uint16
    uint32 = _ndt._dim_uint32
    int32 = _ndt._dim_int32
    int64 = _ndt._dim_int64

class kind(Enum):
    any = _ndt._any_kind
    fixed_dim = _ndt._fixed_dim
    symbolic_dim = _ndt._symbolic_dim
    var_dim = _ndt._var_dim
    ellipsis_dim = _ndt._ellipsis_dim
    array = _ndt._array
    option = _ndt._option
    option_item = _ndt._option_item
    nominal = _ndt._nominal
    constr = _ndt._constr

def type(kind):
    return _ndt.type(kind.value)

#class type(_ndt.type):

#    def __init__(self, kind):
#        super(type, self).__init__(kind.value)

#    #def __rmul__(self, dim):
#    #    """Build compound types from Python expressions."""
#    #    return _ndt.fixed_dim(dim, self)

def signed(size, endian): return _ndt.signed(size, endian.value)
def unsigned(size, endian): return _ndt.unsigned(size, endian.value)

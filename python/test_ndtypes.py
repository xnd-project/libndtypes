import ndtypes as ndt
import _ndtypes as _ndt
import pytest
import sys

# TODO: can this be moved into module initialization ?
# (finalization is only supported in Python 3)
ndt.init()
#ndt.finish()

def test_any():
    t1 = ndt.type(ndt.kind.any)
    assert str(t1) == 'Any'
    t2 = ndt.any()
    assert str(t2) == 'Any'
    assert t1 == t2
    
def test_attributes():
    t = ndt.type(ndt.kind.any)
    assert t.is_abstract == True
    assert t.is_concrete == False

def test_equal():
    t1 = ndt.type(ndt.kind.any)
    t2 = ndt.type(ndt.kind.any)
    assert t1 == t2

def test_fixed_dim():
    t1 = ndt.type(ndt.kind.any)
    t2 = ndt.fixed_dim(8, t1)
    print(str(t2))

def test_expr():

    t = ndt.type(ndt.kind.any)
    t1 = t*8
    print(t1)
    t = ndt.signed(2, ndt.endian.big)
    t2 = t*16
    print(t2)

def test_error():

    with pytest.raises(RuntimeError) as e:
        t1 = _ndt.signed(2, b'x')


if sys.version_info >= (3,0):
    # the syntax in these tests requires Python 3
    from p3_test_ndtypes import *

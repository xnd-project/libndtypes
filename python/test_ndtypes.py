import ndtypes

# TODO: can this be moved into module initialization ?
# (finalization is only supported in Python 3)
ndtypes.init()
#ndtypes.finish()

def test_any():
    t1 = ndtypes.type(0)
    assert str(t1) == 'Any'
    t2 = ndtypes.any()
    assert str(t2) == 'Any'
    assert t1 == t2
    
def test_attributes():
    t = ndtypes.type(0)
    assert t.is_abstract == True
    assert t.is_concrete == False

def test_equal():
    t1 = ndtypes.type(0)
    t2 = ndtypes.type(0)
    assert t1 == t2

def test_fixed_dim():
    t1 = ndtypes.type(0)
    t2 = ndtypes.fixed_dim(8, t1)
    print(str(t2))

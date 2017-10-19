
def pretty_repr(self):
    return """\
ApplySpec(
  func=%r,
  args=%r,
  ret=%r,
  outer_dims=%r
)""" % (self.func, self.args, self.ret, self.outer_dims)

from . import _ndtypes
_ndtypes.ApplySpec.__repr__ = pretty_repr
del _ndtypes

from ._ndtypes import *


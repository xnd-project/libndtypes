
def _pretty_repr(self):
    return """\
ApplySpec(
  func=%r,
  args=%r,
  ret=%r,
  outer_dims=%r
)""" % (self.func, self.args, self.ret, self.outer_dims)

from . import _ndtypes
_ndtypes.ApplySpec.__repr__ = _pretty_repr
from ._ndtypes import *



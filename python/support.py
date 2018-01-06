import sys
import unittest

def skip_if(condition, reason):
    if condition:
        raise unittest.SkipTest(reason)

HAVE_PYTHON_36 = sys.version_info >= (3, 6, 0)
HAVE_32_BIT_LINUX = sys.maxsize == 2**31-1 and sys.platform == 'linux'

requires_py36 = unittest.skipUnless(
    sys.version_info > (3, 6),
    "test requires Python 3.6 or greater")


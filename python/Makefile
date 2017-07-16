PYTHON ?= python
CPPFLAGS := -I..
CPPFLAGS += -I$(shell $(PYTHON) -c "import distutils.sysconfig as c; print(c.get_python_inc())")
CFLAGS   := -std=c11 -fPIC -g
LDFLAGS  := -shared -L.. -g
LIBS     := -lndtypes

all: #_ndtypes.so
	$(PYTHON) setup.py build_ext --inplace

_ndtypes.so: ndtypes.o
	$(CC) $(LDFLAGS) -o $@ $< $(LIBS)

ndtypes.o: ndtypes.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

check: all #_ndtypes.so
	$(PYTHON) -m pytest -sv test_ndtypes.py

clean:
	rm -rf *.o *.so

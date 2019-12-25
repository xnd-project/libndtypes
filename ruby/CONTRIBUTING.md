# Developer notes

## Contributing to ndtypes Ruby wrapper

If you're interested in contributing to the ndtypes Ruby wrapper, follow the following
steps for getting started:

* Clone the ndtypes repo: `git clone https://github.com/plures/ndtypes.git`.
* Switch to the `ruby-wrapper` branch: `git checkout ruby-wrapper`.
* Run `bundle install`.
* Run `rake develop`. This will build ndtypes with the latest working master branch
of libndtypes.
* Run `rspec` for running the tests.

## Interfacing with Ruby's GC

### Background

Ruby uses a mark-and-sweep GC that scans the entire Ruby interpreter stack for
objects that have gone out of scope and can be freed from memory. It does not 
offer any of the reference counting mechanism that the Python GC offers. 

While both approaches have their pros and cons, in the context of the ndtypes 
wrapper, it becomes risky to have 'internal' Ruby objects that are only visible
to the C API and are shared between multiple user-facing Ruby objecs. If one of
the user-facing objects goes out of scope there is a possibilty that the GC will
clean up the internal object that is shared between multiple user-facing objects
(some of which might still be in use) and that will lead to segfaults.

To avoid such a situation, in ndtypes we use a 'global GC guard' (inspired by @mrkn's
[pycall.rb](https://github.com/mrkn/pycall.rb) gem) that stores the reference to the internal objects in a global Hash
so that they don't go out of scope. When a user-facing object needs to be freed, we remove
the reference to the user-facing object and its corresponding internal object from the
global Hash.

### Details

More concretely, the `NdtObject` struct houses a `VALUE` object called `rbuf`. The struct
has the following definition:
```
typedef struct {
  VALUE rbuf;                  /* resource buffer */
  ndt_t *ndt;                   /* type */
} NdtObject;
```
In the above, the `rbuf` is a Ruby object that contains a struct of type `ResourceBufferObject`.
This is the internal object that need to be shared among multiple user-facing `NDTypes` objects.

The `gc_guard.c` file contains functions that help us interface with a global hash called
`__gc_guard_table` that is present under the `NDTypes::GCGuard` module as a instance variable 
on the module object.

### Impact on contributor

Whenever you allocate an `NDTypes` object you call the `gc_guard_register` function and pass
it the pointer of the `NdtObject` struct that you have allocated along with the `rbuf` object.

When an `NDTypes` objects needs to be freed (for example by the GC using the `NDTypes_dfree`
function), you must call the `gc_guard_unregister` function that will remove the reference
to the `rbuf` object from the global Hash.

Forgetting to call above procedures can lead to hard-to-trace GC errors.

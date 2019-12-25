require 'ndtypes/errors'

begin
  require "ruby_ndtypes.so"
rescue LoadError
  require 'ruby_ndtypes/ruby_ndtypes.so'
end

class NDTypes
  Struct.new("ApplySpec", :flags, :sig, :in_types,
             :out_types, :in_broadcast, :outer_dims)
  ApplySpec = Struct::ApplySpec
  
  # It so happens that over riding the .new method in a super class also
  # tampers with the default .new method for class that inherit from the
  # super class (Index in this case). Thus we first alias the original
  # new method (from Object) to __new__ when the Index class is evaluated,
  # and then we use an inherited hook such that the old new method (from
  # Object) is once again the default .new for the subclass.
  # Refer http://blog.sidu.in/2007/12/rubys-new-as-factory.html
  class << self
    alias :__new__ :new

    def inherited subclass
      class << subclass
        alias :new :__new__
      end
    end
  end
  
  def dup
    str = self.serialize
    NDT.deserialize str
  end

  # We over-ride the .new method so that even sending an NDTypes object can
  # will allow the .new method to act as copy constructor and simply return
  # copy of the argument.
  def self.new *args, &block
    type = args.first

    return type.dup if type.is_a?(NDTypes)

    allocate.tap { |i| i.send :initialize, *args, &block }
  end
end

NDT = NDTypes


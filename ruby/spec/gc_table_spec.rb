require 'spec_helper'

describe "global GC guard table" do
  it "stores NDT objects as key and corresponding rbuf object as value" do
    ndt = NDTypes.new("3 * int64")

    gc_table = NDTypes::GCGuard.instance_variable_get(:@__gc_guard_table)
    expect(gc_table.keys.size).to eq(1)
  end
end

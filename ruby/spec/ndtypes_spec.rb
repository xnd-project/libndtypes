require 'spec_helper'

describe NDTypes do  
  context "::MAX_DIM" do
    it "returns the maximum dimension possible" do
      expect(NDTypes::MAX_DIM > 0).to eq(true)
    end
  end

  context ".typedef" do
    it "creates a typedef for an official type" do
      NDTypes.typedef "node", "int32"

      t = NDTypes.new "4 * node"
      u = NDTypes.deserialize t.serialize

      expect(t).to eq(u)
    end
  end

  context ".instantiate" do
    context "Typedef" do
      it "instantiates NDTypes object from typedef and type" do
        NDT.typedef "node", "int32"
        NDT.typedef "cost", "int32"
        NDT.typedef "graph", "var * var * (node, cost)"

        t = NDT.new "var(offsets=[0,2]) * var(offsets=[0,3,10]) * (node, cost)"
        u = NDT.instantiate "graph", t
        expect(u.concrete?).to eq(true)

        t = NDT.new "var(offsets=[0,2]) * var(offsets=[0,2,3]) * var(offsets=[0,1,2,3]) * (node, cost)"
        expect {
          NDT.instantiate("graph", t)
        }.to raise_error(ValueError)
      end      
    end
  end
  
  context ".new" do
    it "creates a basic shaped memory block" do
      o = NDTypes.new("3 * uint64")

      expect_serialize o
    end

    context "tests roundtrip" do
      [
        "2 * 3 * float64",
        "2 * 3 * {a : uint8, b : complex64}",
      ].each do |s|
        it "type: #{s}" do
          t = NDT.new s

          expect(t.to_s).to eq(s)
        end
      end
    end

    context "from NDT" do
      [
        "2 * 3 * {a : 10 * bytes, b : 20 * string}",
        "var(offsets=[0,2]) * var(offsets=[0,3,10]) * complex128"
      ].each do |s|
        it "type: #{s}" do
          t = NDT.new s
          u = NDT.new t

          expect(u).to eq(t)
        end
      end

      it "test with offset" do
        t = NDT.new "{x: complex128, y: float64}", [[0, 2], [0, 3, 5]]
        u = NDT.new t

        expect(u).to eq(t)
      end
    end

    context "raises errors" do
      [
        "", "xyz", "var() * int64"
      ].each do |t|
        it "type #{t} raises ValueError" do
          expect { NDT.new(t) }.to raise_error(ValueError)
        end
      end

      it "raises TypeError" do
        expect { NDT.new(nil) }.to raise_error(TypeError)
      end
    end

    context "FixedString" do
      it "checks predicates" do
        t = NDT.new "fixed_string(380, 'utf16')"

        expect_serialize t

        expect(t.abstract?).to eq(false)
        expect(t.complex?).to eq(false)
        expect(t.concrete?).to eq(true)
        expect(t.float?).to eq(false)
        expect(t.optional?).to eq(false)
        expect(t.scalar?).to eq(true)
        expect(t.signed?).to eq(false)
        expect(t.unsigned?).to eq(false)

        expect(t.c_contiguous?).to eq(true)
        expect(t.f_contiguous?).to eq(true)
      end

      it "checks fixed string common fields" do
        [
          ['ascii', 1],
          ['utf8', 1],
          ['utf16', 2],
          ['utf32', 4]
        ].each do |encoding, codepoint_size|
          t = NDT.new "fixed_string(20, '#{encoding}')"
          
          expect_serialize t
          expect(t.ndim).to eq(0)

          if RUBY_PLATFORM != "i686-linux"
            expect(t.itemsize).to eq(20 * codepoint_size)
            expect(t.align).to eq(codepoint_size)
          end

          # raises tests.
        end
      end
    end # context FixedDim

    context "VarDim" do
      it "test_var_dim_predicates" do
        
      end
    end
  end

  context "#match" do
    it "needs to contiguity requirements" do
      p = NDT.new("... * 2 * 3 * float32")
      c = NDT.new("2 * 3 * float32")

      expect(p.match(c)).to eq(true)
    end

    context "C-contiguity required for inner dimensions" do
      before do
        @p = NDT.new "... * C[2 * 3 * float32]"
      end

      it "with C input" do
        c = NDT.new "2 * 3 * float32"
        expect(@p.match(c)).to eq(true)
      end

      it "with inner dimensionss C-contiguous" do
        c = NDT.new "4 * 2 * 3 * float32"
        expect(@p.match(c)).to eq(true)
      end

      it "with inner dimensions C-contiguous" do
        c = NDT.new "fixed(shape=4, step=-6) * 2 * 3 * float32"
        expect(@p.match(c)).to eq(true)
      end

      it "with F input" do
        c = NDT.new "!2 * 3 * float32"
        expect(@p.match(c)).to eq(false)
      end
    end

    context "F-contiguity required for inner dimensions" do
      before do
        @p = NDT.new "... * F[2 * 3 * float32]"        
      end

      it "with F input" do
        c = NDT.new "fixed(shape=10, step=6) * fixed(shape=2, step=1) * fixed(shape=3, step=2) * float32"
        expect(@p.match(c)).to eq(true)
      end

      it "with C input" do
        c = NDT.new "2 * 3 * float32"
        expect(@p.match(c))
      end
    end

    context "C-contiguity required for all dimensions after broadcast" do
      before do
        @p = NDT.new "C[... * 2 * 3 * float32]"
      end

      it "with C input" do
        c = NDT.new "2 * 3 * float32"
        expect(@p.match(c)).to eq(true)
      end

      it "with all dimensions C-contiguous" do
        c = NDT.new "4 * 2 * 3 * float32"
        expect(@p.match(c)).to eq(true)
      end

      it "with inner dimensions C-contiguous" do
        c = NDT.new "fixed(shape=4, step=-6) * 2 * 3 * float32"
        expect(@p.match(c)).to eq(false)
      end

      it "with F input" do
        c = NDT.new "!2 * 3 * float32"
        expect(@p.match(c)).to eq(false)
      end
    end

    context "F-contiguity required for all dimensions after broadcast." do
      before do
        @p = NDT.new "F[... * 2 * 3 * float32]"
      end

      it "with C input" do
        c = NDT.new "2 * 3 * float32"
        expect(@p.match(c)).to eq(false)

        c = NDT.new "4 * 2 * 3 * float32"
        expect(@p.match(c)).to eq(false)
      end

      it "with F input" do
        c = NDT.new "!2 * 3 * float32"
        expect(@p.match(c)).to eq(true)
      end

      it "with all dimensions F-contiguous" do
        c = NDT.new "!4 * 2 * 3 * float32"
        expect(@p.match(c)).to eq(true)
      end

      it "with inner dimensions C-contiguous" do
        c = NDT.new "fixed(shape=10, step=6) * fixed(shape=2, step=1) * fixed(shape=3, step=2) * float32"
        expect(@p.match(c)).to eq(false)
      end
    end
  end

  context "#serialize" do
    it "serializes the given shape" do
      s = NDTypes.new "3 * char"
      s.serialize
    end
  end

  context "#to_s" do
    it "converts to string using ndt_as_string" do
      t = NDTypes.new "5 * int64"

      expect(t.to_s).to eq("5 * int64")
    end
  end

  context "#hidden_dtype" do
    it "has hidden type" do
      
    end
  end

  context ".deserialize" do
    it "deserializes the NDT object" do 
      s = NDTypes.new "3 * char"
      t = s.serialize
      u = NDTypes.deserialize t
      
      expect(s).to eq(u)
    end
  end

  context "#dup" do
    DTYPE_TEST_CASES.each do |dtype, mem|
      it "dtype: #{dtype}" do
        t = NDT.new dtype
        u = t.dup

        expect(t).to eq(u)
        expect(t.ast).to eq(u.ast)        
      end
    end

    skip "tests GC -- figure how to do this." do
      
    end
  end
end

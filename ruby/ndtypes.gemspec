# coding: utf-8
$:.unshift File.expand_path("../lib", __FILE__)

require 'ndtypes/version.rb'

def self.get_files
  files = []
  ['ext', 'lib', 'spec'].each do |folder|
    files.concat Dir.glob "#{folder}/**/*"
  end

  files.concat(
    ["CONTRIBUTING.md", "Gemfile", "History.md", "ndtypes.gemspec",
     "README.md", "Rakefile"
    ])
  
  files
end
files = get_files

NDTypes::DESCRIPTION = <<MSG
NDtypes is library for defining memory shapes using a easy and friendly interface.
 
It is a wrapper over the libndtypes C library.
MSG

Gem::Specification.new do |spec|
  spec.name          = 'ndtypes'
  spec.version       = NDTypes::VERSION
  spec.authors       = ['Sameer Deshmukh']
  spec.email         = ['sameer.deshmukh93@gmail.com']
  spec.summary       = %q{Ruby wrapper over libndtypes. A library for typing memory blocks.}
  spec.description   = NDTypes::DESCRIPTION
  spec.homepage      = "https://github.com/plures/ndtypes"
  spec.license       = 'BSD-3-Clause'

  spec.files         = files
  spec.extensions    =  "ext/ruby_ndtypes/extconf.rb"
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]

  spec.add_development_dependency 'minitest', '~> 5.11'
  spec.add_development_dependency 'minitest-hooks'
  spec.add_development_dependency 'rspec', '~> 3.8'
  spec.add_development_dependency 'rake-compiler'
  spec.add_development_dependency 'pry'
  spec.add_development_dependency 'pry-byebug'
end

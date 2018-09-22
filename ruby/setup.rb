# Script for deploying the ndtypes extension on rubygems.org.
require 'fileutils'
require_relative "lib/ndtypes/version.rb"

INTENDED_VERSION = "0.2.0dev3"

if INTENDED_VERSION != NDTypes::VERSION
  raise "Version of library is not #{INTENDED_VERSION}"
end
COMMIT = NDTypes::COMMIT

action = ARGV[0]

ext_ndtypes = "ext/ruby_ndtypes/ndtypes"
puts "deleting previously created #{ext_ndtypes} directory..."
FileUtils.rm_rf(ext_ndtypes)
Dir.mkdir(ext_ndtypes)

puts "cloning ndtypes repo into ext/ folder..."
system("git clone https://github.com/plures/ndtypes #{ext_ndtypes}")

if action == "deploy"  
  puts "checking out version #{INTENDED_VERSION}..."
  Dir.chdir(ext_ndtypes) do
    system("git checkout #{COMMIT}")
  end  
end

puts "building gem with rake build..."
system("rake build")

# Script for deploying the ndtypes extension on rubygems.org.

require_relative "lib/ndtypes/version.rb"

INTENDED_VERSION = "0.2.0dev3"

if INTENDED_VERSION != NDTypes::VERSION
  raise "Version of library is not #{INTENDED_VERSION}"
end
COMMIT = NDTypes::COMMIT

action = ARGV[0]



if action == "deploy"  
  puts "checking out version #{INTENDED_VERSION}..."

end



require 'mkmf'

def unix?
  true
end

puts "compiling libndtypes on your machine..."
Dir.chdir(File.join(File.dirname(__FILE__) + "/ndtypes")) do
  if unix?
    system("./configure --prefix=#{File.expand_path("../")}")
    system("make -j4")
    system("make install")
  elsif windows?
    raise NotImplementedError, "need to specify build instructions for windows."
  end
end

FileUtils.rm_rf("share")

$INSTALLFILES = [
  ["ruby_ndtypes.h", "$(archdir)"]
]
binaries = File.expand_path(File.join(File.dirname(__FILE__) + "/lib/"))
headers = File.expand_path(File.join(File.dirname(__FILE__) + "/include/"))
$LOAD_PATH << File.expand_path(binaries)

["ndtypes"].each do |lib|
  find_library(lib, nil, binaries)
end

["ndtypes.h"].each do |header|
  find_header(header, headers)
  have_header(header)
end

dir_config("ndtypes", [headers], [binaries])

basenames = %w{gc_guard ruby_ndtypes}
$objs = basenames.map { |b| "#{b}.o"   }
$srcs = basenames.map { |b| "#{b}.c" }

puts "LOAD_PATH :: #{$LOAD_PATH}."

$CFLAGS += " -O0 -g "
create_makefile("ruby_ndtypes/ruby_ndtypes")


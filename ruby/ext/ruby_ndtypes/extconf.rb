require 'mkmf'

def windows?
  (/cygwin|mswin|mingw|bccwin|wince|emx/ =~ RUBY_PLATFORM) != nil
end

def mac?
  (/darwin/ =~ RUBY_PLATFORM) != nil
end

def unix?
  !windows?
end

puts "compiling libndtypes on your machine..."
Dir.chdir(File.join(File.dirname(__FILE__) + "/ndtypes")) do
  if unix?
    ["libndtypes", "libndtypes/compat", "libndtypes/serialize"].each do |f|
      Dir.chdir(f) do
        Dir.mkdir(".objs") unless Dir.exists? ".objs"
      end
    end

    system("./configure --prefix=#{File.expand_path("../")} --with-docs=no")
    system("make")
    system("make install")
  elsif windows?
    raise NotImplementedError, "need to specify build instructions for windows."
  end
end

$INSTALLFILES = [
  ["ruby_ndtypes.h", "$(archdir)"],
  ["ndtypes.h", "$(archdir)"]
]

binaries = File.expand_path(File.join(File.dirname(__FILE__) + "/lib/"))
headers = File.expand_path(File.join(File.dirname(__FILE__) + "/include/"))
$LOAD_PATH << File.expand_path(binaries)
append_ldflags("-Wl,-rpath #{binaries}")

["ndtypes"].each do |lib|
  find_library(lib, nil, binaries)
end

["ndtypes.h"].each do |header|
  find_header(header, headers)
  have_header(header)
end

FileUtils.copy_file File.expand_path(File.join(File.dirname(__FILE__) +
                                               "/ruby_ndtypes.h")),
                    "#{headers}/ruby_ndtypes.h"

dir_config("ndtypes", [headers], [binaries])

basenames = %w{gc_guard ruby_ndtypes}
$objs = basenames.map { |b| "#{b}.o"   }
$srcs = basenames.map { |b| "#{b}.c" }

$CFLAGS += " -O0 -g "
create_makefile("ruby_ndtypes/ruby_ndtypes")

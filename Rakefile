require 'rake/clean'

ERBS = Dir.glob('**/*.erb')
ERB_SOURCES = ERBS.map {|f| f[0..-5]}  # strip off the .erb at the end.

# compiler flags
$C_FLAGS = ['-Wall',
            "-I#{File.expand_path('../mruby/include')}",
            "-I#{File.expand_path('../mruby/include/mruby')}",
            "-I#{File.expand_path('../mruby/src')}",
            '-I/opt/local/include'
           ]

$DEBUG_C_FLAGS = ['-g', '-DDEBUG',
                  #'-DDUMP_SHADER_VARIABLES',
                  #'-DDEBUG_DESTRUCTORS',
                  #'-DDEBUG_TRAVERSAL',
                 ]

$OPT_C_FLAGS = ['-O3']

# linker flags
L_FLAGS = ['-lstdc++',
           '../mruby/lib/ritevm.a',
          ]

OBJECTS = [# sdl
           'main.o',
          ]

DEPS = OBJECTS.map {|f| f[0..-3] + '.d'}
EXE = 'test'

# Use the compiler to build makefile rules for us.
# This will list all of the pre-processor includes this source file depends on.
def make_deps t
  sh "gcc -MM -MF #{t.name} #{$C_FLAGS.join ' '} -c #{t.source}"
end

# Compile a single compilation unit into an object file
def compile obj, src
  sh "gcc #{$C_FLAGS.join ' '} -c #{src} -o #{obj}"
end

# Link all the object files to create the exe
def link exe, objects
  sh "gcc #{objects.join ' '} -o #{exe} #{L_FLAGS.join ' '}"
end

# generate source code from erbs
rule '.cpp' => ['.cpp.erb', 'src/render/states.rb'] do |t| 
  generate_erb_template t
end
rule '.h' => ['.h.erb', 'src/render/states.rb'] do |t| 
  generate_erb_template t
end

# generate makefile rules from source code
rule '.d' => '.cpp' do |t|
  make_deps t
end
rule '.d' => '.m' do |t|
  make_deps t
end

# adds .o rules so that objects will be recompiled if any of the contributing source code has changed.
task :add_deps => ERB_SOURCES + DEPS do
  OBJECTS.each do |obj|
    dep = obj[0..-3] + '.d'
    raise "Could not find dep file for object #{obj}" unless dep

    # open up the .d file, which is a makefile rule (built by make_deps)
    deps = []
    File.open(dep, 'r') {|f| f.each {|line| deps |= line.split}}
    deps.reject! {|x| x == '\\'}  # remove '\\' entries

    # Add a new file rule which will build the object file from the source file.
    # Note: this object file depends on all the pre-processor includes as well
    file obj => deps[1,deps.size] do |t|
      compile t.name, t.prerequisites[0]
    end
  end
end

file EXE => [:add_deps] + OBJECTS do
  link EXE, OBJECTS
end

task :build => EXE
task :add_opt_flags do
  $C_FLAGS += $OPT_C_FLAGS
end
task :add_debug_flags do
  $C_FLAGS += $DEBUG_C_FLAGS
end

desc "Optimized Build"
task :opt => [:add_opt_flags, EXE]

desc "Debug Build"
task :debug => [:add_debug_flags, EXE]

desc "Optimized Build, By Default"
task :default => [:opt]

CLEAN.include ERB_SOURCES, DEPS, OBJECTS
CLOBBER.include EXE

override CFLAGS += -std=c11
override CXXFLAGS += -std=c++11

# you can add your own libs need to link.
override LDLIBS += -lpthread

# the aux variable, used to identify the directory this Makefile in.
dir_self := $(shell dirname $(shell readlink -fe $(lastword ${MAKEFILE_LIST})))

# dst directory, all the output files will be put in it.
dst_dir := ${dir_self}/output
# the main name of this program
main_name := exe-model
# include directories, use `:' as seperator
inc_dir_set := ${dir_self}/include
# recursive directory, will compile files in it, and in it's subdirectories.
src_recdir_set:=${dir_self}/src
# solo directory, will only compile the files under it directly, no recursive.
#src_solodir_set:=${dir_self}/src

# the default value of `output_types' is `bin'
# you can assign multi value to it.
# the selective values:
# 	bin:		*	exe program.
# 	sharedlib:	*.so	if you selected it, you must define `lib_version'
# 			simultaneously.
#	staticlib:	*.a	used to link with other module to final .so or bin.
#output_types := bin,sharedlib,staticlib
#lib_version :=
include ${dir_self}/scripts/Makefile.base

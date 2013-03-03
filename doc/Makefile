tex_name=fusion-splicer-SD

tex_deps:=$(shell find . -type f -name \*.tex)
tex_deps+=$(shell find . -type f -name \*.bib)
tex_deps+=$(shell find . -type f -name \*.lua)
tex_deps+=$(shell find . -type f -name \*.mkiv)

mp_dir  :=mp
mp_files:=$(shell find ${mp_dir} -type f -name \*.mp)
mp_objs :=$(patsubst %.mp,%.log,$(mp_files))

all: $(tex_name).pdf

clean:
	@rm -f $(tex_name).tuc $(tex_name).log $(tex_name).pdf
	@rm -f $(tex_name).aux $(tex_name).bbl $(tex_name).blg $(tex_name).top
	@find ${mp_dir} -type f ! -name \*.mp | xargs rm

$(tex_name).pdf: $(tex_deps) $(mp_objs)
	context $(tex_name).tex

mp/%.log: mp/%.mp
	@echo "compiling $<"
	cd mp; mptopdf `basename $<`


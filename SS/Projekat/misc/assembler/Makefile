inc_dir = ../../h
obj_dir = ../../obj/assembler
common_obj_dir = ../../obj/common
src_dir = ../../src/assembler
out_dir = ../..

c_flags = -Werror -I$(inc_dir) -g -std=c2x
cpp_flags = -Werror -I$(inc_dir) -g -std=c++20

c_src = $(wildcard $(src_dir)/*.c)
c_dep = $(patsubst $(src_dir)/%.c, $(src_dir)/%.d, $(c_src))
c_obj = $(patsubst $(src_dir)/%.c, $(obj_dir)/%.o, $(c_src)) $(obj_dir)/parser.o $(obj_dir)/lexer.o

cpp_src = $(wildcard $(src_dir)/*.cpp)
cpp_dep = $(patsubst $(src_dir)/%.cpp, $(src_dir)/%.d, $(cpp_src))
cpp_obj = $(patsubst $(src_dir)/%.cpp, $(obj_dir)/%.o, $(cpp_src))

build: $(c_obj) $(cpp_obj)
	@cd ../common && make --no-print-directory

	g++ -lfl -g -o $(out_dir)/assembler $^ $(common_obj_dir)/*.o

$(src_dir)/parser.c: parser.y
	bison --defines="$(inc_dir)/assembler/parser.h" -o $@ $<
	sed -i 's/parser\.h/assembler\/parser\.h/g' $(src_dir)/parser.c

$(src_dir)/lexer.c: lexer.l
	flex -o $@ $<

$(obj_dir)/%.o: $(src_dir)/%.c
	gcc $(c_flags) -c -o $@ $<

$(obj_dir)/%.o: $(src_dir)/%.cpp
	g++ $(cpp_flags) -c -o $@ $<

%.d: %.c
	@gcc $(c_flags) -MM -MT $(patsubst $(src_dir)/%.c, $(obj_dir)/%.o, $<) $< > $@

%.d: %.cpp
	@g++ $(cpp_flags) -MM -MT $(patsubst $(src_dir)/%.cpp, $(obj_dir)/%.o, $<) $< > $@

include $(c_dep)
include $(cpp_dep)

clean:
	rm -f $(obj_dir)/*.o
	rm -f $(src_dir)/*.d
	rm -f $(out_dir)/assembler

.PHONY: clean
inc_dir = ../../h
obj_dir = ../../obj/linker
common_obj_dir = ../../obj/common
src_dir = ../../src/linker
out_dir = ../..

cpp_flags = -Wall -flto -I$(inc_dir) -g -std=c++20

cpp_src = $(wildcard $(src_dir)/*.cpp)
cpp_dep = $(patsubst $(src_dir)/%.cpp, $(src_dir)/%.d, $(cpp_src))
cpp_obj = $(patsubst $(src_dir)/%.cpp, $(obj_dir)/%.o, $(cpp_src))

build: $(cpp_obj)
	@cd ../common && make --no-print-directory

	g++ $(cpp_flags) -o $(out_dir)/linker $^ $(common_obj_dir)/*.o

$(obj_dir)/%.o: $(src_dir)/%.cpp
	g++ $(cpp_flags) -c -o $@ $<

%.d: %.cpp
	@g++ $(cpp_flags) -MM -MT $(patsubst $(src_dir)/%.cpp, $(obj_dir)/%.o, $<) $< > $@

include $(cpp_dep)

clean:
	rm -f $(obj_dir)/*.o
	rm -f $(src_dir)/*.d
	rm -f $(out_dir)/linker

.PHONY: clean
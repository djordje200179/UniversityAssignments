inc_dir = ../../h
obj_dir = ../../obj/common
src_dir = ../../src/common

cpp_flags = -Werror -I$(inc_dir) -std=c++20

src = $(wildcard $(src_dir)/*.cpp)
obj = $(patsubst $(src_dir)/%.cpp, $(obj_dir)/%.o, $(src))
dep = $(patsubst $(src_dir)/%.cpp, $(src_dir)/%.d, $(src))

build: $(obj)

$(obj_dir)/%.o: $(src_dir)/%.cpp
	g++ $(cpp_flags) -c -o $@ $<

%.d: %.cpp
	@g++ $(cpp_flags) -MM -MT $(patsubst $(src_dir)/%.cpp, $(obj_dir)/%.o, $<) $< > $@

include $(dep)

clean: 
	rm -f $(obj_dir)/*.o
	rm -f $(src_dir)/*.d

.PHONY: clean
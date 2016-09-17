
.DEFAULT_GOAL := test.exe

COMP = g++-5
FLAGS = -std=c++11

OPENCV = `pkg-config --cflags --libs opencv`
XLIBS = -lX11 -lXtst -lXext -lxdo

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
DPND = $(SRC:.cpp=.d)

TARFILES = $(wildcard *.tar.gz *.tgz)


define extract_tmpl
$(shell tar tf $(1) | head -1):
	tar zxf $(1)
endef

TAR_DIRS := $(foreach file, $(TARFILES), $(shell tar tf $(file) | head -1))

data: $(TAR_DIRS)

$(foreach file, $(TARFILES), $(eval $(call extract_tmpl, $(file))))

.PHONY: data


test.exe: $(OBJ)
	$(COMP) $(FLAGS) $^ -o $@ $(OPENCV) $(XLIBS)


%.o: %.cpp
	$(COMP) $(FLAGS) -MMD -MP -c $< -o $@


clean:
	rm -f *.o *.d


cleandata:
	rm -r scoreData


cleanall:
	rm -f *.o *.d *.exe
	rm -fr $(TAR_DIRS)


-include $(DPND)


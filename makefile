
COMP = g++-5
FLAGS = -std=c++11
OPENCV = `pkg-config --cflags --libs opencv`
XLIBS = -lX11 -lXtst -lXext -lxdo
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
DPND = $(SRC:.cpp=.d)


test.exe: $(OBJ)
	$(COMP) $(FLAGS) $^ -o $@ $(OPENCV) $(XLIBS)

%.o: %.cpp
	$(COMP) $(FLAGS) -MMD -MP -c $< -o $@

clean:
	rm -f *.o *.d

cleanall:
	rm -f *.o *.d *.exe

-include $(DPND)


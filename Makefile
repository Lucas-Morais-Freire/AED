COMP:=g++
CXX_FLAGS:=-Wall -Wextra -g -DDEBUG # -fopenmp -fcf-protection=none -fno-stack-protector -O3
SRCS:=$(wildcard src/*.cpp) main.cpp
INCLUDE:=./include ./templates
TARGET:=main

OBJS=$(patsubst %.cpp,%.o,$(SRCS))
DEPS=$(patsubst %.cpp,%.d,$(SRCS))
INCLUDE_FLAGS=$(patsubst %,-I%,$(INCLUDE))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(COMP) $(CXX_FLAGS) $^ -o $@

%.o: %.d
	$(COMP) $(CXX_FLAGS) -c $*.cpp -o $@ $(INCLUDE_FLAGS)

%.d: %.cpp
	$(COMP) $(CXX_FLAGS) -MMD -MP -c $< -o $*.o $(INCLUDE_FLAGS)

clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)

run: $(TARGET)
	./$(TARGET)

remake:
	make clean
	make -j

-include $(DEPS)
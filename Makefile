CC := g++
CFLAGS := -std=c++11 -pthread

SRCS := main.cpp
OBJS := $(SRCS:.cpp=.o)
TARGET := prime_numbers

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

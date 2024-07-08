CC = gcc
CFLAGS = -g -fno-trapping-math -fno-signed-zeros -ffinite-math-only -funsafe-math-optimizations -fno-math-errno -ffast-math -fwhole-program -ftree-loop-distribution -funsafe-loop-optimizations -Wunsafe-loop-optimizations -march=native -Ofast -funroll-loops -Wall -Wextra -std=c99
TARGET = password_masks
LDFLAGS = -lm
SRCS = main.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	# strip $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)



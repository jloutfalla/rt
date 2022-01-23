CFLAGS	= -std=c99 -march=native -O2 -pipe -Wall -Wextra -pedantic -DTHREADED
LIBS	= -lm -lpthread
LDFLAGS =

INCLUDEDIR	= include
SRCDIR		= src
BUILDDIR	= build

SRCS = $(shell find $(SRCDIR) -type f -name '*.c')
OBJS = $(SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
DEPS = $(OBJS:.o=.d)

TARGET = rt

.PHONY: all
all: $(TARGET)

.PHONY: run
run: $(TARGET)
	./$(TARGET)
	# ./$(TARGET) > out.ppm

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

$(BUILDDIR):
	mkdir $(BUILDDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	-@mkdir -p ${@D}
	$(CC) $(CFLAGS) -MMD -MP $< -c -o $@ -I$(INCLUDEDIR)

-include $(DEPS)

.PHONY: clean
clean:
	rm -rf $(OBJS) $(BUILDDIR) $(TARGET)

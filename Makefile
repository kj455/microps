TAPDEV = tap0
TAPADDR = 192.0.2.1/24

CFLAGS += -g -W -Wall -Wno-unused-parameter
INCFLAGS += -iquote .

ifeq ($(shell uname),Linux)
  # Linux specific settings
  PLATFORM := ./platform/linux
  LDFLAGS += -pthread
  INCFLAGS += -iquote $(PLATFORM)
endif

ifeq ($(shell uname),Darwin)
  # macOS specific settings
endif

EXCLUDE := -path ./platform -prune -o \
           -path ./test -prune -o \

SRCS := $(shell find . $(PLATFORM) $(EXCLUDE) -type f -name '*.c' -print | sort -V)
TARGETS := $(shell find ./test -type f -name '*.c' -print | sort -V)

OBJS := $(SRCS:%.c=%.o)
EXES := $(TARGETS:%.c=%.exe)

DEPDIR := .deps
$(shell mkdir $(DEPDIR) > /dev/null 2>&1 || :)
DEPFLAGS = -MMD -MP -MF $(DEPDIR)/$(@F:.o=.d)

.SUFFIXES:
.SUFFIXES: .c .o

.PHONY: all clean tap

all: $(EXES)

$(EXES): %.exe : %.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) $(DEPFLAGS) $(CFLAGS) $(INCFLAGS) -c $< -o $@

clean:
	rm -rf $(EXES) $(EXES:.exe=.o) $(OBJS) $(DEPDIR)

tap:
	@ip addr show $(TAPDEV) 2>/dev/null || (echo "Create '$(TAPDEV)'"; \
	  sudo ip tuntap add mode tap user $${USER:-root} name $(TAPDEV); \
	  sudo sysctl -w net.ipv6.conf.$(TAPDEV).disable_ipv6=1 2>/dev/null || true; \
	  sudo ip addr add $(TAPADDR) dev $(TAPDEV); \
	  sudo ip link set $(TAPDEV) up; \
	  ip addr show $(TAPDEV); \
	)

include $(wildcard $(DEPDIR)/*)

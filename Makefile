PRODUCT := example
BINDIR := bin
INCDIR := include
SRCDIR := src
OBJDIR := obj
OUTDIR := out

CC := gcc
LINKER := gcc
INCDIRS := -I$(INCDIR)
CFLAGS := -Wall -Wextra -Werror -g3 

SRCFILES := $(wildcard $(SRCDIR)/*.c)
OBJFILES := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCFILES))
OUTFILES := $(patsubst $(SRCDIR)/%.c,$(OUTDIR)/%.c,$(SRCFILES))

build: makedirs $(BINDIR)/$(PRODUCT)
.PHONY: build

cpp: makedirs $(OUTFILES)

$(BINDIR)/$(PRODUCT): $(OBJFILES)
	$(LINKER) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/*.h
	$(CC) $(CFLAGS) $(INCDIRS) -c $< -o $@

$(OUTDIR)/%.c: $(SRCDIR)/%.c $(INCDIR)/*.h
	$(CC) $(CFLAGS) -E $(INCDIRS) -c $< -o $@

makedirs:
	@mkdir -p $(BINDIR)
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OUTDIR)
.PHONY: makedirs


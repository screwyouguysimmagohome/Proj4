CC = gcc
CFLAGS = -Wall -I.
LDFLAGS = -lm -lsndfile -lfftw3 
SRC = load_files.c spectral_bandwidth.c test.c
OBJ = $(SRC:.c=.o)
EXEC = test

all: $(EXEC)

$(EXEC): $(OBJ)
	@echo "Linking $^ into $@..."
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo "Cleaning up..."
	rm -f $(EXEC) $(OBJ)

.PHONY: all clean

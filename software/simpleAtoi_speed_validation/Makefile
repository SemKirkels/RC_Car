CC     = gcc
CFLAGS = -O2 -Wall -Wextra
BINS   = bench_simple bench_stdlib bench_scaling
 
all: $(BINS)
 
bench_simple.exe: bench_simple.c
	$(CC) $(CFLAGS) -o $@ $<
 
bench_stdlib.exe: bench_stdlib.c
	$(CC) $(CFLAGS) -o $@ $<
 
bench_scaling.exe: bench_scaling.c
	$(CC) $(CFLAGS) -o $@ $<
 
run: all
	@echo "=== 100 million iterations (mixed input) ==="
	./bench_simple
	./bench_stdlib
	@echo ""
	@echo "=== Scaling: short vs long strings ==="
	./bench_scaling
 
clean:
	del /Q $(BINS) 2>nul || rm -f $(BINS)
 
.PHONY: all run clean
 
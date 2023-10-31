OMP = -fopenmp
CFLAGS = -g
LFLAGS = -lm

all: heat

misc.o: misc.c
	clang -c $(CFLAGS) $< -o $@

heat: heat.c misc.o
	clang $(CFLAGS) $(OMP) -DNB=8 $+ $(LFLAGS) -o heat

run: heat
	./heat test.dat

clean:
	rm -rf heat heat.o heat.pcf heat.prv heat.row heat.ppm misc.o

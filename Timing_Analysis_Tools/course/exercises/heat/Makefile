CC = clang
OMP = -fopenmp
CFLAGS = -O2 -g
LFLAGS = -lm

OUT_DIR = bin
TDG = -fopenmp-taskgraph -static-tdg -L${OMP_PATH}

EXTRAE = -DEXTRAE -I${TAFLOW_PATH}/extrae/include -L${TAFLOW_PATH}/extrae/lib -lomptrace

BIN= heat

NB=8

all: $(BIN)

misc.o: misc.c
	$(CC) -fdebug-default-version=3 -c $(CFLAGS) $< -o $@


heat: heat.c misc.o
	$(CC) -DNB=$(NB) -DTDG -DOMP_TASK_DEPENDS $(CFLAGS) $(OMP) $(TDG)  $+ $(LFLAGS) -o $@ $(EXTRAE)

heat_static: heat.o heat_tdg.cpp misc.o
	$(CC) $(CFLAGS) $(OMP) $+ $(LFLAGS) -o heat $(EXTRAE)  -L${OMP_PATH}

clean:
	rm -fr *.o $(BIN) *ppm tdg.dot tdg.c *_tdg.c


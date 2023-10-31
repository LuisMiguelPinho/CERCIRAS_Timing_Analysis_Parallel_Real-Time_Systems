TDG = -fopenmp-taskgraph -static-tdg -L${OMP_PATH}

test: main.c
	${CC} -I${TAFLOW_PATH}/extrae/include -L${TAFLOW_PATH}/extrae/lib main.c ${TDG} -fopenmp -lomptrace -o test

test_static: main.o tdg.cpp
	${CC} -L${TAFLOW_PATH}/extrae/lib main.o tdg.cpp ${TDG} -fopenmp -lomptrace -o test
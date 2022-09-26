OBJS = AES_basic.cpp basic.cpp 

build: $(OBJS)
	g++ -o AES_basic.o -I ${GUROBI_HOME}/include -L ${Gurobi_HOME}/lib $(OBJS) -lgurobi_g++5.2 -lgurobi91 -lm

2round_optimal: clean build
	./AES_basic.o 2 NoObjBound 1
2round_optimal_enum: clean build
	./AES_basic.o 2 4 2

3round_optimal: clean build
	./AES_basic.o 3 NoObjBound 1

3round_optimal_enum: clean build
	./AES_basic.o 3 8 1
4round_optimal: clean build
	./AES_basic.o 4 NoObjBound 1

4round_optimal_enum: clean build
	./AES_basic.o 4 22 1


clean:
	rm -f AES_basic.o
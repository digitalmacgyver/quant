FILES=\
	complex.C\
	qureg.C\
	util.C\
	shor.C

OBJS=$(FILES:.C=.o)

BINARY=shor-simulator

%.o : %.cpp
	g++ -O3 -c $< -o $@

all : $(OBJS)
	g++ -O3 -o $(BINARY) $(OBJS)

clean :
	rm -f $(OBJS) $(BINARY)


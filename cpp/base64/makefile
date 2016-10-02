

all: encode64 decode64

clean: 
	rm *.o || true
	rm *.exe || true

# Link and generate output
encode64: encode64.o
	g++ -Wall encode64.o -o encode64

# Compile
encode64.o: encode64.cpp
	g++ -Wall -c encode64.cpp


decode64: decode64.o
	g++ -Wall decode64.o -o decode64

decode64.o: decode64.cpp
	g++ -Wall -c decode64.cpp






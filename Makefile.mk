test : test.cc CoHash.cc CoHash.h
	g++ $^ -o $@ -g


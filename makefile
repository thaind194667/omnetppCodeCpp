
run:
	g++ -std=c++17 -g main.cpp pugixml.cc TENodeAlgorithm.cc -o main

bai3:
	make clean
	g++ -std=c++17 -g Bai3.cpp pugixml.cc -o bai3

bai4:
	make clean
	g++ -std=c++17 -g Bai4.cpp pugixml.cc -o bai4

clean:
	rm -f AllParts.txt bai3 bai4 main out1.txt
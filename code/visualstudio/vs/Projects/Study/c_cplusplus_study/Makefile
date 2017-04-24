class : main.o ClassLayout.o ClassFragments.o 
	g++ -o class main.o ClassLayout.o ClassFragments.o
main.o : main.cpp
	g++ -c main.cpp
ClassLayout.o : ClassLayout.cpp ClassLayout.h
	g++ -c ClassLayout.cpp
ClassFragments.o : ClassFragments.cpp ClassFragments.h
	g++ -c ClassFragments.cpp

clean :
	rm class main.o ClassLayout.o ClassFragments.o

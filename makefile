P2.out : Project2.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o
	g++ -g -o P2.out Project2.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o

Project2.o : Project2.cpp SetLimits.h SyntacticalAnalyzer.h
	g++ -g -c Project2.cpp

SetLimits.o : SetLimits.cpp SetLimits.h
	g++ -g -c SetLimits.cpp

LexicalAnalyzer.o : LexicalAnalyzer.cpp LexicalAnalyzer.h
	g++ -g -c LexicalAnalyzer.cpp

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h
	g++ -g -c SyntacticalAnalyzer.cpp

clean : 
	rm *.o P2.out *.gch


submit : Project2.cpp LexicalAnalyzer.h LexicalAnalyzer.cpp SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp makefile README.txt
	rm -rf Team?P2
	mkdir Team?P2
	cp Project2.cpp Team?P2
	cp LexicalAnalyzer.h Team?P2
	cp LexicalAnalyzer.cpp Team?P2
	cp SyntacticalAnalyzer.h Team?P2
	cp SyntacticalAnalyzer.cpp Team?P2
	cp makefile Team?P2
	cp README.txt Team?P2
	tar cfvz Team?P2.tgz Team?P2
	cp Team?P2.tgz ~tiawatts/cs460drop

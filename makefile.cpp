# Makefile

# Compiler και flags
CXX = g++
CXXFLAGS = -Wall -std=c++11

# Πλατφόρμα Linux (για Windows μπορεί να χρειαστεί τροποποίηση ή χρήση MinGW)
LIBS = -lGL -lGLU -lglut

# Τα 2 εκτελέσιμά μας
EXEC1 = views
EXEC2 = visiting3Dhouse

all: $(EXEC1) $(EXEC2)

$(EXEC1): views.o
	$(CXX) views.o -o $(EXEC1) $(LIBS)

$(EXEC2): visiting3Dhouse.o
	$(CXX) visiting3Dhouse.o -o $(EXEC2) $(LIBS)

views.o: views.cpp
	$(CXX) $(CXXFLAGS) -c views.cpp

visiting3Dhouse.o: visiting3Dhouse.cpp
	$(CXX) $(CXXFLAGS) -c visiting3Dhouse.cpp

clean:
	rm -f *.o $(EXEC1) $(EXEC2)

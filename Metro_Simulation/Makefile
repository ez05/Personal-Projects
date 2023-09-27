#########################################################
#             PROJECT 1: MetroSim Makefile             #
#########################################################

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS  = -g3 

MetroSim: main.o MetroSim.o PassengerQueue.o Passenger.o
	${CXX} ${LDFLAGS} -o MetroSim $^

Passenger.o: Passenger.cpp
	${CXX} ${CXXFLAGS} -c Passenger.cpp

MetroSim.o: MetroSim.cpp MetroSim.h PassengerQueue.h Passenger.h
	${CXX} ${CXXFLAGS} -c MetroSim.cpp

# This rule builds PassengerQueue.o
PassengerQueue.o: PassengerQueue.cpp PassengerQueue.h Passenger.h 
	$(CXX) $(CXXFLAGS) -c PassengerQueue.cpp

main.o: main.cpp MetroSim.h
	${CXX} ${CXXFLAGS} -c main.cpp

unit_test: unit_test_driver.o PassengerQueue.o Passenger.o
	$(CXX) $(CXXFLAGS) $^


# This rule provides your final submission 
# NOTE: Don't forget to add any additional files you want to submit to this
#		rule! If you do not add them, they will not be submitted.
provide:
	provide comp15 proj1_MetroSim PassengerQueue.h \
				    PassengerQueue.cpp \
				    Passenger.h Passenger.cpp  \
				    unit_tests.h Makefile README \
					MetroSim.h MetroSim.cpp \
					main.cpp test_main_1.cpp \
					station_s.txt test_output.txt \
					my_output.txt \

# remove executables, object code, and temporary files from the current folder
# -- the executable created by unit_test is called a.out
clean: 
	rm *.o *~ a.out

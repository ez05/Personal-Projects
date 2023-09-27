/*
 *  main.cpp
 *  Eric Zhao
 *  22 Feb
 *
 *  CS 15 PROJ 1 MetroSim
 *
 *  Client program for class MetroSim
 *
 */

#include <iostream>
#include <fstream>
#include <string>
#include "MetroSim.h"
#include "Passenger.h"
#include "PassengerQueue.h"


using namespace std;

void metroSim(int argc, char *argv[]);
template<typename streamtype>
void open_files(streamtype &stream, string file_name);

int main(int argc, char *argv[])
{
    if ((argc != 3) and (argc != 4)) {
        cerr << "Usage: ./MetroSim stationsFile outputFile";
        cerr << " [commandsFile]" << endl;
        exit(EXIT_FAILURE);
    }
    ifstream s_station;
    ofstream s_output;
    ifstream s_command;

    open_files(s_station, argv[1]);
    open_files(s_output, argv[2]);

    //check for commands and run
    if (argc == 3) {
        MetroSim metrosim(s_station);
        metrosim.run_command(std::cin, s_output);
        s_station.close();
        s_output.close();
    } else if (argc == 4) {
        MetroSim metrosim(s_station);
        open_files(s_command, argv[3]);
        metrosim.run_command(s_command, s_output);
        s_command.close();
        s_station.close();
        s_output.close();
    }
    return 0;
}

/* FUNCTION:  open_files(streamtype &stream, string file_name)
 * Purpose:   Open a stream file and check for successful opening
 * Arg:       A stream file file and a string
 * Returns:   N/A
 * Notes:     N/A
 */
template<typename streamtype>
void open_files(streamtype &stream, string file_name)
{
    stream.open(file_name);
    if (not stream.is_open()) {
        cerr << "Error: could not open file " << file_name << endl;
        exit(EXIT_FAILURE);
    }
}
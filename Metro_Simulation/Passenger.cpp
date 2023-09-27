/*
 *  Passenger.cpp
 *  Eric Zhao
 *  10 Feb
 *
 *  CS 15 PROJ 1: A Metro Simulator
 *
 *  Implementation for class Passenger
 *
 */

#include "Passenger.h"
#include <iostream>
#include <string>
#include <fstream>

/* FUNCTION:  print(std::ostream &output)
 * Purpose:   print the passenger's id, arrival and departure station
 * Arg:       An output stream
 * Returns:   N/A
 * Notes:     N/A
 */
void Passenger::print(std::ostream &output)
{
        output << "[" << id << ", " << from << "->" << to << "]";
}

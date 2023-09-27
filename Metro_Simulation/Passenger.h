/*
 *  Passenger.h
 *  Eric Zhao
 *  10 Feb
 *
 *  CS 15 PROJ 1: A Metro Simulator
 *
 *  Interface for class Passenger
 *
 */

#ifndef __PASSENGER_H__
#define __PASSENGER_H__

#include <iostream>
#include <fstream>
#include <string>

struct Passenger
{
        int id, from, to;
        
        Passenger()
        {
                id   = -1;
                from = -1;
                to   = -1;
        }

        Passenger(int newId, int arrivalStation, int departureStation)
        {
                id   = newId;
                from = arrivalStation;
                to   = departureStation;
        }

        void print(std::ostream &output);

};

#endif

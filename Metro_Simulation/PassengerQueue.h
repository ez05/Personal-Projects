/*
 *  PassengerQueue.h
 *  Eric Zhao
 *  10 Feb
 *
 *  CS 15 PROJ 1: A Metro Simulator
 *
 *  Interface for class PassengerQueue
 *
 */

#ifndef _PASSENGERQUEUE_H_
#define _PASSENGERQUEUE_H_

#include "Passenger.h"
#include "PassengerQueue.h"
#include <string>
#include <iostream>
#include <vector>


class PassengerQueue {
public:
    Passenger front();
    void dequeue();
    void enqueue(const Passenger &passenger);
    int size();
    void print(std::ostream &output);

private:
    std::vector<Passenger> queue;
};

#endif
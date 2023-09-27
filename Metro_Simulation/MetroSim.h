/*
 *  MetroSim.h
 *  Eric Zhao
 *  22 Feb
 *
 *  CS 15 PROJ 1 MetroSim
 *
 *  Interface for class MetroSim
 *
 */

#ifndef _METROSIM_H_
#define _METROSIM_H_

#include "Passenger.h"
#include "PassengerQueue.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class MetroSim
{
public:

    MetroSim(std::ifstream &stationsFile);
    ~MetroSim();
    void run_command(std::istream &input, std::ofstream &output);

private:

    //a vector of type PassengerQueue that represents
    //passengers on each "cabin" of the train
    std::vector<PassengerQueue> train;

    struct Station {
        //a vector of type string that represents the name of each station
        std::vector<std::string> name;

        //a vector of type bool that represents the position of the train
        std::vector<bool> isTrain;

        //a vector of type PassengerQueue that represents 
        //passengers on each station
        std::vector<PassengerQueue> passenger_awaiting;
    };

    Station station;
    void print_train();
    void get_off(std::ofstream &output);
    int destination(Passenger passenger);
    void add_passenger(int id, int from, int to);
    void get_on(int train_position);
    void print_station();
    void move_train(std::ofstream &output);
    int find_train();
    void logged_exiting_passenger(std::ostream &output, Passenger passenger, 
    std::string station_name);
    void circular_staiton();
    void print_status();
};

#endif
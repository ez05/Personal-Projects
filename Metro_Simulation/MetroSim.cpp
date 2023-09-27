/*
 *  MetroSim.cpp
 *  Eric Zhao
 *  22 Feb
 *
 *  CS 15 PROJ 1 MetroSim
 *
 *  Implementation for class MetroSim
 *
 */

#include "MetroSim.h"
#include "Passenger.h"
#include "PassengerQueue.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

/* FUNCTION:  MetroSim(std::ifstream &stationsFile)
 * Purpose:   Initialise a MetroSim instance with the given station file
 * Arg:       An ifstream station file
 * Returns:   N/A
 * Notes:     N/A
 */
MetroSim::MetroSim(std::ifstream &stationsFile)
{
    //initialise station_name
    std::string current_line1;
    while(getline(stationsFile, current_line1)) {
        station.name.push_back(current_line1);
    }

    //initialise station_is_Train
    station.isTrain.assign(station.name.size() - 1, false);
    station.isTrain.insert(station.isTrain.begin(), true);

    // initialise the number of stations
    station.passenger_awaiting.resize(station.name.size());

    //initialise the number of cabins in the train
    train.resize(station.name.size());
} 


/* FUNCTION:  ~MetroSim()
 * Purpose:   Recycles used memories
 * Arg:       N/A
 * Returns:   N/A
 * Notes:     Nothing is being allocated on the heap: no destructor needed
 */
MetroSim::~MetroSim()
{

}


/* FUNCTION:  run_command(std::istream &input, std::ofstream &output)
 * Purpose:   main logic for the program: run commands and call 
 *            the corresponding funtions
 * Arg:       istream input file and ofstream output file
 * Returns:   N/A
 * Notes:     Does not check for invalid file reading (only checks in main)
 */
void MetroSim::run_command(std::istream &input, std::ofstream &output)
{
    print_status();
    bool end_of_file = false;
    int passenger_count = 1;
    while(not end_of_file) {
        char cm_1;
        std::cout << "Command? "; //prompt for first command
        input >> cm_1;
        if (cm_1 == 'm') {
            char cm_2;
            input >> cm_2;
            if (cm_2 == 'm') { //move train
                move_train(output);
            } else { //exit program
                std::cout << "Thanks for playing MetroSim. Have a nice day!";
                std::cout << std::endl;
                return;
            }
        } else { //add passengers
            int from, to;
            input >> from;
            input >> to;
            add_passenger(passenger_count, from, to);
            passenger_count++;
        }
        end_of_file = input.eof(); //check for end of file
    }
}


/* FUNCTION:  move_train(std::ofstream &output)
 * Purpose:   Check if passengers need to be dropped off or picked up;
 *            move the train then print status
 * Arg:       ofstream output file
 * Returns:   N/A
 * Notes:     Check for when train is at the last station -> if so,
 *            the train goes back to the first station
 */
void MetroSim::move_train(std::ofstream &output)
{
    //find train
    unsigned long train_at = find_train();

    //check if passengers on the current station need to be picked up
    get_on(train_at);

    //check if passengers on train need to be dropped off on the next stop
    get_off(output);

    //move train
    if (station.isTrain[station.name.size() - 1] == true) {
        circular_staiton();
    } else {
        station.isTrain[train_at] = false;
        station.isTrain[train_at + 1] = true;
    }

    //print status
    print_status();
}


/* FUNCTION:  get_on(int train_position)
 * Purpose:   Checks if passenger needs to be picked up from the current 
 *            station, if so, pick up the passenger and remove them from 
 *            that station
 * Arg:       integer value of the position of the train
 * Returns:   N/A
 * Notes:     N/A
 */
void MetroSim::get_on(int train_position)
{
    while (station.passenger_awaiting[train_position].size() > 0) {
        //place the passenger into the train
        train.at(station.passenger_awaiting[train_position].front().to).
            enqueue(station.passenger_awaiting[train_position].front());
        station.passenger_awaiting[train_position].dequeue();
    }
}


/* FUNCTION:  get_off(std::ofstream &output)
 * Purpose:   Checks if passengers need to be dropped off from the train at
 *            the next station; if so, log the passenger into the output file
 *            and remove the passenger from the train
 * Arg:       ofstream output file
 * Returns:   N/A
 * Notes:     Checks if the next stop is the first station;
 */
void MetroSim::get_off(std::ofstream &output)
{
    //find train
    unsigned long train_at = find_train();
    train_at++;
    //check if the train is at the last station
    if (train_at == station.isTrain.size()) {
        train_at = 0;
    }
    //print and remove passengers
    while (train[train_at].size() > 0) {
        //log the exiting passenger into the given output file
        logged_exiting_passenger(output, train[train_at].front(), 
        station.name[train_at]);
        //remove the passenger from the train
        train[train_at].dequeue();
    };
}


/* FUNCTION:  print_train()
 * Purpose:   Print the train status
 * Arg:       N/A
 * Returns:   N/A
 * Notes:     N/A
 */
void MetroSim::print_train()
{
    std::cout << "Passengers on the train: {";
    for (unsigned long i = 0; i < train.size(); i++) {
        train[i].print(std::cout);
    }
    std::cout << "}" << std::endl;
}


/* FUNCTION:  add_passenger(int id, int from, int to)
 * Purpose:   Add a passenger to the a specific cabinet of the train based
 *            on their destination
 * Arg:       Integer values of id, boarding station, and arrivng station
 * Returns:   N/A
 * Notes:     N/A
 */
void MetroSim::add_passenger(int id, int from, int to)
{
    Passenger p(id, from, to);
    station.passenger_awaiting[from].enqueue(p);

    //print status
    print_status();
}


/* FUNCTION:  print_station()
 * Purpose:   Print the station status
 * Arg:       N/A
 * Returns:   N/A
 * Notes:     Checks where the train is at
 */
void MetroSim::print_station()
{
    for (unsigned long i = 0; i < station.isTrain.size(); i++) {
        if (station.isTrain.at(i) == true) {
            std::cout << "TRAIN: ";
            std::cout << "[" << i << "] " << station.name.at(i);
            std::cout << " {";
            station.passenger_awaiting[i].print(std::cout);
            std::cout << "}" << std::endl;
        } else {
            std::cout << "       [" << i << "] " << station.name.at(i);
            std::cout << " {";
            station.passenger_awaiting[i].print(std::cout);
            std::cout << "}" << std::endl;
        }
    }
}


/* FUNCTION:  find_train()
 * Purpose:   Returns the position of the train
 * Arg:       N/A
 * Returns:   An integer value of the position of the train
 * Notes:     N/A
 */
int MetroSim::find_train()
{
    int train_at = 0;
    for (unsigned long i = 0; i < station.isTrain.size(); i++) {
        if (station.isTrain[i] == true) {
            train_at = i;
        }
    }
    return train_at;
}


/* FUNCTION:  logged_exiting_passenger(std::ostream &output, 
 *            Passenger passenger, std::string station_name)
 * Purpose:   Print the exiting passengers into an output file
 * Arg:       ostream output file, a passenger instance, and a string 
 * Returns:   N/A
 * Notes:     Return but not remove; check for an empty queue
 */
void MetroSim::logged_exiting_passenger(std::ostream &output, 
    Passenger passenger, std::string station_name)
{
    output << "Passenger " << passenger.id << " left train at station ";
    output << station_name << std::endl;
}

/* FUNCTION:  circular_staiton()
 * Purpose:   Move the train to the first station
 * Arg:       N/A
 * Returns:   N/A
 * Notes:     Connectst the last station to the first station
 */
void MetroSim::circular_staiton()
{
    station.isTrain[station.isTrain.size() - 1] = false;
    station.isTrain[0] = true;
}

/* FUNCTION:  print_status()
 * Purpose:   Print the status of the train and stations
 * Arg:       N/A
 * Returns:   N/A
 * Notes:     
 */
void MetroSim::print_status()
{
    print_train();
    print_station();
}



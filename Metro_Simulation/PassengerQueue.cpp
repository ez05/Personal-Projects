/*
 *  PassengerQueue.cpp
 *  Eric Zhao
 *  10 Feb
 *
 *  CS 15 PROJ 1: A Metro Simulator
 *
 *  Implementation for class PassengerQueue
 *
 */

#include "PassengerQueue.h"
#include "Passenger.h"
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>


/* FUNCTION:  front()
 * Purpose:   Returns the first element in the list
 * Arg:       N/A
 * Returns:   A passenger instance
 * Notes:     Return but not remove; check for an empty queue
 */
Passenger PassengerQueue::front()
{
    if (size() == 0) {
        throw std::runtime_error("cannot access an element in an empty queue");
    }
    return queue.front();
}


/* FUNCTION:  dequeue()
 * Purpose:   Remove the first element in the list
 * Arg:       N/A
 * Returns:   N/A
 * Notes:     Check for an empty queue
 */
void PassengerQueue::dequeue()
{
    if (size() == 0) {
        throw std::runtime_error("cannot pop an element in an empty queue");
    }
    queue.erase(queue.begin());
}


/* FUNCTION:  enqueue(const Passenger &passenger)
 * Purpose:   Inserts a new passenger at the end of the queue
 * Arg:       A passenger instance
 * Returns:   N/A
 * Notes:     N/A
 */
void PassengerQueue::enqueue(const Passenger &passenger)
{
    queue.push_back(passenger);
}


/* FUNCTION:  size()
 * Purpose:   Returns the number of elements in the queue
 * Arg:       N/A
 * Returns:   An integer
 * Notes:     N/A
 */
int PassengerQueue::size()
{
    return queue.size();
}

/* FUNCTION:  print(std::ostream &output)
 * Purpose:   Print each Passenger in the PassengerQueue
 * Arg:       An output stream
 * Returns:   N/A
 * Notes:     Print to the given output stream from front to back
 */
void PassengerQueue::print(std::ostream &output)
{
    for (unsigned int i = 0; i < queue.size(); i++) {
        output << "[" << queue.at(i).id << ", " << queue.at(i).from;
        output << "->" << queue.at(i).to << "]";
    }
}
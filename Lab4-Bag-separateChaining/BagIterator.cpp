#include <exception>
#include "BagIterator.h"
#include "Bag.h"

using namespace std;


BagIterator::BagIterator(const Bag& c): bag(c)
{
    currentPosition = 0;
    currentNode = nullptr;
    while (currentPosition < bag.tableSizeM && bag.T[currentPosition] == nullptr) {
        currentPosition++;
    }
    if (currentPosition < bag.tableSizeM) {
        currentNode = bag.T[currentPosition];
    } else {
        currentNode = nullptr;
    }
}

void BagIterator::first() {
    currentPosition = 0;
    currentNode = nullptr;
    while (currentPosition < bag.tableSizeM && bag.T[currentPosition] == nullptr) {
        currentPosition++;
    }
    if (currentPosition < bag.tableSizeM) {
        currentNode = bag.T[currentPosition];
    } else {
        currentNode = nullptr;
    }
}


void BagIterator::next() {
    if (!valid()) {
        throw exception();
    }

    if (currentNode->next != nullptr) {
        currentNode = currentNode->next;
    } else {
        currentPosition++;
        while (currentPosition < bag.tableSizeM && bag.T[currentPosition] == nullptr) {
            currentPosition++;
        }
        if (currentPosition < bag.tableSizeM) {
            currentNode = bag.T[currentPosition];
        } else {
            currentNode = nullptr;
        }
    }
}


bool BagIterator::valid() const {
    return currentPosition < bag.tableSizeM && currentNode != nullptr;
}



TElem BagIterator::getCurrent() const
{
    if (!valid()) {
        throw exception();
    }
    return currentNode->key;
}

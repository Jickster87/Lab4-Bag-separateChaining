#include "Bag.h"
#include "BagIterator.h"
#include <exception>
#include <iostream>
using namespace std;

Bag::Bag() {
    tableSizeM = 310000;
    noElementsN = 0;
    alphaLoadFactor = 0.0f;
    T = new Node*[tableSizeM];
    for (int i = 0; i < tableSizeM; i++) {
        T[i] = nullptr;
    }
}

// Hash function
int Bag::h(TElem key) const {
    return (key % tableSizeM + tableSizeM) % tableSizeM;
}

// Load factor function
void Bag::updateAlpha() {
    alphaLoadFactor = static_cast<float>(noElementsN) / tableSizeM;
}

void Bag::bagResize() {
    int oldTableSize = tableSizeM;
    tableSizeM *= 2;  // Double the table size
    Node** newTable = new Node*[tableSizeM];

    // Initialize new table buckets to nullptr
    for (int i = 0; i < tableSizeM; i++) {
        newTable[i] = nullptr;
    }

    // Rehash all elements from the old table to the new table
    for (int i = 0; i < oldTableSize; i++) {
        Node* current = T[i];
        while (current != nullptr) {
            int newIndex = current->key % tableSizeM; // Recalculate with new table size
            Node* nextNode = current->next;

            // Insert current node into new table at newIndex

            current->next = newTable[newIndex];
            newTable[newIndex] = current;

            current = nextNode;
        }
    }

    // Delete the old table and update the reference
    delete[] T;
    T = newTable;

    // Update the load factor after resizing
    updateAlpha();
  
}

void Bag::add(TElem elem) {
    // Resize if needed
    if (alphaLoadFactor > 0.7) {
        bagResize();
    }

    // Add element
    int pos = h(elem);
    Node* newNode = new Node(elem);
    newNode->next = T[pos];  // Link to existing list if any
    T[pos] = newNode;        // Insert at head

    // Increment count and update load factor
    noElementsN++;
    updateAlpha();

}

bool Bag::remove(TElem elem) {
    int pos = h(elem);
    Node* prevNomad = nullptr;
    Node* nomad = T[pos];

    while (nomad != nullptr && nomad->key != elem) {
        prevNomad = nomad;
        nomad = nomad->next;
    }
    if (nomad == nullptr) {
        return false;  // Element not found
    }
    if (prevNomad == nullptr) {
        T[pos] = nomad->next;  // Removing head node
    } else {
        prevNomad->next = nomad->next;  // Bypass nomad node
    }
    delete nomad;  // Clean up
    
    noElementsN--;
    updateAlpha();
    return true;
}

bool Bag::search(TElem elem) const {
    int pos = h(elem);
    Node* current = T[pos];
    while (current != nullptr) {
        if (current->key == elem) {
            return true;
        }
        current = current->next;
    }
    return false;
}

int Bag::nrOccurrences(TElem elem) const {
    int pos = h(elem);
    int count = 0;
    Node* nomad = T[pos];
    while (nomad != nullptr) {
        if (nomad->key == elem) {
            count++;
        }
        nomad = nomad->next;
    }
    return count;
}

int Bag::size() const {
    return noElementsN;
}

bool Bag::isEmpty() const {
    return noElementsN == 0;
}

BagIterator Bag::iterator() const {
    return BagIterator(*this);
}

Bag::~Bag() {
    for (int i = 0; i < tableSizeM; i++) {
        Node* nomad = T[i];
        while (nomad != nullptr) {
            Node* toDel = nomad;
            nomad = nomad->next;
            delete toDel;
        }

    }
    delete[] T;
}

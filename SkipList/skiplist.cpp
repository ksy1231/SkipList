//
//  skiplist.cpp
//  SkipList
//
//  Created by Soo Yun Kim on 4/10/20.
//  Copyright © 2020 Soo Yun Kim. All rights reserved.
//

#include "skiplist.hpp"

// print each level's data
ostream &operator<<(ostream &os, const SkipList &list) {
    for (int i = list.depth - 1; i >= 0; i--) {
        SkipList::SNode* curr = list.frontGuards[i];
        os << "Level: " << i << " -- " << curr->data << ", ";
        while (curr->next != nullptr) {
            curr = curr->next;
            os << curr->data << ", ";
        }
        os << endl;
    }
    return os;
}

// set dynamic arrays based on the depth as INT_MIN and INT_MAX
// tie all the SNode objects together (both prev-next and up-down)
SkipList::SkipList(int depth) : depth{depth} {
    frontGuards = new SNode*[depth];
    rearGuards = new SNode*[depth];
    
    for (int i = 0; i < depth; i++) {
        frontGuards[i] = new SNode(INT_MIN);
        rearGuards[i] = new SNode(INT_MAX);
        
        frontGuards[i]->next = rearGuards[i];
        rearGuards[i]->prev = frontGuards[i];
    }
    
    for (int i = 1; i < depth; i++) {
        frontGuards[i - 1]->upLevel = frontGuards[i];
        rearGuards[i - 1]->upLevel = rearGuards[i];
        
        frontGuards[i]->downLevel = frontGuards[i - 1];
        rearGuards[i]->downLevel = rearGuards[i - 1];
    }
}

// delete all data and arrays
SkipList::~SkipList() {
    for (int i = 0; i < depth; i++) {
        SNode* curr = frontGuards[i];
        while (curr != nullptr) {
            SNode* temp = curr->next;
            delete curr;
            curr = temp;
        }
    }
    delete[] frontGuards;
    delete[] rearGuards;
}

// add and build data and connect each other
bool SkipList::Add(int data) {
    // Set SNode*, such nextNode to be frontGuards[0]->next
    SNode* nextNode = frontGuards[0]->next;
    // As long as nextNode->next is not null and nextNode->data < data , keep
    // moving nextNode to the right
    while (nextNode->next != nullptr && nextNode->data < data) {
        nextNode = nextNode->next;
    }
    // If nextNode->data == data , return false stating duplicates are not allowed
    if (nextNode->data == data) {
        return false;
    } else {
        // If not, create a new SNode called newNode , put the data in it and call
        // addBefore( newNode , nextNode); to connect all the pointers together.
        SNode* newNode = new SNode(data);
        addBefore(newNode, nextNode);
        if (alsoHigher()) {
            for (int i = 1; i < depth; i++) {
                // Create another node to be inserted at the higher level
                SNode* newUpper = new SNode(data);

                // Connect newNode and newUpper (up-down)
                newNode->upLevel = newUpper;
                newUpper->downLevel = newNode;

                // Starting from newNode that was inserted, keep going back towards INT_MIN
                // until you find a node that has a valid upLevel link
                while (newNode->data != INT_MIN) {
                    newNode = newNode->prev;
                    if (newNode->upLevel != nullptr) {
                        break;
                    }
                }
                // Go up one level
                newNode = newNode->upLevel;
                // Go right towards INT_MAX
                while (newNode->data != INT_MAX) {
                    newNode = newNode->next;
                    if (newNode != nullptr) {
                        break;
                    }
                }
                // Call addBefore to insert newUpper to come before the node you just got to
                addBefore(newUpper, newNode);
                // Set newNode to be newUpper
                newNode = newUpper;
                // Toss a coin to decide if you need to insert it at a higher level again
                if (!alsoHigher()) {
                    break;
                }
            }
        }
    }
    return true;
}

// remove data bottom to up
bool SkipList::Remove(int data) {
    if (Contains(data)) {
        for (int i = 0; i < depth; i++) {
            SNode* curr = frontGuards[i];
            while (curr->next != nullptr) {
                curr = curr->next;
                if (curr->data == data) {
                    curr->prev->next = curr->next;
                    curr->next->prev = curr->prev;
                    delete curr;
                }
            }
        }
        return true;
    }
    return false;
}

// search whether the data exists or not
// do not want to search at level-0 that will be O(n)
bool SkipList::Contains(int data) {
    // Start at top-left, highest level frontGuards[depth-1]
    SNode* current = frontGuards[depth - 1];
    // As long as current is not nullptr
    while (current != nullptr) {
        // Keep moving right as long as the current->next->data is less than value
        while (current->next->data < data) {
            current = current->next;
        }
        // If current->next->data is equal to value, return true
        if (current->next->data == data) {
            return true;
        } else {
            // else, set current to be the node down one level from current node
            current = current->downLevel;
        }
    }
    // If current is nullptr, the item is not in SkipList
    return false;
}

// initialize data and pointers
SkipList::SNode::SNode(int data) : data{data} {
    next = nullptr;
    prev = nullptr;
    upLevel = nullptr;
    downLevel = nullptr;
}

// connect previous pointer, newNode pointer, and nextNode pointer
void SkipList::addBefore(SNode *newNode, SNode *nextNode) {
    assert(newNode != nullptr && nextNode != nullptr &&
           newNode->data < nextNode->data);
    
    nextNode->prev->next = newNode;
    newNode->next = nextNode;
    newNode->prev = nextNode->prev;
    nextNode->prev = newNode;

    // after nodes have been linked
    assert(newNode->next == nextNode &&
           nextNode->prev == newNode);
    assert(newNode->prev != nullptr &&
           newNode->prev->data < newNode->data);
}

// check based on a coin toss
bool SkipList::alsoHigher() const {
    return rand() % 2 == 1;
}

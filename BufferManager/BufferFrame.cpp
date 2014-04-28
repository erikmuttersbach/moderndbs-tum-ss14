//
//  BufferFrame.cpp
//  BufferManager
//
//  Created by Erik Muttersbach on 28/04/14.
//
//

#include "BufferFrame.h"

#include <string.h>
#include <errno.h>
#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;

BufferFrame::BufferFrame() : pageId(0), dirty(false) {
    cout << "Crearting empty buffer frame" << endl;
}

BufferFrame::BufferFrame(uint64_t pageId) : pageId(pageId), dirty(false) {
    // TODO mmap data from file
    int r = pthread_rwlock_init(&this->lock, NULL);
    if(r != 0) {
        throw runtime_error("Could not initialize rwlock (" + to_string(r) + "): " + strerror(r));
    }
    
    cout << "Created " << pageId << endl;
}

BufferFrame::~BufferFrame() {
    cout << "destroyed " << pageId << endl;
    
    int r = pthread_rwlock_destroy(&this->lock);
    if(r != 0) {
        throw runtime_error("Could not destroy rwlock (" + to_string(r) + "): " + strerror(r));
    }
}

void BufferFrame::write() {
    // TODO implement
}


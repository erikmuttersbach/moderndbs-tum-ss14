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
    if(pthread_rwlock_init(&this->lock, NULL) != 0) {
        throw runtime_error("Could not initialize rwlock (" + to_string(errno) + "): " + strerror(errno));
    }
    
    cout << "Created lock " << pageId << endl;
}

BufferFrame::~BufferFrame() {
    cout << "Unlocking " << pageId << endl;
    
    if(pthread_rwlock_destroy(&this->lock) != 0) {
        throw runtime_error("Could not destroy rwlock (" + to_string(errno) + "): " + strerror(errno));
    }
}


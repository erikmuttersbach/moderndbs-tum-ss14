//
//  BufferManager.cpp
//  BufferManager
//
//  Created by Erik Muttersbach on 28/04/14.
//
//

#include "BufferManager.h"

using namespace std;

BufferManager::BufferManager(unsigned int size) : size(size) {
    
}

BufferFrame& BufferManager::fixPage(uint64_t pageId, bool exclusive) {
    
    // if the frame is not loaded, load it if possible
    if(this->frames.find(pageId) == this->frames.end()) {
        // TODO check if we can load a page, maybe by swapping out
        // an unused one
        this->frames.insert(make_pair(pageId, new BufferFrame(pageId)));
    }
    
    BufferFrame *frame = this->frames[pageId];
    
    // lock the frame shared or exclusively in a blocking manner
    if(exclusive) {
        if(pthread_rwlock_wrlock(&frame->lock) != 0) {
            throw runtime_error("Could not write lock (" + to_string(errno) + "): " + strerror(errno));
        }
    } else {
        if(pthread_rwlock_rdlock(&frame->lock) != 0) {
            throw runtime_error("Could not read lock (" + to_string(errno) + "): " + strerror(errno));
        }
    }
    
    return *this->frames[pageId];
}

void BufferManager::unfixPage(BufferFrame& frame, bool isDirty) {
    frame.dirty = isDirty;
    
    if(pthread_rwlock_unlock(&frame.lock) != 0) {
        throw runtime_error("Could not unlock (" + to_string(errno) + "): " + strerror(errno));
    }
}

BufferManager::~BufferManager() {
    // TODO write all dirty pages back
}

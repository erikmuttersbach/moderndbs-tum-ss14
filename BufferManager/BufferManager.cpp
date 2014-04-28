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
        int r = pthread_rwlock_wrlock(&frame->lock);
        if(r != 0) {
            throw runtime_error("Could not write lock (" + to_string(r) + "): " + strerror(r));
        }
    } else {
        int r = pthread_rwlock_rdlock(&frame->lock);
        if(r != 0) {
            throw runtime_error("Could not read lock (" + to_string(r) + "): " + strerror(r));
        }
    }
    
    cout << "Locked " << pageId << endl;
    
    return *this->frames[pageId];
}

void BufferManager::unfixPage(BufferFrame& frame, bool isDirty) {
    frame.dirty = isDirty;
    
    int r = pthread_rwlock_unlock(&frame.lock);
    if(r != 0) {
        throw runtime_error("Could not unlock (" + to_string(r) + "): " + strerror(r));
    }
    
    cout << "Unlocked " << frame.pageId << endl;
}

BufferManager::~BufferManager() {
    for(auto it = this->frames.begin(); it != this->frames.end(); it++) {
        if(it->second->dirty) {
            cout << "Writing " << it->second->pageId << endl;
            it->second->write();
        }
    }
}

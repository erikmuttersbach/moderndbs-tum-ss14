//
//  BufferManager.cpp
//  BufferManager
//
//  Created by Erik Muttersbach on 28/04/14.
//
//

#include "BufferManager.h"

#include <unistd.h>
#include <assert.h>

#include <mutex>

using namespace std;

// TOOD Remove
std::string thread_name() {
    char name[256];
    pthread_getname_np(pthread_self(), name, 256);
    return std::string(name);
}

//unsigned int BufferManager::frameSize = getpagesize();
size_t BufferManager::frameSize = 4096;

BufferManager::BufferManager(unsigned int size) : size(size) {
    
}

BufferFrame *BufferManager::fixPage(uint64_t pageId, bool exclusive) {
    this->framesLock.lock();
    
    // if the frame is not loaded, load it if possible
    auto end = this->frames.end();
    if(this->frames.find(pageId) == end) {
        // if the maximum amount of frames is stored, try to free one
        if(this->frames.size() == this->size) {
            for(auto it=this->frames.begin(); it!=this->frames.end(); it++) {
                int r = pthread_rwlock_trywrlock(&it->second->lock);
                if(r == EBUSY) {
                    // the frame is already locked
                    continue;
                } else if(r == 0) {
                    // the frame was locked by us, write it if dirty and
                    // free the space
                    if(it->second->dirty || true) {
                        it->second->write();
                    }
                    
                    this->frames.erase(it);
                    
                    break;
                } else {
                    throw runtime_error("Could not try lock (" + to_string(r) + "): " + strerror(r));
                }
            }
            
            // if there was no page we could free, throw an
            // error
            if(this->frames.size() == this->size) {
                throw std::runtime_error("Could not free a frame");
            }
        }
        
        BufferFrame *frame = new BufferFrame(pageId);
        this->frames.insert(make_pair(pageId, frame));
    }
    
    this->framesLock.unlock();
    
    
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
    
    assert(this->frames.size() <= this->size);
    
    return this->frames[pageId];
}

void BufferManager::unfixPage(BufferFrame *frame, bool isDirty) {
    frame->dirty = isDirty;
    
    int r = pthread_rwlock_unlock(&frame->lock);
    if(r != 0) {
        throw runtime_error("Could not unlock (" + to_string(r) + "): " + strerror(r));
    }
    
}

void BufferManager::writeAll() {
    for(auto it = this->frames.begin(); it != this->frames.end(); ++it) {
        BufferFrame *frame = (*it).second;
        if(frame->dirty) {
            it->second->write();
        }
    }
}

BufferManager::~BufferManager() {
    for(auto it = this->frames.begin(); it != this->frames.end(); ++it) {
        BufferFrame *frame = (*it).second;
        if(frame->dirty) {
            it->second->write();
        }
        delete frame;
    }
}

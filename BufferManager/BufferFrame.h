//
//  BufferFrame.h
//  BufferManager
//
//  Created by Erik Muttersbach on 28/04/14.
//
//

#ifndef __BufferManager__BufferFrame__
#define __BufferManager__BufferFrame__

#include <stdint.h>
#include <pthread.h>

class BufferManager;

class BufferFrame {
friend BufferManager;
    
private:
    pthread_rwlock_t lock;
    
public:
    uint64_t pageId;
    bool dirty;
    
    BufferFrame();
    BufferFrame(uint64_t pageId);
    ~BufferFrame();
    
    void *getData();
    
    
};

#endif

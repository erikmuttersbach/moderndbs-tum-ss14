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

#include <streambuf>

class BufferManager;

class BufferFrame {
friend class BufferManager;
    
private:
    bool dirty;
    void *data;
    
    pthread_rwlock_t lock;
    
public:
    uint64_t pageId;
    
    BufferFrame();
    BufferFrame(uint64_t pageId);
    ~BufferFrame();
    
    void *getData();
    void write();
    
};

#endif

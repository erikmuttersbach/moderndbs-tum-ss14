//
//  BufferManager.h
//  BufferManager
//
//  Created by Erik Muttersbach on 28/04/14.
//
//

#ifndef __BufferManager__BufferManager__
#define __BufferManager__BufferManager__

#include <iostream>
#include <unordered_map>
#include <list>
#include <mutex>
#include <string>

#include "BufferFrame.h"

// Creates a page 64bit id from a segment and page number
#define PAGEID(segment, page)   (((uint64_t)segment << 32) + (page & 0x00000000FFFFFFFF))

// Returns the 32 bit segment number component of a page id
#define PAGEID_SEGMENT(pageId)  ((uint32_t)(pageId >> 32))

// Returns the 32 bit page number component of a page id
#define PAGEID_PAGE(pageId)     (pageId & 0x00000000FFFFFFFF)

// TOODO Remove
std::string thread_name();

class BufferManager {
public:
    unsigned int size;
    static size_t frameSize;
    
    BufferManager(unsigned int size);
    
    BufferFrame* fixPage(uint64_t pageId, bool exclusive);
    void unfixPage(BufferFrame* frame, bool isDirty);
    
    ~BufferManager();
    
private:
    std::mutex framesLock;
    std::unordered_map<uint64_t, BufferFrame*> frames;
};

#endif

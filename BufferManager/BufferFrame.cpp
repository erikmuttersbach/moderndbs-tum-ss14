//
//  BufferFrame.cpp
//  BufferManager
//
//  Created by Erik Muttersbach on 28/04/14.
//
//

#include "BufferFrame.h"

#include "BufferManager.h"

#include <string.h>
#include <errno.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>

using namespace std;

BufferFrame::BufferFrame() : pageId(0), dirty(false) {
    cout << "Crearing" << endl;
}

BufferFrame::BufferFrame(uint64_t pageId) : pageId(pageId), dirty(false) {
    // create the lock for this page
    int r = pthread_rwlock_init(&this->lock, NULL);
    if(r != 0) {
        throw runtime_error("Could not initialize rwlock (" + to_string(r) + "): " + strerror(r));
    }
    
    // load the data into memory
    this->data = malloc(BufferManager::frameSize);
    if(this->data == 0) {
        throw runtime_error("Could not malloc (" + to_string(errno) + "): " + strerror(errno));
    }
    
    string path = "/tmp/"+to_string(PAGEID_SEGMENT(pageId));
    fstream file(path, ios::in|ios::binary);
    if(file.bad()) {
        throw runtime_error("Could not open file (" + to_string(errno) + "): " + strerror(errno));
    }
    
    file.seekg(BufferManager::frameSize*PAGEID_PAGE(pageId));
    if(file.bad()) {
        throw runtime_error("Could not seek file (" + to_string(errno) + "): " + strerror(errno));
    }
    
    file.read(static_cast<char*>(this->data), BufferManager::frameSize);
    if(file.bad()) {
        throw runtime_error("Could not read file (" + to_string(errno) + "): " + strerror(errno));
    }
    
    file.close();
}

BufferFrame::~BufferFrame() {
    free(this->data);
    
    cout << "Destructing frame " << to_string(this->pageId) << endl;
    
    int r = pthread_rwlock_destroy(&this->lock);
    if(r != 0) {
        throw runtime_error("Could not destroy rwlock (" + to_string(r) + "): " + strerror(r));
    }
}

void BufferFrame::write() {
    string path = "/tmp/"+to_string(PAGEID_SEGMENT(this->pageId));
    
    int f = open(path.c_str(), O_WRONLY | O_CREAT, 0700);
    if(f < 0) {
        throw runtime_error("Could not open file (" + to_string(errno) + "): " + strerror(errno));
    }
    
    off_t offset = BufferManager::frameSize*PAGEID_PAGE(this->pageId);
    int r = pwrite(f, this->data, BufferManager::frameSize, offset);
    if(r < 0) {
        throw runtime_error("Could not pwrite file (" + to_string(errno) + "): " + strerror(errno));
    }
    
    close(f);
    
    this->dirty = false;
}

void *BufferFrame::getData() {
    return this->data;
}


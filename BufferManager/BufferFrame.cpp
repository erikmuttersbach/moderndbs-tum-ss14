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
    
    cout << (thread_name()  + ": Created " + to_string(pageId) + " ->data: " + to_string(static_cast<unsigned int*>(this->data)[0])) << endl;
}

BufferFrame::~BufferFrame() {
    cout << (thread_name() + ": destroyed " + to_string(pageId)) << endl;
    
    free(this->data);
    
    int r = pthread_rwlock_destroy(&this->lock);
    if(r != 0) {
        throw runtime_error("Could not destroy rwlock (" + to_string(r) + "): " + strerror(r));
    }
}

void BufferFrame::write() {
    string path = "/tmp/"+to_string(PAGEID_SEGMENT(this->pageId));
    
    fstream file(path, ios::binary | ios::out | ios::in );
    if(file.bad()) {
        throw runtime_error("Could not open file (" + to_string(errno) + "): " + strerror(errno));
    }
    
    int a = BufferManager::frameSize*PAGEID_PAGE(this->pageId);
    file.seekp(BufferManager::frameSize*PAGEID_PAGE(this->pageId), ios_base::beg);
    if(file.bad() || file.fail()) {
        throw runtime_error("Could not seek file (" + to_string(errno) + "): " + strerror(errno));
    }
    
    int s = file.tellp();
    
    file.write(static_cast<char*>(this->data), BufferManager::frameSize);
    if(file.bad() || file.fail()) {
        throw runtime_error("Could not write file (" + to_string(errno) + "): " + strerror(errno));
    }
    
    cout << (thread_name() + ": Wrote " + to_string(PAGEID_PAGE(this->pageId)) + ": " + to_string((static_cast<uint32_t*>(this->data))[0])) << endl;
    
    file.close();
    
    this->dirty = false;
}

void *BufferFrame::getData() {
    return this->data;
}


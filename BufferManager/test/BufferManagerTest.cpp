//
//  File.cpp
//  BufferManager
//
//  Created by Erik Muttersbach on 05/05/14.
//
//

#include <gtest/gtest.h>

#include "BufferManager.h"

using namespace std;

/*
TEST(BufferManagerTest, TestWriteAndLoad3Pages) {
    BufferManager *bufferManager = new BufferManager(1);
    
    BufferFrame *frame = bufferManager->fixPage(PAGEID(1, 0), true);
    static_cast<int*>(frame->getData())[0] = 42;
    bufferManager->unfixPage(frame, true);
    
    frame = bufferManager->fixPage(PAGEID(1, 1), true);
    static_cast<int*>(frame->getData())[0] = 43;
    bufferManager->unfixPage(frame, true);
    
    frame = bufferManager->fixPage(PAGEID(1, 3), true);
    static_cast<int*>(frame->getData())[0] = 45;
    bufferManager->unfixPage(frame, true);
    
    delete bufferManager;
    
    bufferManager = new BufferManager(2);
    
    frame = bufferManager->fixPage(PAGEID(1, 0), false);
    ASSERT_EQ(static_cast<int*>(frame->getData())[0], 42);
    bufferManager->unfixPage(frame, false);
    
    frame = bufferManager->fixPage(PAGEID(1, 1), false);
    ASSERT_EQ(static_cast<int*>(frame->getData())[0], 43);
    bufferManager->unfixPage(frame, false);
    
    frame = bufferManager->fixPage(PAGEID(1, 3), false);
    ASSERT_EQ(static_cast<int*>(frame->getData())[0], 45);
    bufferManager->unfixPage(frame, false);
    
    delete bufferManager;
}

TEST(BufferManagerTest, TestSchemaCodePathFail) {
    BufferManager bufferManager(5);
    
    BufferFrame *frame = bufferManager.fixPage(PAGEID(0, 1), false);
    bufferManager.unfixPage(frame, false);
    
    frame = bufferManager.fixPage(PAGEID(0, 1), true);
    void *data = malloc(BufferManager::frameSize);
    memset(data, 'A', BufferManager::frameSize);
    memcpy(frame->getData(), data, BufferManager::frameSize);
    bufferManager.unfixPage(frame, true);
    
    frame = bufferManager.fixPage(PAGEID(0, 1), false);
    ASSERT_EQ(static_cast<char*>(frame->getData())[0], 'A');
    ASSERT_EQ(static_cast<char*>(frame->getData())[BufferManager::frameSize-1], 'A');
    bufferManager.unfixPage(frame, false);
}*/

//
//  Schema.cpp
//  BufferManager
//
//  Created by Erik Muttersbach on 05/05/14.
//
//

#include "Schema.h"

#include <sstream>
#include <string>
#include <fstream>

using namespace std;

// TODO bug: pageId 0 cannot be used

Schema::Schema(BufferManager *bufferManager) : bufferManager(bufferManager) {
    this->deserialize();
}

void Schema::deserialize() {
    BufferFrame *frame = bufferManager->fixPage(PAGEID(0, 1), false);
    
    std::string str(static_cast<char*>(frame->getData()));
    if(str.length() > 0) {
    
    stringstream stream(str);
    boost::archive::text_iarchive archive(stream);
    
    this->relations.clear();
    
/*    uint32_t relationCount = 0;
    stream >> relationCount;
    
    for(uint32_t i=0; i<relationCount; i++) {
        Relation relation;
        stream >> relation.name;
        stream >> relation.segmentId;
        stream >> relation.pageCount;
        this->relations.push_back(relation);
    }*/
    
    archive >> this->relations;
        
    }
    
    
    bufferManager->unfixPage(frame, false);
}

void Schema::serialize() {
    BufferFrame *frame = bufferManager->fixPage(PAGEID(0, 1), true);
    
    std::stringstream stream;
    boost::archive::text_oarchive archive(stream);
    
    archive << this->relations;
/*    for (auto it = this->relations.begin(); it != this->relations.end(); it++) {
        stream << ((*it).name);
        stream << (*it).segmentId;
        stream << (*it).pageCount;
        stream << (*it).columns.size();
        for (auto it2 = (*it).columns.begin(); it2 != (*it).columns.end(); it2++) {
            stream << (*it2).first;
            stream << (*it2).second;
        }
    }
*/

    std::string str = stream.str();
    size_t len = strlen(str.c_str());
    if(len > BufferManager::frameSize) {
        throw std::runtime_error("Schema too large");
    }
    
    //memcpy(frame->getData(), str.c_str(), len);
    strcpy(static_cast<char*>(frame->getData()), str.c_str());
    
    bufferManager->unfixPage(frame, true);
}


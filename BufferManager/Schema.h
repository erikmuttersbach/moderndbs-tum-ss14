//
//  Schema.h
//  BufferManager
//
//  Created by Erik Muttersbach on 05/05/14.
//
//

#ifndef __BufferManager__Schema__
#define __BufferManager__Schema__

#include <vector>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "Relation.h"
#include "BufferManager.h"

class Schema {
private:
    BufferManager *bufferManager;
    
public:
    std::vector<Relation> relations;
    
    Schema(BufferManager *bufferManager);
    
    /**
     * Saves the schema to disk (segment 0).
     */
    void serialize();
    
private:
    void deserialize();
};

#endif /* defined(__BufferManager__Schema__) */

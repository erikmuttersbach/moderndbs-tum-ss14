//
//  Relation.cpp
//  BufferManager
//
//  Created by Erik Muttersbach on 05/05/14.
//
//

#include "Relation.h"

Relation::Relation() {
    
}

Relation::Relation(std::string name, unsigned int segmentId) : name(name), segmentId(segmentId), pageCount(1) {
    
}

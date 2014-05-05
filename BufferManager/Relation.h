//
//  Relation.h
//  BufferManager
//
//  Created by Erik Muttersbach on 05/05/14.
//
//

#ifndef __BufferManager__Relation__
#define __BufferManager__Relation__

#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <string>
#include <vector>

#include "Attribute.h"

class Relation {
public:
    Relation();
    Relation(std::string name, unsigned int segmentId);
    
    std::string name;
    unsigned int segmentId;
    size_t pageCount;
    std::vector<Attribute> attributes;
    
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & segmentId;
        ar & attributes;
    }
};

#endif

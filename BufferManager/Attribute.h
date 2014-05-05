//
//  Attributes.h
//  BufferManager
//
//  Created by Erik Muttersbach on 05/05/14.
//
//

#ifndef __BufferManager__Attribute__
#define __BufferManager__Attribute__

#include <boost/serialization/access.hpp>

#include <string>

typedef enum {
    string,
    uint32,
    int32
} AttributeType;

class Attribute {
public:
    Attribute();
    Attribute(std::string name, AttributeType type);
    
    std::string name;
    AttributeType type;
    
    // boost serialization support
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & type;
    }
};

#endif

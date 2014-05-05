//
//  SchemaTest.cpp
//  BufferManager
//
//  Created by Erik Muttersbach on 05/05/14.
//
//

#include <gtest/gtest.h>

#include "Schema.h"

using namespace std;

class SchemaTest : public ::testing::Test {
public:
    virtual void SetUp() {
        for(unsigned int i=0; i<10; i++) {
            remove(std::string("/tmp/"+to_string(i)).c_str());
        }
    }
    
    void TearDown() {

    }

};

TEST_F(SchemaTest, TestSerialization) {
    BufferManager bufferManager(5);
    Schema schema(&bufferManager);
    
    Relation relation1("relation1", 1);
    schema.relations.push_back(relation1);
    
    Relation relation2("relation2", 2);
    relation2.attributes.push_back(Attribute("foo", AttributeType::string));
    relation2.attributes.push_back(Attribute("bar", AttributeType::uint32));
    relation2.attributes.push_back(Attribute("abc", AttributeType::int32));
    schema.relations.push_back(relation2);
    
    schema.serialize();
    bufferManager.writeAll();
    
    // reload the schema
    Schema schema2(&bufferManager);
    ASSERT_EQ(schema2.relations.size(), 2);
    
    ASSERT_EQ(std::string("relation1"), schema2.relations[0].name);
    ASSERT_EQ(1, schema2.relations[0].segmentId);
    
    ASSERT_EQ(std::string("relation2"), schema2.relations[1].name);
    ASSERT_EQ(2, schema2.relations[1].segmentId);
    ASSERT_EQ(std::string("foo"), schema2.relations[1].attributes[0].name);
    ASSERT_EQ(AttributeType::string, schema2.relations[1].attributes[0].type);
    ASSERT_EQ(std::string("bar"), schema2.relations[1].attributes[1].name);
    ASSERT_EQ(AttributeType::uint32, schema2.relations[1].attributes[1].type);
    ASSERT_EQ(std::string("abc"), schema2.relations[1].attributes[2].name);
    ASSERT_EQ(AttributeType::int32, schema2.relations[1].attributes[2].type);
}

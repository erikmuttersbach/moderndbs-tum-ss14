//
//  main.cpp
//  BufferManager
//
//  Created by Erik Muttersbach on 28/04/14.
//
//

#include <iostream>

#include "BufferManager.h"

using namespace std;



int main(int argc, const char * argv[])
{
    BufferManager mgr(2);
    
    BufferFrame& frame1 = mgr.fixPage(PAGEID(1, 2), false);
    BufferFrame& frame2 = mgr.fixPage(PAGEID(1, 3), false);
    
    cout << "Fixed the pages" << endl;
    
    mgr.unfixPage(frame1, false);
    mgr.unfixPage(frame2, false);
    
    return 0;
}


//
//  List.cpp
//  List
//
//  Created by Nikita-Mac on 06.03.17.
//  Copyright © 2017 Nikita-Mac. All rights reserved.
//

#include "List.h"


List::List(int _element_size, int _element_count) {
    element_size = _element_size;
    element_count = _element_count;
    first_index = 0;
    last_index = -1;
    _error = false;
    last = NULL;
    first =NULL;
}

List::~List(){
    Segment* cur = last;
    while (last)
    {
        last = cur->prev;
        delete_segment(cur);
        cur = last;
    }
    last = NULL;
    first = NULL;
}

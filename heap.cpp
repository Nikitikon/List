//
//  Heap.cpp
//  List
//
//  Created by Nikita-Mac on 06.03.17.
//  Copyright © 2017 Nikita-Mac. All rights reserved.
//

#include "heap.h"
#include <cstdlib>



void* Heap::get_mem(int size) // This function allocates memory in existing segment
{
    if (size > segment_size) // If the free memory is less than required, then we create a new segment
    {
        make_segment();
    }
    else
        if (current == NULL) // If the segment does not exist, then a new segment is created
        {
            make_segment();
        }
    
    Segment* workSegment = current;
    
    int bestSize = 0;
    
    while (workSegment) // While there is a segment
    {
        bestSize = findSizeInSegment(workSegment, size); // Looking for a suitable free space in the segment
        if (bestSize != 0)
        {
            break;
        }
        
        workSegment = workSegment->prev;	// If we do not have the required memory in this segment, then go to the previous one
    }
    
    if (bestSize == 0)
    {
        make_segment();
        current->descriptor[1].offset = (int*)((size_t)current->descriptor[0].offset + size);
        current->descriptor[1].size = current->descriptor[0].size - size;
        current->descriptor[1].used = false;
        current->descriptor[0].used = true;
        current->descriptor[0].size = size;
        current->descriptor_count++;
        if (current->descriptor[0].size == size)
        {
            segment_size -= size;
            return current->data;
        }
    }
    
    for (int i = 0; i < workSegment->descriptor_count; i++)
    {
        if (bestSize == workSegment->descriptor[i].size && !workSegment->descriptor[i].used)
        {
            if (i == workSegment -> descriptor_count - 1) // If the desired subsegment is the last one
            {
                // We divide the last segment into two parts
                // One of the sub - segments occupies the remaining memory in the segment
                workSegment->descriptor[i + 1].offset = (int*)((size_t)workSegment->descriptor[i].offset + size);
                workSegment->descriptor[i + 1].size = workSegment->descriptor[i].size - size;
                workSegment->descriptor[i + 1].used = false;
                // The second segment is allocated to the needs of the user
                workSegment->descriptor[i].size = size;
                workSegment->descriptor[i].used = true;
                workSegment->descriptor_count++;
                
                // Return the data pointer
                segment_size -= size;
                return (void*)((size_t)workSegment->data + (size_t)workSegment->descriptor[i].offset);
            }
            else // If the free memory is not found at the end of the segment
            {
                if (workSegment->descriptor_count < SEGMENTCOUNT && workSegment->descriptor[i].size != size)
                {
                    for (int j = workSegment->descriptor_count - 1; j > i; j--)
                    {
                        workSegment->descriptor[j + 1] = workSegment->descriptor[j];
                    }
                    workSegment->descriptor[i].size = size;
                    workSegment->descriptor[i].used = true;
                    workSegment->descriptor_count++;
                    workSegment->descriptor[i + 1].offset = (int*)((size_t)workSegment->descriptor[i].offset + size);
                    workSegment->descriptor[i + 1].size = workSegment->descriptor[i].size - size;
                    workSegment->descriptor[i + 1].used = false;
                }
                workSegment->descriptor[i].used = true;
                // Return the data pointer
                segment_size -= size;
                return (void*)((size_t)workSegment->data + (size_t)workSegment->descriptor[i].offset);
            }
        }
    }
    
    return NULL;
}

void Heap::free_mem(void* p)
{
    Segment* workSegment = current;
    
    while (workSegment)	// While there is a segment
    {
        for (int i = 0; i <= workSegment->descriptor_count - 1; i++)
        {
            if (p == (void*)((size_t)workSegment->descriptor[i].offset + (size_t)workSegment->data)) // If you find the handle you want to remove
            {
                workSegment->descriptor[i].used = false;
                segment_size += workSegment->descriptor[i].size;
            }
        }
        defrag(workSegment);
        workSegment = workSegment->prev;	// go to the previous one segment
    }
    
    return;
}

int Heap::findSizeInSegment(Segment *temp, int size)
{
    int min = 0;
    for (int i = 0; i <= temp->descriptor_count - 1; i++)
    {
        if ((temp->descriptor[i].size >= size) && !(temp->descriptor[i].used))
        {
            if (!min)
                min = temp->descriptor[i].size;
            if (min > temp->descriptor[i].size)
                min = temp->descriptor[i].size;
        }
    }
    return min;
}

void Heap::defrag(Segment *defragSegment)
{
    for (int i = 0; i < defragSegment->descriptor_count - 1; i++)
    {
        if (!defragSegment->descriptor[i].used && !defragSegment->descriptor[i + 1].used) // Looking for two neighboring elements that are not used
        {
            defragSegment->descriptor[i].size += defragSegment->descriptor[i + 1].size; // We combine their sizes
            
            for (int j = i + 1; j < defragSegment->descriptor_count - 1; j++)
            {
                defragSegment->descriptor[j] = defragSegment->descriptor[j + 1];
            }
            
            defragSegment->descriptor[defragSegment->descriptor_count - 1] = { 0 }; // Delete last Segment_def
            
            defragSegment->descriptor_count--;
        }
        
    }
}

int Heap::make_segment()
{
    Segment* newSegment = new Segment();
    if (current == NULL)
    {
        newSegment->prev = NULL;
        current = newSegment;
    }
    else
    {
        newSegment->prev = current;
        current = newSegment;
    }
    current->data = malloc(SEGMENTSIZE);	
    if (current->data == NULL)
    {
        current = current->prev;
        return -2;
    }
    
    current->descriptor_count = 1;
    current->descriptor[0].size = SEGMENTSIZE;
    current->descriptor[0].offset = 0;
    segment_size += SEGMENTSIZE;
    return 0;
}

void Heap::delete_segments()
{
    Segment* next;
    
    while (current)	
    {
        free(current->descriptor);
        free(current->data);
        next = current->prev;
        delete current;
        current = next;
    }
}


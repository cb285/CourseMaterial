
// FILE: dnode.cpp
// IMPLEMENTS: The functions of the doubly-linked list toolkit 
//   (see dnode2.h for documentation).
// INVARIANT for the dnode struct:
//   The data of a node is stored in the data field,
//   and the links in link fields.

#include "dnode2.h"
#include <cstdlib>    // Provides size_t
#include <iostream>   // Provides ostream, endl
using namespace std;

size_t dlist_length(const dnode* head_ptr)
// Library facilities used: cstdlib
{
   const dnode *cursor;
   size_t count;

   count = 0;
   for (cursor = head_ptr->next_link; 
        cursor != head_ptr; 
        cursor = cursor->next_link)
      ++count;
        
   return count;
}
    
void dlist_write(const dnode* head_ptr, ostream & out)
{
   const dnode *cursor;

   for (cursor = head_ptr->next_link; 
        cursor != head_ptr; 
        cursor = cursor->next_link)
      out << cursor->data << endl;
}

void dlist_insert(dnode* previous_ptr, const dnode::value_type& entry) 
{
   dnode *next_ptr = previous_ptr->next_link;
   dnode *insert_ptr = new dnode (entry, previous_ptr, next_ptr);
   previous_ptr->next_link = insert_ptr;
   next_ptr->prev_link = insert_ptr;
}

void dlist_remove(dnode* remove_ptr)
{
   // could test that remove_ptr doesn't point to itself...
   dnode *prev_ptr = remove_ptr->prev_link;
   dnode *next_ptr = remove_ptr->next_link;
   prev_ptr->next_link = next_ptr;
   next_ptr->prev_link = prev_ptr;
   delete remove_ptr;
}

void dlist_clear(dnode* head_ptr)
{
   dnode *cursor, *remove_ptr;

   cursor = head_ptr->next_link; 
   while (cursor != head_ptr)
   {
      remove_ptr = cursor;
      cursor = cursor->next_link;
      delete remove_ptr;
   }
   head_ptr->prev_link = head_ptr;
   head_ptr->next_link = head_ptr;
}

void dlist_copy(const dnode* source_ptr, dnode*& head_ptr) 
{
   head_ptr = new dnode;  // header node
   const dnode *cursor;
   for (cursor = source_ptr->next_link; 
        cursor != source_ptr; 
        cursor = cursor->next_link)
      dlist_insert (head_ptr->prev_link, cursor->data);  // append to tail
}

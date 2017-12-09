// FILE: dnode2.h
// PROVIDES: A structure for a node in a doubly-linked list, and list 
// manipulation functions for a circular, doubly-linked list with header node
//
// TYPEDEF for the dnode struct:
//     Each node of the list contains a piece of data and a pointer to the
//     previous and next nodes. The type of the data is defined as 
//     node::value_type in a typedef statement. The value_type may be any
//     of the built-in C++ classes (int, char, ...) or a class with a copy
//     constructor, an assignment operator, and a test for equality (x == y).
//
// CONSTRUCTORS for the dnode struct:
//   dnode()
//     Postcondition: The node contains the default value for dataItem obtained
//     from the default constructor of the value_type. In the ANSI/ISO standard,
//     this notation is also allowed for the built-in types, providing a default
//     value of zero.  The links that point to itself.  This node is suitable 
//     for the header node of an empty circular, doubly-linked list with header
//     node.
//   dnode(
//     const value_type& init_data,
//     dnode* init_prev_link = 0,
//     dnode* init_next_link = 0,
//   )
//     Postcondition: The node contains the specified data and links.
//     The init_*_links have a default value of 0.
//
// FUNCTIONS in the doubly-linked list toolkit:
//   size_t dlist_length(const dnode* head_ptr)
//     Precondition: head_ptr is the head pointer of a circular, doubly-linked 
//     list with header node.
//     Postcondition: The value returned is the number of nodes in the linked
//     list.
//
//   void dlist_write (const dnode * head_ptr, ostream & out)
//     Precondition: head_ptr is the head pointer of a a circular, doubly-linked
//     list with header node.
//     Postcondition: The values in the list have written to the output stream
//     one on a line.
//
//   void dlist_insert(dnode* previous_ptr, const dnode::value_type& entry) 
//     Precondition: previous_ptr points to a node in a circular, doubly-linked
//     list with header node.
//     Postcondition: A new node containing the given entry has been added
//     after the node that previous_ptr points to.
//
//   void dlist_remove(dnode* remove_ptr) 
//     Precondition: remove_ptr points to a node in a circular, doubly-linked 
//     list with header node, and this is not the header node of the list.  
//     Postcondition: The remove_ptr's node has been removed from the
//     linked list.
//
//   void dlist_clear(dnode* head_ptr) 
//     Precondition: head_ptr is the head pointer of a circular, doubly-linked 
//     list with header node.
//     Postcondition: All nodes except the header node of the list have been 
//     returned to the heap, and the header node links now point to itself.
//
//   void dlist_copy(const dnode* source_ptr, dnode*& head_ptr) 
//     Precondition: source_ptr is the head pointer of a circular, doubly-linked
//     list with header node.
//     Postcondition: head_ptr is the head pointer for a new list that contains
//     the same items as the list pointed to by source_ptr. The original list 
//     is unaltered.
//
// DYNAMIC MEMORY usage by the toolkit: 
//   If there is insufficient dynamic memory, then the following functions throw
//   bad_alloc: the constructor, dlist_insert, dlist_copy.

#ifndef DNODE_H  
#define DNODE_H
#include <cstdlib>  // Provides size_t
#include <iostream> // Provides ostream

struct dnode
{
   // TYPEDEF
   typedef std::string value_type;
    
   // CONSTRUCTORS
   dnode ()
   {
      data = value_type();
      prev_link = this;
      next_link = this;
   }
      
   dnode(const value_type& init_data,
         dnode* init_prev_link = 0,
         dnode* init_next_link = 0
	)
   { 
      data = init_data; 
      prev_link = init_prev_link; 
      next_link = init_next_link; 
   }  // note: do not need ';' here
        
      // FIELDS
   value_type data;     // data field
   dnode* prev_link;    // link to previous node
   dnode* next_link;    // link to next node
};

// FUNCTIONS for the doubly-linked list toolkit
std::size_t dlist_length(const dnode* head_ptr);
void dlist_write(const dnode* head_ptr, std::ostream &);
void dlist_insert(dnode* previous_ptr, const dnode::value_type& entry);  
void dlist_remove(dnode* remove_ptr);
void dlist_clear(dnode* head_ptr);
void dlist_copy(const dnode* source_ptr, dnode*& head_ptr);

#endif

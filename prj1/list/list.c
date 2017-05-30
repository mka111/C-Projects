#include "list.h"
#include <stdio.h>
#include <stdlib.h>


/*
 * Allocate memory for a node of type struct nodeStruct and initialize
 * it with the value item. Return a pointer to the new node.
 */
struct nodeStruct* List_createNode(int item)
{
  struct nodeStruct *newNode;
  newNode = (struct nodeStruct*)malloc(sizeof(struct nodeStruct));
  newNode->item = item;
  newNode->next = NULL;
  return newNode;
}
/*
 * Insert node at the head of the list.
 */
void List_insertHead (struct nodeStruct **headRef, struct nodeStruct *node)
{
  struct nodeStruct *current = *headRef;
  if( current == NULL)
  {
    *headRef = node;
  }
  else
  {
    node->next = current;
    *headRef = node;
  }
}
/*
 * Insert node after the tail of the list.
 */
void List_insertTail (struct nodeStruct **headRef, struct nodeStruct *node)
{
  struct nodeStruct *current = *headRef;
  if(current == NULL)
  {
    *headRef= node;
  }
  else
  {
    while(current->next != NULL)
    {
      current=current->next;
    }
    current->next = node;
  }
}
/*
 * Count number of nodes in the list.
 * Return 0 if the list is empty, i.e., head == NULL
 */
int List_countNodes (struct nodeStruct *head)
{
  int size=0;
  struct nodeStruct *current = head;
  while(current!=NULL)
  {
    size++;
    current = current->next;
  }
  return size;
}
/*
 * Return the first node holding the value item, return NULL if none found
 */
struct nodeStruct* List_findNode(struct nodeStruct *head, int item)
{
  struct nodeStruct *current = head;
  while(current!=NULL)
  {
    if(current->item == item)
    {
      return current;
    }
    current = current->next;
  }
  return current;
}
/*
 * Delete node from the list and free memory allocated to it.
 * This function assumes that node has been properly set to a valid node
 * in the list. For example, the client code may have found it by calling
 * List_findNode(). If the list contains only node, the head of the list
 * should be set to NULL.
 */
void List_deleteNode(struct nodeStruct **headRef, struct nodeStruct *node)
{
  struct nodeStruct *current = *headRef;
  if(current->item == node->item)
  {
    *headRef = current->next;
    free(current);
    return;
  }
  while(current->next != NULL && current->next->item != node->item)
  {
    current = current->next;
  }
  struct nodeStruct *current2;
  current2= current->next;
  current->next = current->next->next;
  free(current2);
  return;
}
/*
 * Sort the list in ascending order based on the item field.
 * Any sorting algorithm is fine.
 */
void List_sort (struct nodeStruct **headRef)
{
  int size, i, j;
  struct nodeStruct *current = *headRef;
  size = List_countNodes(current);
  struct nodeStruct *temp = NULL;
  struct nodeStruct *prev = NULL;
  for(i=0; i<size-1; i++)
  {
    current = *headRef;
    for( j=0; j<size-i-1; j++)
    {
      if(current->item > current->next->item)
      {
        temp = current->next;
        current->next = current->next->next;
        temp->next=current;
        if(j>0)
        {
          prev->next = temp;
        }
        prev = temp;
        if(j==0)
        {
          *headRef = prev;
        }
      }
      else
      {
        prev= current;
        current = current->next;
      }
    }
  }
  return;
}

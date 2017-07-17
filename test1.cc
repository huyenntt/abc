/*
 * test1.cc
 *
 *  Created on: May 25, 2016
 *      Author: Huyen
 */
#include <algorithm>
#include <cstddef>
#include <stdio.h>
#include <cmath>
#include <cstdio>
#include <assert.h>
#include "test1.h"

#if 0
template <class T, int SS>
Node<T,SS>:: Node()
:  pre(NULL)
,  skip_preds(NULL)
{
   depth    = 0;
   sp_size  = 0;
}
//--------------
template <class T, int SS>
Node<T,SS>:: ~Node()
{
   delete pre;
   free(skip_preds);
}
#endif
//------------
template <class T, int SS>
int  Node<T,SS>::compute_size() // ss: skip step
{
   if (depth == 0) return 0;

   int temp = 1;
   while (true)
   {
      int skip = 1;
      for (int i = 0; i < temp; i++ )
         skip = skip * SS;

      if (depth % skip > 0)
      {
         temp--;
         break;
      }
      temp++;
   }
   sp_size = temp;
   return temp;
}
//-----------------------
#if 0
template <class T, int S, int SS> // SS: skip step
MultiNode<T,S,SS>:: MultiNode()
{
}

//------------------

template <class T, int S, int SS>
MultiNode<T,S,SS>:: ~MultiNode()
{
  // delete [] node;
}

#endif
//------------------
template <class T, int SS>
template <int idx>
void Node<T,SS>:: set_skip_preds()
{
   sp_size = compute_size(); // including immediate predecessor
   int size = sp_size;
   printf(", size: %d", size);

  // initialize the elements
  if (size == 0) return;

  assert(size > 0);

  // mallocate the skip_preds
  skip_preds = (Event**) malloc(sizeof(Event*) * size);

  // the first element skip_preds[0]
  T * p = pre;
  int k = 1;

  /* go back k times by pre*/

  while ((k < SS) and (p->node[idx].depth > 0))
  {
     p = p->node[idx].pre;
     k++;
  }

  skip_preds[0] = p;

  // the rest
  if (size > 1)
  {
     for (unsigned i = 1; i < size; i++)
     {
        p = skip_preds[i - 1];
        int k = 1;

        /* go back k times by pre*/
        while ((k < SS) and (p->node[idx].depth > 0))
        {
           p = p->node[idx].skip_preds[i -1];
           k++;
        }

        skip_preds[i] = p;
     }
  }
  delete p;
}
//------------------

template <class T, int SS>
template <int idx>
void  Node<T,SS>:: print_skip_preds()
{
  printf(", depth: %d", depth);
  printf(", Pre: %p", pre);
  int size = sp_size;
  if (size == 0)
     {
        printf("\nNo skip predecessor");
        return;
     }
  printf("\nSkip_preds: ");
  for (unsigned i = 0; i < size; i++)
  {
     if (i == size-1)
        printf ("%p", skip_preds[i]);
     else
        printf ("%p, ", skip_preds[i]);
  }
}
//------------------
/*
 * Find an event which precedes this event and has the depth d2
 * assume d2 < this->node[idx].depth
 */
template <class T, int SS>
template <int idx>
void  Node<T,SS>:: find_node(int d2)
{
   T * next;
   int dis, temp, skip;
   int size = sp_size;
   dis = depth - d2;

   if (dis == 0)
   {
      printf("At the depth");
      return;
   }

   if (size == 0)
      next = pre;
   else
   {
      temp = 0;
      while (true)
      {
         // comppute pow(SS, temp)
         skip = 1;
         for (int i = 0; i < temp; i++ )
            skip = skip * SS;

         if (dis % skip > 0)
            break;
         else
            temp++;
      }
      next = skip_preds[temp-1];
   }

   while (next->node[idx].depth != d2)
   {
      dis = next->node[idx].depth - d2;
      temp = 0;
      while (true)
      {
         // comppute pow(SS, temp)
         skip = 1;
         for (int i = 0; i < temp; i++ )
            skip = skip * SS;

         if (dis % skip > 0)
            break;
         else
            temp++;
      }
      next = next->node[idx].skip_preds[temp-1];
   }

   printf("\n Node at the depth 5 is: %p", next);
}
//---------------------
#if 0
Event:: Event()
:  other_stuff(0)
,  pre_mem(NULL)
,  pre_proc(NULL)
{
   //this->node[1].pre = pre_mem;
   //this->node[0].pre = pre_proc;
   MultiNode<Event,2,2>();
}
//-------------------
Event:: ~Event()
{
   //printf("Object is being deleted");
   delete pre_proc;
   delete pre_mem;
   //~ MultiNode<Event,2,3> ();
   //~MultiNode();

}
#endif
//---------------------

int main (int argc, char **argv)
{
   Event evt[100];

   evt[0].proc().depth = 0;
   evt[0].pre_proc     = &evt[0];

/*
   for (int i = 1; i < 20; i++)
   {
      if (i < 10)
      {
         evt[i].proc().depth = i;
         evt[i].pre_proc     = &evt[i-1];
      }
      else
      {
         evt[i].proc().depth = i - 9;
         evt[i].pre_proc     = &evt[i-10];
      }
   }

   for (int i = 0; i < 20; i++)
   {
      printf("\nEvent %d: %p", i, &evt[i]);
      evt[i].proc_set_preds();
      evt[i].proc_print_preds();
   }
*/
   for (int i = 0; i < 10; i++)
      {
         evt[i].proc().depth = i;
         evt[i].pre_proc     = &evt[i-1];
      }

   for (int i = 0; i < 10; i++)
      {
         printf("\nEvent %d: %p", i, &evt[i]);
         evt[i].proc_set_preds();
         evt[i].proc_print_preds();
      }

   //evt[8].find_proc_event(&evt[5]); // from 9, find the causal node in process tree at the depth of 5
   printf("\nThe End.");
   return 0;
}


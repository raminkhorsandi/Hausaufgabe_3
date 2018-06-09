#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include "../Bool.h"
#include "Task.h"

#ifndef ElementQueue_H
#define ElementQueue_H

typedef struct ElementQueue{//TaskQueueElement
    Task *task;    //current Task
    struct ElementQueue *prev;    //previous QueueElement
    struct ElementQueue *next;    //next QueueElement
    //bool last;      //is this Task the last Element of the queue?
} ElementQueue;


/**
 *@brief    create a new Element of the Queue
 *@param    task    Task to be converted in Queue-Element
*/
ElementQueue* createElem(Task *ta);

//Anfang Methoden LCFS--------------------------------------------
/**
 *@brief    create an Element of the queue and add the Element to queue for LCFS, meaning last Element comes First in the Queue sequence
 *@param    elem    Last Element of the Queue;
 *@return   new last Element of the Queue
*/
ElementQueue* addLCFS(ElementQueue *new_Elem, ElementQueue *last);

/**
 *@brief    execute 1 Tick of the last Element of the Queue
 *@param    elem    Last Element of the Queue;
 *@return   new Last Element of the Queue 
 */
ElementQueue* execLCFS(ElementQueue *last);
#endif

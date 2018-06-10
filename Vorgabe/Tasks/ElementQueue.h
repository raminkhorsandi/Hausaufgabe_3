#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include "../Bool.h"
#include "Task.h"

#ifndef ElementQueue_H
#define ElementQueue_H

typedef struct ElementQueue{    //TaskQueueElement
    Task *task;                 //current Task
    struct ElementQueue *prev;  //previous QueueElement
    struct ElementQueue *next;  //next QueueElement
    int wz;                     //Wartezeit fuer HRRN
} ElementQueue;


/**
 *@brief    create a new Element of the Queue
 *@param    task    Task to be converted in Queue-Element
*/
ElementQueue* createElem(Task *ta);


//Methoden LCFS---------------------------------------------------------------------------------------
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


//Methoden LCFS-PR-------------------------------------------------------------------------------------
/**
 *@brief    create an Element of the queue and add the Element to queue for LCFS, meaning last Element comes First in the Queue sequence
 *@param    elem    Last Element of the Queue;
 *@return   new last Element of the Queue
*/
ElementQueue* addLCFS_PR(ElementQueue *new_Elem, ElementQueue *last);

/**
 *@brief    execute 1 Tick of the last Element of the Queue
 *@param    elem    Last Element of the Queue;
 *@return   new Last Element of the Queue 
 */
ElementQueue* execLCFS_PR(ElementQueue *last);

//Methoden HRRN----------------------------------------------------------------------------------------------
/**
 *@brief    create a new Element of HRRN of the Queue
 *@param    task    Task to be converted in Queue-Element
 */
ElementQueue* createHRRN(Task *ta);

/**
 *@brief    create an Element of the queue and add the Element to queue for HRRN
 *@param    elem    Last Element of the Queue;
 *@return   new last Element of the Queue
 */
ElementQueue* addHRRN(ElementQueue *new_Elem, ElementQueue *last);

/**
 *@brief    execute 1 Tick of the Element with highest response rate of the Queue or the current element if its still working
 *@param    elem    Last Element of the Queue;
 *@return   Element with highest respoe rate of the Queue or the current Element if its not done
 */
ElementQueue* execHRRN(ElementQueue *last);


#endif

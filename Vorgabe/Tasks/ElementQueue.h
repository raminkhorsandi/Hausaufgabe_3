#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include "../Termcolor.h"
#include "../Bool.h"

typedef struct _ElementQueue{//TaskQueueElement
    Task **task;    //current Task
    Task **prev;    //previous Task
    Task **next;    //next Task
    //bool last;      //is this Task the last Element of the queue?
}


/**
 *@brief    create a new Element of the Queue
 *@param    task    Task to be converted in Queue-Element
*/
ElementQueue* createElem(const Task *ta);
 
/**
 *@brief    create an Element of the queue and add the Element to queue for LCFS, meaning last Element comes First in the Queue sequence
 *@param    elem    Last Element of the Queue;
 *@return   new last Element of the Queue
*/
ElementQueue* addLCFS(ElementQueue *new_Elem, ElementQueue *last);

/**
 *@brief    execute the last Element of the Queue
 *@param    elem    Last Element of the Queue;
 *@return   new Last Element of the Queue 
 */
ElementQueue* execLCFS(const ElementQueue *last);

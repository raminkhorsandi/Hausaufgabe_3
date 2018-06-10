#include "HRRN.h"

void schedule_HRRN(const TaskPool *task_pool) 
{
    //HRRN ist nicht Verdraengung!
    int currTick = 0;
    
    ElementQueue *last = NULL;//Queue empty
    ElementQueue *searcher = NULL;   //elementQueue to add Wartezeit + 1
    while(!allDone(task_pool)){
        if(checkArrivals(task_pool,currTick)==NULL){//if keine Arrivals at current Tick
            if(last != NULL){   //If Queue not empty
                searcher = last;
                
                while(searcher->prev != NULL){  //go over all Elements of the queue
                    searcher = searcher->prev;//no need to see last-element
                    searcher->wz++;    //Wartezeit+1 fuer jedes Element das nicht gerade durchgefuehrt wird
                }
                searcher = NULL;
        
            }
        }
        else{ //else Task arrives at the current tick
            
            if(last == NULL){//If Queue empty
                last = createHRRN(checkArrivals(task_pool,currTick));   // create new HRRN-Queue Element
            }
            else{//If Queue not empty
                last = addHRRN(createHRRN(checkArrivals(task_pool,currTick)), last); //add new Element to the Queue
            }
        }
        if(last != NULL){ //if Queue not empty
            last = execHRRN(last);
        }
        currTick++;
    }
    //Last check if all Tasks were succesfully executed
    if(!allDone(task_pool)){
        printf("Taskpool didn't complete all Tasks!");
    }
    printf("\n");
    return;
}

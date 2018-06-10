#include "SRTN.h"


void schedule_SRTN(const TaskPool *task_pool) {
    //SRTN ist Verdraengend
    
    int currTick = 0;
    
    ElementQueue* last = NULL;//Queue empty
    
    while(!allDone(task_pool)){
        
        if(checkArrivals(task_pool,currTick)==NULL){//if keine Arrivals at current Tick
        }
        else{ //else Task arrives at the current tick <- check shortest remaing time hierarchy!
            
            if(last == NULL){//If Queue empty
                last = createElem(checkArrivals(task_pool,currTick));   // create new Queue Element
            }
            else{//If Queue not empty
                last = addSRTN(createElem(checkArrivals(task_pool,currTick)), last); //add new Element to the Queue with the constraint SRTN!
            }
        }
        if(last != NULL){ //if Queue not empty
            last = execSRTN(last);
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

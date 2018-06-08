#include "LCFS.h"
#include "../Tasks/cJSON.h"

void schedule_LCFS(const TaskPool *task_pool) 
{
    //LCFS ist nicht Verdraengend!
    //Define new Task
    Task *CPU = NULL;
    //Define current Tick
    int currTick = 0;
    int nextTask = 0;//Index of next Task
    Task *currTask;
    //use CJSON-Structure *prev, *next to do a Queue -> string with Task->thread_id to save A,B,C....
    if(!allDone(task_pool)){
        cJSON last = cJSON_CreateString(task_pool->task[0]->thread_id)//Ende des Queues
        nextTask++;
    }
    while (!allDone(task_pool)) {
        //NOTE: This is where you COULD check for new arrivals at the current tick...
        currTask = task_pool->task[nextTask]//->thread_id
        //cJSON currJSON = first;
        if(currTask->arrival_tick == currTick){//neue Task kommt in die Queue
            nextTask++;
            /*
            while(currJSON.next != NULL){
                currJSON = currJSON.next;
            }
            //jetzt ist currJSON letzte JSON in der Queue
             */
            //add angekommene Task to Queue
            cJSON currJSON = cJSON_CreateString(currTask->thread_id);
            currJSON.prev = last;
            last.next = currJSON;
            last = currJSON;
        }
        else{
            currTask = NULL;//execTask() von NULL ergibt -1
        }
        //NOTE: This is where you COULD select a Task for execution, based on the
        //      specifications of the algorithm (in this case randomness)...
        CPU = last;//Nehme letztes Element von der Queue
        CSJON newLast = last.prev;
        newLast.next = NULL;
        last = newLast;
        //NOTE: This is where you execute the Task for however long you need...
        //      (Could be one tick, could be several... it really depends on YOUR OWN ideas).
        if (execTask(CPU, CPU->total_ticks) < 0) {//by execTask() <- realize Task gleichzeitig
            printf("%sERROR:%s No Task selected to be executed.\n", COLOR_RED, COLOR_RESET);
            break;
        }
        
        //NOTE: This is where you COULD increase a variable, holding the current tick...
        currTick++;
    }
    printf("\n");
    
    return;
}

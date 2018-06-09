#include "LCFS-PR.h"

void schedule_LCFS_PR(const TaskPool *task_pool) 
{
    //LCFS ist Verdraengend!
    //Define new Task
    Task *CPU = NULL;
    int currTick = 0;
    while(!allDone(task_pool)){
        if(checkArrivals(task_pool,currTick)==NULL){
            currTick++;
        }
        else{
            CPU = checkArrivals(task_pool,currTick);
            currTick++;
        }
        if(CPU != NULL){
            execTask(CPU, CPU->total_ticks);    //Task zum Ende fuehren da nicht verdraengend
            CPU = NULL;
        }
    }
    //Last check if all Tasks were succesfully executed
    if(!allDone(task_pool)){
        printf("Taskpool didn't complete all Tasks!");
    }
}

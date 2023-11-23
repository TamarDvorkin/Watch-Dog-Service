
#include <stdio.h> /*printf*/
#include <unistd.h> /*sleep*/
#include "../include/watchdog.h"
#include "../include/utilis.h"


int main(int argc, char *argv[])
{
    /*user args*/
    int i = 0;
    int sleep_time = 0;
    size_t n = 10;
    unsigned int interval = 2;
    char *wd_path = NULL;
    
    printf("hello Im main\n");
    
    /*printf("IM IN CLIENT PROCESS\n");*/
    wd_path = "./wd.out";
    MakeMeImmortal(n,interval,wd_path,argv);
    /*code of user to keep alive*/
    
    for( i = 0; i < n; i++)
    {
    	printf("back to main from sched\n");
        sleep_time = 2;
        while (0 < (sleep_time = sleep(sleep_time)))
        {
            ;
        }

        printf("n: %d\n",i);
    }
    
    
    printf("is seg here?\n");
    DoNotResuscitata();
 
    
    printf("after DNR\n");

    return 0;

}


/*status_ty MakeMeImmortal(size_t n, unsigned int interval, char *wd_path, char **argv)*/

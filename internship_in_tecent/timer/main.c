#include "timer.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
using namespace std;

void task(int val)
{
	cout << "task func is doing ,and val is : " << val << endl;
}

void handler(int)
{
    printf("handler invoked!\n");
    tick();
	alarm(2);
}


int main()
{
    init_time_wheel();

    if (signal(SIGALRM, handler) == SIG_ERR)
    {
        printf("signal func error \n");
        return -1;
    }

	alarm(1);

	TIMER_TASK *timer = add_timer(5,1);
	timer->task_func = task;


	TIMER_TASK *timer2 = add_timer(10,2);
	timer2->task_func = task;



    del_timer(timer2);


    //destory_time_wheel();

    while(1)
    {

    }

	return 0;
}

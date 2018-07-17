#ifndef TIME_H
#define TIME_H

struct TIMER_TASK
{
    int rotation;               //定时器在时间轮上转多少圈后生效
    int time_slot;              //定时器属于时间轮上的哪个槽
    void(*task_func)(int);      //定时器的回调函数
    int task_func_parameter;    //回调函数的数据
    TIMER_TASK * prev;          //指向上一个定时器
    TIMER_TASK * next;          //指向下一个定时器
};

int init_timer_wheel();
int destory_time_wheel();
TIMER_TASK * add_timer(int timeout,int task_func_parameter);
int del_timer(TIMER_TASK *timer);
void tick();
#endif

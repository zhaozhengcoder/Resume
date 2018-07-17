#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include <malloc.h>
#include <string.h>

const int slot_num = 256;   //时间轮里面槽的个数
const int TI       = 20;    //假设硬件可以提供的最小时间间隔是20ms

struct TIME_WHEEL
{
    int cur_slot;           //时间轮的当前槽
    TIMER_TASK * slots[slot_num];   //时间轮的槽，其中每个元素指向一个定时器链表
};

TIME_WHEEL time_wheel;

int init_timer_wheel()
{
    time_wheel.cur_slot = 0;
    for(int i = 0;i<slot_num;i++)
    {
        time_wheel.slots[i] = NULL;
    }
  printf("init timer \n");
    return 0;
}

int destory_time_wheel()
{
    for(int i=0; i<slot_num; i++)
    {
        TIMER_TASK *tmp = time_wheel.slots[i];
    while (tmp) {
      time_wheel.slots[i] = tmp->next;
      free(tmp);
      tmp = time_wheel.slots[i];
      printf("destory > del a task \n");
    }
    }
}

//添加定时任务 , timeout 指的是多少毫秒之后执行任务，task_func_parameter 指的是回调函数的参数
TIMER_TASK * add_timer(int timeout,int task_func_parameter)
{
  if (timeout < 0)
  {
        return NULL;
    }
  int ticks = 0;              //待插入定时器所需要总ticks
  if (timeout < TI)
    {
    ticks = 1;
    }
  else
    {
    ticks = timeout / TI;
    }
  int rotation = ticks / slot_num;            //计算待插入的定时器在时间轮上要转动多少圈后触发
  int ts = (time_wheel.cur_slot + ticks) % slot_num;    //计算待持入定时器应该被插入的位置

  //创建TIMER_TASK
    TIMER_TASK * timer_task = (TIMER_TASK*)malloc(sizeof(TIMER_TASK));
  memset(timer_task, 0, sizeof(TIMER_TASK));
  timer_task->rotation  = rotation;
    timer_task->time_slot = ts;
  timer_task->task_func_parameter = task_func_parameter;

  //如果槽为空，则它新定时器插入，并设置为该槽的头节点
  if (!time_wheel.slots[ts])
  {
    printf("add timer_task, rotation is %d, ts is %d, cur slot is %d\n",rotation, ts, time_wheel.cur_slot);
    time_wheel.slots[ts] = timer_task;
  }
  else
  {
    timer_task->next = time_wheel.slots[ts];
    time_wheel.slots[ts]->prev = timer_task;
    time_wheel.slots[ts] = timer_task;
  }
  return timer_task;
}

int del_timer(TIMER_TASK *timer)
{
  if (!timer)
  {
        return -1;
    }
  int ts = timer->time_slot;
  if (timer == time_wheel.slots[ts])
    {
        //如果是头结点
    time_wheel.slots[ts] = time_wheel.slots[ts]->next;
    if (time_wheel.slots[ts])
    {
            time_wheel.slots[ts]->prev = NULL;
        }
    free(timer);
  }
  else
    {
    timer->prev->next = timer->next;
    if (timer->next)
    {
            timer->next->prev = timer->prev;
        }
    free(timer);
  }
    printf("delete timer \n");
    return 0;
}

void tick()
{
  //取得时间轮上当前槽的头结点
  TIMER_TASK * tmp = time_wheel.slots[time_wheel.cur_slot];
  //printf("current slot is %d\n", time_wheel.cur_slot);
  while (tmp!=NULL)
  {
    //如果定时器的rotation值大于0，则未到时，不处理
    if (tmp->rotation > 0)
        {
      tmp->rotation--;
      tmp = tmp->next;
    }
    else
        {
      //执行定时器的任务
            tmp->task_func(100);

      //删除任务
            if (tmp == time_wheel.slots[time_wheel.cur_slot])
            {
        printf("delete header in cur_slot\n");
        time_wheel.slots[time_wheel.cur_slot] = tmp->next;
        free(tmp);
        if (time_wheel.slots[time_wheel.cur_slot])
        {
                    time_wheel.slots[time_wheel.cur_slot]->prev = NULL;
                }
        tmp = time_wheel.slots[time_wheel.cur_slot];
      }
      else
            {
        tmp->prev->next = tmp->next;
        if (tmp->next)
        {
                    tmp->next->prev = tmp->prev;
                }
        TIMER_TASK *tmp2 = tmp->next;
        free(tmp);
        tmp = tmp2;
      }
    }
  }
  //更新时间轮的当前槽，以反映时间轮的转动
  time_wheel.cur_slot = ++ time_wheel.cur_slot % slot_num;
}



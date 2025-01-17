#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "utility.h"

Process_Statistics *create_Process_Statistics(Process *proc);

int compare_priority(void *data1, void *data2)
{
  Process_Statistics *ps1 = (Process_Statistics *)data1;
  Process_Statistics *ps2 = (Process_Statistics *)data2;
  if (((((Process *)ps1->proc)->priority)) < ((((Process *)ps2->proc)->priority)))
    return -1;
  else if (((((Process *)ps1->proc)->priority)) > ((((Process *)ps2->proc)->priority)))
    return 1;
  else
    return ((((Process *)ps1->proc)->arrival_time - (((Process *)ps2->proc)->arrival_time)));
}

void printQueue(Queue *q)
{
  if (q->head != NULL)
  {
    Node *n = q->head;
    printf("Queue Contains:\n");
    while (n != NULL)
    {
      Process_Statistics *ps = (Process_Statistics *)n->data;
      Process *p = ps->proc;
      printf("Process Id %c priority %u arrival time %f \n", p->pid, p->priority, p->arrival_time);
      n = n->next;
    }
  }
  return;
}

Average_Statistics highest_priority_first_preemptive(Linked_List *processes)
{
  int t = 0;
  Queue *pq1 = create_Queue();
  Queue *pq2 = create_Queue();
  Queue *pq3 = create_Queue();
  Queue *pq4 = create_Queue();
  Linked_List *ll1 = create_Linked_List();
  Linked_List *ll2 = create_Linked_List();
  Linked_List *ll3 = create_Linked_List();
  Linked_List *ll4 = create_Linked_List();

  Node *processTrav = processes->head;
  if (processes->head == NULL)
    printf("No Process to schedule\n");
  printf("\nHighest Priority First Preemptive Algorithm:\n");
  Process_Statistics *scheduled_process = NULL;

  while (t < 300 || scheduled_process != NULL)
  {
    if (scheduled_process != NULL)
    {
      if (scheduled_process->proc->priority == 1)
        enQueue(pq1, scheduled_process);
      if (scheduled_process->proc->priority == 2)
        enQueue(pq2, scheduled_process);
      if (scheduled_process->proc->priority == 3)
        enQueue(pq3, scheduled_process);
      if (scheduled_process->proc->priority == 4)
        enQueue(pq3, scheduled_process);
    }

    if (processTrav != NULL)
    {
      Process *new_process = (Process *)(processTrav->data);
      while (processTrav != NULL && new_process->arrival_time <= t)
      {
        if (new_process->priority == 1)
          enQueue(pq1, create_Process_Statistics(new_process));
        if (new_process->priority == 2)
          enQueue(pq2, create_Process_Statistics(new_process));
        if (new_process->priority == 3)
          enQueue(pq3, create_Process_Statistics(new_process));
        if (new_process->priority == 4)
          enQueue(pq3, create_Process_Statistics(new_process));
        sort(pq1, compare_priority);
        sort(pq2, compare_priority);
        sort(pq3, compare_priority);
        sort(pq4, compare_priority);
        processTrav = processTrav->next;
        if (processTrav != NULL)
          new_process = (Process *)(processTrav->data);
      }
    }
    if (scheduled_process == NULL)
    {
      if (pq1->size > 0)
        scheduled_process = (Process_Statistics *)deQueue(pq1);
      else if (pq2->size > 0)
        scheduled_process = (Process_Statistics *)deQueue(pq2);
      else if (pq3->size > 0)
        scheduled_process = (Process_Statistics *)deQueue(pq3);
      else if (pq4->size > 0)
        scheduled_process = (Process_Statistics *)deQueue(pq4);

      if (t >= 300 && scheduled_process->start_time == -1)
      {
        scheduled_process = NULL;
        continue;
      }
    }
    if (scheduled_process != NULL)
    {
      Process *proc = scheduled_process->proc;
      printf("%c", proc->pid);
      if (scheduled_process->start_time == -1)
        scheduled_process->start_time = t;
      scheduled_process->run_time++;

      if (scheduled_process->run_time >= proc->run_time)
      {
        scheduled_process->end_time = t;
        if (scheduled_process->proc->priority == 1)
          add_Node(ll1, scheduled_process);
        else if (scheduled_process->proc->priority == 2)
          add_Node(ll2, scheduled_process);
        else if (scheduled_process->proc->priority == 3)
          add_Node(ll3, scheduled_process);
        else if (scheduled_process->proc->priority == 4)
          add_Node(ll4, scheduled_process);
        scheduled_process = NULL;
      }
    }
    else
      printf("_");
    t++;
  }

  Average_Statistics avg1, avg2, avg3, avg4, avg;
  printf("\nFor Priority Queue 1");
  avg1 = print_policy_stat(ll1);
  printf("\nFor Priority Queue 2");
  avg2 = print_policy_stat(ll2);
  printf("\nFor Priority Queue 3");
  avg3 = print_policy_stat(ll3);
  printf("\nFor Priority Queue 4");
  avg4 = print_policy_stat(ll4);

  avg.avg_response_time = (avg1.avg_response_time + avg2.avg_response_time + avg3.avg_response_time + avg4.avg_response_time) / 4;
  avg.avg_wait_time = (avg1.avg_wait_time + avg2.avg_wait_time + avg3.avg_wait_time + avg4.avg_wait_time) / 4;
  avg.avg_turnaround = (avg1.avg_turnaround + avg2.avg_turnaround + avg3.avg_turnaround + avg4.avg_turnaround) / 4;
  avg.avg_throughput = (avg1.avg_throughput + avg2.avg_throughput + avg3.avg_throughput + avg4.avg_throughput);

  printf("\nAvg times of High Priority First Preemptive for all queues:\n");
  printf("Average Response Time(RT) : %.1f\n", avg.avg_response_time);
  printf("Average Wait Time(WT) : %.1f\n", avg.avg_wait_time);
  printf("Average Turn Around Time(TAT) :%.1f\n", avg.avg_turnaround);

  return avg;
}

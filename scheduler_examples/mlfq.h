//
// Created by claudia.machado on 04/10/2025.
//
#ifndef MLFQ_H
#define MLFQ_H

#include "queue.h"

void mlfq_scheduler(uint32_t current_time_ms, queue_t *queues[], pcb_t **cpu_task);

#endif

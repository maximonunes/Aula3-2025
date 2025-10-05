#include "mlfq.h"
#include "msg.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h> // para uint32_t

#define MLFQ_LEVELS 3
#define TIME_SLICE_MS 500

void mlfq_scheduler(uint32_t current_time_ms, queue_t *queues[], pcb_t **cpu_task) {

    printf("[MLFQ] Scheduler chamado no tempo %u ms\n", current_time_ms);

    static uint32_t slice_used = 0;
    static int current_level = 0;

    if (*cpu_task) {
        (*cpu_task)->ellapsed_time_ms += TICKS_MS;
        slice_used += TICKS_MS;

        if ((*cpu_task)->ellapsed_time_ms >= (*cpu_task)->time_ms) {
            msg_t msg = {
                .pid = (*cpu_task)->pid,
                .request = PROCESS_REQUEST_DONE,
                .time_ms = current_time_ms
            };
            write((*cpu_task)->sockfd, &msg, sizeof(msg_t));
            free(*cpu_task);
            *cpu_task = NULL;
            slice_used = 0;
        } else if (slice_used >= TIME_SLICE_MS) {
            int next_level = (current_level + 1 < MLFQ_LEVELS) ? current_level + 1 : current_level;
            enqueue_pcb(queues[next_level], *cpu_task);
            *cpu_task = NULL;
            slice_used = 0;
        }
    }

    if (*cpu_task == NULL) {
        for (int i = 0; i < MLFQ_LEVELS; i++) {
            pcb_t *next = dequeue_pcb(queues[i]);
            if (next) {
                *cpu_task = next;
                current_level = i;
                break;
            }
        }
    }
}
//feito por mim
// Created by claudia.machado on 04/10/2025.
//
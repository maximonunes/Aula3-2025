#include "rr.h"
#include "msg.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h> // para uint32_t


#define RR_SLICE_MS 500

void rr_scheduler(uint32_t current_time_ms, queue_t *rq, pcb_t **cpu_task) {

    static uint32_t slice_used = 0;

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
        } else if (slice_used >= RR_SLICE_MS) {
            enqueue_pcb(rq, *cpu_task);
            *cpu_task = NULL;
            slice_used = 0;
        }
    }

    if (*cpu_task == NULL)
        *cpu_task = dequeue_pcb(rq);
}//
//
// Created by claudia.machado on 04/10/2025.
//

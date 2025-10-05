#include "sjf.h"
#include "msg.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h> // para uint32_t

/**
 * Shortest Job First (SJF) Scheduler
 * Escolhe a tarefa com menor tempo de execução (time_ms).
 */
void sjf_scheduler(uint32_t current_time_ms, queue_t *rq, pcb_t **cpu_task) {

    printf("[SJF] Scheduler chamado no tempo %u ms\n", current_time_ms);
    if (*cpu_task) {
        (*cpu_task)->ellapsed_time_ms += TICKS_MS;
        if ((*cpu_task)->ellapsed_time_ms >= (*cpu_task)->time_ms) {
            // Task finished
            msg_t msg = {
                .pid = (*cpu_task)->pid,
                .request = PROCESS_REQUEST_DONE,
                .time_ms = current_time_ms
            };
            write((*cpu_task)->sockfd, &msg, sizeof(msg_t));
            free(*cpu_task);
            *cpu_task = NULL;
        }
    }

    // Se CPU está livre → escolher o processo mais curto
    if (*cpu_task == NULL && rq->head) {
        queue_elem_t *best_prev = NULL;
        queue_elem_t *best = rq->head;
        queue_elem_t *prev = rq->head;
        queue_elem_t *it = rq->head->next;

        while (it != NULL) {
            if (it->pcb->time_ms < best->pcb->time_ms) {
                best_prev = prev;
                best = it;
            }
            prev = it;
            it = it->next;
        }

        // Retirar best da fila
        if (best_prev)
            best_prev->next = best->next;
        else
            rq->head = best->next;

        if (best == rq->tail)
            rq->tail = best_prev;

        *cpu_task = best->pcb;
        free(best);
    }
}
//
// Created by claudia.machado on 04/10/2025.
//
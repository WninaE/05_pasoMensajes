/*
 * process2.c - Escucha el primer mensaje, responde y recibe la replica final.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSGSZ 128

typedef struct {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main() {
    int msqid;
    key_t key = 1234;
    message_buf sbuf, rbuf;

    // Obtener la cola de mensajes
    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget process2");
        exit(1);
    }

    // Paso 1: Recibir "Are you hearing me?" (mtype = 1)
    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
        perror("msgrcv P2 step 1");
        exit(1);
    }
    printf("[Process 2] Recibido: %s\n", rbuf.mtext);

    // Paso 2: Responder "Loud and clear" (mtype = 2)
    sbuf.mtype = 2;
    strcpy(sbuf.mtext, "Loud and clear");
    printf("[Process 2] Respondiendo: %s\n", sbuf.mtext);
    if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0) {
        perror("msgsnd P2 step 2");
        exit(1);
    }

    // Paso 3: Recibir replica "I can hear you too" (mtype = 3)
    if (msgrcv(msqid, &rbuf, MSGSZ, 3, 0) < 0) {
        perror("msgrcv P2 step 3");
        exit(1);
    }
    printf("[Process 2] Recibido final: %s\n", rbuf.mtext);

    // Limpieza de la cola
    msgctl(msqid, IPC_RMID, NULL);
    return 0;
}
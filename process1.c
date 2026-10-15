/*
 * process1.c - Inicia el dialogo, recibe respuesta y envia replica final.
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

    // Crear/Obtener la cola de mensajes
    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget process1");
        exit(1);
    }

    // Paso 1: Enviar "Are you hearing me?" (mtype = 1)
    sbuf.mtype = 1;
    strcpy(sbuf.mtext, "Are you hearing me?");
    printf("[Process 1] Enviendo: %s\n", sbuf.mtext);
    if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0) {
        perror("msgsnd P1 step 1");
        exit(1);
    }

    // Paso 2: Recibir respuesta "Loud and clear" (mtype = 2)
    if (msgrcv(msqid, &rbuf, MSGSZ, 2, 0) < 0) {
        perror("msgrcv P1 step 2");
        exit(1);
    }
    printf("[Process 1] Recibido: %s\n", rbuf.mtext);

    // Paso 3: Enviar replica "I can hear you too" (mtype = 3)
    sbuf.mtype = 3;
    strcpy(sbuf.mtext, "I can hear you too");
    printf("[Process 1] Enviando replica: %s\n", sbuf.mtext);
    if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0) {
        perror("msgsnd P1 step 3");
        exit(1);
    }

    return 0;
}
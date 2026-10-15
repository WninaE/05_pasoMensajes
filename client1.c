/*
 * client1.c - Cliente 1 (Envia en mtype=10, Recibe en mtype=101)
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
    key_t key = 5678;
    message_buf sbuf, rbuf;

    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget client1");
        exit(1);
    }

    // Solicitud del Cliente 1 (mtype = 10)
    sbuf.mtype = 10;
    strcpy(sbuf.mtext, "Solicitud de datos desde Cliente 1");
    printf("[CLIENTE 1] Enviando peticion (mtype=10)...\n");
    msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0);

    // Esperar respuesta EXCLUSIVAMENTE en el canal 101
    printf("[CLIENTE 1] Esperando respuesta en canal privado (mtype=101)...\n");
    msgrcv(msqid, &rbuf, MSGSZ, 101, 0);
    printf("[CLIENTE 1] Respuesta recibida: %s\n", rbuf.mtext);

    return 0;
}
/*
 * client2.c - Cliente 2 (Envia en mtype=20, Recibe en mtype=102)
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
        perror("msgget client2");
        exit(1);
    }

    // Solicitud del Cliente 2 (mtype = 20)
    sbuf.mtype = 20;
    strcpy(sbuf.mtext, "Solicitud de datos desde Cliente 2");
    printf("[CLIENTE 2] Enviando peticion (mtype=20)...\n");
    msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0);

    // Esperar respuesta EXCLUSIVAMENTE en el canal 102
    printf("[CLIENTE 2] Esperando respuesta en canal privado (mtype=102)...\n");
    msgrcv(msqid, &rbuf, MSGSZ, 102, 0);
    printf("[CLIENTE 2] Respuesta recibida: %s\n", rbuf.mtext);

    return 0;
}
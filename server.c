/*
 * server.c - Servidor que atiende a dos clientes de forma privada en una cola unica.
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
    message_buf rbuf, sbuf;

    // Crear la unica cola de mensajes
    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget server");
        exit(1);
    }
    printf("[SERVIDOR] Servidor listo en la cola (key=%d, msqid=%d)...\n", key, msqid);

    // Atender peticiones de ambos clientes
    for (int i = 0; i < 2; i++) {
        // msgrcv con msgtyp = 0 para leer cualquier peticion entrante
        if (msgrcv(msqid, &rbuf, MSGSZ, 0, 0) < 0) {
            perror("msgrcv server");
            exit(1);
        }

        printf("[SERVIDOR] Peticion recibida (mtype=%ld): %s\n", rbuf.mtype, rbuf.mtext);

        if (rbuf.mtype == 10) { // Peticion de Cliente 1
            sbuf.mtype = 101;  // Canal privado de respuesta a Cliente 1
            strcpy(sbuf.mtext, "Hola Cliente 1, tu respuesta privada es: DATOS_PRIVADOS_C1");
            msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0);
            printf("[SERVIDOR] Respuesta privada enviada a Cliente 1 (mtype=101)\n");
        } 
        else if (rbuf.mtype == 20) { // Peticion de Cliente 2
            sbuf.mtype = 102;  // Canal privado de respuesta a Cliente 2
            strcpy(sbuf.mtext, "Hola Cliente 2, tu respuesta privada es: DATOS_PRIVADOS_C2");
            msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0);
            printf("[SERVIDOR] Respuesta privada enviada a Cliente 2 (mtype=102)\n");
        }
    }

    printf("[SERVIDOR] Atencion finalizada. Eliminando cola...\n");
    msgctl(msqid, IPC_RMID, NULL);
    return 0;
}
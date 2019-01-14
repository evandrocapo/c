/*Codigo pthread
Nome: Evandro Douglas Capovilla Junior RA: 16023905
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h> // Para a função sleep
#include <sys/types.h> // Para pthread_mutex
#include <sys/ipc.h>  // Para argumento IPC_CREATE
#include <sys/msg.h> // Para fila de mensagem
#include <sys/sem.h> // Para struct sembuf
#include <sys/shm.h> // Para memória compartilhada
#include <string.h> // Para manusear strings

#define NO_OF_CHILDREN 3
#define MESSAGE_QUEUE_ID 3102

int g_sem_id; // ID do semáforo -> cada processo tem o seu
int g_shm_id; // ID da memoria compartilhada -> cada processo tem o seu
int queue_id;
struct sembuf g_sem_op1[1]; // ID para abrir semáforo
struct sembuf g_sem_op2[1]; // ID para fechar semáforo

typedef struct{
	int cadeiras;
	int barbeiros_disponiveis;
} memoria_t;

typedef struct{
	int n_cliente;
	int n_barbeiro;
	char cabelo[4000];
	char atualizado[4000];
	struct timeval tempo_chegada;
} data_t;

typedef struct {
	long int mtype;
	char mtext[sizeof(data_t)];	
} msgbuf_t;

memoria_t *g_shm_struct; // ID para memória compartilhada
pthread_t filho[NO_OF_CHILDREN];

int queue_id;

void* pthread_filho();

void main(){
int pid;
int count;
int key = MESSAGE_QUEUE_ID;

	if ((queue_id = msgget(key, IPC_CREAT | 0666)) == -1 ){
		fprintf(stderr, "Impossivel acessar a fila de mensagem!\n");
		exit(1);
	}


	for(count = 0; count < NO_OF_CHILDREN; count++)
	{
		pid = pthread_create(&filho[count], NULL, (void *) pthread_filho, (void *)count+1);
		if(pid)
		{
			printf("ERRO: Impossivel criar um thread filho\n");
			exit(-1);
		}
	}

	for(count = 0; count < NO_OF_CHILDREN; count++)
	{
		pthread_join(filho[count], NULL);
	}

printf("Encerrando o thread pai!\n");

}


void* pthread_filho(void* id){
	int id_filho = (int)id;
	msgbuf_t message_buffer;
	data_t *data_ptr = (data_t *)(message_buffer.mtext);
	message_buffer.mtype = id; // FILA COM ID 
	int g_shm_id,g_sem_id;

	//printf("Encerrado o processo filho %d e o queue_id é: %d !\n", id_filho, queue_id); 

	if(msgrcv(queue_id, (struct msgbuf *) &message_buffer,sizeof(data_t),id_filho,0) == -1){
		fprintf(stderr, "Impossivel receber mensagem! CLIENTE\n");
		exit(1);
	}
	
	printf("Thread: %d - PID: %d, - Memoria Compartilhada: %d - Semaforo: %d",data_ptr->pid, data_ptr->chave_memoria_compartilhada, data_ptr->chave_do_semaforo);
	

}

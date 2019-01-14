/*Codigo processo
Nome: Evandro Douglas Capovilla Junior RA: 16023905
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
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
#define SEM_KEY1 0x1249 // 1249
#define SEM_KEY2 0x1250
#define SEM_KEY3 0x1251

int g_sem_id; // ID do semáforo -> cada processo tem o seu
int g_shm_id; // ID da memoria compartilhada -> cada processo tem o seu
struct sembuf g_sem_op1[1]; // ID para abrir semáforo
struct sembuf g_sem_op2[1]; // ID para fechar semáforo

typedef struct{
	int soma;
} memoria_t;

typedef struct{
	int pid;
	int chave_memoria_compartilhada;
	int chave_do_semaforo;
} data_t;

typedef struct {
	long int mtype;
	char mtext[sizeof(data_t)];	
} msgbuf_t;

void processo_filho(int id, int queue_id);

void main(){
	int rtn;
	int queue_id;
	int key = MESSAGE_QUEUE_ID;
	int count;
	pid_t pid[NO_OF_CHILDREN];

	g_sem_op1[0].sem_num = 0; //bloqueia
	g_sem_op1[0].sem_op = -1;
	g_sem_op1[0].sem_flg = 0;

	g_sem_op2[0].sem_num = 0; //libera
	g_sem_op2[0].sem_op = 1;
	g_sem_op2[0].sem_flg = 0;


// criação da fila de mensagem KEY = 3102

	if ((queue_id = msgget(key, IPC_CREAT | 0666)) == -1 ){
		fprintf(stderr, "Impossivel criar a fila de mensagens!\n");
		exit(1);
	}

// criação dos processos filhos

	rtn = 1;
	for(count = 0; count < NO_OF_CHILDREN; count++){
		if(rtn != 0){
			rtn = fork();
			pid[count] = rtn;
		}
		else{
			break;
		}
	}

	if(rtn == 0){
		processo_filho(count, queue_id); // cria filho
	}
	else{
		for(count = 0; count < NO_OF_CHILDREN; count++){
			wait(NULL);
		}		

		if(msgctl(queue_id, IPC_RMID, NULL) == -1){
			fprintf(stderr, "Impossivel remover a fila!\n"); // remove a fila
			exit(1);
		}
	printf("Encerrado o processo pai\n\n");
	}

}

void processo_filho(int id, int queue_id){
	memoria_t *g_shm_struct; // ID para memória compartilhada
	msgbuf_t message_buffer;
	data_t *data_ptr = (data_t *)(message_buffer.mtext);
	message_buffer.mtype = id; // FILA COM ID 
	int SEM_KEY;
	int g_shm_id,g_sem_id;
	//printf("Encerrado o processo filho %d e o queue_id da fila é: %d!\n", id, queue_id);

	if(id == 1){
		SEM_KEY = SEM_KEY1;
	}
	else{
		if(id == 2){	
			SEM_KEY = SEM_KEY2;
		}
		else{
			SEM_KEY = SEM_KEY3;
		}
	
	}

/*
//criar memoria compartilhada

	if((g_shm_id = shmget(IPC_PRIVATE,sizeof(memoria_t),IPC_CREAT | 0666)) == -1){
		fprintf(stderr, "Impossivel criar memoria compartilhada!\n" );
		exit(1);
	}

//criar semaforo
	if((g_sem_id = semget(SEM_KEY,1,IPC_CREAT | 0666)) == -1){ // semaforo para a memoria.
		fprintf(stderr, "CHAMADA SEMGET FALHOU!");
		exit(1);
	}
*/

data_ptr->pid = getpid();
data_ptr->chave_memoria_compartilhada = g_shm_id;
data_ptr->chave_do_semaforo = g_sem_id;

//enviar msg com MTYPE do seu id
	if(msgsnd(queue_id,(struct msgbuf *)&message_buffer,sizeof(data_t),0)==-1){ 
			fprintf(stderr, "Impossivel enviar mensagem! CLIENTE\n");
			exit(1);
		}


	printf("PID: %d (%d) - Memoria Compartilhada: %d - Semaforo: %d\n", getpid(),id, g_shm_id, g_sem_id);
/*
	if(semctl(g_sem_id,0,IPC_RMID,0) ==-1){
		fprintf(stderr, "Impossivel remover o semaforo\n" );
		exit(1);
	}

	if(shmctl(g_shm_id,IPC_RMID,NULL) == -1 ) {
		fprintf(stderr,"Impossivel remover o segmento de memoria compartilhada!\n");
		exit(1);
	}
*/

}

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define SEMKEY 110
#define SHMKEY 75

#define EM  0  //Semaforo de Exclusion Mutua
#define EL  1  //Semáforo de Espacios Libres
#define ED  2  //Semaforo de Elementos Disponibles

int ed=0,el=0;
int semid;           
char caracter=65;
char carac[15];

struct sembuf sem_oper; // Para 
union semun
 {
               int val;
               struct semid_ds *semstat;
               unsigned short *array;
  }
arg;

void wait(int id)
{
      sem_oper.sem_num=id;  //actuamos sobre el semáforo id
      sem_oper.sem_op=-1;   //decremento
      sem_oper.sem_flg=SEM_UNDO; // Para evitar interbloqueos
      semop(semid, &sem_oper,1);
}

void signal(int id)
{
      sem_oper.sem_num=id;  //actuamos sobre el semáforo id
      sem_oper.sem_op=1;    //incremento
      sem_oper.sem_flg=SEM_UNDO; // Para evitar interbloqueos
      semop(semid, &sem_oper,1);
}

int main () {

      int n;
      int shmid[2];
      char *addr, *buf;
    
    // Crear la región de memoria y obtener la dirección 
    if((shmid[0] = shmget (SHMKEY, sizeof(char)*15, 0777 | IPC_CREAT))==-1){
          printf("No se pudo reservar memoria");
          exit(-1);
      }

    // Enlazar región al espacio de direccionamiento del proceso 
    
    addr = shmat (shmid[0], 0, 0);
    buf = addr;       //Reservar addr

    //crear 3 semáforos

      semid=semget(SEMKEY,3,0777|IPC_CREAT);

    // inicializar los semáforos
      
   arg.array=(unsigned short *) malloc(sizeof(short)*3);
        arg.array[0]=1;
        arg.array[1]=15;
        arg.array[2]=0;
        semctl (semid,3,SETALL,arg);
   
    // operamos sobre los semáforos
       for(n=1;n<=200;n++)
        {
            wait(EL);           //ver si hay espacios libres
            wait(EM);           // sección critica
                buf=addr;                    //Acceder a memoria compartida
                buf[el]=caracter;         //escribir el caracter en memoria
                el=(el+1)%15;              //actualizar el indice 
                if(caracter=='Z')           
                    caracter='a';          //vuelve a empezar el abecedario   
                else if(caracter=='z')     //a z pase a A.
                    caracter='A';
                else
                    caracter++;
                signal(EM);    //salir de sc

            signal(ED);        //actualizar los elementos disponibles
        }
      
        free(arg.array);

    // Separar la región del espacio de direccionamiento del proceso 
        
      shmdt (addr);
      for(n=0;n<2;n++)
      shmctl(shmid[n],IPC_RMID,0);
      free(arg.array);
      return 0;

} // fin del main

//PROCESO CONSUMIDOR

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define SEMKEY 110
#define SHMKEY 75 

#define EM  0  //Semaforo de Exclusion Mutua
#define EL  1  //Semáforo de Espacios Libres
#define ED  2  //Semaforo de Elementos Disponibles

int ed=0;
int semid;          
char caracter=200;
char carac[15];

struct sembuf sem_oper;
union semun 
{
               int val;
               struct semid_ds *semstat;
               unsigned short *array;
 }
arg;

void wait(int id)
{
      sem_oper.sem_num=id;  //actuar sobre el semáforo correspondiente
      sem_oper.sem_op=-1;   //decremento 1 el semáforo
      sem_oper.sem_flg=SEM_UNDO; // Para evitar interbloqueos si un proceso acaba inesperadamente
      semop(semid, &sem_oper,1);
}

void signal(int id)
{
      sem_oper.sem_num=id;  //actuar sobre el semáforo correspondiente
      sem_oper.sem_op=1;    //incremento
      sem_oper.sem_flg=SEM_UNDO; // Para evitar interbloqueos
      semop(semid, &sem_oper,1);
}

int main () {

      int eof;
      int n;
      int shmid[2];
      char *addr, *buf;
     
    //Crear la región de memoria y obtener la dirección 

    if((shmid[0] = shmget (SHMKEY, sizeof(char)*15, 0777 | IPC_CREAT))==-1){
          printf("No se pudo reservar memoria");
          exit(-1);
      }

    //Enlazar región al espacio de direccionamiento del proceso

    addr = shmat (shmid[0], 0, 0);    
    if((shmid[1] = shmget (SHMKEY, sizeof(int), 0777 | IPC_CREAT))==-1){
          printf("No se pudo reservar memoria");
          exit(-1);
      }

    // Enlazar región al espacio de direccionamiento del proceso
 
    eof = shmat (shmid[1], 0, 0);

    //creación de 3 semáforos

      semid=semget(SEMKEY,3,0777|IPC_CREAT);
   
    // inicialización de los semáforos

      arg.array=(unsigned short *) malloc(sizeof(short)*3);
      arg.array[0]=1;
      arg.array[1]=15;
      arg.array[2]=0;
      semctl (semid,3,SETALL,arg);

      // operamos sobre los semáforos

        for(n=1;n<=200;n++)          // 200 caracteres
        {
            wait(ED);                 //Comprobar que hay elementos disponibles
            wait(EM);                 // sección critica
            buf=addr;                   //acceder a memoria compartida
            caracter=buf[ed];           //leer el caracter    
            printf("%c",caracter);   //imprimir  por pantalla
            ed=(ed+1)%15;             //actualizar el indice 
            signal(EM);               //salir de sección critica
            signal(EL);              //actualizar los espacios libres
        }

        semctl (semid,3,IPC_RMID,0);
        
   // Separar la región del espacio de direccionamiento del proceso
        
   shmdt (addr);
        shmdt (eof);
        for(n=0;n<2;n++)
        shmctl(shmid[n],IPC_RMID,0);
        free(arg.array);

      return 0;

} // fin del main
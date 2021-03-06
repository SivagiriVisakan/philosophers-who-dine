#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <semaphore.h> 
#include <sys/ipc.h>
#include <sys/shm.h>

#include "philsophers_base.h"

#define TOTAL_PHILOSOPHERS 5
#define LOG(message) logToFile(logFile, message, &fileMutex)

sem_t forks[TOTAL_PHILOSOPHERS];
pthread_t philosopher_thread_data[TOTAL_PHILOSOPHERS];

struct philosopher_state *philosopher_info;
struct  fork_info *forks_state_info;

FILE *logFile;
pthread_mutex_t fileMutex;

char *state_messages[] = { 
    "Eating",
    "Thinking",
    "Waiting for left fork",
    "Waiting for right fork"
};

struct coordinates
{
    int x, y;
}philosopher_coordinates[TOTAL_PHILOSOPHERS];

int modulo(int x,int N){
    return (x % N + N) %N;
}


void logToFile(FILE *file, char *message, pthread_mutex_t *mutex)
{
    pthread_mutex_lock(&mutex);
    fprintf(file, "%s\n", message);
    fflush(file);
    pthread_mutex_unlock(&mutex);
}

int get_left(int i)
{
    int left_fork_index = i;
    philosopher_info[i].current_state = WAITING_FOR_FORK_ONE;
    sem_wait(&forks[left_fork_index]);
    forks_state_info[left_fork_index].owner_id = i;
    return left_fork_index;
}

int get_right(int i)
{
    int right_fork_index = modulo(i-1, TOTAL_PHILOSOPHERS);
    philosopher_info[i].current_state = WAITING_FOR_FORK_TWO;
    sem_wait(&forks[right_fork_index]);
    forks_state_info[right_fork_index].owner_id = i;
    return right_fork_index;
}

void eat(int i)
{
    philosopher_info[i].current_state = EATING;
    sleep(8);
    philosopher_info[i].total_meals_eaten++;

}

void think(int i)
{
    philosopher_info[i].current_state = THINKING;
    sleep(5);
    sleep(rand() % 20 + 2);
}



void *philosopher(void *arg)
{
    int i = (int *)arg;
    while (1)
    {
        LOG(philosopher_info[i].fork_held);
        think(i);
        int left = get_left(i);
        snprintf(philosopher_info[i].fork_held, 200, "Acquired left - having  %d\n\0",left);
        LOG(philosopher_info[i].fork_held);

        // sleep(5);   // Un-comment this line to create a deadlock
        int right = get_right(i);
        snprintf(philosopher_info[i].fork_held, 200, "Acquired left and right - having  %d - %d\n\0", left,right);
        LOG(philosopher_info[i].fork_held);
        eat(i);
        // put_down_left(i); TODO: Implement by extracting the sem post and print to a seperate function
        // put_down_right(i);
        sem_post(&forks[left]);
        forks_state_info[left].owner_id = -1;
        snprintf(philosopher_info[i].fork_held, 200, "  Dropped left - having - %d\n\0", right);
        sem_post(&forks[right]);
        forks_state_info[right].owner_id = -1;
        snprintf(philosopher_info[i].fork_held, 200, "\n\0");

    }
}


int main(int argc, char const *argv[])
{

    srand(time(NULL));
    pthread_mutex_init(&fileMutex, NULL);
    logFile = fopen("dine.log", "w+");
    philosopher_coordinates[0].x = 0;
    philosopher_coordinates[0].y = 35;

    philosopher_coordinates[1].x = 8;
    philosopher_coordinates[1].y = 65;

    philosopher_coordinates[2].x = 20;
    philosopher_coordinates[2].y = 65;

    philosopher_coordinates[3].x = 25;
    philosopher_coordinates[3].y = 35;

    philosopher_coordinates[4].x = 15;
    philosopher_coordinates[4].y = 10;

    key_t philosophers_state_key = 567;
    key_t forks_state_key = 569;

   // shmget returns an identifier in shmid
   int shmid = shmget(philosophers_state_key, sizeof(struct philosopher_state) * TOTAL_PHILOSOPHERS, 0666|IPC_CREAT);
   if (shmid < 0) {
        printf("*** shmget error (can't acquire for philosopher's state) ***\n");
        perror("shmget error:");
        exit(1);
   }
    philosopher_info = (struct philosopher_state *) shmat(shmid,NULL,0);

   // shmget returns an identifier in shmid
   shmid = shmget(forks_state_key, sizeof(struct fork_info) * TOTAL_PHILOSOPHERS, 0666|IPC_CREAT);
   if (shmid < 0) {
        printf("*** shmget error (can't acquire for fork's state) ***\n");
        perror("shmget error:");
        exit(1);
   }
    forks_state_info = (struct fork_info *) shmat(shmid,NULL,0);



    for (int i = 0; i < TOTAL_PHILOSOPHERS; i++)
    {
        sem_init(&forks[i], 0, 1);
        forks_state_info[i].id = i;
        forks_state_info[i].owner_id = -1;

        philosopher_info[i].total_meals_eaten = 0;
        philosopher_info[i].id = i;
        philosopher_info[i].forks_allowed[0] = &forks_state_info[i];
        philosopher_info[i].forks_allowed[1] = &forks_state_info[modulo(i-1, TOTAL_PHILOSOPHERS)];
        pthread_create(&philosopher_thread_data[i], NULL, &philosopher, (void *)i);
    }
    initscr();

    while(1)
    {
        for(int i = 0; i < TOTAL_PHILOSOPHERS; i++)
        {
            char *message = state_messages[philosopher_info[i].current_state];
            mvprintw(philosopher_coordinates[i].x, philosopher_coordinates[i].y,"%d. %s\n", philosopher_info[i].id, message);
            char forks_message[100] = {'\0'};
            mvprintw(philosopher_coordinates[i].x+2, philosopher_coordinates[i].y, philosopher_info[i].fork_held);
            mvprintw(philosopher_coordinates[i].x+3, philosopher_coordinates[i].y, "Total meals had: %d\n", philosopher_info[i].total_meals_eaten);

            mvprintw(0, 5*i, " %d ", forks_state_info[i].owner_id);
        }
            refresh();

    }
    
	for (int i = 0; i < TOTAL_PHILOSOPHERS; i++)
    {
		pthread_join(philosopher_thread_data[i], NULL);
	}

    shmctl(shmid,IPC_RMID,NULL);

    return 0;
}

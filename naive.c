#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <semaphore.h> 

#define TOTAL_PHILOSOPHERS 5

sem_t forks[TOTAL_PHILOSOPHERS];
bool fork_available[TOTAL_PHILOSOPHERS] = {true};
pthread_t philosopher_thread_data[TOTAL_PHILOSOPHERS];

struct messages
{
    char current_status[200];
    char fork_held[200];
    int total_meals_eaten;
}philosopher_messages[TOTAL_PHILOSOPHERS];

char message_to_print[5][200] = {0};

struct coordinates
{
    int x, y;
}philosopher_coordinates[TOTAL_PHILOSOPHERS];

int modulo(int x,int N){
    return (x % N + N) %N;
}

int get_left(int i)
{
    int left_fork_index = i;
    snprintf(message_to_print[i], 200, "%d. Waiting for left fork\n\0", i);
    sem_wait(&forks[left_fork_index]);
    snprintf(message_to_print[i], 200, "%d. Acquired left fork\n\0", i);
    return left_fork_index;
}

int get_right(int i)
{
    int right_fork_index = modulo(i-1, TOTAL_PHILOSOPHERS);
    snprintf(message_to_print[i], 200, "%d. Waiting for right fork\n\0", i);
    sem_wait(&forks[right_fork_index]);
    snprintf(message_to_print[i], 200, "%d. Acquired right fork\n\0", i);
    return right_fork_index;
}

void eat(int i)
{
    snprintf(message_to_print[i], 200, "%d. Eating\n\0", i);
    sleep(8);
    philosopher_messages[i].total_meals_eaten++;

}

void think(int i)
{
    snprintf(message_to_print[i], 200, "%d. Thinking\n\0", i);
    sleep(5);
}



void *philosopher(void *arg)
{
    int i = (int *)arg;
    while (1)
    {

        think(i);
        int left = get_left(i);
        snprintf(philosopher_messages[i].fork_held, 200, "Acquired left - having  %d\n\0",left);
        // sleep(5);   // Un-comment this line to create a deadlock
        int right = get_right(i);
        snprintf(philosopher_messages[i].fork_held, 200, "Acquired left and right - having  %d - %d\n\0", left,right);
        eat(i);
        // put_down_left(i); TODO: Implement by extracting the sem post and print to a seperate function
        // put_down_right(i);
        sem_post(&forks[left]);
        snprintf(philosopher_messages[i].fork_held, 200, "  Dropped left - having - %d\n\0", right);
        sem_post(&forks[right]);
        snprintf(philosopher_messages[i].fork_held, 200, "\n\0");

        // sleep(5);
    }
}


int main(int argc, char const *argv[])
{

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

    for (int i = 0; i < TOTAL_PHILOSOPHERS; i++)
    {
        sem_init(&forks[i], 0, 1); 
        philosopher_messages[i].total_meals_eaten = 0;
        // philosopher_coordinates[i].x = 10 * i;
        // philosopher_coordinates[i].y = 10;
        pthread_create(&philosopher_thread_data[i], NULL, &philosopher, (void *)i);
    }
    initscr();

    while(1)
    {
        for(int i = 0; i < TOTAL_PHILOSOPHERS; i++)
        {
            mvprintw(philosopher_coordinates[i].x, philosopher_coordinates[i].y, message_to_print[i]);
            mvprintw(philosopher_coordinates[i].x+2, philosopher_coordinates[i].y, philosopher_messages[i].fork_held);
            mvprintw(philosopher_coordinates[i].x+3, philosopher_coordinates[i].y, "Total had: %d\n", philosopher_messages[i].total_meals_eaten);
        }
            refresh();
        // sleep(1);

    }
    
	for (int i = 0; i < TOTAL_PHILOSOPHERS; i++)
    {
		pthread_join(philosopher_thread_data[i], NULL);
	}
	return 0;
    return 0;
}

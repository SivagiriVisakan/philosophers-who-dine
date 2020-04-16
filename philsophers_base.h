
#ifndef PHILOSOPHERS_BASE
#define PHILOSOPHERS_BASE


/**
 * This structure will manage the state of forks
 */
struct fork_info
{
    int id;
    int owner_id; // The number of the philosopher owning it or -1 if it is free on the table
};

enum philosopher_states
{
    EATING=0,
    THINKING,
    WAITING_FOR_FORK_ONE,
    WAITING_FOR_FORK_TWO
};

struct philosopher_state
{
    int id;
    enum philosopher_states current_state;
    struct fork_info *forks_allowed[2];
    unsigned int total_meals_eaten;
    char fork_held[200];
};


#endif
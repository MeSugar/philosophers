# Description

This project covers the basics of multi-threading and mutexes.

From the subject:
```
Here are the things you need to know if you want to succeed this assignment:
• One or more philosophers sit at a round table.
• There is a large bowl of spaghetti in the middle of the table.
• The philosophers alternatively eat, think, or sleep.
• While they are eating, they are not thinking nor sleeping; while thinking,
they are not eating nor sleeping; and, of course, while sleeping, they are not eating nor thinking.
• There are also forks on the table. There are as many forks as philosophers.
• Because serving and eating spaghetti with only one fork is very inconvenient,
a philosopher takes their right and their left forks to eat, one in each hand.
• When a philosopher has finished eating, they put their forks back on the table and start sleeping.
Once awake, they start thinking again. The simulation stops when a philosopher dies of starvation.
• Every philosopher needs to eat and should never starve.
• Philosophers don’t speak with each other.
• Philosophers don’t know if another philosopher is about to die.
• No need to say that philosophers should avoid dying!
• The program must not have any data races.
```
Any is formatted as follows:
- **timestamp_in_ms X** has taken a fork
- **timestamp_in_ms X** is eating
- **timestamp_in_ms X** is sleeping
- **timestamp_in_ms X** is thinking
- **timestamp_in_ms X** died

*X is the philosopher number*


## Usage
The program should take the following arguments:
- **number_of_philosophers:** The number of philosophers and also the number of forks.
- **time_to_die (in milliseconds):** If a philosopher didn’t start eating time_to_die milliseconds since the beginning of their last meal or the beginning of the simulation, they die.
- **time_to_eat (in milliseconds):** The time it takes for a philosopher to eat. During that time, they will need to hold two forks.
- **time_to_sleep (in milliseconds):** The time a philosopher will spend sleeping.
- **number_of_times_each_philosopher_must_eat (optional argument):** If all philosophers have eaten at least number_of_times_each_philosopher_must_eat times, the simulation stops. If not specified, the simulation stops when a philosopher dies
```
git clone https://github.com/MeSugar/philosophers.git
cd philosophers
make
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

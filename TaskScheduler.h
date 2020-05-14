#ifndef _TASKSCHEDULER_H_
#define _TASKSCHEDULER_H_

#ifndef PRECISION
#define PRECISION micros
#endif

#include <Arduino.h> // Required for the use of the 'micros()' function and the 'NULL' and 'uint64_t' data types.

typedef void (*functionPointer) (void);

class Task {
  public:
    Task(functionPointer function, Task * next = NULL, uint64_t period = 0); // Creates a task that repeats a function periodically a set of time (in microseconds). If no period is provided, the task is executed as soon as possible.
    ~Task(); // Task destruction.
    void setNext(Task * next); // Modifies the pointer to the next task.
    Task * getNext(); // Returns the next task in the queue.
    uint64_t getTimestamp(); // Returns the timestamps of the last execution of the task.
    void setTimestamp(); // Modifies the timestamp of the task.
    uint64_t getPeriod(); // Returns the amount of time between executions of a task.
    void setPeriod(uint64_t period); // Modifies the amount of time between executions of a task.
    bool isFunction(functionPointer function); // Returns if the task is associated with a function.
    functionPointer getFunction(); // Returns the function associated to the task.
    void flag(); // Toggles the overflow flag.
    bool getFlag(); // Returns the state of the overflow flag.
    void run(); // Executes the task.
  private:
    functionPointer function; // Function it points to.
    Task * next; // Pointer to the next task in the queue.
    uint64_t timestamp; // Timestamp of the next time the task will be executed.
    uint64_t period; // Amount of time between executions.
    bool overflow; // If true it indicates that the task has exceeded the period and is a candidate for execution.
};

class Scheduler {
  public:
    Scheduler(); // Create a queue of tasks.
    ~Scheduler(); // Scheduler destruction.
    //void add(functionPointer function); // Adds a task to the queue that will repeat itself as soon as possible.
    void add(functionPointer function, uint64_t period = 0); // Adds a task to the queue that will repeat itself periodically.
    bool remove(functionPointer function = NULL); // Removes a function from the queue. Returns '0' on success or '1' on failure to find a task with that function. 
    bool setPeriod(functionPointer function, uint64_t period); // Modifies the amount of time between executions of a function. If set to '0', the function runs as soon as possible; in any other case, the function will not run for the first time for the selected period of time.
    void run(); // Each time this function is called, one task in the queue is called, and once it is executed, the pointer is left pointing to the next task in the queue.
  private:
    Task * current; // Pointer to the task that is currently being executed.
    Task * last; // Pointer to the last task in the queue.
};

#endif  // _TASKSCHEDULER_H_

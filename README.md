# TaskScheduler.
Arduino library of a simple task scheduler to orchestrate the call of functions.
## Description.
This library allows to schedule the execution of tasks. It creates a circular queue of tasks that execute periodically. Features:
- **Add a task** to the queue.
- **Remove a task** from the queue.
- **Modify the periodicity** at which a task is executed.
## Usage.
### Libraries.
Include the `TaskScheduler` library. Example:
```cpp
    #include "TaskScheduler.h"
```
### Object instantiation.
Declare an object of class `Scheduler`. For example:
```cpp
    Scheduler task;
```
### Loop.
Declare inside the `loop` function a declaration to the `run()` method (no arguments). For example:
```cpp
    task.run();
```
### Adding and removing tasks.
Tasks can be enqueued to the end of the queue to be executed periodically as soon as possible using the `add` method. Add the name of the function as the first parameter. Example:
```cpp
    task.add(foo);
```
Tasks can also be added to run every set period of time －by default in microseconds－ set in the second parameter (optional) of type `uint_64`. Example:
```cpp
    task.add(foo, 1000);
```
Tasks can be unqueued with the `remove` method. Example:
```cpp
    task.remove(foo);
```
### Change periodicity.
Use the ´setPeriod´ method to change the period at which a task is executed. The first argument must contain the name of the function, the second parameter the new period. Set to '0' if the task must run as soon as possible. Example:
```cpp
    task.setPeriod(foo, 0);
```
### Macros.
```cpp
    #define PRECISION micros
```
Defines the unit of meassurement of time. The possible values are:
1. `micros` (microseconds).

2. `millis` (milliseconds).

```cpp
    #define LOOP_PREVENTION
```
Task can call each other and create a loop, preventing other tasks from being executed. To prevent this situation, enable this feature by defining this macro. It requires no tokens.
## Notes.
A pattern of usage can be to have a couple of functions that mutually add each other and remove themselves from the queue. This can lead to an undesired behaviour that leads to a an infinite loop. As exmplained before, use the `LOOP_PREVENTION` macro for this purpose.

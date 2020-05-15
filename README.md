# TaskScheduler
Arduino library of a simple task scheduler for cooperative multitasking without the use of *interrupt* nor *delay* functions. Each task can queue and unqueue itself, as well as other tasks, on demand. The developer must be aware of what is queued at every given moment for a correct synchronization.
## Description
This library allows to schedule the execution of tasks. It creates a circular queue of tasks that execute *one after another*. Features:
- **Add a task** to the queue.
- **Remove a task** from the queue.
- **Modify the periodicity** at which a task is executed.
The terms *add* and *remove* are used instead of *queue* and *unqueue* to make the abstraction independent of the underlining data structure.
## Files
- **TaskScheduler.h**: c++ header file containing the declaration of two classes: `class Task` and `class Scheduler`. Only the last one needs to be invoqued. *See bellow*: **Usage**.
- **TaskScheduler.cpp**: c++ class code with the implementation of both classes.
- **TaskScheduler.ino**: Arduino project file with and example of a fictional usage (not a real case scenario).
## Usage
### Libraries
Include the `TaskScheduler` library. Example:
```cpp
#include "TaskScheduler.h"
```
**IMPORTANT**: There is a dependency of the `Arduino.h` file.
### Object instantiation
Declare an object of class `Scheduler`. For example:
```cpp
Scheduler task;
```
### Loop
Declare inside the `loop` function a declaration to the `run()` method (no arguments). For example:
```cpp
task.run();
```
### Adding and removing tasks
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
### Change periodicity
Use the ´setPeriod´ method to change the period at which a task is executed. The first argument must contain the name of the function, the second parameter the new period. Set to '0' if the task must run as soon as possible. Example:
```cpp
task.setPeriod(foo, 0);
```
### Macros
```cpp
#define PRECISION micros
```
Defines the unit of meassurement of time. Time related arguments in this library (such as adding tasks or setting their periodicity) depend on the unit here selected. The possible values are:
1. `micros` (microseconds).

2. `millis` (milliseconds).
```cpp
#define LOOP_PREVENTION
```
As tasks can call each other this can eventually lead to and infinite loop in which a couple of task call each other and prevent other tasks from being executed. To prevent this situation enable this feature by defining this macro. It requires no tokens.
## Notes.
A pattern of usage can be to have a couple of functions that mutually add each other and remove themselves from the queue. This can lead to an undesired behaviour that leads to a an infinite loop. As exmplained before, use the `LOOP_PREVENTION` macro for this purpose.

#include "TaskScheduler.h"

Task::Task(functionPointer function, Task * next, uint64_t period) {
  // Inicialization of values.
  this->function = function;
  this->setNext(next);
  this->setPeriod(period);
  #if defined LOOP_PREVENTION // Variable only needed for the feature of loop prevention.
  this->overflow = false;
  #endif
}

Task::~Task() {}

void Task::setNext(Task * next) {
  // Set the task in the parameter as the next task.
  this->next = next;
}

Task* Task::getNext() {
  // Return a pointer to the next task.
  return this->next;
}

uint64_t Task::getTimestamp() {
  // Return the timestamp of last execution.
  return this->timestamp;
}

void Task::setTimestamp() {
  // Set the new timestamp.
  this->timestamp = PRECISION() + this->getPeriod();;
}

uint64_t Task::getPeriod() {
  // Return the number of microseconds between exectuions.
  return this->period;
}

void Task::setPeriod(uint64_t period) {
  // Set the new period.
  this->period = period;
  // Set the new timestamp after the period has been changed.
  if (this->period == 0) this->timestamp = 0;
  else this->setTimestamp();
}

bool Task::isFunction(functionPointer function) {
  // Compare the functions and return if they are the same.
  return (this->function == function);
}

functionPointer Task::getFunction() {
  return this->function;
}

#if defined LOOP_PREVENTION
void Task::flag() {
  this->overflow = true;
}

bool Task::getFlag() {
  return this->overflow;
}
#endif

void Task::run() {
  // Execute the function.
  this->function();
  #if defined LOOP_PREVENTION
  // Unflag the overflow.
  this->overflow = false;
  #endif
}

Scheduler::Scheduler() {
  // Default inicialization.
  this->current = NULL;
  this->last = this->current;
}

Scheduler::~Scheduler() {}

void Scheduler::add(functionPointer function, uint64_t period) {
  // Empty queue: 'current' and 'last' point to the new task.
  if (this->current == NULL) {
    this->current = new Task { function, this->current, period };
    this->current->setNext(this->current);
    this->last = this->current;
  }
  // Rest of cases: queue at the end and rearrange pointers.
  else {
    // Pointer to the first task in the queue.
    Task * temp = this->last->getNext();
    // The pointer currently pointing to the last task will be the one before the last.
    Task * prev = this->last;
    // Create the new task and set it to be the last task in the queue, making it point to the first task in the queue as the next one to follow in the queue.
    this->last = new Task { function, temp, period };
    // Redirect the task before the last to point to the new last.
    prev->setNext(this->last);
  }
}

bool Scheduler::remove(functionPointer function) {
  // Pointer used to navigate across the queue in search of the task with the function. Start by the first task of the queue.
  Task * temp = this->last->getNext();
  // Keep track of the previous task for later reassignation of pointers.
  Task * prev = this->last;
  // The abscense of an argument means that the current task will be removed.
  if (function == NULL) function = this->current->getFunction();
  do {
    // Check if the task was found.
    if (temp->isFunction(function)) {
      // Redirect the 'current' pointer if it is pointing to the task that will be removed.
      if (temp == this->current) {
        // If it is the only task in the queue, then leave the 'current' pointer to null.
        if (temp->getNext() == temp) this->current = NULL;
        // Otherwise make it point to the previous task in the queue.
        else this->current = prev;
      }
      // Do the same if the 'last' pointer is pointing to the task.
      if (temp == this->last) {
        // If it is the only task in the queue, then leave the 'last' pointer to null.
        if (temp->getNext() == temp) this->last = NULL;
        // Otherwise make it point to the previous task in the queue.
        else this->last = temp->getNext();
      }
      // Redirect the previous task to point to the one after the task that will be deleted.
      prev->setNext(temp->getNext());
      // Delete the task.
      delete temp;
      // Success, function found and task removed.
      return 0;
    }
    // Find the next task in the queue.
    prev = temp;
    temp = temp->getNext();
  } while (temp != this->last->getNext());
  // Failure, no task found with that function.
  return 1;
}

bool Scheduler::setPeriod(functionPointer function, uint64_t period) {
  // Pointer used to navigate the queue. Start by the last task.
  Task * temp = this->last;
  // Search a task with the function.
  do {
    // Check if the task was found.
    if (temp->isFunction(function)) {
      // If found, set the new period.
      temp->setPeriod(period);
      // Success.
      return 0;
    }
    // Point to the next task.
    temp = temp->getNext();
  } while (temp != this->last);
  // Failure to find a task with that function.
  return 1;
}

void Scheduler::run() {
  // Verify if it is the right time to run the task.
  if (this->current->getTimestamp() <= PRECISION()) {
    // Timestamp before the execution, otherwise if the task incluse removing the current task it will timestamp the incorrect task.
    this->current->setTimestamp();
    // Run the task.
    this->current->run();
  }
  #if defined LOOP_PREVENTION // Only executed this fragment of code if the macro is defined. This macro may be considered a pre-emptive scheduling.
  // Flag the tasks that have overflown their waiting period and are ready for exectuion.
  Task * temp = this->current;
  // Find the flagged task that has overflown the longest amount of time (if any).
  Task * longest = temp;
  do {
    // The only condition to flag a task is if has waited the expected period of time.
    if (temp->getTimestamp() <= PRECISION()) {
      // If the task was previously flagged, then check if it is the one that has spent the longest amount of time waiting for execution.
      if (temp->getFlag() == true) {
        if (temp->getTimestamp() < longest->getTimestamp()) longest = temp;
      }
      // Otherwise simply flag it as overflown.
      else temp->flag();
    }
    // Examine the next task...
    temp = temp->getNext();
    //... until we return to the current one.
  } while (temp != this->current);
  // If the longest was found, make it the next task to run...
  if (longest != this->current) this->current = longest;
  // ...otherwise move to the next task in the queue.
  else
  #endif
  // If the previous macro is not defined, this line of code will run anyways.
  this->current = this->current->getNext();
}

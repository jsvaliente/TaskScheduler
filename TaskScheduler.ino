/* 
 * Example file with a fictional case for demonstration purposes.
 * The following tasks are called:
 * start(): calls upon the other tasks.
 * dot(): performs an animation printing 3 dots in the terminal console.
 * taskA(): runs once and call itself to be removed from the queue, as well as it calls for taskB() to run a second afterwards.
 * taskB(): prints a message periodically; the period of time between executions decreases in each execution.
 * taskC(): prints a message one second after taskB().
 * taskD(): prints a message at the end.
 * end(): removes all the tasks from the queue.
 * ledOn(): turns on a led and calls for ledOff() to run half a second later.
 * ledOff(): turns off a led and calls for ledOn() to run half a second later.
 */

#include "TaskScheduler.h" // Mandatory step.

#define PRECISION micros // Optional step. The unit of time can be milliseconds or microseconds. Try to change the token of this definiton from 'micros' to 'millis'.
#define LOOP_PREVENTION // Optional step. The loop prevention will avoid two tasks from topping the queue and preventing other tasks from running.

// The following macros are exclusive for this example and is not mandatory for the use of the library.
#if PRECISION == micros
# define SECOND 1000000 // 1 s = 1000000 µs.
# define UNIT "µs"
#elif PRECISION == millis
# define SECOND 1000 // 1 s = 1000 ms.
# define UNIT "ms"
#endif

Scheduler task; // Declaring a 'Scheduler' class object is mandatory.

volatile int second = 0; // This is a counter used exclusively in this example and is not mandatory for the usage of this library.

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // A led light is periodically switched on in this example.
  Serial.begin(9600); // Messages are displayed in the terminal console.
  while (!Serial) {};
  task.add(secondCounter, SECOND); // This task will be permanently running along the program.
  task.add(start, SECOND/2); // This task will kick-off all the other tasks. Now there are 2 tasks in the queue.
}

void loop() {
  task.run(); // It is mandatory to call the 'run()' method in the loop.
}

void start() { // This task prints a message and call another task that will make an animation with 3 dots.
  task.remove(start); // The task removes itself of the queue. The argument in this case if optional, and can be left blank in this kind of cases when a task wants to remove it self, as seen in other cases bellow.
  task.add(dot, SECOND/4);
  Serial.print(" " UNIT);
  printTab(2);
  Serial.print("Sec");
  printTab(1);
  Serial.print("Task");
  timestamp();
  Serial.print("Start of program");
}

void dot() { // Prints the dots periodically to create an animation in the console terminal and when it finishes it calls a couple of tasks.
  static int dots = 0;
  Serial.print(".");
  dots++;
  if (dots == 3) {
    task.remove(); // In this case the task removes itself, but unlike in the task above, no argument is used.
    task.add(ledOn, SECOND/2);
    task.add(taskA);
  }
}

void end() {
  task.remove();
  task.remove(timestamp);
  task.remove(ledOn);
  task.remove(ledOff);
  timestamp();
  Serial.print("End of program.");
}

void taskA() {
  task.remove();
  timestamp();
  Serial.print("Task A: run once. Task B will run in a second.");
  task.add(taskB);
}

void taskB() {
  static int counter = 0;
  const int repetitions = 5;
  if (counter < repetitions) {
    timestamp();
    Serial.print("Task B: run ");
    int times = repetitions - 1 - counter;
    if (times > 0) {
      Serial.print(repetitions - 1 - counter);
      Serial.print(" more");
    }
    else Serial.print("one last");
    Serial.print(" time");
    if (times > 0) {
      if (counter < repetitions - 2) Serial.print("s");
      Serial.print(", next in ");
      Serial.print(repetitions - counter);
      Serial.print(" second");
      if (counter < repetitions - 1) Serial.print("s");
    }
    Serial.print(".");
    task.setPeriod(taskB, (repetitions - counter) * SECOND);
    task.add(taskC, SECOND);
  }
  else {
    task.remove();
    timestamp();
    Serial.print("Call task D.");
    task.add(taskD);
  }
  counter++;
}

void taskC() {
  timestamp();
  Serial.print("Task C: run 1 second after task B.");
  task.remove();
}

void taskD() {
  timestamp();
  Serial.print("Task D: Run once at the end.");
  task.remove(taskD);
  task.add(end);
}

void timestamp() {
  Serial.println();
  Serial.print(" [");
  Serial.print(PRECISION());
  Serial.print(" ] ");
  printTab(1);
  Serial.print("(");
  Serial.print(second);
  Serial.print(")");
  printTab(1);
}

void secondCounter() {
  second++;
}

void ledOn() { // This and the next task could lead to a loop, as they mutually call each other.
  task.remove();
  digitalWrite(LED_BUILTIN, HIGH);
  timestamp();
  Serial.print("Led on.");
  task.add(ledOff, SECOND/2);
}

void ledOff() {
  task.remove();
  digitalWrite(LED_BUILTIN, LOW);
  timestamp();
  Serial.print("Led off.");
  task.add(ledOn, SECOND/2);
}

void printTab(int number) {
  const char TAB = '\t';
  while (number > 0) {
    Serial.print(TAB);
    number--;
  }
}

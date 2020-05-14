#include "TaskScheduler.h"

#define PRECISION micros
#define LOOP_PREVENTION

#if PRECISION == micros
# define SECOND 1000000 // 1 s = 1000000 µs.
# define UNIT "µs"
#elif PRECISION == millis
# define SECOND 1000 // 1 s = 1000 ms.
# define UNIT "ms"
#endif

Scheduler task;

volatile int second = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {};
  task.add(secondCounter, SECOND);
  task.add(start, SECOND/2);
}

void loop() {
  task.run();
}

void start() {
  task.remove(start);
  task.add(dot, SECOND/4);
  Serial.print(" " UNIT);
  printTab(2);
  Serial.print("Sec");
  printTab(1);
  Serial.print("Task");
  timestamp();
  Serial.print("Start of program");
}

void dot() {
  static int dots = 0;
  Serial.print(".");
  dots++;
  if (dots == 3) {
    task.remove(dot);
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

void ledOn() {
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

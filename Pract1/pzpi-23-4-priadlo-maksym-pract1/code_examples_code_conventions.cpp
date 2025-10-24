// Поганий приклад
#include "my_header.h"
#include <iostream>
#include <vector>
#include <stdio.h>

// Гарний приклад
#include "project/my_header.h"   // відповідний заголовок
#include <stdio.h>               // C headers
#include <iostream>              // C++ STL
#include <vector>
#include "third_party/json/json.h"  // сторонні бібліотеки
#include "project/utils/log.h"      // власні заголовки


// Поганий приклад
int MyVariable = 0;
void DoSomethingCool();
const int PI = 3.14;
class my_class_name {};

// Гарний приклад
int my_variable = 0;
void DoSomethingCool();
const int kPi = 3.14;
class MyClassName {};

// Поганий приклад
int tmp;
void Proc();

 // Гарний приклад
int temporary_value;
void ProcessInputData();

// Поганий приклад
if (is_valid) {
    DoSomething();
}

// Гарний приклад
if (is_valid) {
  DoSomething();
}


// Поганий приклад
if (condition)
{
  DoSomething();
}

// Гарний приклад
if (condition) {
  DoSomething();
}

// Поганий приклад
i++; // інкрементуємо i

// Гарний приклад
// Використовуємо індекс для переходу до наступного елемента списку
i++;

// Поганий приклад
using namespace std;
cout << "Hello";

// Гарний приклад
#include <iostream>
using std::cout;
cout << "Hello";



class StringWrapper {
 public:
  StringWrapper(const char* s) : str_(s) {}
 private:
  std::string str_;
};

void PrintString(StringWrapper s);

PrintString("Hello");  


// Поганий приклад
int i;
for (i = 0; i < numbers.size(); ++i) {
  int sum = 0;  // занадто далеко від використання
}

// Гарний приклад
for (int i = 0; i < numbers.size(); ++i) {
  int sum = 0;
  sum += numbers[i];
}


// Поганий приклад
class StringWrapper {
 public:
  StringWrapper(const char* s) : str_(s) {}
 private:
  std::string str_;
};

void PrintString(StringWrapper s);

PrintString("Hello");  


// Гарний приклад
class StringWrapper {
 public:
  explicit StringWrapper(const char* s) : str_(s) {}
 private:
  std::string str_;
};

void PrintString(StringWrapper s);
PrintString(StringWrapper("Hello"));  



project/
├── base/
│   ├── logging.h
│   └── logging.cc
├── utils/
│   ├── file_reader.h
│   └── file_reader.cc
└── main.cc

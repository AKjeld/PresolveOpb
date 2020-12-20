#include "Student.h"
#include <iostream>
#include <papilo/misc/MultiPrecision.hpp>

int main(int argc, char *argv[]){
   Student s("Joe");
   papilo::Quad test = 12323;
   std::cout << test << std::endl;
   s.display();
   return 0;
}

#ifndef SECRETMATHS_H
#define SECRETMATHS_H

#include <Arduino.h>

class SECRETMaths {
  public:
    // Constructor
    SECRETMaths();

    // Adds two integer numbers and returns the result
    int add(int a, int b);

    // Multiplies two integer numbers and returns the result
    int multiply(int a, int b);
};

#endif


#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stddef.h>

double Addition(size_t count, const double args[]);
double Subtraction(size_t count, const double args[]);
double Multiplication(size_t count, const double args[]);
int Division(double arg_1, double arg_2, double* result);
double Max(size_t count, const double args[]);
double Min(size_t count, const double args[]);

#endif

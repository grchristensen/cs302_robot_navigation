#pragma once

#include <iostream>

enum class Sign { //Simple enum to help determine if the number is positive or negative, based off of previous quadrants on the map
  Positive,
  Negative,
};

std::ostream& operator<<(std::ostream& os, const Sign sign);

int operator+(const Sign sign, const int number);
int operator+(const int number, const Sign sign);
double operator+(const Sign sign, const double number);
double operator+(const double number, const Sign sign);
int operator*(const Sign sign, const int number);
int operator*(const int number, const Sign sign);
double operator*(const Sign sign, const double number);
double operator*(const double number, const Sign sign);

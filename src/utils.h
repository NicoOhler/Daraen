#pragma once

#include "definitions.h"
#include "Logger.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <mutex>

// board utilities
constexpr uint64 modPow2(uint64 n, uint64 m)
{
    return n & (m - 1); // fast modulo for power of 2
}
Position getRow(Position square);
Position getCol(Position square);
Position getSquareIndex(const std::string &square);
Piece getPieceFromSymbol(char symbol);
std::string getSquareName(Position square);

// input/output
std::string readLineFromStdin();
void print(const std::string &message, bool newline = true);

// string manipulation
bool contains(const std::string &str, const std::string &substr);
bool getBoolFromString(const std::string &str);
std::string getFormattedPercentage(uint64 numerator, uint64 denominator, int precision = 3);
std::string getFormattedInteger(int value);
std::string convertMillisecondsToTimeString(Milliseconds milliseconds);
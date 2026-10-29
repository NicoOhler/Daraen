#include "utils.h"
#include "Evaluation.h"

Position getRow(Position square)
{
    return square / NUM_COLS;
}

Position getCol(Position square)
{
    return modPow2(square, NUM_COLS);
}

std::string getSquareName(Position square)
{
    if (square < 0 || square >= NUM_SQUARES)
        throw std::invalid_argument("Invalid square");
    return char('a' + getCol(square)) + std::to_string(getRow(square) + 1);
}

Position getSquareIndex(const std::string &square)
{
    if (square.length() != 2)
        throw std::invalid_argument("Invalid square");
    char col = square[0];
    char row = square[1];
    if (col < 'a' || col > 'h' || row < '1' || row > '8')
        throw std::invalid_argument("Invalid square");
    return NUM_COLS * (row - '1') + (col - 'a');
}

Piece getPieceFromSymbol(char symbol)
{
    switch (symbol)
    {
    case WHITE_PAWN_SYMBOL:
        return WHITE_PAWN;
    case WHITE_ROOK_SYMBOL:
        return WHITE_ROOK;
    case WHITE_KNIGHT_SYMBOL:
        return WHITE_KNIGHT;
    case WHITE_BISHOP_SYMBOL:
        return WHITE_BISHOP;
    case WHITE_QUEEN_SYMBOL:
        return WHITE_QUEEN;
    case WHITE_KING_SYMBOL:
        return WHITE_KING;
    case BLACK_PAWN_SYMBOL:
        return BLACK_PAWN;
    case BLACK_ROOK_SYMBOL:
        return BLACK_ROOK;
    case BLACK_KNIGHT_SYMBOL:
        return BLACK_KNIGHT;
    case BLACK_BISHOP_SYMBOL:
        return BLACK_BISHOP;
    case BLACK_QUEEN_SYMBOL:
        return BLACK_QUEEN;
    case BLACK_KING_SYMBOL:
        return BLACK_KING;
    default:
        throw std::invalid_argument("Invalid piece symbol: " + std::string(1, symbol));
    }
}

uint64 modPow2(uint64 n, uint64 m)
{
    return n & (m - 1); // fast modulo for power of 2
}

std::string getFormattedPercentage(uint64 numerator, uint64 denominator, int precision)
{
    if (denominator == 0)
        return "0%";
    double percentage = 100 * static_cast<double>(numerator) / denominator;
    std::stringstream string_stream;
    string_stream << std::fixed << std::setprecision(2) << percentage << "%";
    return string_stream.str();
}

std::string getFormattedInteger(int value)
{
    std::string digits = std::to_string(value);
    const bool is_negative = digits.front() == '-';
    const std::size_t first_digit = is_negative ? 1 : 0;

    for (std::size_t position = digits.size(); position > first_digit + 3; position -= 3)
        digits.insert(position - 3, 1, ',');

    return digits;
}

void print(const std::string &message, bool newline)
{
    std::cout << message;
    if (newline)
        std::cout << std::endl;
}

std::string readLineFromStdin()
{
    std::string line;
    std::getline(std::cin, line);
    return line;
}

bool contains(const std::string &str, const std::string &substr)
{
    return str.find(substr) != std::string::npos;
}

bool getBoolFromString(const std::string &str)
{
    if (str == "true" || str == "1")
        return true;
    else if (str == "false" || str == "0")
        return false;
    else
        throw std::invalid_argument("Invalid boolean string: " + str);
}

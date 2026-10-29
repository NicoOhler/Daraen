#include "Bitboard.h"

std::vector<bool> getBits(const Bitboard &board)
{
    std::vector<bool> bits(NUM_SQUARES);
    for (int i = 0; i < NUM_SQUARES; i++)
        bits[i] = board & (1ULL << i);
    return bits;
}

void printBitboard(const Bitboard &board, const std::string &title)
{
    std::vector<bool> bits = getBits(board);
    std::cout << title << std::endl;
    for (Position row = NUM_ROWS - 1; row >= 0; row--)
    {
        std::cout << row + 1 << " ";
        for (Position col = 0; col < NUM_COLS; col++)
            std::cout << (bits[row * NUM_COLS + col] ? "1" : "0");
        std::cout << std::endl;
    }
    std::cout << "  ";
    for (Position col = 0; col < NUM_COLS; col++)
        std::cout << (char)('a' + col);
    std::cout << std::endl
              << std::endl;
}
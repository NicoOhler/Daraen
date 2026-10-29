#pragma once
#include "definitions.h"
#include "Logger.h"
#include "MoveGenerator.h"

// material values
constexpr Score PAWN_VALUE = 100;
constexpr Score KNIGHT_VALUE = 300;
constexpr Score BISHOP_VALUE = 330;
constexpr Score ROOK_VALUE = 500;
constexpr Score QUEEN_VALUE = 900;
constexpr Score KING_VALUE = 500000;
constexpr Score DELTA_VALUE = 800;
constexpr Score ISOLATED_PAWN_VALUE = -50;
constexpr Score DOUBLED_PAWN_VALUE = -50;
constexpr Score BLOCKED_PAWN_VALUE = -50;
constexpr Score DRAW_VALUE = -200;
constexpr Score MATE_VALUE = 6000000;
constexpr Score MIN_MATE_VALUE = MATE_VALUE - MAX_SEARCH_DEPTH;

constexpr Score PIECE_VALUES[NUM_PIECES] = {
    PAWN_VALUE,
    PAWN_VALUE,
    KNIGHT_VALUE,
    KNIGHT_VALUE,
    BISHOP_VALUE,
    BISHOP_VALUE,
    ROOK_VALUE,
    ROOK_VALUE,
    QUEEN_VALUE,
    QUEEN_VALUE,
    KING_VALUE,
    KING_VALUE,
};

class Evaluation
{
private:
    MoveGenerator move_generator = MoveGenerator::getInstance();
    Score evaluateKingSafety(Board board);
    Score evaluateMaterial(Board board);
    Score evaluateMobility(Board board);
    Score evaluatePawnStructure(Board board);

public:
    Score evaluate(Board board);
};
#pragma once

#include "Move.h"
#include "definitions.h"

#define KILLER_MOVES_ENABLED true
#define GLOBAL_HISTORY_ENABLED false

#define MAX_HISTORY 32768

// move ordering values
const Score PROMOTION_VALUE = 400000;
const Score CAPTURE_VALUE = 100000;
const Score PRIMARY_KILLER_VALUE = CAPTURE_VALUE / 10 * 9;
const Score SECONDARY_KILLER_VALUE = CAPTURE_VALUE / 10 * 8;

struct UndoHistory
{
    UndoInfo undo_infos[MAX_SEARCH_DEPTH];
    int size = 0;

    void push(UndoInfo undo);
    UndoInfo pop();
    void clear();
};

class History
{
public:
    History();

    void update(const Board &board, Move move, int ply);
    Score getMoveScore(const Board &board, Move move, int ply);
    void clear();

    void pushUndo(UndoInfo undo);
    UndoInfo popUndo();

    Move killer_moves[MAX_PV_SEARCH_DEPTH][NUM_KILLER_MOVES];

private:
    UndoHistory undos;
    // Score global_history[NUM_SQUARES][NUM_SQUARES] = {0};                              // [from][to]
    // Score follow_up_move_history[NUM_PIECES][NUM_SQUARES][NUM_PIECES][NUM_SQUARES] = {0}; // [piece][to][enemy_piece][enemy_to]
    Score global_history[NUM_PIECES][NUM_SQUARES] = {0}; // [piece][to]
    // Score counter_move_history[NUM_PIECES][NUM_SQUARES][NUM_PIECES][NUM_SQUARES] = {0}; // [enemy_piece][enemy_to][piece][to]

    void updateKillerMove(Move move, int ply);
    void updateGlobalHistory(Piece piece, Position to, int ply, bool penalty = false);
};
#pragma once

#include "Move.h"
#include "Evaluation.h"
#include "definitions.h"

const bool KILLER_MOVES_ENABLED = true;
const bool GLOBAL_HISTORY_ENABLED = false;
const bool COUNTER_MOVES_ENABLED = false;

// move ordering values
const Score MAX_HISTORY = 16384;
const Score PROMOTION_MOVE_SCORE = 400000;
const Score CAPTURE_MOVE_SCORE = 200000;
const Score PRIMARY_KILLER_MOVE_SCORE = CAPTURE_MOVE_SCORE / 10 * 9;
const Score SECONDARY_KILLER_MOVE_SCORE = CAPTURE_MOVE_SCORE / 10 * 8;
const Score COUNTER_MOVE_SCORE = CAPTURE_MOVE_SCORE / 10;
const Score QUIET_MOVE_SCORE = 1;

class MoveOrder
{
public:
    MoveOrder();
    void clear();

    // move ordering methods
    void calculateMoveScores(const Board &board, MoveList &moves, Move move, int ply = -1);
    Move pickBestMove(MoveList &moves, int start_index);
    void trackQuietMove(const Board &board, Move move, int ply, int remaining_depth, bool is_cutoff);
    Score getQuietMoveScore(const Board &board, Move move, int ply);

    // history methods
    void pushMoveToHistory(Move move, UndoInfo undo);
    UndoInfo popMoveFromHistory();
    Move peekAtHistory() const;

private:
    // previous moves
    int num_moves;
    Position previous_move_from;
    Position previous_move_to;
    Move moves[MAX_SEARCH_DEPTH];
    UndoInfo undo_infos[MAX_SEARCH_DEPTH];

    // move ordering heuristics
    Move killer_moves[MAX_PV_SEARCH_DEPTH][NUM_KILLER_MOVES];           // [ply][0..1]
    Score global_history[NUM_PIECES][NUM_SQUARES] = {QUIET_MOVE_SCORE}; // [piece][to]
    Move counter_moves[NUM_SQUARES][NUM_SQUARES];                       // [from][to]
    Score depth_bonus[MAX_PV_SEARCH_DEPTH];                             // [depth]
    Score depth_bonus_normalized[MAX_PV_SEARCH_DEPTH];                  // [depth]

    void storeKillerMove(Move move, int ply);
    void storeCounterMove(Move move);
    void updateGlobalHistory(Piece piece, Position to, int remaining_depth, bool penalty);
};
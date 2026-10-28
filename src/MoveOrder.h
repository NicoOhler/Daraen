#pragma once

#include "Move.h"
#include "Evaluation.h"
#include "definitions.h"

#define KILLER_MOVES_ENABLED true
#define GLOBAL_HISTORY_ENABLED false

#define MAX_HISTORY 16384

// move ordering values
const Score PROMOTION_MOVE_SCORE = 400000;
const Score CAPTURE_MOVE_SCORE = 200000;
const Score PRIMARY_KILLER_MOVE_SCORE = CAPTURE_MOVE_SCORE / 10 * 9;
const Score SECONDARY_KILLER_MOVE_SCORE = CAPTURE_MOVE_SCORE / 10 * 8;
const Score QUIET_MOVE_SCORE = 1;

struct MoveHistory
{
    Move moves[MAX_SEARCH_DEPTH];
    UndoInfo undo_infos[MAX_SEARCH_DEPTH];
    int size = 0;

    void push(Move move, UndoInfo undo);
    UndoInfo pop();
    Move peek() const;
    void clear();
};

class MoveOrder
{
public:
    MoveOrder();
    Move killer_moves[MAX_PV_SEARCH_DEPTH][NUM_KILLER_MOVES];

    void calculateMoveScores(const Board &board, MoveList &moves, Move move, int ply = -1);
    Move pickBestMove(MoveList &moves, int start_index);
    void trackQuietMove(const Board &board, Move move, int ply, int remaining_depth, bool is_cutoff);
    Score getQuietMoveScore(const Board &board, Move move, int ply);
    void pushMove(Move move, UndoInfo undo);
    UndoInfo popMove();
    void clear();

private:
    MoveHistory undos;
    Score global_history[NUM_PIECES][NUM_SQUARES] = {QUIET_MOVE_SCORE}; // [piece][to]
    Score depth_bonus[MAX_PV_SEARCH_DEPTH];                             // [depth]
    Score depth_bonus_normalized[MAX_PV_SEARCH_DEPTH];                  // [depth]
    void storeKillerMove(Move move, int ply);
    void updateGlobalHistory(Piece piece, Position to, int remaining_depth, bool penalty);
};
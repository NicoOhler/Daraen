#pragma once
#include "utils.h"
#include "definitions.h"
#include "Evaluation.h"
#include "MoveGenerator.h"
#include "ZobristHash.h"
#include "Bitboard.h"

typedef enum
{
    EXACT,
    LOWER_BOUND,
    UPPER_BOUND
} BoundType;

struct TranspositionEntry
{
    Hash hash = 0;
    Score score = 0;
    Move best_move = NULL_MOVE;
    int remaining_depth = 0;
    BoundType type = EXACT;
    // ? alpha beta

    TranspositionEntry() = default;
};

class TranspositionTable
{
public:
    TranspositionTable(int megabytes = TRANSPOSITION_TABLE_SIZE_MB);
    void store(Hash hash, Score score, Move best_move, int remaining_depth, BoundType type);
    TranspositionEntry *probe(Hash hash);
    void clear();
    void printStatistics();

    static Score computeTranspositionScore(Score score, int ply)
    {
        if (score >= MIN_MATE_VALUE)
            score += ply;
        else if (score <= -MIN_MATE_VALUE)
            score -= ply;
        return score;
    }

    static Score computeOriginalScore(Score score, int ply)
    {
        if (score >= MIN_MATE_VALUE)
            score -= ply;
        else if (score <= -MIN_MATE_VALUE)
            score += ply;
        return score;
    }

    static BoundType getBoundType(Score score, Score lower_bound, Score upper_bound)
    {
        BoundType type = EXACT;
        if (score <= lower_bound)
            type = UPPER_BOUND;
        else if (score >= upper_bound)
            type = LOWER_BOUND;
        return type;
    }

private:
    uint64 table_size;
    TranspositionEntry *table;

    // statistics
    uint64 hash_store_attempts = 0;
    uint64 hash_store_updates = 0;
    uint64 hash_store_collisions = 0;
    uint64 hash_probe_collisions = 0;
    uint64 hash_probe_hits = 0;
    uint64 hash_probe_misses = 0;
};

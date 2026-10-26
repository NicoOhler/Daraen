#include "History.h"

History::History()
{
}

void History::update(const Board &board, Move move, int ply)
{
    // only store quiet moves
    // others are already prioritized in move ordering
    if (!move.isQuiet(board))
        return;

    if (KILLER_MOVES_ENABLED)
        updateKillerMove(move, ply);
    if (GLOBAL_HISTORY_ENABLED)
        updateGlobalHistory(board.pieces[move.from], move.to, ply);
}

Score History::getMoveScore(const Board &board, Move move, int ply)
{
    if (KILLER_MOVES_ENABLED)
    {
        if (move == killer_moves[ply][0])
            return PRIMARY_KILLER_VALUE;
        else if (move == killer_moves[ply][1])
            return SECONDARY_KILLER_VALUE;
    }

    if (GLOBAL_HISTORY_ENABLED)
        return global_history[board.pieces[move.from]][move.to];
    return 0;
}

void History::clear()
{
    undos.clear();
    global_history[NUM_PIECES][NUM_SQUARES] = {0};
    for (int ply = 0; ply < MAX_PV_SEARCH_DEPTH; ply++)
        for (int i = 0; i < NUM_KILLER_MOVES; i++)
            killer_moves[ply][i] = NULL_MOVE;
}

void History::pushUndo(UndoInfo undo)
{
    undos.push(undo);
}

UndoInfo History::popUndo()
{
    return undos.pop();
}

void History::updateKillerMove(Move move, int ply)
{
    // least recently used replacement strategy
    if (move == killer_moves[ply][0])
        return;
    killer_moves[ply][1] = killer_moves[ply][0];
    killer_moves[ply][0] = move;
}

void History::updateGlobalHistory(Piece piece, Position to, int ply, bool penalty)
{
    // history gravity formula
    Score bonus = 300 * ply - 250;
    if (penalty)
        bonus = -bonus;
    if (bonus > MAX_HISTORY)
        bonus = MAX_HISTORY;
    else if (bonus < -MAX_HISTORY)
        bonus = -MAX_HISTORY;
    global_history[piece][to] += bonus - global_history[piece][to] * std::abs(bonus) / MAX_HISTORY;
}

void UndoHistory::push(UndoInfo undo)
{
    undo_infos[size++] = undo;
}

UndoInfo UndoHistory::pop()
{
    return undo_infos[--size];
}

void UndoHistory::clear()
{
    size = 0;
}
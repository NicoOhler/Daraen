#include "MoveOrder.h"

MoveOrder::MoveOrder()
{
    clear();
    for (int depth = 0; depth < MAX_PV_SEARCH_DEPTH; depth++)
    {
        // arbitrary bonus formula, can be tuned
        Score bonus = depth * depth;                                                        // encourages deeper cutoffs that save more time
        depth_bonus[depth] = std::max(std::min(bonus, MAX_HISTORY), -MAX_HISTORY);          // clamped bonus
        depth_bonus_normalized[depth] = std::abs(depth_bonus[depth]) / MAX_PV_SEARCH_DEPTH; // normalized bonus
    }
}

void MoveOrder::calculateMoveScores(const Board &board, MoveList &moves, Move best_move, int ply)
{
    bool best_move_provided = best_move != NULL_MOVE;
    bool en_passant_possible = board.en_passant != NO_EN_PASSANT;
    Piece own_pawn = board.white_to_move ? WHITE_PAWN : BLACK_PAWN;
    Piece enemy_pawn = board.white_to_move ? BLACK_PAWN : WHITE_PAWN;

    for (int i = 0; i < moves.size; i++)
    {
        Move move = moves.moves[i];
        // search previously best move first, which will likely cause a beta cutoff
        if (best_move_provided && move == best_move)
        {
            moves.scores[i] = POS_INFINITY;
            continue;
        }

        // then promotions
        if (move.isPromotion())
        {
            moves.scores[i] = PIECE_VALUES[move.getPromotionPiece(board.white_to_move)] + PROMOTION_MOVE_SCORE;
            continue;
        }

        // then captures according to Most Valuable Victim – Least Valuable Attacker
        // i.e., prioritize captures of high value with low value pieces
        Piece moving_piece = board.pieces[move.from];
        Piece captured_piece = board.pieces[move.to];
        if (en_passant_possible && move.to == board.en_passant && moving_piece == own_pawn)
            moves.scores[i] = PIECE_VALUES[enemy_pawn] - PIECE_VALUES[own_pawn] + CAPTURE_MOVE_SCORE;
        else if (captured_piece != EMPTY)
            moves.scores[i] = PIECE_VALUES[captured_piece] - PIECE_VALUES[moving_piece] + CAPTURE_MOVE_SCORE;
        // then quiet non-capture moves
        else
            moves.scores[i] = getQuietMoveScore(board, move, ply);
    }
}

Move MoveOrder::pickBestMove(MoveList &moves, int start_index)
{
    // incremental selection sort
    // find best move
    int best_move_index = start_index;
    Score best_score = moves.scores[start_index];

    for (int i = start_index + 1; i < moves.size; i++)
    {
        Score score = moves.scores[i];
        if (score > best_score)
        {
            best_score = score;
            best_move_index = i;
        }
    }

    // move best move to the front of the list
    if (best_move_index != start_index)
    {
        std::swap(moves.moves[start_index], moves.moves[best_move_index]);
        std::swap(moves.scores[start_index], moves.scores[best_move_index]);
    }

    return moves.moves[start_index];
}
void MoveOrder::trackQuietMove(const Board &board, Move move, int ply, int remaining_depth, bool is_cutoff)
{
    if (!move.isQuiet(board))
        return;

    if (KILLER_MOVES_ENABLED && is_cutoff)
        storeKillerMove(move, ply);
    if (GLOBAL_HISTORY_ENABLED)
        updateGlobalHistory(board.pieces[move.from], move.to, remaining_depth, is_cutoff);
}

void MoveOrder::storeKillerMove(Move move, int ply)
{
    // least recently used replacement strategy
    if (move == killer_moves[ply][0])
        return;
    killer_moves[ply][1] = killer_moves[ply][0];
    killer_moves[ply][0] = move;
}

void MoveOrder::updateGlobalHistory(Piece piece, Position to, int remaining_depth, bool penalty)
{
    Score bonus = depth_bonus[remaining_depth];
    if (penalty)
        bonus = -bonus;
    // history gravity formula => prevents overflow, larger bonus for unexpected cutoffs, smaller bonus for expected cutoffs^
    global_history[piece][to] += bonus - global_history[piece][to] * depth_bonus_normalized[remaining_depth];
}

Score MoveOrder::getQuietMoveScore(const Board &board, Move move, int ply)
{
    Score score = QUIET_MOVE_SCORE;
    if (ply < 0)
        return score;

    if (KILLER_MOVES_ENABLED)
    {
        if (move == killer_moves[ply][0])
            score += PRIMARY_KILLER_MOVE_SCORE;
        else if (move == killer_moves[ply][1])
            score += SECONDARY_KILLER_MOVE_SCORE;
    }

    if (GLOBAL_HISTORY_ENABLED)
        score += global_history[board.pieces[move.from]][move.to];
    return score;
}

void MoveOrder::clear()
{
    undos.clear();
    global_history[NUM_PIECES][NUM_SQUARES] = {0};
    for (int ply = 0; ply < MAX_PV_SEARCH_DEPTH; ply++)
        for (int i = 0; i < NUM_KILLER_MOVES; i++)
            killer_moves[ply][i] = NULL_MOVE;
}

void MoveOrder::pushMove(Move move, UndoInfo undo)
{
    undos.push(move, undo);
}

UndoInfo MoveOrder::popMove()
{
    return undos.pop();
}

void MoveHistory::push(Move move, UndoInfo undo)
{
    moves[size] = move;
    undo_infos[size++] = undo;
}

UndoInfo MoveHistory::pop()
{
    return undo_infos[--size];
}

Move MoveHistory::peek() const
{
    return moves[size - 1];
}

void MoveHistory::clear()
{
    size = 0;
}
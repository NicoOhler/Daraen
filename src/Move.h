#pragma once
#include "utils.h"
#include "Board.h"
#include <vector>

struct Move
{
    Position from;
    Position to;
    Promotion promotion = NO_PROMOTION;

    Move(Position from = 0, Position to = 0, Promotion promotion = NO_PROMOTION)
        : from(from), to(to), promotion(promotion) {}
    Move(Move16 encoded_move);
    Move(std::string move_string);

    bool isQuiet(const Board &board) const;
    bool isCastling(bool is_king_move) const;
    bool isUndo() const
    {
        return promotion == UNDO;
    }
    bool isPromotion() const
    {
        return promotion & (PROMOTE_TO_QUEEN | PROMOTE_TO_ROOK | PROMOTE_TO_BISHOP | PROMOTE_TO_KNIGHT);
    }
    Piece getPromotionPiece(bool white_to_move)
    {
        return promotion + (WHITE * white_to_move);
    }

    std::string toString() const;
    bool operator==(const Move &rhs) const
    {
        return from == rhs.from && to == rhs.to && promotion == rhs.promotion;
    }
    bool operator!=(const Move &rhs) const
    {
        return from != rhs.from || to != rhs.to || promotion != rhs.promotion;
    }
};

const Move UNDO_MOVE = Move{0, 0, UNDO};
const Move NULL_MOVE = Move{0, 0, NO_PROMOTION};

struct MoveList
{
    Move moves[MAX_MOVES];
    Score scores[MAX_MOVES];
    int size = 0;

    void append(Move move)
    {
        moves[size++] = move;
    }

    void clear()
    {
        size = 0;
    }

    bool empty()
    {
        return size == 0;
    }
};

struct UndoInfo
{
    Piece captured_piece;
    Position en_passant;
    CastlingRights castling_rights;
    HalfMoveClock half_move_clock;
};

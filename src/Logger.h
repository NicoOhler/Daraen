#pragma once
#include "definitions.h"
#include "iostream"
#include <fstream>

typedef int LogType;

const std::string LOG_FILE = "log.txt"; // does not write to file if empty string
constexpr bool LOG_TO_CONSOLE = true;
constexpr bool APPEND_TO_FILE = false;

constexpr LogType DISABLED = 0x0;
constexpr LogType ENABLED = 0x1;

constexpr LogType CHESS_BOARD = ENABLED;
constexpr LogType MOVE_GENERATOR = DISABLED;
constexpr LogType PAWN_MOVE = DISABLED;
constexpr LogType KNIGHT_MOVE = DISABLED;
constexpr LogType BISHOP_MOVE = DISABLED;
constexpr LogType ROOK_MOVE = DISABLED;
constexpr LogType QUEEN_MOVE = DISABLED;
constexpr LogType KING_MOVE = DISABLED;
constexpr LogType REMOVE_ILLEGAL_MOVES = DISABLED;
constexpr LogType REMAINING_MOVES = DISABLED;
constexpr LogType PERFT = ENABLED;
constexpr LogType SEARCH = DISABLED;
constexpr LogType HISTORY = ENABLED;
constexpr LogType MOVE_ORDER = ENABLED;
constexpr LogType SEARCH_DEPTHS = ENABLED;
constexpr LogType FEN = DISABLED;
constexpr LogType TIME = ENABLED;
constexpr LogType EVALUATION = DISABLED;
constexpr LogType BENCHMARK = ENABLED;
constexpr LogType TRANSPOSITION_TABLE = ENABLED;
constexpr LogType TRANSPOSITION_TABLE_MATCH = DISABLED;
constexpr LogType ENGINE_SETTINGS = DISABLED;
constexpr LogType OPENING_BOOK = DISABLED;
constexpr LogType OPENING_BOOK_MOVE = DISABLED;
constexpr LogType UI = ENABLED;

void log(LogType logType, const std::string &message);

class Logger
{
private:
    static Logger *instance;
    bool print_to_console;
    std::ofstream file;
    std::string filename;
    Logger();

public:
    Logger(Logger &other) = delete;
    void operator=(const Logger &) = delete;
    static Logger *GetInstance();
    void log(LogType logType, const std::string &message);
    ~Logger();
};

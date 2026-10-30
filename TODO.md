### TODO
+ disable NMP if own position is already quite bad
+ evaluation using NNUE
    + read up on them
    + check for state of the art alternatives
    + define its input output formats
    + fetch/create dataset from played games
    + setup training environment 
+ reduce sizeof(Move)
    + use single 16bit uint for from (6 bits) + to (6 bits) + promotion (3 bits)
+ staged move generation/ordering/exploration
    + retrieve best previous move and check it
    + generate captures and promotions
    + promotions
    + good captures
    + killer moves
    + generate quiet moves
    + quiet moves
    + bad captures
+ Static Exchange Evaluation (SEE)
+ setup Sequential Probability Ratio Test
    + evaluates whether a patch makes the engine stronger compared to live version
    + e.g. using fast-chess
+ transposition table aging
    + deep nodes from early in the game lose relevance later in the game
    + e.g. remove entries that have not been used for a while

### Minor TODOs
+ efficiently generate evasion moves if king is in check
+ transposition table with bucket system instead of single entry per index
    + reduces collisions
    + e.g. most recent + highest depth 
+ dynamic delta pruning

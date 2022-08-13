#ifndef __shogiboard
#define __shogiboard

#include <vector>
#include <utility>
#include <map>
#include <stack>

#define SUJI_MAX 11
#define DAN_MAX 11

enum  KOMAINF{
    OUT_OF_BOARD = 64,
    EMPTY  = 0,
    FU = 1,
    KY = 2,
    KE = 3,
    GI = 4,
    KI = 5,
    KA = 6,
    HI = 7,
    OU = 8,
    PROMOTED = 8,
    TO = PROMOTED + FU,
    NY = PROMOTED + KY,
    NK = PROMOTED + KE,
    NG = PROMOTED + GI,
    UM = PROMOTED + KA,
    RY = PROMOTED + HI,
    ENEMY = 16,
    EFU = ENEMY + FU,
    EKY = ENEMY + KY,
    EKE = ENEMY + KE,
    EGI = ENEMY + GI,
    EKI = ENEMY + KI,
    EKA = ENEMY + KA,
    EHI = ENEMY + HI,
    EOU = ENEMY + OU,
    ETO = ENEMY + TO,
    ENY = ENEMY + NY,
    ENK = ENEMY + NK,
    ENG = ENEMY + NG,
    EUM = ENEMY + UM,
    ERY = ENEMY + RY
};

enum class Player{
    PLAYER1,
    PLAYER2
};



void init_move_vec();

int vect_len(std::pair<int,int> v);


class shougiEngine{
private:
    // KOMAINF Board[DAN_MAX][SUJI_MAX]; // 実際の盤面の数字のつけ方と若干違うんですよね. baord[漢数字][ラテン数字]
    std::vector<std::vector<KOMAINF>> Board;
    std::vector<std::vector<int>> Hand;
    Player current_player;
    int cummulative_turn;
    std::stack<std::vector<std::vector<KOMAINF>>> kifu;
public:
    shougiEngine(); // constructor. in charge of initializing
    void init_game(); // initialize game
    Player get_current_player(); 
    bool move(Player,std::pair<int,int> from, std::pair<int,int> to);
    void start(); // start a game
    bool is_oute(std::vector<std::vector<KOMAINF>> Board);
    bool is_valid_move(Player,std::pair<int,int> from, std::pair<int,int> to);
    bool is_valid_board(std::vector<std::vector<KOMAINF>> Board, std::pair<int,int> from, std::pair<int,int> to); // 現局面に変更を加えた局面がゲームとして続いているか
    bool is_nari(Player,std::pair<int,int> from,std::pair<int,int> to);
    bool is_enemy(Player,std::pair<int,int>); // pair<dan,suji>
    bool is_in_enemy_field(Player,std::pair<int,int>);
    bool is_end();
    bool is_in_the_middle(std::pair<int,int> from,std::pair<int,int> to);
    bool is_koma_inbetween(std::pair<int,int> from,std::pair<int,int> to);
    bool is_in_hand(Player,int);
    std::pair<int,int> input_from(Player player);
    std::pair<int,int> input_to(Player player);
    int num_of_avoiding_moves();
    void possible_state(std::vector<std::vector<KOMAINF>> Board);
    void printBoard();
    double evaluate(); // 局面の評価値を返す
    void init_move_vec();
    std::map<KOMAINF,std::vector<std::pair<int,int>>> move_vec;

};


#endif

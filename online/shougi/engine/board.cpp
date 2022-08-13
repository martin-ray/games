#include"board.hpp"
#include<cstdio>
#include<iostream>
#include<cmath>

char *komaStr[]={
	"   "," 歩"," 香"," 桂"," 銀"," 金"," 角"," 飛"," 玉"," と"," 杏"," 圭"," 全"," 金"," 馬"," 竜",
	"   ","v歩","v香","v桂","v銀","v金","v角","v飛","v王","vと","v杏","v圭","v全","v金","v馬","v竜"
};



shougiEngine::shougiEngine(){
	// initialize a game
	Hand = std::vector<std::vector<int>> (2,std::vector<int>(8,0));
	Board = std::vector<std::vector<KOMAINF>> (11,std::vector<KOMAINF>(11,EMPTY));
	init_game();

}

void shougiEngine::printBoard()
{
	int dan,suji;
	int koma;
	printf("後手 持ち駒 ");
	for(koma=FU;koma<=HI;koma++) {
		if (Hand[0][koma]==1) {
			printf("%s(%d) ",komaStr[koma],(int)koma);
		} else if (Hand[0][koma]>1) {
			printf("%s(%d)%d ",komaStr[koma],(int)koma,Hand[0][koma]);
		}
	}
	printf("\n");
	printf("  　９　８　７　６　５　４　３　２　１ \n");
	printf("  +---+---+---+---+---+---+---+---+---+\n");
	for(dan=1;dan<=9;dan++) {
		printf("  |");
		for(suji=9;suji>=1;suji--) { // 逆からプリントしていることに注意 これで整合性が合うという話だ
			printf("%s|",komaStr[Board[dan][suji]]);
		}
		printf(" 漢%d",dan);
		printf("\n");
		printf("  +---+---+---+---+---+---+---+---+---+\n");
	}
	printf("先手 持ち駒 ");
	for(koma=FU;koma<=HI;koma++) {
		if (Hand[1][koma]==1) {
			printf("%s(%d) ",komaStr[koma],(int)koma);
		} else if (Hand[1][koma]>1) {
			printf("%s(%d)%d ",komaStr[koma],(int)koma,Hand[1][koma]);
		}
	}
	std::cout << std::endl;
}

void shougiEngine::init_game(){
	cummulative_turn = 0;

	// 全部をout_of_boardに
	for(int dan=0;dan<11;dan++){
		for(int suji=0;suji<11;suji++){
			Board[dan][suji] = OUT_OF_BOARD;
		}
	}
	// 9*9をemptyに
	for(int dan=1;dan<=9;dan++){
		for(int suji=1;suji<=9;suji++){
			Board[dan][suji] = EMPTY;
		}
	}
	
	// 歩を並べる
	for(int suji=1;suji<10;suji++){
		Board[3][suji] = EFU;
		Board[7][suji] = FU;
	}
	// 玉を並べる
	Board[1][5] = EOU;
	Board[9][5] = OU;
	// 飛車を並べる
	Board[2][2] = EKA;
	Board[8][8] = KA;
	// 角を並べる
	Board[2][8] = EHI;
	Board[8][2] = HI;
	// 金を並べる
	Board[1][4] = Board[1][6] = EKI;
	Board[9][4] = Board[9][6] = KI;
	// 銀を並べる
	Board[1][3] = Board[1][7] = EGI;
	Board[9][3] = Board[9][7] = GI;
	// 桂を並べる
	Board[1][2] = Board[1][8] = EKE;
	Board[9][2] = Board[9][8] = KE;
	// 香を並べる
	Board[1][1] = Board[1][9] = EKY;
	Board[9][1] = Board[9][9] = KY;

	// 持ち駒を全部0に
	for(int koma=FU;koma<HI;koma++){
		Hand[0][koma] = 0;
		Hand[1][koma] = 0;
	}

	init_move_vec();
}

void shougiEngine::init_move_vec(){
	typedef std::vector<std::pair<int,int>> a_move;
	a_move EFU_move = {{1,0}};
	a_move FU_move = {{-1,0}};
	a_move OU_move = {
		{1,1},{1,0},{1,-1},{0,1},{0,-1},{-1,1},{-1,-1},{-1,0}
	};
	a_move EKI_move = {
		{1,0},{1,1},{1,-1},{0,1},{0,-1},{-1,0}
	};
	a_move KI_move = {
		{-1,0},{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,0}
	};
	a_move EGI_move = {
		{1,1},{1,0},{1,-1},{-1,-1},{-1,1}
	};
	a_move GI_move = {
		{-1,-1},{-1,0},{-1,1},{1,1},{1,-1}
	};
	a_move EKE_move = {
		{2,1},{2,-1}
	};
	a_move KE_move = {
		{-2,1},{-2,-1}
	};
	a_move EKY_move = {
		{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0},{8,0}
	};
	a_move KY_move = {
		{-1,0},{-2,0},{-3,0},{-4,0},{-5,0},{-6,0},{-7,0},{-8,0}
	};
	
	a_move KA_move;
	for(int move = -8;move<=8;move++){
		if(move==0)continue;
		KA_move.push_back(std::make_pair(move,move));
		KA_move.push_back(std::make_pair(move,-move));
	}

	a_move HI_move;
	for(int move = -8;move <9;move++){
		if(move==0)continue;
		HI_move.push_back(std::make_pair(move,0));
		HI_move.push_back(std::make_pair(0,move));
	}
	a_move RY_move = HI_move;
	for(auto m : OU_move){
		RY_move.push_back(m);
	}
	a_move UM_move = KA_move;
		for(auto m : OU_move){
		UM_move.push_back(m);
	}

	//a_move KI
	move_vec[EFU] = EFU_move;
	move_vec[FU] = FU_move;
	move_vec[EOU] = OU_move;
	move_vec[OU] = OU_move;
	move_vec[EKI] = EKI_move;
	move_vec[KI] = KI_move;
	move_vec[EGI] = EGI_move;
	move_vec[GI] = GI_move;
	move_vec[EKE] = EKE_move;
	move_vec[KE] = KE_move;
	move_vec[EKY] = EKY_move;
	move_vec[KY] = KY_move;
	move_vec[EHI] = HI_move;
	move_vec[EKA] = KA_move;
	move_vec[HI] = HI_move;
	move_vec[KA] = KA_move;
	move_vec[EUM] = UM_move;
	move_vec[UM] = UM_move;
	move_vec[ERY] = RY_move;
	move_vec[RY] = RY_move;

	
	// 成り金も
	// move_vec[]
}

void shougiEngine::start(){
	Player current_player = Player::PLAYER1;
	cummulative_turn = 1;
	while(!is_end()){
		// input 
		std::pair<int,int> from,to;
		do{
			// print board
			printBoard();
			// print turn
			if(current_player==Player::PLAYER1)std::cout << "先手番"<<std::endl;
			else std::cout << "後手番" << std::endl;
			from = input_from(current_player);
			to = input_to(current_player);
		}while(!(is_valid_move(current_player,from,to)&is_valid_board(Board,from,to)));

		// is nari
		bool nari = is_nari(current_player,from,to);

		// 敵ゴマをとるか
		bool gonna_get_enemy = is_enemy(current_player,to);

		// 敵ゴマを持ち駒に加える
		if(current_player == Player::PLAYER1 && gonna_get_enemy){
			// なりごまだったらその分も引く
			Hand[1][Board[to.first][to.second] - ENEMY]+=1;
		}
		else if(current_player == Player::PLAYER2 && gonna_get_enemy){
			Hand[0][Board[to.first][to.second]]+=1;
		}

		// change board
		if(from.first==0){// hand to board
			if(current_player==Player::PLAYER1){
				KOMAINF k = (KOMAINF)from.second;
				Hand[1][from.second]-=1; //持ち駒から一つ召喚
				Board[to.first][to.second] = k;
			}else{
				KOMAINF k = (KOMAINF)from.second;
				Hand[0][from.second]-=1; //持ち駒から一つ召喚
				Board[to.first][to.second] = k;				
			}
		}
		else{// board to board
			KOMAINF k = Board[from.first][from.second];
			Board[from.first][from.second] = EMPTY;
			if(nari){
				Board[to.first][to.second] = (KOMAINF)(k + PROMOTED);
			}
			else{
				Board[to.first][to.second] = (KOMAINF)k;
			}			
		}

		// player change
		cummulative_turn++;
		if(current_player==Player::PLAYER1)current_player = Player::PLAYER2;
		else current_player = Player::PLAYER1;

	}
}

bool shougiEngine::is_end(){
	return false;
}

std::pair<int,int> shougiEngine::input_from(Player player){
	int dan;
	int suji;

	while(true){
		std::cout << "From" << std::endl;
		std::cout << "suji : dan(0 from Hand)> ";
		std::cin >> suji >> dan;
		if(suji < 0 || suji > 9 || dan < 0 || dan > 9){
			std::cout << "non valid" << std::endl;
			continue;
		}

		if(dan==0){
			if(is_in_hand(current_player,suji)){
				return std::make_pair(dan,suji);
			}
			else{
				std::cout << "not in your hand" << std::endl;
			}
		}
		
		if(player == Player::PLAYER1){
			// 選んだコマが自分のこまか. dan==0は手駒
			if(Board[dan][suji] < ENEMY && Board[dan][suji] > EMPTY || dan==0){
				return std::make_pair(dan,suji);
			}
		}
		else{
			if(Board[dan][suji]>=ENEMY && Board[dan][suji] <=ERY){
				return std::make_pair(dan,suji);
			}
		}
		printBoard();
		std::cout << "not your troop or empty" << std::endl;
	}
}

std::pair<int,int> shougiEngine::input_to(Player player){
	int dan;
	int suji;
	while(true){
		std::cout << "To" << std::endl;
		std::cout << "suji : dan> ";
		std::cin >> suji >> dan;
			if(suji < 0 || suji > 9 || dan < 0 || dan > 9){
			std::cout << "non valid" << std::endl;
			continue;
		}
		// 移動先が自分のコマではないか
		if(player == Player::PLAYER1){
			if(Board[dan][suji] > ENEMY || Board[dan][suji] == EMPTY){
				return std::make_pair(dan,suji);
			}
		}
		else{
			if(Board[dan][suji] < ENEMY || Board[dan][suji] == EMPTY){
				return std::make_pair(dan,suji);
			}
		}
		printBoard();
		std::cout << "Your koma is on that point" << std::endl;		
	}
}

bool shougiEngine::is_valid_move(Player player,std::pair<int,int> from, std::pair<int,int> to){

	// from hand
	if(is_in_hand(player,from.second) && from.first==0 && is_enemy(player,to)){
		return true;
	}

	// is koma in between from "from" and "to"
	if( !is_koma_inbetween(from,to)){
		std::cout << "invalid move" << std::endl;
		return false;
	}

	// check whether player tries to move opponents koma
	KOMAINF koma = Board[from.first][from.second];
	if(player == Player::PLAYER1 && koma >= ENEMY ||  player== Player::PLAYER2 && koma < ENEMY){
		std::cout << "You are tring to move opponent's koma" << std::endl;
		return false;
	}

	std::pair<int,int> vec;
	vec.first = to.first - from.first;
	vec.second = to.second - from.second;
	std::cout << "vec = ("<<vec.first << "," << vec.second <<")" << std::endl;  
	for(auto v : move_vec[koma]){
		std::cout << "v = ("<<v.first << "," << v.second <<")" << std::endl; 
		if(v == vec){
			return true;
		}
	}
	// 選んだコマの動きがvalidではない
	std::cout << "Non valid move" << std::endl;
	return false;
}

// palyerから見た時にその地点の駒が敵ゴマか否か
bool shougiEngine::is_enemy(Player player,std::pair<int,int> to){
	if(player==Player::PLAYER1){
		if(Board[to.first][to.second] >=ENEMY && Board[to.first][to.second]!=EMPTY){
			return true;
		}
		else{
			return false;
		}
	}else{
		if(Board[to.first][to.second] < ENEMY && Board[to.first][to.second]!=EMPTY){
			return true;
		}
		else{
			return false;
		}
	}
	return false;
}

// 変更を加えた局面で王手が生じないか
bool shougiEngine::is_valid_board(std::vector<std::vector<KOMAINF>> Board, std::pair<int,int> from, std::pair<int,int> to){
	if(from.first == 0)return true; //手持ちからの時はとりあえずtrueで
	KOMAINF k = Board[from.first][from.second];
	Board[from.first][from.second] = EMPTY;
	Board[to.first][to.second] = k;
	return !is_oute(Board); // 王手が生じたらだめ
}

// 成れるか否か
bool shougiEngine::is_nari(Player player,std::pair<int,int> from,std::pair<int,int> to){

	std::string nari;
	if(!is_in_enemy_field(player,from)&is_in_enemy_field(player,to)){
		std::cout << "成りますか？ > (y/n)";
		std::cin >> nari;
		if(nari=="y" || nari=="Y" || nari=="yes"){
			return true;
		}
		else{
			return false;
		}
	}
	if(is_in_enemy_field(player,from)){
		if(nari=="y" || nari=="Y" || nari=="yes"){
			return true;
		}
		else{
			return false;
		}		
	}
	return false;
}

bool shougiEngine::is_in_enemy_field(Player player,std::pair<int,int> point){
	if(player==Player::PLAYER1){
		if(1 <= point.first && point.first <= 3)return true;
	}
	else{
		if(7 <= point.first && point.first <= 9)return true;
	}
	return false;
}

// from to の間にコマが存在するか
bool shougiEngine::is_koma_inbetween(std::pair<int,int> from,std::pair<int,int> to){
		
		std::pair<int,int> from_to_vec = std::make_pair(from.first-to.first,from.second-to.second);
		int len = vect_len(from_to_vec);
		std::pair<int,int> unit_vec;

		// 単位ベクトルを求める
		unit_vec.first = from_to_vec.first/len;
		unit_vec.second = from_to_vec.second/len;

		from.first += unit_vec.first;
		from.second += unit_vec.second;

		while(to != from){
			if(Board[from.first][from.second]!=EMPTY){
				return true;
			}
			from.first += unit_vec.first;
			from.second += unit_vec.second;	
		}
		return false;
}

// vector's len
int vect_len(std::pair<int,int> v)
{
	return sqrt(v.first*v.first + v.second*v.second);
}

// 王手か？
bool shougiEngine::is_oute(std::vector<std::vector<KOMAINF>> Board){
	// 王様の位置を探索
	std::pair<int,int> ou; // player1の王の位置
	std::pair<int,int> gyoku; // player2の王の位置
	for(int dan=1;dan<=9;dan++){
		for(int suji=1;suji<=9;suji++){
			if(Board[dan][suji] == OU){
				ou.first=dan;
				ou.second=suji;
			}
			if(Board[dan][suji] == EOU){
				gyoku.first=dan;
				gyoku.second=suji;
			}
		}
	}

	// 盤上のコマが行けるところに王様がいるかを全探索
	for(int dan = 1;dan<=9;dan++){
		for(int suji=1;suji<=9;suji++){
			if(Board[dan][suji]!=EMPTY){
				KOMAINF k;
				k = Board[dan][suji];
				if(k >= ENEMY){
					std::pair<int,int> koma_to_ou;
					koma_to_ou = std::make_pair(ou.first-dan,ou.second-suji);
					for(auto v : move_vec[k]){
						if(v == koma_to_ou && k == EKY){
							return true; // 桂馬は飛び越えます
						}
						else if(v == koma_to_ou && v.first*v.first+v.second*v.second==1){ // ワンホップで王手
							return true;
						}
						else if(v == koma_to_ou){ // 2ホップ以上かかる王手。間にコマが存在しないか
							int vec_len = sqrt(v.first*v.first+v.second*v.second);
							std::pair<int,int> unit_v = std::make_pair(v.first/vec_len,v.second/vec_len);
							bool koma_in_the_middle = false;
							while(unit_v!=v){
								if(Board[dan+unit_v.first][suji+unit_v.second]!=EMPTY){
									koma_in_the_middle = true;
									break;
								}
							}
							if(!koma_in_the_middle){
								return true;
							}
						}

					}
				}else{
					std::pair<int,int> koma_to_ou;
					koma_to_ou = std::make_pair(gyoku.first-dan,gyoku.second-suji);
					for(auto v : move_vec[k]){
						if(v == koma_to_ou && k == KY){
							return true; // 桂馬は飛び越えます
						}
						else if(v == koma_to_ou && v.first*v.first+v.second*v.second==1){ // ワンホップで王手
							return true;
						}
						else if(v == koma_to_ou){ // 2ホップ以上かかる王手。間にコマが存在しないか
							int vec_len = sqrt(v.first*v.first+v.second*v.second);
							std::pair<int,int> unit_v = std::make_pair(v.first/vec_len,v.second/vec_len);
							bool koma_in_the_middle = false;
							while(unit_v!=v){
								if(Board[dan+unit_v.first][suji+unit_v.second]!=EMPTY){
									koma_in_the_middle = true;
									break;
								}
							}
							if(!koma_in_the_middle){
								return true;
							}
						}

					}
				}

			}

		}
	}

	// 王手ではない
	return false;
}

bool shougiEngine::is_in_hand(Player player,int koma){
	if(player == Player::PLAYER1){
		if(Hand[1][koma]>1){
			return true;
		}
		else{
			return false;
		}
	}
	else{
		if(Hand[0][koma]>1){
			return true;
		}
		else{
			return false;
		}		
	}
}
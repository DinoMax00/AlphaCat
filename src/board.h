
/********************************************************
类名称：Board
功能：通过16*16的棋盘来表示棋盘，以减小9*10的棋盘的除法对效率影响
周围放置哨兵防止大量的边界检测
********************************************************/
class Board
{
public:
	//成员变量
	unsigned char player;			//当前走棋玩家
	unsigned char board[256];		//棋盘上某一位上的棋子
	unsigned char pos_of_king[2];	//棋盘上将帅的位置
	int num_of_move[8];				//每一种棋子的走棋方案数
	int num_of_chess;				//当前棋盘的棋子个数
	int num_of_rounds;				//回合数
	int num_of_no_eat_rounds;		//未吃子的回合数
	long long zobrist;				//当前棋盘的Zobrist键值
	
	//成员函数
	/****************************************
	名称：Board()
	功能：默认构造函数
	参数：无
	返回值：构造的Board对象
	****************************************/
	Board();

	/****************************************
	名称：Board()
	功能：读取FEN格式来初始化对象的构造函数
	参数：const char* fen，构造当前局面的FEN格式串
	返回值：构造的Board对象
	****************************************/
	Board(const char *fen);

	/****************************************
	名称：buildBoardFromFen()
	功能：通过FEN数组来初始化对象
	参数：const char* fen，构造当前局面的FEN格式串
	返回值：构造的Board对象
	****************************************/
	void buildBoardFromFen(const char* fen);
};
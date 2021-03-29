
/********************************************************
类名称：Board
功能：通过16*16的棋盘来表示棋盘，以减小9*10的棋盘的除法对效率影响
周围放置哨兵防止大量的边界检测
********************************************************/
class Board
{
public:
	//成员变量
<<<<<<< Updated upstream
	unsigned char player;			//当前走棋玩家
	unsigned char board[256];		//棋盘上某一位上的棋子
	unsigned char pos_of_king[2];	//棋盘上将帅的位置
	int num_of_move[8];				//每一种棋子的走棋方案数
	int num_of_chess;				//当前棋盘的棋子个数
	int num_of_rounds;				//回合数
	int num_of_no_eat_rounds;		//未吃子的回合数
	long long zobrist;				//当前棋盘的Zobrist键值
=======
	bool player = 1;							//当前走棋玩家
	unsigned char board[256];				//棋盘上某一位上的棋子
	unsigned char pos_of_kings[2];			//棋盘上将帅的位置
	int redValue;				    // 红棋子力
	int blackValue;				// 黑棋子力
	//unsigned int num_of_move[8];			//每一种棋子的走棋方案数
	//unsigned int num_of_chess;				//当前棋盘的棋子个数
	//unsigned int num_of_rounds;				//回合数
	//unsigned int num_of_no_eat_rounds;		//未吃子的回合数
	//unsigned long long zobrist;				//当前棋盘的Zobrist键值

	std::vector<Move> mov;					//记录当前棋盘所有可行的走法
>>>>>>> Stashed changes
	
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
<<<<<<< Updated upstream
	void buildBoardFromFen(const char* fen);
};
=======
	void buildBoardFromFen(std::string fen);

	/****************************************
	名称：genOneMove()
	功能：通过move数组来移动
	参数：string move，构造当前局面的移动串,bool whether_get_all 默认 GET_ALL对新局面生成所有可行走法
	返回值：
	****************************************/
	void genOneMove(std::string& move);
	void genOneMove(Move& move);

	/****************************************
	名称：printBoardForDebug()
	功能：打印棋盘看查情况
	参数：
	返回值：
	****************************************/
	void printBoardForDebug();
	void printBoardForDebug2();

	/****************************************
	名称：generateMoves()
	功能：生成所有能走的步数
	参数：
	返回值：
	****************************************/
	void generateMoves();
	void generateMoves(bool);

	/****************************************
	名称：deleteOneMove()
	功能：返回上一步的局面
	参数：
	返回值：
	****************************************/
	void deleteOneMove(Move&);


	/****************************************
	名称：randomRunMove()
	功能：给出随机走法
	参数：
	返回值：
	****************************************/
	Move randomRunMove();

	/*******************************************
	函数名称：checkJiang
	功能：在generateMoves中进行调用，判断在执行某一次的走子之后，是否构成将军
	参数：int from, int to,要进行的走子的起点和终点
	返回值：bool 判断是否将军
	*******************************************/
	bool checkJiang(Move& mov);
	bool checkJiang(Move& mov, bool);

	/*******************************************
	函数名称：mctsMove
	功能：蒙特卡洛搜索中的随机走子
	参数：
	返回值：GameStatus
	*******************************************/
	GameStatus mctsMove();
};


#endif
>>>>>>> Stashed changes

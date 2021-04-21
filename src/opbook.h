#include <fstream>

class opBook
{
private:
	std::ifstream infile;
public:
	opBook();
	int opBook_search(unsigned long long);
};
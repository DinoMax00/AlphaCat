#ifndef UCCI_H 
#define UCCI_H

#include <iostream>
#include <string.h>
#include <vector>

#include "board.h"

class UcciEngine
{
private:
    Board board;
    // ����������ַ���
    std::string commandStr;
    // �����ַ����ָ��Ľ��
    std::vector<std::string> commandVec;
    // ��������
    void getCommand();
    // �ָ��ַ���
    void split();
    // ����ַ���������vector
    void clear();
public:
    // ��������
    void bootEngine();
    // ��ʽ����
    void run();
};

#endif
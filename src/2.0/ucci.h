/*****************************************************************//**
 * \file   ucci.h
 * \brief  ucciЭ��ͷ�ļ�
 *
 * \author AlphaCat
 * \date   March 2021
 *********************************************************************/
#ifndef UCCI_H 
#define UCCI_H

#include <iostream>
#include <string.h>
#include <vector>

#include "game.h"
#include "move.h"

class UcciEngine
{
private:
    Game game;
    MoveSort move_sort;
    /// ����������ַ���
    std::string commandStr;
    /// �����ַ����ָ���ŵ�Vec
    std::vector<std::string> commandVec;

    /**
     * @brief ��ȡһ���������commandStr
     *
     */
    void getCommand();

    /**
     * @brief �������ַ������ո���зָ����commandVec
     *
     */
    void split();

    /**
     * @brief ��������ַ���������Vec
     *
     */
    void clear();

    /**
     * @brief ���µ�ǰִ��
     *
     */
    void updWhichPlayer();

public:
    /**
     * @brief �����������Ԥ�����Ľӿ�
     *
     */
    void bootEngine();

    /**
     * @brief ��������������еĽӿ�
     *
     */
    void run();
};



#endif
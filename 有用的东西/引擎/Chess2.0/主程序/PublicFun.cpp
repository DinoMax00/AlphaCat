#include "stdafx.h"
#include "Define.h"

//sort������CHESSMOVE����ʱ�õ�
bool UDgreater(CHESSMOVE cm1, CHESSMOVE cm2)
{
	return cm1.iScore>cm2.iScore;
}
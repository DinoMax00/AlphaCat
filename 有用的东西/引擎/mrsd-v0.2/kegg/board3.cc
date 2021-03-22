/*
Ki11egg , Xiangqi(Chinese Chess) engine
Copyright (C) Tzer-jen Wei

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "board.h"
#include "cbdb.h"
#include <time.h>
XQ& XQ::operator=(XQ &bd2)
{
  int i, j;
  _MaxPly = bd2._MaxPly;
  _Ply = bd2._Ply;
  for(i=0;i<_MaxPly;i++)
          tr[i]=bd2.tr[i];
  for (i = 0; i < maxy; i++)
         for (j = 0; j < maxx; j++)
                bd(i,j) = bd2.bd(i,j);
        for(i=0;i<34;i++)
   cch_p[i]=bd2.cch_p[i];
return *this;                                                 
}
XQ::XQ (XQ &bd2)
{
  int i, j;
 _MaxPly = bd2._MaxPly;
  _Ply = bd2._Ply;
  for(i=0;i<_MaxPly;i++)
          tr[i]=bd2.tr[i];
  for (i = 0; i < maxy; i++)
         for (j = 0; j < maxx; j++)
                bd(i,j) = bd2.bd(i,j);
        for(i=0;i<34;i++)
   cch_p[i]=bd2.cch_p[i];                             
}
#ifndef GB
static char pc_name[2][8][3] = {{ "��", "�K", "��", "�X","��",  "��", "�L" ,"xx" },
 { "�N", "�h", "�H", "��", "��", "�]", "��","xx" }};
static char *number1[] = { "�@", "�G", "�T", "�|", "��", "��", "�C", "�K", "�E" };
#else
static char pc_name[2][8][3] = {{ "˧", "��", "��", "��","��",  "��", "��" ,"xx" },
 { "��", "ʿ", "��", "��", "��", "��", "��","xx" }};
static char *number1[] = { "һ", "��", "��", "��", "��", "��", "��", "��", "��" };
#endif
static char *number2[] = { " 1", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9" };
static char **number;
char *XQ::Chinese_Notation(int turn)
{
	static char movestr[19];
	YX s,d;
	int n,t;
if(turn==-1) turn=Ply()-1;
if(turn<0|| turn>=MaxPly()) {sprintf(movestr,"ErrorPly");return movestr;}
n=Ply();
GotoPly(turn);
number= turn&1 ? number2 : number1;
	movepos(s,d,turn);
	if(s==0xff && d==0xff) {sprintf(movestr,"���ۤ�");GotoPly(n);return movestr;}
	if(s>=90 || d>=90) {sprintf(movestr,"Error%x%x",s,d);GotoPly(n);return movestr;} 
	t=_bd(s);
	if((id2type(t)|1)==5
	   && 	YX_X(_p(t^2))==YX_X(s))
	sprintf(movestr,"%s%s",
		(turn &1)^(s> _p(t^2))? "�e":"��",
		pc_name[_bd(s)&1][id2type(_bd(s))]
		);
	else
	sprintf(movestr,"%s%s",
		pc_name[_bd(s)&1][id2type(_bd(s))],
	number[turn&1 ? YX_X(s) : 8-YX_X(s) ]);
	strcat(movestr,
#ifndef 	GB
	YX_Y(s) == YX_Y(d) ?  "��" :((s<d) ==  (turn&1)? "�h" : "�i"));
#else
	YX_Y(s) == YX_Y(d) ?  "ƽ" :((s<d) ==  (turn&1)? "��" : "��"));
#endif	
	strcat(movestr,
	 YX_X(s) == YX_X(d) ?
	number[s>d ? YX_Y(s)-YX_Y(d)-1 :YX_Y(d)-YX_Y(s)-1]:
	number[turn&1 ? YX_X(d):8-YX_X(d)] );
	movestr[8] = 0;
	GotoPly(n);
	return movestr;
}
int XQ::FollowBook()
{
int list[200],sum;
}

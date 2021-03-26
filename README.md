# 😾AlphaCat😾

---

> **"我会带你们进表演赛"** 

人工智能课程实践大作业。

---
* 参考资料
  
  - 1 https://www.techgrow.cn/posts/26ffa5e3.html
  - 2 https://www.xqbase.com/
  - 3 ./有用的东西

---
* 图形化界面

  [鲨鱼象棋(这个好看)](https://sharkchess.lanzous.com/b0b87cetc)
  
  [天机象棋](http://www.xqpt.com/)
  
  [另一个](https://exp.newsmth.net/topic/ed55126a4bc9831c6b148f8c47800fad)
  
  配置引擎
  ![](./docImg/引擎.PNG)

---

## 文件

- AlphaCat.cpp

  main()

  读入串

  调用ucci

  更新棋盘

  搜索

  反馈()

- board.h
  
  棋盘类
  
  init()

  void getMove(string move, side) 

  void newBoard(string fen)

  vector\< string \> getMoves()

- ucci.h

  ucci协议

  调用棋盘的移动函数

- tools.h

- base.h

  宏定义

  搜索深度

  棋子对应的数字

- search.h

---

### 代码规范

大括号换行

函数小驼峰

类大驼峰

=带空格

#ifndef XXX_H
 
  
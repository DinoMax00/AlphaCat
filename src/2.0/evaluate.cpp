#include <algorithm>
#include "game.h"
#include "pregen.h"
#include "evaluate.h"
/* 局面预评价就是初始化局面预评价数据(PreEval)的过程。
 * 局面预评价主要分以下两个方面：
 * 1. 判断局势处于开中局还是残局阶段；
 * 2. 判断每一方是否对对方形成威胁。
 */

const int JU_MIDGAME_VALUE = 6;
const int MA_PAO_MIDGAME_VALUE = 3;
const int OTHER_MIDGAME_VALUE = 1;
const int TOTAL_MIDGAME_VALUE = JU_MIDGAME_VALUE * 4 + MA_PAO_MIDGAME_VALUE * 8 + OTHER_MIDGAME_VALUE * 18;
const int TOTAL_ADVANCED_VALUE = 4;
const int TOTAL_ATTACK_VALUE = 8;
const int SHI_XIANG_ATTACKLESS_VALUE = 80;
const int TOTAL_ADVISOR_LEAKAGE = 80;

EVALSTRUCT normalEval;

void Game::evaBoard() {
  int i, sq, midGameValue, redAttack, blackAttack, redSimpleValue, blackSimpleValue;
  uint8_t bingAttack[256], bingUnAttack[256];
  // 首先判断局势处于开中局还是残局阶段，方法是计算各种棋子的数量，按照车=6、马炮=3、其它=1相加。
  midGameValue = popCnt32(this->bitPieces & bothBitpiece(SHI_BITPIECE | XIANG_BITPIECE | BING_BITPIECE)) * OTHER_MIDGAME_VALUE;
  midGameValue += popCnt32(this->bitPieces & bothBitpiece(MA_BITPIECE | PAO_BITPIECE)) * MA_PAO_MIDGAME_VALUE;
  midGameValue += popCnt32(this->bitPieces & bothBitpiece(JU_BITPIECE)) * JU_MIDGAME_VALUE;
  // 使用二次函数，子力很少时才认为接近残局
  midGameValue = (2 * TOTAL_MIDGAME_VALUE - midGameValue) * midGameValue / TOTAL_MIDGAME_VALUE;
  for (sq = 0; sq < 256; sq ++) {
    // 将马车炮
    if (inBoard(sq)) {
      normalEval.redPieces[0][sq] = normalEval.blackPieces[0][squareFlip(sq)] = (uint8_t)
          ((midGameJiangBingAttack[sq] * midGameValue + endGameJiangBingAttack[sq] * (TOTAL_MIDGAME_VALUE - midGameValue)) / TOTAL_MIDGAME_VALUE);
      normalEval.redPieces[3][sq] = normalEval.blackPieces[3][squareFlip(sq)] = (uint8_t)
          ((midGameMa[sq] * midGameValue + endGameMa[sq] * (TOTAL_MIDGAME_VALUE - midGameValue)) / TOTAL_MIDGAME_VALUE);
      normalEval.redPieces[4][sq] = normalEval.blackPieces[4][squareFlip(sq)] = (uint8_t)
          ((midGameJu[sq] * midGameValue + endGameJu[sq] * (TOTAL_MIDGAME_VALUE - midGameValue)) / TOTAL_MIDGAME_VALUE);
      normalEval.redPieces[5][sq] = normalEval.blackPieces[5][squareFlip(sq)] = (uint8_t)
          ((midGamePao[sq] * midGameValue + endGamePao[sq] * (TOTAL_MIDGAME_VALUE - midGameValue)) / TOTAL_MIDGAME_VALUE);
      bingAttack[sq] = normalEval.redPieces[0][sq];
      bingUnAttack[sq] = (uint8_t)
          ((midGameJiangBingUnAttack[sq] * midGameValue + endGameJiangBingUnAttack[sq] * (TOTAL_MIDGAME_VALUE - midGameValue)) / TOTAL_MIDGAME_VALUE);
    }
  }

  // 然后判断各方是否处于进攻状态，方法是计算各种过河棋子的数量，按照车马2炮兵1相加。
  redAttack = blackAttack = 0;
  for (i = sideTag(RED) + MA_FROM; i <= sideTag(RED) + JU_TO; i ++) {
    if (this->pieces[i] != 0 && blackHalf(this->pieces[i])) {
      redAttack += 2;
    }
  }
  for (i = sideTag(RED) + PAO_FROM; i <= sideTag(RED) + BING_TO; i ++) {
    if (this->pieces[i] != 0 && blackHalf(this->pieces[i])) {
      redAttack ++;
    }
  }
  for (i = sideTag(BLACK) + MA_FROM; i <= sideTag(BLACK) + JU_TO; i ++) {
    if (this->pieces[i] != 0 && redHalf(this->pieces[i])) {
      blackAttack += 2;
    }
  }
  for (i = sideTag(BLACK) + PAO_FROM; i <= sideTag(BLACK) + BING_TO; i ++) {
    if (this->pieces[i] != 0 && redHalf(this->pieces[i])) {
      blackAttack ++;
    }
  }
  // 如果本方轻子数比对方多，那么每多一个轻子(车算2个轻子)威胁值加2。威胁值最多不超过8。
  redSimpleValue = popCnt16(this->splited_bitPieces[RED] & JU_BITPIECE) * 2 + popCnt16(this->splited_bitPieces[RED] & (MA_BITPIECE | PAO_BITPIECE));
  blackSimpleValue = popCnt16(this->splited_bitPieces[BLACK] & JU_BITPIECE) * 2 + popCnt16(this->splited_bitPieces[BLACK] & (MA_BITPIECE | PAO_BITPIECE));
  if (redSimpleValue > blackSimpleValue) {
    redAttack += (redSimpleValue - blackSimpleValue) * 2;
  } else {
    blackAttack += (blackSimpleValue - redSimpleValue) * 2;
  }
  redAttack = std::min(redAttack, TOTAL_ATTACK_VALUE);
  blackAttack = std::min(blackAttack, TOTAL_ATTACK_VALUE);
  for (sq = 0; sq < 256; sq ++) {
    if (inBoard(sq)) {
      // 士象兵
      normalEval.redPieces[1][sq] = normalEval.redPieces[2][sq] = (uint8_t) ((gameShiXiangTreatened[sq] * blackAttack +
          gameShiXiangTreatless[sq] * (TOTAL_ATTACK_VALUE - blackAttack)) / TOTAL_ATTACK_VALUE);
      normalEval.blackPieces[1][sq] = normalEval.blackPieces[2][sq] = (uint8_t) ((gameShiXiangTreatened[squareFlip(sq)] * redAttack +
          gameShiXiangTreatless[squareFlip(sq)] * (TOTAL_ATTACK_VALUE - redAttack)) / TOTAL_ATTACK_VALUE);
      normalEval.redPieces[6][sq] = (uint8_t) ((bingAttack[sq] * redAttack +
          bingUnAttack[sq] * (TOTAL_ATTACK_VALUE - redAttack)) / TOTAL_ATTACK_VALUE);
      normalEval.blackPieces[6][sq] = (uint8_t) ((bingAttack[squareFlip(sq)] * blackAttack +
          bingUnAttack[squareFlip(sq)] * (TOTAL_ATTACK_VALUE - blackAttack)) / TOTAL_ATTACK_VALUE);
    }
  }
  // 调整不受威胁方少掉的仕(士)相(象)分值
  this->red_val = SHI_XIANG_ATTACKLESS_VALUE * (TOTAL_ATTACK_VALUE - blackAttack) / TOTAL_ATTACK_VALUE;
  this->black_val = SHI_XIANG_ATTACKLESS_VALUE * (TOTAL_ATTACK_VALUE - redAttack) / TOTAL_ATTACK_VALUE;
  // 最后重新计算子力位置分
  for (i = 16; i < 32; i ++) {
    sq = this->pieces[i];
    if (sq != 0) {
      this->red_val += normalEval.redPieces[pieceType[i]][sq];
    }
  }
  for (i = 32; i < 48; i ++) {
    sq = this->pieces[i];
    if (sq != 0) {
      this->black_val += normalEval.blackPieces[pieceType[i]][sq];
    }
  }
}
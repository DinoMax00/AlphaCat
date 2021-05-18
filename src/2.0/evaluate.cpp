#include <algorithm>
#include "game.h"
#include "pregen.h"
#include "evaluate.h"
/* ����Ԥ���۾��ǳ�ʼ������Ԥ��������(PreEval)�Ĺ��̡�
 * ����Ԥ������Ҫ�������������棺
 * 1. �жϾ��ƴ��ڿ��оֻ��ǲоֽ׶Σ�
 * 2. �ж�ÿһ���Ƿ�ԶԷ��γ���в��
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
  // �����жϾ��ƴ��ڿ��оֻ��ǲоֽ׶Σ������Ǽ���������ӵ����������ճ�=6������=3������=1��ӡ�
  midGameValue = popCnt32(this->bitPieces & bothBitpiece(SHI_BITPIECE | XIANG_BITPIECE | BING_BITPIECE)) * OTHER_MIDGAME_VALUE;
  midGameValue += popCnt32(this->bitPieces & bothBitpiece(MA_BITPIECE | PAO_BITPIECE)) * MA_PAO_MIDGAME_VALUE;
  midGameValue += popCnt32(this->bitPieces & bothBitpiece(JU_BITPIECE)) * JU_MIDGAME_VALUE;
  // ʹ�ö��κ�������������ʱ����Ϊ�ӽ��о�
  midGameValue = (2 * TOTAL_MIDGAME_VALUE - midGameValue) * midGameValue / TOTAL_MIDGAME_VALUE;
  for (sq = 0; sq < 256; sq ++) {
    // ������
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

  // Ȼ���жϸ����Ƿ��ڽ���״̬�������Ǽ�����ֹ������ӵ����������ճ���2�ڱ�1��ӡ�
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
  // ��������������ȶԷ��࣬��ôÿ��һ������(����2������)��вֵ��2����вֵ��಻����8��
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
      // ʿ���
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
  // ����������в���ٵ�����(ʿ)��(��)��ֵ
  this->red_val = SHI_XIANG_ATTACKLESS_VALUE * (TOTAL_ATTACK_VALUE - blackAttack) / TOTAL_ATTACK_VALUE;
  this->black_val = SHI_XIANG_ATTACKLESS_VALUE * (TOTAL_ATTACK_VALUE - redAttack) / TOTAL_ATTACK_VALUE;
  // ������¼�������λ�÷�
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
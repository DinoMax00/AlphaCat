
#include <algorithm>
#include "evaluate.h"
#include "game.h"
#include "pregen.h"
/* 局面预评价就是初始化局面预评价数据(PreEval)的过程。
 * 局面预评价主要分以下两个方面：
 * 1. 判断局势处于开中局还是残局阶段；
 * 2. 判断每一方是否对对方形成威胁。
 */
const int MA_PAO_MIDGAME_VALUE = 3;
const int OTHER_MIDGAME_VALUE = 1;
const int TOTAL_MIDGAME_VALUE = JU_MIDGAME_VALUE * 4 + MA_PAO_MIDGAME_VALUE * 8 + OTHER_MIDGAME_VALUE * 18;
const int TOTAL_ADVANCED_VALUE = 4;
const int TOTAL_ATTACK_VALUE = 8;
const int SHI_XIANG_ATTACKLESS_VALUE = 80;
const int TOTAL_SHI_LEAKAGE = 80;

EVALSTRUCT normalEval;
EVALSTRUCTEX exEval;

static bool cntInit = false;

void Game::evaBoard() {
    int i, sq, midGameValue, redAttack, blackAttack, redSimpleValue, blackSimpleValue;
    uint8_t bingAttack[256], bingUnAttack[256];
    // 首先判断局势处于开中局还是残局阶段，方法是计算各种棋子的数量，按照车=6、马炮=3、其它=1相加。
    midGameValue = popCnt32(this->bitPieces & bothBitpiece(SHI_BITPIECE | XIANG_BITPIECE | BING_BITPIECE)) * OTHER_MIDGAME_VALUE;
    midGameValue += popCnt32(this->bitPieces & bothBitpiece(MA_BITPIECE | PAO_BITPIECE)) * MA_PAO_MIDGAME_VALUE;
    midGameValue += popCnt32(this->bitPieces & bothBitpiece(JU_BITPIECE)) * JU_MIDGAME_VALUE;
    // 使用二次函数，子力很少时才认为接近残局
    midGameValue = (2 * TOTAL_MIDGAME_VALUE - midGameValue) * midGameValue / TOTAL_MIDGAME_VALUE;
    for (sq = 0; sq < 256; sq++) {
        // 将马车炮
        if (inBoard(sq)) {
            normalEval.redPieces[0][sq] = normalEval.blackPieces[0][squareFlip(sq)] = (uint8_t)((midGameJiangBingAttack[sq] * midGameValue + endGameJiangBingAttack[sq] * (TOTAL_MIDGAME_VALUE - midGameValue)) / TOTAL_MIDGAME_VALUE);
            normalEval.redPieces[3][sq] = normalEval.blackPieces[3][squareFlip(sq)] = (uint8_t)((midGameMa[sq] * midGameValue + endGameMa[sq] * (TOTAL_MIDGAME_VALUE - midGameValue)) / TOTAL_MIDGAME_VALUE);
            normalEval.redPieces[4][sq] = normalEval.blackPieces[4][squareFlip(sq)] = (uint8_t)((midGameJu[sq] * midGameValue + endGameJu[sq] * (TOTAL_MIDGAME_VALUE - midGameValue)) / TOTAL_MIDGAME_VALUE);
            normalEval.redPieces[5][sq] = normalEval.blackPieces[5][squareFlip(sq)] = (uint8_t)((midGamePao[sq] * midGameValue + endGamePao[sq] * (TOTAL_MIDGAME_VALUE - midGameValue)) / TOTAL_MIDGAME_VALUE);
            bingAttack[sq] = normalEval.redPieces[0][sq];
            bingUnAttack[sq] = (uint8_t)((midGameJiangBingUnAttack[sq] * midGameValue + endGameJiangBingUnAttack[sq] * (TOTAL_MIDGAME_VALUE - midGameValue)) / TOTAL_MIDGAME_VALUE);
        }
    }

    // 然后判断各方是否处于进攻状态，方法是计算各种过河棋子的数量，按照车马2炮兵1相加。
    redAttack = blackAttack = 0;
    for (i = sideTag(RED) + MA_FROM; i <= sideTag(RED) + JU_TO; i++) {
        if (this->pieces[i] != 0 && blackHalf(this->pieces[i])) {
            redAttack += 2;
        }
    }
    for (i = sideTag(RED) + PAO_FROM; i <= sideTag(RED) + BING_TO; i++) {
        if (this->pieces[i] != 0 && blackHalf(this->pieces[i])) {
            redAttack++;
        }
    }
    for (i = sideTag(BLACK) + MA_FROM; i <= sideTag(BLACK) + JU_TO; i++) {
        if (this->pieces[i] != 0 && redHalf(this->pieces[i])) {
            blackAttack += 2;
        }
    }
    for (i = sideTag(BLACK) + PAO_FROM; i <= sideTag(BLACK) + BING_TO; i++) {
        if (this->pieces[i] != 0 && redHalf(this->pieces[i])) {
            blackAttack++;
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
    for (sq = 0; sq < 256; sq++) {
        if (inBoard(sq)) {
            // 士象兵
            normalEval.redPieces[1][sq] = normalEval.redPieces[2][sq] = (uint8_t)((gameShiXiangTreatened[sq] * blackAttack +
                                                                                   gameShiXiangTreatless[sq] * (TOTAL_ATTACK_VALUE - blackAttack)) /
                                                                                  TOTAL_ATTACK_VALUE);
            normalEval.blackPieces[1][sq] = normalEval.blackPieces[2][sq] = (uint8_t)((gameShiXiangTreatened[squareFlip(sq)] * redAttack +
                                                                                       gameShiXiangTreatless[squareFlip(sq)] * (TOTAL_ATTACK_VALUE - redAttack)) /
                                                                                      TOTAL_ATTACK_VALUE);
            normalEval.redPieces[6][sq] = (uint8_t)((bingAttack[sq] * redAttack +
                                                     bingUnAttack[sq] * (TOTAL_ATTACK_VALUE - redAttack)) /
                                                    TOTAL_ATTACK_VALUE);
            normalEval.blackPieces[6][sq] = (uint8_t)((bingAttack[squareFlip(sq)] * blackAttack +
                                                       bingUnAttack[squareFlip(sq)] * (TOTAL_ATTACK_VALUE - blackAttack)) /
                                                      TOTAL_ATTACK_VALUE);
        }
    }
    // 调整不受威胁方少掉的仕(士)相(象)分值
    this->red_val = SHI_XIANG_ATTACKLESS_VALUE * (TOTAL_ATTACK_VALUE - blackAttack) / TOTAL_ATTACK_VALUE;
    this->black_val = SHI_XIANG_ATTACKLESS_VALUE * (TOTAL_ATTACK_VALUE - redAttack) / TOTAL_ATTACK_VALUE;
    // 最后重新计算子力位置分
    for (i = 16; i < 32; i++) {
        sq = this->pieces[i];
        if (sq != 0) {
            this->red_val += normalEval.redPieces[pieceType[i]][sq];
        }
    }
    for (i = 32; i < 48; i++) {
        sq = this->pieces[i];
        if (sq != 0) {
            this->black_val += normalEval.blackPieces[pieceType[i]][sq];
        }
    }

    // 初始化exEval
    if (!cntInit) {
        cntInit = true;
        // 初始化"exEval.popCnt16"数组，只需要初始化一次
        for (i = 0; i < 65536; i++) {
            exEval.popCnt16[i] = popCnt16(i);
        }
        for (i = 0; i < 16; i++) {
            exEval.hollowThreatValue[i] = hollowThreat[i] * (midGameValue + TOTAL_MIDGAME_VALUE) / (TOTAL_MIDGAME_VALUE * 2);
            exEval.centralThreatValue[i] = centralThreat[i];
        }

        exEval.blackShiLeakage = TOTAL_SHI_LEAKAGE * redAttack / TOTAL_ATTACK_VALUE;
        exEval.redShiLeakage = TOTAL_SHI_LEAKAGE * blackAttack / TOTAL_ATTACK_VALUE;
        for (i = 0; i < 16; i++) {
            exEval.redBottomThreatValue[i] = bottomThreat[i] * blackAttack / TOTAL_ATTACK_VALUE;
            exEval.blackBottomThreatValue[i] = bottomThreat[i] * redAttack / TOTAL_ATTACK_VALUE;
        }
    }
}

// 特殊棋型的评价
int Game::advisorShapeValue() {
    // SlideMaskStruct *colMaskPtr(int x, int y) const {
    //     return preGen.colMaskTab[y - RANK_TOP] + bitCol[x];
    // }

    // SlideMaskStruct *rowMaskPtr(int x, int y) const {
    //   return preGen.rowMaskTab[y - RANK_TOP] + bitRow[x];
    // }
    int paoPos, juPos, sq, shiPos1, shiPos2, x, y, shiShape;
    int redPenal, blackPenal;
    SlideMaskStruct *slideMask;
    redPenal = blackPenal = 0;
    if ((this->splited_bitPieces[0] & SHI_BITPIECE) == SHI_BITPIECE) {
        if (this->pieces[sideTag(RED) + JIANG_FROM] == 0xc7) {
            shiPos1 = this->pieces[sideTag(RED) + SHI_FROM];
            shiPos2 = this->pieces[sideTag(RED) + SHI_TO];
            if (shiPos1 == 0xc6) { // 红方一个仕在左侧底线
                shiShape = (shiPos2 == 0xc8 ? SHAPE_CENTER : shiPos2 == 0xb7 ? SHAPE_LEFT
                                                                             : SHAPE_NONE);
            } else if (shiPos1 == 0xc8) { // 红方一个仕在右侧底线
                shiShape = (shiPos2 == 0xc6 ? SHAPE_CENTER : shiPos2 == 0xb7 ? SHAPE_RIGHT
                                                                             : SHAPE_NONE);
            } else if (shiPos1 == 0xb7) { // 红方一个仕在花心
                shiShape = (shiPos2 == 0xc6 ? SHAPE_LEFT : shiPos2 == 0xc8 ? SHAPE_RIGHT
                                                                           : SHAPE_NONE);
            } else {
                shiShape = SHAPE_NONE;
            }
            switch (shiShape) {
            case SHAPE_NONE:
                break;
            case SHAPE_CENTER:
                for (paoPos = sideTag(BLACK) + PAO_FROM; paoPos <= sideTag(BLACK) + PAO_TO; paoPos++) {
                    sq = this->pieces[paoPos];
                    if (sq != 0) {
                        x = getIdxCol(sq);
                        if (x == FILE_CENTER) {
                            y = getIdxRow(sq);
                            slideMask = preGen.rowMaskTab[y - RANK_TOP] + bitRow[x];
                            if ((slideMask->ju_cap & RED_JIANG_BITFILE) != 0) {
                                // 计算空头炮的威胁
                                redPenal += exEval.hollowThreatValue[idxRowFlip(y)];
                            }
                            // 等dino加
                            // else if ((slideMask->wSuperCap & RED_JIANG_BITFILE) != 0 &&
                            //     (this->board[0xb7] == 21 || this->board[0xb7] == 22)) {
                            //   // 计算炮镇窝心马的威胁
                            //   redPenal += exEval.centralThreatValue[idxRowFlip(y)];
                            // }
                        }
                    }
                }
                break;
            case SHAPE_LEFT:
            case SHAPE_RIGHT:
                for (paoPos = sideTag(BLACK) + PAO_FROM; paoPos <= sideTag(BLACK) + PAO_TO; paoPos++) {
                    sq = this->pieces[paoPos];
                    if (sq != 0) {
                        x = getIdxCol(sq);
                        y = getIdxRow(sq);
                        if (x == FILE_CENTER) {
                            // 等dino加
                            // if ((this->rowMaskPtr(x, y)->wSuperCap & RED_JIANG_BITFILE) != 0) {
                            //   // 计算一般中炮的威胁，帅(将)门被对方控制的还有额外罚分
                            //   redPenal += (exEval.centralThreatValue[idxRowFlip(y)] >> 2) +
                            //       (this->isProtected(1, shiShape == SHAPE_LEFT ? 0xc8 : 0xc6) ? 20 : 0);
                            //   // 如果车在底线保护帅(将)，则给予更大的罚分！
                            //   for (juPos = sideTag(RED) + JU_FROM; juPos <= sideTag(RED) + JU_TO; juPos ++) {
                            //     sq = this->pieces[juPos];
                            //     if (sq != 0) {
                            //       y = getIdxRow(sq);
                            //       if (y == RANK_BOTTOM) {
                            //         x = getIdxCol(sq);
                            //         if ((this->colMaskPtr(x, y)->JuCap & KING_BITRANK) != 0) {
                            //           redPenal += 80;
                            //         }
                            //       }
                            //     }
                            //   }
                            // }
                        } else if (y == RANK_BOTTOM) {
                            if (((preGen.colMaskTab[y - RANK_TOP] + bitCol[x])->ju_cap & KING_BITRANK) != 0) {
                                // 计算沉底炮的威胁
                                redPenal += exEval.redBottomThreatValue[x];
                            }
                        }
                    }
                }
                break;
            default:
                break;
            }
        } else if (this->pieces[sideTag(RED) + JIANG_FROM] == 0xb7) {
            // 有双仕(士)但花心被帅(将)占领，要罚分
            redPenal += 20;
        }
    } else {
        if ((this->splited_bitPieces[1] & JU_BITPIECE) == JU_BITPIECE) {
            // 缺仕(士)怕双车，有罚分
            redPenal += exEval.redShiLeakage;
        }
    }
    if ((this->splited_bitPieces[1] & SHI_BITPIECE) == SHI_BITPIECE) {
        if (this->pieces[sideTag(BLACK) + JIANG_FROM] == 0x37) {
            shiPos1 = this->pieces[sideTag(BLACK) + SHI_FROM];
            shiPos2 = this->pieces[sideTag(BLACK) + SHI_TO];
            if (false) {
            } else if (shiPos1 == 0x36) { // 黑方一个士在左侧底线
                shiShape = (shiPos2 == 0x38 ? SHAPE_CENTER : shiPos2 == 0x47 ? SHAPE_LEFT
                                                                             : SHAPE_NONE);
            } else if (shiPos1 == 0x38) { // 黑方一个士在右侧底线
                shiShape = (shiPos2 == 0x36 ? SHAPE_CENTER : shiPos2 == 0x47 ? SHAPE_RIGHT
                                                                             : SHAPE_NONE);
            } else if (shiPos1 == 0x47) { // 黑方一个士在花心
                shiShape = (shiPos2 == 0x36 ? SHAPE_LEFT : shiPos2 == 0x38 ? SHAPE_RIGHT
                                                                           : SHAPE_NONE);
            } else {
                shiShape = SHAPE_NONE;
            }
            switch (shiShape) {
            case SHAPE_NONE:
                break;
            case SHAPE_CENTER:
                for (paoPos = sideTag(RED) + PAO_FROM; paoPos <= sideTag(RED) + PAO_TO; paoPos++) {
                    sq = this->pieces[paoPos];
                    if (sq != 0) {
                        x = getIdxCol(sq);
                        if (x == FILE_CENTER) {
                            y = getIdxRow(sq);
                            slideMask = preGen.rowMaskTab[y - RANK_TOP] + bitRow[x];
                            if ((slideMask->ju_cap & BLACK_JIANG_BITFILE) != 0) {
                                // 计算空头炮的威胁
                                blackPenal += exEval.hollowThreatValue[y];
                            }

                            // 等dino加
                            // else if ((slideMask->wSuperCap & BLACK_JIANG_BITFILE) != 0 &&
                            //     (this->board[0x47] == 37 || this->board[0x47] == 38)) {
                            //   // 计算炮镇窝心马的威胁
                            //   blackPenal += exEval.centralThreatValue[y];
                            // }
                        }
                    }
                }
                break;
            case SHAPE_LEFT:
            case SHAPE_RIGHT:
                for (paoPos = sideTag(RED) + PAO_FROM; paoPos <= sideTag(RED) + PAO_TO; paoPos++) {
                    sq = this->pieces[paoPos];
                    if (sq != 0) {
                        x = getIdxCol(sq);
                        y = getIdxRow(sq);
                        if (x == FILE_CENTER) {
                            // 等dino加
                            // if ((this->rowMaskPtr(x, y)->wSuperCap & BLACK_JIANG_BITFILE) != 0) {
                            //   // 计算一般中炮的威胁，帅(将)门被对方控制的还有额外罚分
                            //   blackPenal += (exEval.centralThreatValue[y] >> 2) +
                            //       (this->isProtected(0, shiShape == SHAPE_LEFT ? 0x38 : 0x36) ? 20 : 0);
                            //   // 如果车在底线保护帅(将)，则给予更大的罚分！
                            //   for (juPos = sideTag(BLACK) + JU_FROM; juPos <= sideTag(BLACK) + JU_TO; juPos ++) {
                            //     sq = this->pieces[juPos];
                            //     if (sq != 0) {
                            //       y = getIdxRow(sq);
                            //       if (y == RANK_TOP) {
                            //         x = getIdxCol(sq);
                            //         if ((this->colMaskPtr(x, y)->JuCap & KING_BITRANK) != 0) {
                            //           blackPenal += 80;
                            //         }
                            //       }
                            //     }
                            //   }
                            // }
                        } else if (y == RANK_TOP) {
                            if (((preGen.colMaskTab[y - RANK_TOP] + bitCol[x])->ju_cap & KING_BITRANK) != 0) {
                                // 计算沉底炮的威胁
                                blackPenal += exEval.blackBottomThreatValue[x];
                            }
                        }
                    }
                }
                break;
            default:
                break;
            }
        } else if (this->pieces[sideTag(BLACK) + JIANG_FROM] == 0x47) {
            // 有双仕(士)但花心被帅(将)占领，要罚分
            blackPenal += 20;
        }
    } else {
        if ((this->splited_bitPieces[0] & JU_BITPIECE) == JU_BITPIECE) {
            // 缺仕(士)怕双车，有罚分
            blackPenal += exEval.blackShiLeakage;
        }
    }
    return sideValue(this->cur_player, blackPenal - redPenal);
}

// 车或炮牵制帅(将)或车的棋型的评价
int Game::stringHoldValue() {
    return 0;
}

int Game::getValue(int vlAlpha, int vlBeta) {
    int value;
    // 偷懒的局面评价函数分以下几个层次：

    // 1. 四级偷懒评价(彻底偷懒评价)，只包括子力平衡；
    value = this->materialValue();
    if (value + EVAL_MARGIN1 <= vlAlpha) {
        return value + EVAL_MARGIN1;
    } else if (value - EVAL_MARGIN1 >= vlBeta) {
        return value - EVAL_MARGIN1;
    }

    // 2. 三级偷懒评价，包括特殊棋型；
    value += this->advisorShapeValue();
    if (value + EVAL_MARGIN2 <= vlAlpha) {
        return value + EVAL_MARGIN2;
    } else if (value - EVAL_MARGIN2 >= vlBeta) {
        return value - EVAL_MARGIN2;
    }

    // 3. 二级偷懒评价，包括牵制；
    // value += this->stringHoldValue();
    // if (value + EVAL_MARGIN3 <= vlAlpha) {
    //   return value + EVAL_MARGIN3;
    // } else if (value - EVAL_MARGIN3 >= vlBeta) {
    //   return value - EVAL_MARGIN3;
    // }

    return value;
}

int Game::materialValue() {
    return cur_player == RED ? red_val - black_val : black_val - red_val;
}
#include <algorithm>
#include <iostream>

#include "evaluate.h"
#include "game.h"
#include "pregen.h"

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

// 预处理
void Game::evaBoard() {
    int i, sq, midGameValue, redAttack, blackAttack, redSimpleValue, blackSimpleValue;
    uint8_t bingAttack[256], bingUnAttack[256];
    // 首先判断局势处于开中局还是残局阶段，方法是计算各种棋子的数量，按照车=6、马炮=3、其它=1相加。
    midGameValue = popCnt32(this->bitPieces & bothBitpiece(SHI_BITPIECE | XIANG_BITPIECE | BING_BITPIECE)) * OTHER_MIDGAME_VALUE;
    midGameValue += popCnt32(this->bitPieces & bothBitpiece(MA_BITPIECE | PAO_BITPIECE)) * MA_PAO_MIDGAME_VALUE;
    midGameValue += popCnt32(this->bitPieces & bothBitpiece(JU_BITPIECE)) * JU_MIDGAME_VALUE;
    // 使用二次函数，子力很少时才认为接近残局
    midGameValue = (2 * TOTAL_MIDGAME_VALUE - midGameValue) * midGameValue / TOTAL_MIDGAME_VALUE;
    normalEval.advanceValue = (TOTAL_ADVANCED_VALUE * midGameValue + TOTAL_ADVANCED_VALUE / 2) / TOTAL_MIDGAME_VALUE;
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

// 棋力平衡
int Game::materialValue() {
    return sideValue(this->cur_player, red_val - black_val) + normalEval.advanceValue;
}

// 特殊棋型的评价
int Game::shiShapeValue() {
    int paoPos, juPos, sq, shiPos1, shiPos2, x, y, shiShape, shiShapeCnt, i, myside;
    SlideMaskStruct *slideMask;
    int penalValue[2];
    for (i = 0; i < 2; i++) {
        // i = 0为红
        myside = 1 - i;
        penalValue[i] = 0;
        // 有双仕
        if ((this->splited_bitPieces[i] & SHI_BITPIECE) == SHI_BITPIECE) {
            // 将在老家
            if (this->pieces[sideTag(myside) + JIANG_FROM] == (i == 0 ? 0xc7 : 0x37)) {
                shiPos1 = this->pieces[sideTag(myside) + SHI_FROM];
                shiPos2 = this->pieces[sideTag(myside) + SHI_TO];
                // 先计算仕的形状
                shiShapeCnt = shiPos1 + shiPos2;
                if (i == 0) {
                    if (shiShape == 0xc6 + 0xc8)
                        shiShape = SHAPE_CENTER;
                    else if (shiShape == 0xc6 + 0xb7)
                        shiShape = SHAPE_LEFT;
                    else if (shiShape == 0xc8 + 0xb7)
                        shiShape = SHAPE_RIGHT;
                    else
                        shiShape = SHAPE_NONE;
                } else {
                    if (shiShape == 0x36 + 0x38)
                        shiShape = SHAPE_CENTER;
                    else if (shiShape == 0x36 + 0x47)
                        shiShape = SHAPE_LEFT;
                    else if (shiShape == 0x38 + 0x47)
                        shiShape = SHAPE_RIGHT;
                    else
                        shiShape = SHAPE_NONE;
                }

                // 双仕都在底线,判断空头炮和炮镇窝心马
                if (shiShape == SHAPE_CENTER) {
                    for (paoPos = sideTag(i) + PAO_FROM; paoPos <= sideTag(i) + PAO_TO; paoPos++) {
                        sq = this->pieces[paoPos];
                        if (sq != 0) {
                            x = getIdxCol(sq);
                            // 如果炮在中路
                            if (x == COL_CENTER) {
                                y = getIdxRow(sq);
                                slideMask = preGen.colMaskTab[y - ROW_TOP] + bitCol[x];
                                if (i == 0) {
                                    if ((slideMask->ju_cap & RED_JIANG_BITFILE) != 0) {
                                        // 计算空头炮的威胁
                                        penalValue[i] += exEval.hollowThreatValue[idxRowFlip(y)];
                                    } else if ((slideMask->cannon_cap & RED_JIANG_BITFILE) != 0 &&
                                               (this->board[0xb7] == 21 || this->board[0xb7] == 22)) {
                                        // 计算炮镇窝心马的威胁
                                        penalValue[i] += exEval.centralThreatValue[idxRowFlip(y)];
                                    }
                                } else {
                                    if ((slideMask->ju_cap & BLACK_JIANG_BITFILE) != 0) {
                                        penalValue[i] += exEval.hollowThreatValue[y];
                                    } else if ((slideMask->cannon_cap & BLACK_JIANG_BITFILE) != 0 &&
                                               (this->board[0x47] == 37 || this->board[0x47] == 38)) {
                                        penalValue[i] += exEval.centralThreatValue[y];
                                    }
                                }
                            }
                        }
                    }
                }
                // 双仕包围帅，判断沉底炮和车封帅门
                else if (shiShape == SHAPE_LEFT || SHAPE_RIGHT) {
                    for (paoPos = sideTag(i) + PAO_FROM; paoPos <= sideTag(i) + PAO_TO; paoPos++) {
                        sq = this->pieces[paoPos];
                        if (sq != 0) {
                            x = getIdxCol(sq);
                            y = getIdxRow(sq);
                            if (x == COL_CENTER) {
                                if (((preGen.colMaskTab[y - ROW_TOP] + bitCol[x])->cannon_cap & (i == 0 ? RED_JIANG_BITFILE : BLACK_JIANG_BITFILE)) != 0) {
                                    // 计算一般中炮的威胁，帅(将)门被对方控制的还有额外罚分
                                    if (i == 0)
                                        penalValue[i] += (exEval.centralThreatValue[idxRowFlip(y)] >> 2) +
                                                         (this->isProtected(sideTag(BLACK), shiShape == SHAPE_LEFT ? 0xc8 : 0xc6) ? 20 : 0);
                                    else
                                        penalValue[i] += (exEval.centralThreatValue[y] >> 2) +
                                                         (this->isProtected(sideTag(RED), shiShape == SHAPE_LEFT ? 0x38 : 0x36) ? 20 : 0);

                                    // 如果车在底线保护帅(将)，则给予更大的罚分
                                    for (juPos = sideTag(myside) + JU_FROM; juPos <= sideTag(myside) + JU_TO; juPos++) {
                                        sq = this->pieces[juPos];
                                        if (sq != 0) {
                                            y = getIdxRow(sq);
                                            if (y == (i == 0 ? ROW_BOTTOM : ROW_TOP)) {
                                                x = getIdxCol(sq);
                                                if (((preGen.rowMaskTab[x - ROW_TOP] + bitRow[y])->ju_cap & KING_BITRANK) != 0) {
                                                    penalValue[i] += 80;
                                                }
                                            }
                                        }
                                    }
                                }
                            } else if (y == (i == 0 ? ROW_BOTTOM : ROW_TOP)) {
                                if (((preGen.rowMaskTab[x - ROW_TOP] + bitRow[y])->ju_cap & KING_BITRANK) != 0) {
                                    // 计算沉底炮的威胁
                                    penalValue[i] += (i == 0 ? exEval.redBottomThreatValue[x] : exEval.blackBottomThreatValue[x]);
                                }
                            }
                        }
                    }
                }
            }
            // 有双仕但花心被帅占领
            else if (this->pieces[sideTag(myside) + JIANG_FROM] == (i == 0 ? 0xb7 : 0x47)) {
                penalValue[i] += 20;
            }
        }
        // 缺仕怕双车
        else if ((this->splited_bitPieces[1 - i] & JU_BITPIECE) == JU_BITPIECE) {
            penalValue[i] += (i == 0 ? exEval.redShiLeakage : exEval.blackShiLeakage);
        }
    }
    return sideValue(this->cur_player, penalValue[1] - penalValue[0]);
}

// 车或炮牵制帅(将)或车的棋型的评价
int Game::stringHoldValue() {
    int side, i, j, strJudge, srcSq, dstSq, strSq;
    bool flagx;
    int x, y, thisSide, oppSide;
    int stringValue[2];
    SlideMoveStruct *slideMove;
    // 遍历两方
    for (side = 0; side < 2; side++) {
        stringValue[side] = 0;
        thisSide = sideTag(side);
        oppSide = sideTag(!side);
        // 用车(炮)牵制,目标是炮（超级炮）能打到的子，被牵制的是本来能吃的
        for (i = JU_FROM; i <= PAO_TO; i++) {
            srcSq = this->pieces[thisSide + i];
            if (srcSq != 0) {
                x = getIdxCol(srcSq);
                y = getIdxRow(srcSq);
                for (j = 0; j < 3; j++) {
                    if (j == 0) // 牵制将
                        dstSq = this->pieces[oppSide + JIANG_FROM];
                    else if (j == 1) // 牵制车
                        dstSq = this->pieces[oppSide + JU_FROM];
                    else
                        dstSq = this->pieces[oppSide + JU_TO];
                    if (dstSq != 0) {
                        flagx = (x == getIdxCol(dstSq));
                        if (flagx || (y == getIdxRow(dstSq))) {
                            slideMove = (flagx ? (preGen.colMoveTab[y - ROW_TOP] + bitCol[x]) : (preGen.rowMoveTab[x - ROW_TOP] + bitRow[y]));
                            strJudge = (srcSq < dstSq ? 0 : 1);
                            if (dstSq == (i <= JU_TO ? slideMove->paoCap[strJudge] : slideMove->cannonCap[strJudge]) + (flagx ? x : (y << 4))) {
                                strSq = (i <= JU_TO ? slideMove->juCap[strJudge] : slideMove->paoCap[strJudge]) + (flagx ? x : (y << 4));
                                if ((this->board[strSq] & oppSide) != 0) {
                                    if (stringPiecesValue[this->board[strSq]] > 0 &&
                                        !this->isProtected(sideTag(!side), strSq, dstSq) &&
                                        (i > JU_TO || j == 0 || !this->isProtected(sideTag(!side), dstSq))) {
                                        stringValue[side] += stringValueTab[dstSq - strSq + 256];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return sideValue(this->cur_player, stringValue[RED] - stringValue[BLACK]);
}

// 车的灵活性的评价
int Game::juMobilityValue() {
    int side, i, srcSq, thisSide, x, y;
    int juMobValue[2];
    for (side = 0; side < 2; side++) {
        juMobValue[side] = 0;
        thisSide = sideTag(side);
        for (i = JU_FROM; i <= JU_TO; i++) {
            srcSq = this->pieces[thisSide + i];
            if (srcSq != 0) {
                x = getIdxCol(srcSq);
                y = getIdxRow(srcSq);
                juMobValue[side] += exEval.popCnt16[(preGen.rowMaskTab[x - ROW_TOP] + bitRow[y])->non_cap] +
                                    exEval.popCnt16[(preGen.colMaskTab[y - ROW_TOP] + bitCol[x])->non_cap];
            }
        }
    }
    return sideValue(this->cur_player, juMobValue[RED] - juMobValue[BLACK]) >> 1;
}

// 马受到阻碍的评价
int Game::maTrapValue() {
    int side, i, srcSq, dstSq, thisSide, maAvPos;
    uint8_t *maMoves, *maLegs;
    int maTrapValue[2];

    for (side = 0; side < 2; side++) {
        maTrapValue[side] = 0;
        thisSide = sideTag(side);
        // 考虑马可以走的位置，走到棋盘边缘上，或者走到对方的控制格，都必须排除
        for (i = MA_FROM; i <= MA_TO; i++) {
            srcSq = this->pieces[thisSide + i];
            if (srcSq != 0) {
                maAvPos = 0;
                maMoves = preGen.preMaMoves[srcSq];
                maLegs = preGen.preMaLegs[srcSq];
                dstSq = *maMoves;
                while (dstSq != 0) {
                    // 排除了走到棋盘边缘和走到对方控制格的着法
                    if (!edgeSquares[dstSq] && this->board[dstSq] == 0 &&
                        this->board[*maLegs] == 0 && !this->isProtected(sideTag(!side), dstSq)) {
                        maAvPos++;
                        if (maAvPos > 1) {
                            break;
                        }
                    }
                    maMoves++;
                    dstSq = *maMoves;
                    maLegs++;
                }
                if (maAvPos == 0) {
                    maTrapValue[side] += 10;
                } else if (maAvPos == 1) {
                    maTrapValue[side] += 5;
                }
            }
        }
    }
    return sideValue(this->cur_player, maTrapValue[BLACK] - maTrapValue[RED]);
}

// 总的估值函数
int Game::getValue(int vlAlpha, int vlBeta) {
    int value;

    // 1. 包括子力平衡；
    value = this->materialValue();
    if (value + EVAL_MARGIN1 <= vlAlpha) {
        return value + EVAL_MARGIN1;
    } else if (value - EVAL_MARGIN1 >= vlBeta) {
        return value - EVAL_MARGIN1;
    }

    // 2. 包括特殊棋型；
    value += this->shiShapeValue();
    if (value + EVAL_MARGIN2 <= vlAlpha) {
        return value + EVAL_MARGIN2;
    } else if (value - EVAL_MARGIN2 >= vlBeta) {
        return value - EVAL_MARGIN2;
    }

    // 3. 包括牵制；
    value += this->stringHoldValue();
    if (value + EVAL_MARGIN3 <= vlAlpha) {
        return value + EVAL_MARGIN3;
    } else if (value - EVAL_MARGIN3 >= vlBeta) {
        return value - EVAL_MARGIN3;
    }

    // 4. 包括车的灵活性；
    value += this->juMobilityValue();
    if (value + EVAL_MARGIN4 <= vlAlpha) {
        return value + EVAL_MARGIN4;
    } else if (value - EVAL_MARGIN4 >= vlBeta) {
        return value - EVAL_MARGIN4;
    }

    // 5. 包括马的阻碍。
    return value + this->maTrapValue();
}

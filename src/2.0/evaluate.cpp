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

// Ԥ����
void Game::evaBoard() {
    int i, sq, midGameValue, redAttack, blackAttack, redSimpleValue, blackSimpleValue;
    uint8_t bingAttack[256], bingUnAttack[256];
    // �����жϾ��ƴ��ڿ��оֻ��ǲоֽ׶Σ������Ǽ���������ӵ����������ճ�=6������=3������=1��ӡ�
    midGameValue = popCnt32(this->bitPieces & bothBitpiece(SHI_BITPIECE | XIANG_BITPIECE | BING_BITPIECE)) * OTHER_MIDGAME_VALUE;
    midGameValue += popCnt32(this->bitPieces & bothBitpiece(MA_BITPIECE | PAO_BITPIECE)) * MA_PAO_MIDGAME_VALUE;
    midGameValue += popCnt32(this->bitPieces & bothBitpiece(JU_BITPIECE)) * JU_MIDGAME_VALUE;
    // ʹ�ö��κ�������������ʱ����Ϊ�ӽ��о�
    midGameValue = (2 * TOTAL_MIDGAME_VALUE - midGameValue) * midGameValue / TOTAL_MIDGAME_VALUE;
    normalEval.advanceValue = (TOTAL_ADVANCED_VALUE * midGameValue + TOTAL_ADVANCED_VALUE / 2) / TOTAL_MIDGAME_VALUE;
    for (sq = 0; sq < 256; sq++) {
        // ������
        if (inBoard(sq)) {
            normalEval.redPieces[0][sq] = normalEval.blackPieces[0][squareFlip(sq)] = (uint8_t)((midGameJiangBingAttack[sq] * midGameValue + endGameJiangBingAttack[sq] * (TOTAL_MIDGAME_VALUE - midGameValue)) / TOTAL_MIDGAME_VALUE);
            normalEval.redPieces[3][sq] = normalEval.blackPieces[3][squareFlip(sq)] = (uint8_t)((midGameMa[sq] * midGameValue + endGameMa[sq] * (TOTAL_MIDGAME_VALUE - midGameValue)) / TOTAL_MIDGAME_VALUE);
            normalEval.redPieces[4][sq] = normalEval.blackPieces[4][squareFlip(sq)] = (uint8_t)((midGameJu[sq] * midGameValue + endGameJu[sq] * (TOTAL_MIDGAME_VALUE - midGameValue)) / TOTAL_MIDGAME_VALUE);
            normalEval.redPieces[5][sq] = normalEval.blackPieces[5][squareFlip(sq)] = (uint8_t)((midGamePao[sq] * midGameValue + endGamePao[sq] * (TOTAL_MIDGAME_VALUE - midGameValue)) / TOTAL_MIDGAME_VALUE);
            bingAttack[sq] = normalEval.redPieces[0][sq];
            bingUnAttack[sq] = (uint8_t)((midGameJiangBingUnAttack[sq] * midGameValue + endGameJiangBingUnAttack[sq] * (TOTAL_MIDGAME_VALUE - midGameValue)) / TOTAL_MIDGAME_VALUE);
        }
    }

    // Ȼ���жϸ����Ƿ��ڽ���״̬�������Ǽ�����ֹ������ӵ����������ճ���2�ڱ�1��ӡ�
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
    for (sq = 0; sq < 256; sq++) {
        if (inBoard(sq)) {
            // ʿ���
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
    // ����������в���ٵ�����(ʿ)��(��)��ֵ
    this->red_val = SHI_XIANG_ATTACKLESS_VALUE * (TOTAL_ATTACK_VALUE - blackAttack) / TOTAL_ATTACK_VALUE;
    this->black_val = SHI_XIANG_ATTACKLESS_VALUE * (TOTAL_ATTACK_VALUE - redAttack) / TOTAL_ATTACK_VALUE;
    // ������¼�������λ�÷�
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

    // ��ʼ��exEval
    if (!cntInit) {
        cntInit = true;
        // ��ʼ��"exEval.popCnt16"���飬ֻ��Ҫ��ʼ��һ��
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

// ����ƽ��
int Game::materialValue() {
    return sideValue(this->cur_player, red_val - black_val) + normalEval.advanceValue;
}

// �������͵�����
int Game::shiShapeValue() {
    int paoPos, juPos, sq, shiPos1, shiPos2, x, y, shiShape, shiShapeCnt, i, myside;
    SlideMaskStruct *slideMask;
    int penalValue[2];
    for (i = 0; i < 2; i++) {
        // i = 0Ϊ��
        myside = 1 - i;
        penalValue[i] = 0;
        // ��˫��
        if ((this->splited_bitPieces[i] & SHI_BITPIECE) == SHI_BITPIECE) {
            // �����ϼ�
            if (this->pieces[sideTag(myside) + JIANG_FROM] == (i == 0 ? 0xc7 : 0x37)) {
                shiPos1 = this->pieces[sideTag(myside) + SHI_FROM];
                shiPos2 = this->pieces[sideTag(myside) + SHI_TO];
                // �ȼ����˵���״
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

                // ˫�˶��ڵ���,�жϿ�ͷ�ں�����������
                if (shiShape == SHAPE_CENTER) {
                    for (paoPos = sideTag(i) + PAO_FROM; paoPos <= sideTag(i) + PAO_TO; paoPos++) {
                        sq = this->pieces[paoPos];
                        if (sq != 0) {
                            x = getIdxCol(sq);
                            // ���������·
                            if (x == COL_CENTER) {
                                y = getIdxRow(sq);
                                slideMask = preGen.colMaskTab[y - ROW_TOP] + bitCol[x];
                                if (i == 0) {
                                    if ((slideMask->ju_cap & RED_JIANG_BITFILE) != 0) {
                                        // �����ͷ�ڵ���в
                                        penalValue[i] += exEval.hollowThreatValue[idxRowFlip(y)];
                                    } else if ((slideMask->cannon_cap & RED_JIANG_BITFILE) != 0 &&
                                               (this->board[0xb7] == 21 || this->board[0xb7] == 22)) {
                                        // �����������������в
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
                // ˫�˰�Χ˧���жϳ����ںͳ���˧��
                else if (shiShape == SHAPE_LEFT || SHAPE_RIGHT) {
                    for (paoPos = sideTag(i) + PAO_FROM; paoPos <= sideTag(i) + PAO_TO; paoPos++) {
                        sq = this->pieces[paoPos];
                        if (sq != 0) {
                            x = getIdxCol(sq);
                            y = getIdxRow(sq);
                            if (x == COL_CENTER) {
                                if (((preGen.colMaskTab[y - ROW_TOP] + bitCol[x])->cannon_cap & (i == 0 ? RED_JIANG_BITFILE : BLACK_JIANG_BITFILE)) != 0) {
                                    // ����һ�����ڵ���в��˧(��)�ű��Է����ƵĻ��ж��ⷣ��
                                    if (i == 0)
                                        penalValue[i] += (exEval.centralThreatValue[idxRowFlip(y)] >> 2) +
                                                         (this->isProtected(sideTag(BLACK), shiShape == SHAPE_LEFT ? 0xc8 : 0xc6) ? 20 : 0);
                                    else
                                        penalValue[i] += (exEval.centralThreatValue[y] >> 2) +
                                                         (this->isProtected(sideTag(RED), shiShape == SHAPE_LEFT ? 0x38 : 0x36) ? 20 : 0);

                                    // ������ڵ��߱���˧(��)����������ķ���
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
                                    // ��������ڵ���в
                                    penalValue[i] += (i == 0 ? exEval.redBottomThreatValue[x] : exEval.blackBottomThreatValue[x]);
                                }
                            }
                        }
                    }
                }
            }
            // ��˫�˵����ı�˧ռ��
            else if (this->pieces[sideTag(myside) + JIANG_FROM] == (i == 0 ? 0xb7 : 0x47)) {
                penalValue[i] += 20;
            }
        }
        // ȱ����˫��
        else if ((this->splited_bitPieces[1 - i] & JU_BITPIECE) == JU_BITPIECE) {
            penalValue[i] += (i == 0 ? exEval.redShiLeakage : exEval.blackShiLeakage);
        }
    }
    return sideValue(this->cur_player, penalValue[1] - penalValue[0]);
}

// ������ǣ��˧(��)�򳵵����͵�����
int Game::stringHoldValue() {
    int side, i, j, strJudge, srcSq, dstSq, strSq;
    bool flagx;
    int x, y, thisSide, oppSide;
    int stringValue[2];
    SlideMoveStruct *slideMove;
    // ��������
    for (side = 0; side < 2; side++) {
        stringValue[side] = 0;
        thisSide = sideTag(side);
        oppSide = sideTag(!side);
        // �ó�(��)ǣ��,Ŀ�����ڣ������ڣ��ܴ򵽵��ӣ���ǣ�Ƶ��Ǳ����ܳԵ�
        for (i = JU_FROM; i <= PAO_TO; i++) {
            srcSq = this->pieces[thisSide + i];
            if (srcSq != 0) {
                x = getIdxCol(srcSq);
                y = getIdxRow(srcSq);
                for (j = 0; j < 3; j++) {
                    if (j == 0) // ǣ�ƽ�
                        dstSq = this->pieces[oppSide + JIANG_FROM];
                    else if (j == 1) // ǣ�Ƴ�
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

// ��������Ե�����
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

// ���ܵ��谭������
int Game::maTrapValue() {
    int side, i, srcSq, dstSq, thisSide, maAvPos;
    uint8_t *maMoves, *maLegs;
    int maTrapValue[2];

    for (side = 0; side < 2; side++) {
        maTrapValue[side] = 0;
        thisSide = sideTag(side);
        // ����������ߵ�λ�ã��ߵ����̱�Ե�ϣ������ߵ��Է��Ŀ��Ƹ񣬶������ų�
        for (i = MA_FROM; i <= MA_TO; i++) {
            srcSq = this->pieces[thisSide + i];
            if (srcSq != 0) {
                maAvPos = 0;
                maMoves = preGen.preMaMoves[srcSq];
                maLegs = preGen.preMaLegs[srcSq];
                dstSq = *maMoves;
                while (dstSq != 0) {
                    // �ų����ߵ����̱�Ե���ߵ��Է����Ƹ���ŷ�
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

// �ܵĹ�ֵ����
int Game::getValue(int vlAlpha, int vlBeta) {
    int value;

    // 1. ��������ƽ�⣻
    value = this->materialValue();
    if (value + EVAL_MARGIN1 <= vlAlpha) {
        return value + EVAL_MARGIN1;
    } else if (value - EVAL_MARGIN1 >= vlBeta) {
        return value - EVAL_MARGIN1;
    }

    // 2. �����������ͣ�
    value += this->shiShapeValue();
    if (value + EVAL_MARGIN2 <= vlAlpha) {
        return value + EVAL_MARGIN2;
    } else if (value - EVAL_MARGIN2 >= vlBeta) {
        return value - EVAL_MARGIN2;
    }

    // 3. ����ǣ�ƣ�
    value += this->stringHoldValue();
    if (value + EVAL_MARGIN3 <= vlAlpha) {
        return value + EVAL_MARGIN3;
    } else if (value - EVAL_MARGIN3 >= vlBeta) {
        return value - EVAL_MARGIN3;
    }

    // 4. ������������ԣ�
    value += this->juMobilityValue();
    if (value + EVAL_MARGIN4 <= vlAlpha) {
        return value + EVAL_MARGIN4;
    } else if (value - EVAL_MARGIN4 >= vlBeta) {
        return value - EVAL_MARGIN4;
    }

    // 5. ��������谭��
    return value + this->maTrapValue();
}

#include <algorithm>
#include "evaluate.h"
#include "game.h"
#include "pregen.h"
/* ����Ԥ���۾��ǳ�ʼ������Ԥ��������(PreEval)�Ĺ��̡�
 * ����Ԥ������Ҫ�������������棺
 * 1. �жϾ��ƴ��ڿ��оֻ��ǲоֽ׶Σ�
 * 2. �ж�ÿһ���Ƿ�ԶԷ��γ���в��
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
    // �����жϾ��ƴ��ڿ��оֻ��ǲоֽ׶Σ������Ǽ���������ӵ����������ճ�=6������=3������=1��ӡ�
    midGameValue = popCnt32(this->bitPieces & bothBitpiece(SHI_BITPIECE | XIANG_BITPIECE | BING_BITPIECE)) * OTHER_MIDGAME_VALUE;
    midGameValue += popCnt32(this->bitPieces & bothBitpiece(MA_BITPIECE | PAO_BITPIECE)) * MA_PAO_MIDGAME_VALUE;
    midGameValue += popCnt32(this->bitPieces & bothBitpiece(JU_BITPIECE)) * JU_MIDGAME_VALUE;
    // ʹ�ö��κ�������������ʱ����Ϊ�ӽ��о�
    midGameValue = (2 * TOTAL_MIDGAME_VALUE - midGameValue) * midGameValue / TOTAL_MIDGAME_VALUE;
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

// �������͵�����
int Game::shiShapeValue() {
    // SlideMaskStruct *rowMaskPtr(int x, int y) const {
    //   return preGen.rowMaskTab[x - RANK_TOP] + bitRow[y];
    // }//rank
    // SlideMaskStruct *colMaskPtr(int x, int y) const {
    //     return preGen.colMaskTab[y - RANK_TOP] + bitCol[x];
    // }//file
    int paoPos, juPos, sq, shiPos1, shiPos2, x, y, shiShape;
    int redPenal, blackPenal;
    SlideMaskStruct *slideMask;
    redPenal = blackPenal = 0;
    if ((this->splited_bitPieces[0] & SHI_BITPIECE) == SHI_BITPIECE) {
        if (this->pieces[sideTag(RED) + JIANG_FROM] == 0xc7) {
            shiPos1 = this->pieces[sideTag(RED) + SHI_FROM];
            shiPos2 = this->pieces[sideTag(RED) + SHI_TO];
            if (shiPos1 == 0xc6) { // �췽һ������������
                shiShape = (shiPos2 == 0xc8 ? SHAPE_CENTER : shiPos2 == 0xb7 ? SHAPE_LEFT
                                                                             : SHAPE_NONE);
            } else if (shiPos1 == 0xc8) { // �췽һ�������Ҳ����
                shiShape = (shiPos2 == 0xc6 ? SHAPE_CENTER : shiPos2 == 0xb7 ? SHAPE_RIGHT
                                                                             : SHAPE_NONE);
            } else if (shiPos1 == 0xb7) { // �췽һ�����ڻ���
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
                        if (x == COL_CENTER) {
                            y = getIdxRow(sq);
                            slideMask = preGen.colMaskTab[y - ROW_TOP] + bitCol[x];
                            if ((slideMask->ju_cap & RED_JIANG_BITFILE) != 0) {
                                // �����ͷ�ڵ���в
                                redPenal += exEval.hollowThreatValue[idxRowFlip(y)];
                            }
                            // ��dino��
                            else if ((slideMask->cannon_cap & RED_JIANG_BITFILE) != 0 &&
                                     (this->board[0xb7] == 21 || this->board[0xb7] == 22)) {
                                // �����������������в
                                redPenal += exEval.centralThreatValue[idxRowFlip(y)];
                            }
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
                        if (x == COL_CENTER) {
                            // ��dino��
                            if (((preGen.colMaskTab[y - ROW_TOP] + bitCol[x])->cannon_cap & RED_JIANG_BITFILE) != 0) {
                                // ����һ�����ڵ���в��˧(��)�ű��Է����ƵĻ��ж��ⷣ��
                                redPenal += (exEval.centralThreatValue[idxRowFlip(y)] >> 2) +
                                            (this->isProtected(1, shiShape == SHAPE_LEFT ? 0xc8 : 0xc6) ? 20 : 0);
                                // ������ڵ��߱���˧(��)����������ķ���
                                for (juPos = sideTag(RED) + JU_FROM; juPos <= sideTag(RED) + JU_TO; juPos++) {
                                    sq = this->pieces[juPos];
                                    if (sq != 0) {
                                        y = getIdxRow(sq);
                                        if (y == ROW_BOTTOM) {
                                            x = getIdxCol(sq);
                                            if (((preGen.rowMaskTab[x - ROW_TOP] + bitRow[y])->ju_cap & KING_BITRANK) != 0) {
                                                redPenal += 80;
                                            }
                                        }
                                    }
                                }
                            }
                        } else if (y == ROW_BOTTOM) {
                            if (((preGen.rowMaskTab[x - ROW_TOP] + bitRow[y])->ju_cap & KING_BITRANK) != 0) {
                                // ��������ڵ���в
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
            // ��˫��(ʿ)�����ı�˧(��)ռ�죬Ҫ����
            redPenal += 20;
        }
    } else {
        if ((this->splited_bitPieces[1] & JU_BITPIECE) == JU_BITPIECE) {
            // ȱ��(ʿ)��˫�����з���
            redPenal += exEval.redShiLeakage;
        }
    }
    if ((this->splited_bitPieces[1] & SHI_BITPIECE) == SHI_BITPIECE) {
        if (this->pieces[sideTag(BLACK) + JIANG_FROM] == 0x37) {
            shiPos1 = this->pieces[sideTag(BLACK) + SHI_FROM];
            shiPos2 = this->pieces[sideTag(BLACK) + SHI_TO];
            if (false) {
            } else if (shiPos1 == 0x36) { // �ڷ�һ��ʿ��������
                shiShape = (shiPos2 == 0x38 ? SHAPE_CENTER : shiPos2 == 0x47 ? SHAPE_LEFT
                                                                             : SHAPE_NONE);
            } else if (shiPos1 == 0x38) { // �ڷ�һ��ʿ���Ҳ����
                shiShape = (shiPos2 == 0x36 ? SHAPE_CENTER : shiPos2 == 0x47 ? SHAPE_RIGHT
                                                                             : SHAPE_NONE);
            } else if (shiPos1 == 0x47) { // �ڷ�һ��ʿ�ڻ���
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
                        if (x == COL_CENTER) {
                            y = getIdxRow(sq);
                            slideMask = preGen.colMaskTab[y - ROW_TOP] + bitCol[x];
                            if ((slideMask->ju_cap & BLACK_JIANG_BITFILE) != 0) {
                                // �����ͷ�ڵ���в
                                blackPenal += exEval.hollowThreatValue[y];
                            }

                            // ��dino��
                            else if ((slideMask->cannon_cap & BLACK_JIANG_BITFILE) != 0 &&
                                     (this->board[0x47] == 37 || this->board[0x47] == 38)) {
                                // �����������������в
                                blackPenal += exEval.centralThreatValue[y];
                            }
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
                        if (x == COL_CENTER) {
                            if (((preGen.colMaskTab[y - ROW_TOP] + bitCol[x])->cannon_cap & BLACK_JIANG_BITFILE) != 0) {
                                // ����һ�����ڵ���в��˧(��)�ű��Է����ƵĻ��ж��ⷣ��
                                blackPenal += (exEval.centralThreatValue[y] >> 2) +
                                              (this->isProtected(0, shiShape == SHAPE_LEFT ? 0x38 : 0x36) ? 20 : 0);
                                // ������ڵ��߱���˧(��)����������ķ��֣�
                                for (juPos = sideTag(BLACK) + JU_FROM; juPos <= sideTag(BLACK) + JU_TO; juPos++) {
                                    sq = this->pieces[juPos];
                                    if (sq != 0) {
                                        y = getIdxRow(sq);
                                        if (y == ROW_TOP) {
                                            x = getIdxCol(sq);
                                            if (((preGen.rowMaskTab[x - ROW_TOP] + bitRow[y])->ju_cap & KING_BITRANK) != 0) {
                                                blackPenal += 80;
                                            }
                                        }
                                    }
                                }
                            }
                        } else if (y == ROW_TOP) {
                            if (((preGen.rowMaskTab[x - ROW_TOP] + bitRow[y])->ju_cap & KING_BITRANK) != 0) {
                                // ��������ڵ���в
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
            // ��˫��(ʿ)�����ı�˧(��)ռ�죬Ҫ����
            blackPenal += 20;
        }
    } else {
        if ((this->splited_bitPieces[0] & JU_BITPIECE) == JU_BITPIECE) {
            // ȱ��(ʿ)��˫�����з���
            blackPenal += exEval.blackShiLeakage;
        }
    }
    return sideValue(this->cur_player, blackPenal - redPenal);
}

// ������ǣ��˧(��)�򳵵����͵�����
int Game::stringHoldValue() {
    int side, i, j, strJudge, srcSq, dstSq, strSq;
    int x, y, thisSide, oppSide;
    int stringValue[2];
    SlideMoveStruct *slideMove;

    for (side = 0; side < 2; side++) {
        stringValue[side] = 0;
        thisSide = sideTag(side);
        oppSide = sideTag(!side);
        // �����ó���ǣ�Ƶ����
        for (i = JU_FROM; i <= JU_TO; i++) {
            srcSq = this->pieces[thisSide + i];
            if (srcSq != 0) {
                // ����ǣ��Ŀ����˧(��)�����
                dstSq = this->pieces[oppSide + JIANG_FROM];
                if (dstSq != 0) {
                    x = getIdxCol(srcSq);
                    y = getIdxRow(srcSq);
                    if (x == getIdxCol(dstSq)) {
                        slideMove = (preGen.rowMoveTab[y - ROW_TOP] + bitRow[x]);
                        strJudge = (srcSq < dstSq ? 0 : 1);
                        // ��������ڵĳԷ�(���ó����ڵ��ŷ�)�ܳԵ�Ŀ����"dstSq"��ǣ�ƾͳ����ˣ���ͬ
                        if (dstSq == slideMove->paoCap[strJudge] + (int)x) {
                            // ��ǣ����"strSq"�ǳ�(��)�����ܳԵ������ӣ���ͬ
                            strSq = slideMove->juCap[strJudge] + (int)x;
                            // ��ǣ���ӱ����ǶԷ����ӣ���ͬ
                            if ((this->board[strSq] & oppSide) != 0) {
                                // �����ǣ�������м�ֵ�ģ����ұ�ǣ����û�б���(��Ŀ���ӱ�������)����ôǣ�����м�ֵ�ģ���ͬ
                                if (stringPiecesValue[this->board[strSq]] > 0 &&
                                    !this->isProtected(sideTag(!side), strSq, dstSq)) {
                                    stringValue[side] += stringValueTab[dstSq - strSq + 256];
                                }
                            }
                        }
                    } else if (y == getIdxRow(dstSq)) {
                        slideMove = (preGen.rowMoveTab[x - ROW_TOP] + bitRow[y]);
                        strJudge = (srcSq < dstSq ? 0 : 1);
                        if (dstSq == slideMove->paoCap[strJudge] + (y<<4)) {
                            strSq = slideMove->juCap[strJudge] + (y<<4);
                            if ((this->board[strSq] & oppSide) != 0) {
                                if (stringPiecesValue[this->board[strSq]] > 0 &&
                                    !this->isProtected(sideTag(!side), strSq, dstSq)) {
                                    stringValue[side] += stringValueTab[dstSq - strSq + 256];
                                }
                            }
                        }
                    }
                }
                // ����ǣ��Ŀ���ǳ������
                for (j = JU_FROM; j <= JU_TO; j++) {
                    dstSq = this->pieces[oppSide + j];
                    if (dstSq != 0) {
                        x = getIdxCol(srcSq);
                        y = getIdxRow(srcSq);
                        if (x == getIdxCol(dstSq)) {
                            slideMove = preGen.rowMoveTab[x - ROW_TOP] + bitRow[y];
                            strJudge = (srcSq < dstSq ? 0 : 1);
                            if (dstSq == slideMove->paoCap[strJudge] + (int)x) {
                                strSq = slideMove->juCap[strJudge] + (int)x;
                                if ((this->board[strSq] & oppSide) != 0) {
                                    // Ŀ�����ǳ�����ͬ��˧(��)��Ҫ��Ҳû�б���ʱ����ǣ�Ƽ�ֵ����ͬ
                                    if (stringPiecesValue[this->board[strSq]] > 0 &&
                                        !this->isProtected(sideTag(!side), dstSq) && !this->isProtected(sideTag(!side), strSq, dstSq)) {
                                        stringValue[side] += stringValueTab[dstSq - strSq + 256];
                                    }
                                }
                            }
                        } else if (y == getIdxRow(dstSq)) {
                            slideMove = (preGen.rowMoveTab[x - ROW_TOP] + bitRow[y]);
                            strJudge = (srcSq < dstSq ? 0 : 1);
                            if (dstSq == slideMove->paoCap[strJudge] + (y<<4)) {
                                strSq = slideMove->juCap[strJudge] + (y<<4);
                                if ((this->board[strSq] & oppSide) != 0) {
                                    if (stringPiecesValue[this->board[strSq]] > 0 &&
                                        !this->isProtected(sideTag(!side), dstSq) && !this->isProtected(sideTag(!side), strSq, dstSq)) {
                                        stringValue[side] += stringValueTab[dstSq - strSq + 256];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // ����������ǣ�Ƶ����
        for (i = PAO_FROM; i <= PAO_TO; i++) {
            srcSq = this->pieces[thisSide + i];
            if (srcSq != 0) {
                // ����ǣ��Ŀ����˧(��)�����
                dstSq = this->pieces[oppSide + JIANG_FROM];
                if (dstSq != 0) {
                    x = getIdxCol(srcSq);
                    y = getIdxRow(srcSq);
                    if (x == getIdxCol(dstSq)) {
                        slideMove = preGen.rowMoveTab[x - ROW_TOP] + bitRow[y];
                        strJudge = (srcSq < dstSq ? 0 : 1);
                        if (dstSq == slideMove->cannonCap[strJudge] + (int)x) {
                            strSq = slideMove->paoCap[strJudge] + (int)x;
                            if ((this->board[strSq] & oppSide) != 0) {
                                if (stringPiecesValue[this->board[strSq]] > 1 &&
                                    !this->isProtected(sideTag(!side), strSq, dstSq)) {
                                    stringValue[side] += stringValueTab[dstSq - strSq + 256];
                                }
                            }
                        }
                    } else if (y == getIdxRow(dstSq)) {
                        slideMove = (preGen.rowMoveTab[x - ROW_TOP] + bitRow[y]);
                        strJudge = (srcSq < dstSq ? 0 : 1);
                        if (dstSq == slideMove->cannonCap[strJudge] + (y<<4)) {
                            strSq = slideMove->paoCap[strJudge] + (y<<4);
                            if ((this->board[strSq] & oppSide) != 0) {
                                if (stringPiecesValue[this->board[strSq]] > 1 &&
                                    !this->isProtected(sideTag(!side), strSq, dstSq)) {
                                    stringValue[side] += stringValueTab[dstSq - strSq + 256];
                                }
                            }
                        }
                    }
                }
                // ����ǣ��Ŀ���ǳ������
                for (j = JU_FROM; j <= JU_TO; j++) {
                    dstSq = this->pieces[oppSide + j];
                    if (dstSq != 0) {
                        x = getIdxCol(srcSq);
                        y = getIdxRow(srcSq);
                        if (x == getIdxCol(dstSq)) {
                            slideMove = preGen.rowMoveTab[x - ROW_TOP] + bitRow[y];
                            strJudge = (srcSq < dstSq ? 0 : 1);
                            if (dstSq == slideMove->cannonCap[strJudge] + (int)x) {
                                strSq = slideMove->paoCap[strJudge] + (int)x;
                                if ((this->board[strSq] & oppSide) != 0) {
                                    if (stringPiecesValue[this->board[strSq]] > 1 &&
                                        !this->isProtected(sideTag(!side), strSq, dstSq)) {
                                        stringValue[side] += stringValueTab[dstSq - strSq + 256];
                                    }
                                }
                            }
                        } else if (y == getIdxRow(dstSq)) {
                            slideMove = (preGen.rowMoveTab[x - ROW_TOP] + bitRow[y]);
                            strJudge = (srcSq < dstSq ? 0 : 1);
                            if (dstSq == slideMove->cannonCap[strJudge] + (y<<4)) {
                                strSq = slideMove->paoCap[strJudge] + (y<<4);
                                if ((this->board[strSq] & oppSide) != 0) {
                                    if (stringPiecesValue[this->board[strSq]] > 1 &&
                                        !this->isProtected(sideTag(!side), strSq, dstSq)) {
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
                    // ���µ��ж�������"move_maker.cpp"�е��ŷ����������ų����ߵ����̱�Ե���ߵ��Է����Ƹ���ŷ�
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
                // û�кõ��ŷ��������10�ַ��֣�ֻ��һ���õ��ŷ��������5�ַ���
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

int Game::getValue(int vlAlpha, int vlBeta) {
    int value;
    // ͵���ľ������ۺ��������¼�����Σ�

    // 1. �ļ�͵������(����͵������)��ֻ��������ƽ�⣻
    value = this->materialValue();
    if (value + EVAL_MARGIN1 <= vlAlpha) {
        return value + EVAL_MARGIN1;
    } else if (value - EVAL_MARGIN1 >= vlBeta) {
        return value - EVAL_MARGIN1;
    }

    // 2. ����͵�����ۣ������������ͣ�
    value += this->shiShapeValue();
    if (value + EVAL_MARGIN2 <= vlAlpha) {
        return value + EVAL_MARGIN2;
    } else if (value - EVAL_MARGIN2 >= vlBeta) {
        return value - EVAL_MARGIN2;
    }

    // 3. ����͵�����ۣ�����ǣ�ƣ�
    value += this->stringHoldValue();
    if (value + EVAL_MARGIN3 <= vlAlpha) {
      return value + EVAL_MARGIN3;
    } else if (value - EVAL_MARGIN3 >= vlBeta) {
      return value - EVAL_MARGIN3;
    }

    // 4. һ��͵�����ۣ�������������ԣ�
    value += this->juMobilityValue();
    if (value + EVAL_MARGIN4 <= vlAlpha) {
        return value + EVAL_MARGIN4;
    } else if (value - EVAL_MARGIN4 >= vlBeta) {
        return value - EVAL_MARGIN4;
    }

    // 5. �㼶͵������(��ȫ����)����������谭��
    return value + this->maTrapValue();
    // return value;
}

int Game::materialValue() {
    return cur_player == RED ? red_val - black_val : black_val - red_val;
}
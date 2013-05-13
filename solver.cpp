#include "solver.h"

Long Solver::pos[MAX_DEEP];
Byte Solver::step[MAX_DEEP];

void Solver::dump(int deep) {
    for (int i = 0; i < deep; i++) {
        printf("%d", step[i]);
    }
    printf("\n\nCount: %6I64d\n", posCnt);
    printf("Time: %7.3f\n", perfCnt.elapsed());
}

bool Solver::checkPos(Long p, int deep) {
    int j = MAX_LOOP;
    for (int i = deep - 1; i >=0; i--) {
        if (pos[i] == p) return true;
        if (--j <= 0) break;
    }
    return false;
}

bool Solver::solve() {
    if (stepCnt >= MAX_DEEP) return false;
    pos[0]    = startPos;
    int delta = getDelta(startPos, endPos);
    int X     = getX(startPos);
    int Y     = getY(startPos);
    return solve(0, delta, X, Y);
}

Long Solver::getStep(Long p, int x, int y, int dx, int dy, int& dd) {
    Long digit = getDigit(p, x + dx, y + dy);
    if (digit == 0) return p;
    if (dx != 0) {
        int delta = getDeltaX(startPos, endPos, digit);
        if (delta * dx <= 0) {
            dd++;
        } else {
            dd--;
        }
    }
    if (dy != 0) {
        int delta = getDeltaY(startPos, endPos, digit);
        if (delta * dy <= 0) {
            dd++;
        } else {
            dd--;
        }
    }
    xorDigit(p, x, y, digit);
    xorDigit(p, x + dx, y + dy, digit);
    return p;
}

bool Solver::solve(int deep, int delta, int X, int Y) {
    if (pos[deep] == endPos) {
        dump(deep);
        return true;
    }
    if (delta > stepCnt - deep) {
        return false;
    }
    for (int i = 0; i < 4; i++) {
        int dd = 0;
        int dx = 0;
        int dy = 0;
        switch (i) {
            case 0:
                dy--;
                break;
            case 1:
                dx++;
                break;
            case 2:
                dy++;
                break;
            case 3:
                dx--;
                break;
        }
        if ((X + dx < 1)||(Y + dy < 1)||(X + dx > 4)||(Y + dy > 4)) continue;
        if (deep + 1 >= MAX_DEEP) return false;
        pos[deep + 1] = getStep(pos[deep], X, Y, dx, dy, dd);
        if (checkPos(pos[deep + 1], deep)) continue;
        step[deep] = i;
        posCnt++;
        if (solve(deep + 1, delta + dd, X + dx, Y + dy)) return true;
    }
    return false;
}

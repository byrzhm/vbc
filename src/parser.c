#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

#include "defs.h"
#include "utils.h"

static bool check_paren_valid() {
    int i, cnt;
    cnt = 0;
    for (i = 0; i < nr_token; ++i) {
        if (tokens[i].type == '(') {
            ++cnt;
        } else if (tokens[i].type == ')') {
            --cnt;
        }
        if (cnt < 0) {
            return false;
        }
    }
    return cnt == 0;
}

static bool check_parentheses(int p, int q) {
    if (tokens[p].type != '(' || tokens[q].type != ')') {
        return false;
    }

    int cnt = 1;
    for (int i = p + 1; i <= q; ++i) {
        if (tokens[i].type == '(') {
            ++cnt;
        } else if (tokens[i].type == ')') {
            --cnt;
        }
        if (cnt == 0 && i < q) {
            panic("Invalid parentheses sequence which should not occur!");
            return false;
        }
    }
    Assert(cnt == 0, "Invalid parentheses sequence which should not occur!");
    return cnt == 0;
}

static int dominant_operator(int p, int q) {
    int cnt = 0;
    int op = -1;
    int priority = 0;
    int i;

    for (i = p; i <= q; ++i) {
        if (tokens[i].type == '(') {
            ++cnt;
        } else if (tokens[i].type == ')') {
            --cnt;
        } else if (cnt == 0) {
            if (tokens[i].type == TK_UMINUS) {
                if (priority < 1) { // right associative
                    priority = 1;
                    op = i;
                }
            } else if (tokens[i].type == '*' || tokens[i].type == '/') {
                if (priority <= 2) { // left associative
                    priority = 2;
                    op = i;
                }
            } else if (tokens[i].type == '+' || tokens[i].type == '-') {
                if (priority <= 3) { // left associative
                    priority = 3;
                    op = i;
                }
            } else if (tokens[i].type == TK_EQ || tokens[i].type == TK_NEQ) {
                if (priority <= 4) { // left associative
                    priority = 4;
                    op = i;
                }
            } else if (tokens[i].type == TK_AND) {
                if (priority <= 5) { // left associative
                    priority = 5;
                    op = i;
                }
            }
        }
    }

    return op;
}

static int32_t eval(int p, int q, bool *success) {
    if (p > q) {
        /* Bad expression */
        panic("Bad expression");
        return -1;
    } else if (p == q) {
        /**
         * Single token.
         */
        if (tokens[p].type == TK_DEC) return strtol(tokens[p].str, NULL, 10);
        else if (tokens[p].type == TK_HEX) return strtol(tokens[p].str, NULL, 16);

        panic("Not a terminal: type=%d", tokens[p].type);
        return -1;
    } else {
        int op = dominant_operator(p, q);
        if (tokens[op].type == TK_UMINUS) {
            Assert(op == p, "Dominant unary minus occurs in the middle");
            return -eval(p + 1, q, success);
        }

        int32_t val1 = eval(p, op - 1, success);
        int32_t val2 = eval(op + 1, q, success);

        switch (tokens[op].type) {
            case '+':    return val1 + val2;
            case '-':    return val1 - val2;
            case '*':    return val1 * val2;
            case '/':    if (val2 == 0) { *success = false; return -1; }
                         return val1 / val2;
            case TK_EQ:  return val1 == val2;
            case TK_NEQ: return val1 != val2;
            case TK_AND: return val1 && val2;
            default: Assert(0, "Invalid operator");
        }
    }
}

uint32_t parse(char *e, bool *success) {
    if (!make_token(e)) {
        *success = false;
        return 0;
    }

    if (!check_paren_valid()) {
        *success = false;
        return 0;
    }

    *success = true;
    return eval(0, nr_token - 1, success);
}

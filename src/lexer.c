#include <string.h>
#include <regex.h>
#include "utils.h"
#include "defs.h"

static struct Rule {
    const char *regex;
    int token_type;
} rules[] = {
    {"0x[0-9]+", TK_HEX},    // hexadecimal
    {"[0-9]+", TK_DEC},      // decimal
    {" +", TK_NOTYPE},       // spaces
    {"&&", TK_AND},          // and
    {"==", TK_EQ},           // equal
    {"!=", TK_NEQ},          // not equal
    {"\\+", '+'},            // plus
    {"-", '-'},              // minus
    {"\\*", '*'},            // multiply
    {"/", '/'},              // divide
    {"\\(", '('},            // left paren
    {")", ')'},              // right paren
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/**
 * Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
    int i;
    char error_msg[128];
    int ret;

    for (i = 0; i < NR_REGEX; ++i) {
        ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
        if (ret != 0) {
            regerror(ret, &re[i], error_msg, 128);
            panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
        }
    }
}

Token tokens[TK_NR_MAX] = {};
int nr_token = 0;

static inline bool is_op(int tok_type) {
    return tok_type == TK_AND
        || tok_type == TK_EQ
        || tok_type == TK_NEQ
        || tok_type == TK_UMINUS
        || tok_type == '+'
        || tok_type == '-'
        || tok_type == '*'
        || tok_type == '/';
}

bool make_token(char *e) {
    int position = 0;
    int i;
    int tok_type, prev_type;
    regmatch_t pmatch;

    nr_token = 0;

    while (e[position] != '\0') {
        for (i = 0; i < NR_REGEX; ++i) {
            if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
                char *substr_start = e + position;
                int substr_len = pmatch.rm_eo;
                
                Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
                        i, rules[i].regex, position, substr_len, substr_len, substr_start);

                position += substr_len;

                tok_type = rules[i].token_type;

                if (tok_type == TK_NOTYPE) {
                    break;
                }

                Assert(nr_token < TK_NR_MAX,
                        "Currently, number of tokens should be less than %d", TK_NR_MAX);

                if (tok_type == '-' && ( nr_token == 0
                        || (prev_type = tokens[nr_token - 1].type) == '('
                        || is_op(prev_type))) {
                        tok_type = TK_UMINUS;
                }


                tokens[nr_token].type = tok_type;
                switch (tok_type) {
                    case TK_DEC: case TK_HEX: {
                                     Assert(substr_len < TK_LEN_MAX,
                                             "Length of decimal should be less than %d", TK_LEN_MAX);
                                     strncpy(tokens[nr_token].str, substr_start, substr_len);
                                     tokens[nr_token].str[substr_len] = '\0';
                    } break;
                    default: break;
                }
                ++nr_token;
                break;
            }
        }

        if (i == NR_REGEX) {
            printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
            return false;
        }
    }

    return true;
}

                

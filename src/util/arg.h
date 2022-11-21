#ifndef SRC_UTIL_ARG_H_
#define SRC_UTIL_ARG_H_

/* simple argument parsing with documentation

 options can be typed as int, char, char*, bool, float, or handled by a
 special function positional parameters are always string and must be listed
 in order a special type Rest means all rest are returned as a pointer.  */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "util/attrs.h"

/* don't change constants unless you fix lots of other stuff.  */
typedef enum {
    Integer   = 1,
    Character = 2,
    String    = 3,
    Boolean   = 4,
    Float     = 5,
    Rest      = 6,
    Help,
    Increment,
    Set,
    Toggle,
    EndOptions /* use this to allow user to indicate end of positional args,
                  typically it will be for -- */
} ArgType;

/* don't renumber these.  We depend on value of KindPositional >
   (KindOption & KindHelp), KindEnd > (all), KindRest > KindPositional.  */
typedef enum {
    KindEnd        = 4,
    KindHelp       = 1,
    KindPositional = 2,
    KindOption     = 0,
    KindRest       = 3
} ArgKind;

#define ArgGetDefault -1
#define ArgGetDesc    -2

typedef struct arg_rest {
    char * const * ptrs;
    size_t         n;
} __attribute__((may_alias)) arg_rest_t;

#define INIT_ARG_REST_T                                                        \
    { NULL, 0 }

typedef struct arg_option arg_option_t;
struct arg_option {
    ArgKind            kind;          /* option, positionl, rest, end, help */
    ArgType            type;          /* type of the argument/option */
    char const * const args_begin[6]; /* also name of positional arg */
    int32_t            required;
    void *             dest;
    uint64_t           dest_sz;     /* hidden to user. */
    int32_t            is_unsigned; /* hidden to user. */
    char const *       desc;
};


#define ADD_ARG(kind, type, names, reqd, var, help)                            \
    {                                                                          \
        kind, type, { DEPAREN(names) }, reqd, var, sizeof_deref(var),          \
            IS_UNSIGNED_INT(var), help                                         \
    }


typedef struct arg_def arg_defs_t;

struct arg_def {
    /* public members.  */
    arg_option_t * args;
    char const *   progdoc;
    char const *   version;
    void (*doneParsing)(void);
};

int32_t parseArgs(int32_t        argc,
                  char * const * argv,
                  arg_defs_t const * restrict def);

/* for more complicated arg parsing, i.e., including parsers of submodules, make
 * an arg_parser_t, then call parse on it.  */

typedef struct arg_parser_node arg_parser_node_t;
struct arg_parser_node {
    int32_t             main;
    arg_defs_t const *  parser;
    arg_parser_node_t * next;
};

typedef struct arg_parser arg_parser_t;
struct arg_parser {
    arg_parser_node_t * parsers;
    arg_defs_t const *  mainProg;
};

arg_parser_t * createArgumentParser(arg_defs_t const * def);
void           freeArgumentParser(arg_parser_t const * ap);
void           addArgumentParser(arg_parser_t * restrict ap,
                                 arg_defs_t const * restrict def,
                                 int32_t order);
int32_t        parseArguments(arg_parser_t * restrict ap,
                              int32_t        argc,
                              char * const * argv);
void           freeCommandLine(void);
int32_t        doParse(arg_defs_t const * restrict argp,
                       int            argc,
                       char * const * argv);

#endif

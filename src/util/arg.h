#ifndef _ARG_H_
#define _ARG_H_

/* simple argument parsing with documentation

 options can be typed as int, char, char*, bool, double, or handled by a
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
    Double    = 5,
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

typedef struct _argoption ArgOption;
struct _argoption {
    ArgKind      kind;    /* option, positionl, rest, end, help */
    ArgType      type;    /* type of the argument/option */
    const char * longarg; /* also name of positional arg for doc purposes */
    int32_t      required;
    void *       dest;
    const char * desc;
};

typedef struct _argdef ArgDefs;

struct _argdef {
    /* public members.  */
    ArgOption *  args;
    const char * progdoc;
    const char * version;
    void (*doneParsing)(void);
};

int32_t parseArgs(int32_t        argc,
                  char * const * argv,
                  ArgDefs const * restrict def);

/* for more complicated arg parsing, i.e., including parsers of submodules, make
 * an ArgParser, then call parse on it.  */

typedef struct _argparserlist ArgParserNode;
struct _argparserlist {
    int32_t         main;
    ArgDefs const * parser;
    ArgParserNode * next;
};

typedef struct _argParser ArgParser;
struct _argParser {
    ArgParserNode * parsers;
    ArgDefs const * mainProg;
};

ArgParser * createArgumentParser(ArgDefs const * def);
void        freeArgumentParser(ArgParser * ap);
void        addArgumentParser(ArgParser * restrict ap,
                              ArgDefs const * restrict def,
                              int32_t order);
int32_t     parseArguments(ArgParser * restrict ap,
                           int32_t        argc,
                           char * const * argv);
void        freeCommandLine(void);
int32_t doParse(ArgDefs const * restrict argp, int argc, char * const * argv);

#endif

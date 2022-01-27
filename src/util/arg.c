////////////////////////////////////////////////////////////////
// simple argument parsing with documentation

// options can be typed as int, char, char*, bool, double, or handled by a
// special function positional parameters are always string and must be listed
// in order a special type Rest means all rest are returned as a pointer

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "util/arg.h"
#include "util/error-util.h"
#include "util/memory-util.h"

#include "lib/commonlib.h"

typedef uint32_t bool;
static const bool false = 0;
static const bool true  = !false;

static int32_t      argp_verbose = 0;
static char *       commandLine;
static const char * pname;

// order is based on how enums are defined in ArgType
static const char * type2fmt[] = { "",       "<int>",    "<char>", "<string>",
                                   "<bool>", "<double>", "<rest>", "",
                                   "",       "" };

static void usage(const char * pname, ArgParser * def);

static void
argdie(ArgParser * def, const char * fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "%s: Usage Error: ", pname);
    vfprintf(stderr, fmt, ap);  // NOLINT
    va_end(ap);
    fprintf(stderr, "\n");
    usage(pname, def);
    exit(-1);
}

static const char *
arg2str(ArgOption * desc) {
    static char buffer[128];
    size_t      n, avail_length = 128;
    char *      p   = buffer;
    char        sep = (desc->kind == KindPositional) ? ':' : ' ';

    if (desc->kind == (ArgKind)Help) {
        return "[-h]";
    }

    if ((desc->kind == KindPositional) && (desc->required)) {
        *p++ = '<';
        --avail_length;
    }
    else if (!desc->required) {
        *p++ = '[';
        --avail_length;
    }

    switch (desc->type) {
        case Integer:
        case Character:
        case String:
        case Boolean:
        case Double:
            n = snprintf(p, avail_length, "%s%c%s", desc->longarg, sep,
                         type2fmt[desc->type]);
            die_assert(n < avail_length,
                       "Buffer overflow creating help message\n");
            avail_length -= n;
            p += strlen_c(p);
            break;
        case Rest:
            die_assert(desc->kind == KindRest);
            n = snprintf(p, avail_length, "...%c%s ", sep,
                         type2fmt[desc->type]);
            die_assert(n < avail_length,
                       "Buffer overflow creating help message\n");
            avail_length -= n;
            p += strlen_c(p);
            break;
        case Help:
        case Toggle:
        case Set:
        case Increment:
            n = snprintf(p, avail_length, "%s", desc->longarg);
            die_assert(n < avail_length,
                       "Buffer overflow creating help message\n");
            avail_length -= n;
            p += strlen_c(p);
            break;

        default:
            // new type that we didn't implement yet?
            die("Unknown type: %d\n", desc->type);
    }
    if (desc->kind == KindRest) {
        n = strlen_c("...") + 1;
        die_assert(avail_length > n, "Buffer overflow creating help message\n");
        memcpy_c(p, "...", n);
        p += n;
        avail_length -= n;
    }

    if ((desc->kind == KindPositional) && (desc->required)) {
        die_assert(avail_length > 1, "Buffer overflow creating help message\n");
        *p++ = '>';
        --avail_length;
    }
    else if (!desc->required) {
        die_assert(avail_length > 1, "Buffer overflow creating help message\n");
        *p++ = ']';
        --avail_length;
    }
    die_assert(avail_length, "Buffer overflow creating help message\n");
    *p = 0;
    return buffer;
}

static void
usage(const char * pname, ArgParser * ap) {
    fprintf(stderr, "%s: ", pname);
    // print out shorthand for arguments
    for (ArgParserNode * apn = ap->parsers; apn; apn = apn->next) {
        ArgOption * args = apn->parser->args;
        int32_t     i    = 0;
        while (args[i].kind != KindEnd) {
            fprintf(stderr, " %s", arg2str(args + i));
            i++;
        }
        fprintf(stderr, "\n%s\n", apn->parser->progdoc);
    }

    // Now print individual descriptions
    for (ArgParserNode * apn = ap->parsers; apn; apn = apn->next) {
        ArgOption * args = apn->parser->args;
        int32_t     i    = 0;
        while (args[i].kind != KindEnd) {
            fprintf(
                stderr, "   %20s\t%s\t", arg2str(args + i),
                args[i].kind == KindHelp ? "Print this message" : args[i].desc);
            switch (args[i].type) {
                case Increment:
                case Integer:
                    fprintf(stderr, "(default: %d)",
                            *(safe_int32_t *)(args[i].dest));
                    break;

                case Character:
                    fprintf(stderr, "(default: %c)", *(char *)(args[i].dest));
                    break;

                case String:
                    fprintf(stderr, "(default: %s)", *(char **)(args[i].dest));
                    break;

                case Toggle:
                case Set:
                case Boolean:
                    fprintf(stderr, "(default: %s)",
                            *(safe_int32_t *)(args[i].dest) ? "true" : "false");
                    break;

                case Double:
                    fprintf(stderr, "(default: %lf)",
                            *(safe_double *)(args[i].dest));
                    break;
                case Rest:
                    fprintf(stderr, "(default: N/A)");
                case Help:
                    break;

                default:
                    die("Unkown Type: %d\n", args[i].type);
            }
            fprintf(stderr, "\n");
            i++;
        }
    }
}

void
makeCommandline(int32_t argc, char ** argv) {
    int32_t len = 2;
    for (int32_t i = 0; i < argc; i++) {
        len += (1 + strlen_c(argv[i]));
    }
    char * p = (char *)safe_calloc(len, 1);
    len      = 0;
    for (int32_t i = 0; i < argc; i++) {
        strcpy_c(p + len, argv[i]);
        len += strlen_c(argv[i]);
        p[len++] = ' ';
    }
    p[len - 1] = 0;
    if (argp_verbose) fprintf(stderr, "[%s]\n", p);
    commandLine = p;
}

// offset is 1 for option args (argv[0] points at option, argv[1] at start of
// data) offset is 0 for positional args (argv[0] points at actual argument)
static int32_t
assignArg(ArgOption * desc,
          char **     argv,
          ArgParser * ap,
          int32_t     offset,
          char **     argv_end) {
    switch (desc->type) {
        case Increment:
            if (argp_verbose) {
                printf("incremement %s\n", desc->longarg);
            }
            {
                safe_int32_t * p = (safe_int32_t *)desc->dest;
                (*p)++;
            }
            break;

        case String:
            if (argp_verbose) {
                printf("Saving %s to %s\n", argv[offset], desc->longarg);
            }
            {
                char ** p = (char **)desc->dest;
                *p        = argv[offset];
                return 1;
            }
            break;

        case Rest:
            die_assert(desc->kind == KindRest);
            if (argp_verbose) {
                printf("Saving %s to Rest\n", argv[offset]);
            }
            {
                arg_rest_t * p = (arg_rest_t *)desc->dest;
                size_t       n = ((argv_end) - (argv + offset));
                p->ptrs        = argv + offset;
                p->n           = n;
                return 1;
            }
            break;

        case Set:
            if (argp_verbose) {
                printf("Setting %s to 1\n", desc->longarg);
            }
            {
                safe_int32_t * p = (safe_int32_t *)desc->dest;
                *p               = 1;
            }
            break;

        case Toggle:
            if (argp_verbose) {
                printf("toggeling %s\n", desc->longarg);
            }
            {
                safe_int32_t * p = (safe_int32_t *)desc->dest;
                *p               = !(*p);
            }
            break;

        case Double:
            if (argp_verbose) {
                printf("double -> [%s] = %lf\n", desc->longarg,
                       atof(argv[offset]));
            }
            {
                safe_double * p = (safe_double *)desc->dest;
                *p              = atof(argv[offset]);
                return 1;
            }
            break;

        case Integer:
            if (argp_verbose) {
                printf("int -> [%s] = %d\n", desc->longarg, atoi(argv[offset]));
            }
            {
                safe_int32_t * p = (safe_int32_t *)desc->dest;
                *p               = atoi(argv[offset]);
                return 1;
            }
            break;

        case Help:
            usage(pname, ap);
            exit(-1);

        default:
            fprintf(stderr, "NIY: type\n");
            exit(-1);
    }
    return 0;
}

static const char *
kind2str(ArgKind k) {
    switch (k) {
        case KindEnd:
            return "End";
        case KindHelp:
            return "Help";
        case KindPositional:
            return "Positional";
        case KindOption:
            return "Option";
        case KindRest:
            return "Rest";
        default:
            assert(0);
    }
    return "Unknown";
}

// return true if has help
bool
checkArgDef(ArgParser * ap, ArgDefs * def, bool main) {
    // optional/help come before postional before rest
    uint32_t    state   = KindOption;
    ArgOption * desc    = def->args;
    int32_t     hashelp = false;

    for (int32_t i = 0; desc[i].kind != KindEnd; i++) {
        if (desc[i].kind > KindEnd) {
            argdie(ap, "Bad kind - no KindEnd?");
        }
        if (desc[i].kind != state) {
            if ((state == KindOption) && (desc[i].kind == KindHelp) &&
                (desc[i].longarg != NULL)) {
                hashelp = true;
                continue;
            }
            if (desc[i].kind < state) {
                argdie(ap,
                       "Bad order of arg defs: %s comes before "
                       "last of %s",
                       kind2str((ArgKind)state), kind2str(desc[i].kind));
            }
            state = desc[i].kind;
        }
        if (((state == KindPositional) || (state == KindRest)) && !main) {
            argdie(ap, "positional args but not main");
        }
    }
    return hashelp;
}

static void
checkArgParser(ArgParser * ap) {
    bool hashelp = false;
    for (ArgParserNode * apn = ap->parsers; apn; apn = apn->next) {
        hashelp |= checkArgDef(ap, apn->parser, apn->main);
    }
    if (!hashelp) argdie(ap, "No help string");
}

int32_t
parseArgs(int32_t argc, char ** argv, ArgDefs * def) {
    ArgParserNode n  = { 1, def, NULL };
    ArgParser     ap = { &n, def };
    return parseArguments(&ap, argc, argv);
}

////////////////////////////////////////////////////////////////
// multiple argument parsers

ArgParser *
createArgumentParser(ArgDefs * def) {
    ArgParser * ap = (ArgParser *)safe_calloc(1, sizeof(ArgParser));
    ap->parsers    = (ArgParserNode *)safe_calloc(1, sizeof(ArgParserNode));
    ap->parsers->parser = def;
    ap->parsers->main   = 1;
    ap->mainProg        = def;
    return ap;
}

void
freeCommandLine(void) {
    safe_free(commandLine);
}

void
freeArgumentParser(ArgParser * ap) {
    ArgParserNode * next;
    ArgParserNode * p;
    for (p = ap->parsers; p; p = next) {
        next = p->next;
        safe_free(p);
    }
    safe_free(ap);
    freeCommandLine();
}

void
addArgumentParser(ArgParser * ap, ArgDefs * def, int32_t order) {
    ArgParserNode * p = (ArgParserNode *)safe_calloc(1, sizeof(ArgParserNode));
    p->parser         = def;
    p->next           = NULL;
    p->main           = 0;

    if (order > 0) {
        ArgParserNode * nextp;
        for (nextp = ap->parsers; nextp->next; nextp = nextp->next)
            ;
        nextp->next = p;
    }
    else {
        p->next     = ap->parsers;
        ap->parsers = p;
    }
}

int32_t
parseArguments(ArgParser * ap, int32_t argc, char ** argv) {
    char ** argv_end = argv + argc;
    // get program name and commandline as a string
    pname = argv[0];
    makeCommandline(argc, argv);
    argv++;
    argc--;

    checkArgParser(ap);

    // process args
    if (argp_verbose) {
        fprintf(stderr, "Processing args for %s: %d\n", pname, argc);
    }

    for (ArgParserNode * apn = ap->parsers; apn; apn = apn->next) {
        ArgOption * desc = apn->parser->args;
        for (int32_t j = 0; (desc[j].kind != KindEnd); ++j) {
            if (desc[j].required && desc[j].required != 1) {
                desc[j].required = 1;
            }
        }
    }

    int32_t i;
    bool    optionsPossible = true;
    for (i = 0; (i < argc) && optionsPossible; i++) {
        char * arg = argv[i];
        if (argp_verbose) {
            fprintf(stderr, "%d -> [%s]\n", i, arg);
        }
        if (arg[0] == '-') {
            // Handle options
            bool ok       = false;
            bool notfound = true;
            for (ArgParserNode * apn = ap->parsers; notfound && apn;
                 apn                 = apn->next) {
                ArgOption * desc = apn->parser->args;
                for (int32_t j = 0; notfound && (desc[j].kind != KindEnd);
                     j++) {
                    if (strcmp_c(desc[j].longarg, arg) == 0) {
                        ok       = true;
                        notfound = false;
                        // see if it is special
                        if (desc[j].type == EndOptions) {
                            optionsPossible = false;
                            break;
                        }
                        // process it
                        int32_t consumed =
                            assignArg(desc + j, argv + i, ap, 1, argv_end);
                        i += consumed;
                        desc[j].required |= (desc[j].required << 1);
                    }
                }
            }
            if (!ok) {
                argdie(ap, "Do not understand the flag [%s]\n", arg);
            }
        }
        else {
            // No more options
            break;
        }
    }
    // ok, now we handle positional args, we handle them in the order they are
    // declared only the main parser can define positional args
    ArgOption * desc = NULL;
    for (ArgParserNode * apn = ap->parsers; apn; apn = apn->next) {
        if (apn->main) {
            desc = apn->parser->args;
            break;
        }
    }
    die_assert(desc != NULL);
    int32_t baseArg = i;
    int32_t baseDestOffset;
    for (baseDestOffset = 0; desc[baseDestOffset].kind != KindEnd;
         baseDestOffset++) {
        if ((desc[baseDestOffset].kind == KindPositional) ||
            (desc[baseDestOffset].kind == KindRest)) {
            break;
        }
    }
    // base is first positional arg we are passed, j is first descriptor for
    // positional arg
    if (argp_verbose) {
        fprintf(stderr, "start pos: j=%d %s kind=%d basearg=%d\n",
                baseDestOffset, desc[baseDestOffset].longarg,
                desc[baseDestOffset].type, baseArg);
    }
    int32_t j = 0; /* positional offset */
    while ((desc[baseDestOffset + j].kind == KindPositional) &&
           ((baseArg + j) < argc)) {
        if (argp_verbose) {
            printf("%d: %s\n", j, baseArg + desc[baseDestOffset + j].longarg);
        }
        int32_t consumed = assignArg(desc + baseDestOffset + j,
                                     argv + baseArg + j, ap, 0, argv_end);
        j += consumed;
        desc[baseDestOffset + j].required |=
            (desc[baseDestOffset + j].required << 1);
    }
    // check that we used all the arguments and don't have any extra
    if (desc[baseDestOffset + j].type == (ArgType)KindPositional) {
        argdie(ap, "Expected more arguments, only given %d", j);
    }
    else if ((desc[baseDestOffset + j].type == (ArgType)KindEnd) &&
             ((baseArg + j) < argc)) {
        argdie(ap, "Too many arguments, given %d", j);
    }
    // see if we have a variable number of args at end
    if (desc[baseDestOffset + j].type == Rest && ((baseArg + j) < argc)) {
        int32_t consumed = assignArg(desc + baseDestOffset + j,
                                     argv + baseArg + j, ap, 0, argv_end);
        j += consumed;
        desc[baseDestOffset + j].required |=
            (desc[baseDestOffset + j].required << 1);
    }

    // if user defined a post parsing function, call it - main prog called last
    for (ArgParserNode * apn = ap->parsers; apn; apn = apn->next) {
        if ((apn->main != 1) && (apn->parser->doneParsing != NULL)) {
            (*(apn->parser->doneParsing))();
        }
    }
    if (ap->mainProg->doneParsing) {
        (*(ap->mainProg->doneParsing))();
    }

    for (ArgParserNode * apn = ap->parsers; apn; apn = apn->next) {
        ArgOption * desc = apn->parser->args;
        for (int32_t j = 0; (desc[j].kind != KindEnd); ++j) {
            if (desc[j].required == 1) {
                argdie(ap, "Missing required argument\n\t%s\n",
                       arg2str(desc + j));
            }
        }
    }
    return 0;
}

int32_t
doParse(ArgDefs * argp, int argc, char ** argv) {
    ArgParser * ap  = createArgumentParser(argp);
    int32_t     ret = parseArguments(ap, argc, argv);
    if (ret) {
        argdie(ap, "Missing required argument\n");
    }
    freeArgumentParser(ap);
    return ret;
}

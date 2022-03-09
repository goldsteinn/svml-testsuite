/* simple argument parsing with documentation

 options can be typed as int, char, char*, bool, double, or handled by a
 special function positional parameters are always string and must be listed
 in order a special type Rest means all rest are returned as a pointer. */

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "util/arg.h"
#include "util/error-util.h"
#include "util/memory-util.h"
#include "util/verbosity.h"

#include "lib/commonlib.h"

static int32_t      argp_verbosity = 0;
static char *       commandLine;
static const char * pname;

/* order is based on how enums are defined in ArgType.  */
static const char * type2fmt[] = { "",       "<int>",    "<char>", "<string>",
                                   "<bool>", "<double>", "<rest>", "",
                                   "",       "" };

enum { BAD_STRING = -1, BAD_REST = -2, BAD_DOUBLE = -3, BAD_INT = -4 };


static void usage(char const * restrict pname, ArgParser const * restrict def);

static void
FORMATF(2, 3)
    argdie(ArgParser const * restrict def, char const * restrict fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "%s: Usage Error: ", pname);
    vfprintf(stderr, fmt, ap); /* NOLINT */
    va_end(ap);
    fprintf(stderr, "\n");
    usage(pname, def);
    exit(-1);
}


static const char *
arg2str(ArgOption const * restrict desc) {
    static char buffer[128];
    ssize_t     n, avail_length = 128;
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
            die_assert(n >= 0 && n < avail_length,
                       "Buffer overflow creating help message\n");
            avail_length -= n;
            p += strlen_c(p);
            break;
        case Rest:
            die_assert(desc->kind == KindRest);
            n = snprintf(p, avail_length, "...%c%s ", sep,
                         type2fmt[desc->type]);
            die_assert(n >= 0 && n < avail_length,
                       "Buffer overflow creating help message\n");
            avail_length -= n;
            p += strlen_c(p);
            break;
        case Help:
        case Toggle:
        case Set:
        case Increment:
            n = snprintf(p, avail_length, "%s", desc->longarg);
            die_assert(n >= 0 && n < avail_length,
                       "Buffer overflow creating help message\n");
            avail_length -= n;
            p += strlen_c(p);
            break;

        default:
            /* new type that we didn't implement yet?  */
            die("Unknown type: %d\n", desc->type);
    }
    if (desc->kind == KindRest) {
        n = strlen_c("...") + 1;
        die_assert(n > avail_length, "Buffer overflow creating help message\n");
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
    die_assert(avail_length > 0, "Buffer overflow creating help message\n");
    *p = 0;
    return buffer;
}

static void
usage(char const * restrict pname, ArgParser const * restrict ap) {
    ArgParserNode const * apn;
    fprintf(stderr, "%s: ", pname);
    /* print out shorthand for arguments.  */
    for (apn = ap->parsers; apn; apn = apn->next) {
        ArgOption const * args = apn->parser->args;
        int32_t           i    = 0;
        while (args[i].kind != KindEnd) {
            fprintf(stderr, " %s", arg2str(args + i));
            i++;
        }
        fprintf(stderr, "\n%s\n", apn->parser->progdoc);
    }

    /* Now print individual descriptions.  */
    for (apn = ap->parsers; apn; apn = apn->next) {
        ArgOption const * args = apn->parser->args;
        int32_t           i    = 0;
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

static void
makeCommandline(int32_t argc, char * const * argv) {
    int32_t i;
    size_t  len = 2;
    char *  p;
    for (i = 0; i < argc; i++) {
        len += (1 + strlen_c(argv[i]));
    }
    p   = (char *)safe_calloc(len, 1);
    len = 0;
    for (i = 0; i < argc; i++) {
        strcpy_c(p + len, argv[i]);
        len += strlen_c(argv[i]);
        p[len++] = ' ';
    }
    p[len - 1] = 0;
    vfprint(stderr, "[%s]\n", p);
    commandLine = p;
}

/* offset is 1 for option args (argv[0] points at option, argv[1] at start of
 * data) offset is 0 for positional args (argv[0] points at actual argument). */
static int32_t
assignArg(ArgOption * restrict desc,
          char * const * argv,
          ArgParser * restrict ap,
          int32_t        offset,
          char * const * argv_end) {
    switch (desc->type) {
        case Increment:
            vprint("incremement %s\n", desc->longarg);
            {
                safe_int32_t * p = (safe_int32_t *)desc->dest;
                (*p)++;
            }
            break;

        case String:
            if (!argv[offset]) {
                return BAD_STRING;
            }
            vprint("Saving %s to %s\n", argv[offset], desc->longarg);
            {
                char const ** p = (char const **)desc->dest;
                *p              = argv[offset];
                return 1;
            }
            break;

        case Rest:
            if (!argv[offset]) {
                return BAD_REST;
            }
            die_assert(desc->kind == KindRest);
            vprint("Saving %s to Rest\n", argv[offset]);
            {
                arg_rest_t * p = (arg_rest_t *)desc->dest;
                size_t       n = ((argv_end) - (argv + offset));
                p->ptrs        = argv + offset;
                p->n           = n;
                return 1;
            }
            break;

        case Set:
            vprint("Setting %s to 1\n", desc->longarg);
            {
                safe_int32_t * p = (safe_int32_t *)desc->dest;
                *p               = 1;
            }
            break;

        case Toggle:
            vprint("toggeling %s\n", desc->longarg);
            {
                safe_int32_t * p = (safe_int32_t *)desc->dest;
                *p               = !(*p);
            }
            break;

        case Double:
            if (!argv[offset]) {
                return BAD_DOUBLE;
            }
            vprint("double -> [%s] = %lf\n", desc->longarg, atof(argv[offset]));
            {
                safe_double * p = (safe_double *)desc->dest;
                *p              = atof(argv[offset]);
                return 1;
            }
            break;

        case Integer:
            if (!argv[offset]) {
                return BAD_INT;
            }
            vprint("int -> [%s] = %d\n", desc->longarg, atoi(argv[offset]));
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
            die("NIY: type\n");
    }
    return 0;
}

static PURE_FUNC const char *
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

/* return true if has help.  */
static PURE_FUNC bool
checkArgDef(ArgParser const * restrict ap,
            ArgDefs const * restrict def,
            bool main) {
    /* optional/help come before postional before rest.  */
    uint32_t          state   = KindOption;
    ArgOption const * desc    = def->args;
    int32_t           hashelp = false;
    int32_t           i;
    for (i = 0; desc[i].kind != KindEnd; i++) {
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
checkArgParser(ArgParser const * ap) {
    bool                  hashelp = false;
    ArgParserNode const * apn;
    for (apn = ap->parsers; apn; apn = apn->next) {
        hashelp |= checkArgDef(ap, apn->parser, apn->main);
    }
    if (!hashelp) argdie(ap, "No help string");
}

int32_t
parseArgs(int32_t argc, char * const * argv, ArgDefs const * restrict def) {
    ArgParserNode n  = { 1, def, NULL };
    ArgParser     ap = { &n, def };
    return parseArguments(&ap, argc, argv);
}

/**************************************************************************/
/* multiple argument parsers. */

ArgParser *
createArgumentParser(ArgDefs const * restrict def) {
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
addArgumentParser(ArgParser * restrict ap,
                  ArgDefs const * restrict def,
                  int32_t order) {
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
parseArguments(ArgParser * restrict ap, int32_t argc, char * const * argv) {
    char * const *        argv_end = argv + argc;
    ArgParserNode const * apn;
    int32_t               i, j;
    /* get program name and commandline as a string.  */
    pname = argv[0];
    makeCommandline(argc, argv);
    argv++;
    argc--;

    checkArgParser(ap);

    /* process args.  */
    vprint("Processing args for %s: %d\n", pname, argc);

    for (apn = ap->parsers; apn; apn = apn->next) {
        ArgOption * desc = apn->parser->args;
        for (i = 0; (desc[i].kind != KindEnd); ++i) {
            if (desc[i].required && desc[i].required != 1) {
                desc[i].required = 1;
            }
        }
    }

    bool optionsPossible = true;
    for (i = 0; (i < argc) && optionsPossible; i++) {
        char const * arg = argv[i];
        vprint("%d -> [%s]\n", i, arg);
        if (arg[0] == '-') {
            /* Handle options.  */
            bool ok       = false;
            bool notfound = true;
            for (apn = ap->parsers; notfound && apn; apn = apn->next) {
                ArgOption * desc = apn->parser->args;
                for (j = 0; notfound && (desc[j].kind != KindEnd); j++) {
                    if (strcmp_c(desc[j].longarg, arg) == 0) {
                        ok       = true;
                        notfound = false;
                        /* see if it is special.  */
                        if (desc[j].type == EndOptions) {
                            optionsPossible = false;
                            break;
                        }
                        /* process it.  */
                        int32_t consumed =
                            assignArg(desc + j, argv + i, ap, 1, argv_end);
                        if (consumed < 0) {
                            argdie(ap,
                                   "Missing expected argument value for [%s]\n",
                                   argv[i]);
                        }
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
            /* No more options.  */
            break;
        }
    }
    /* ok, now we handle positional args, we handle them in the order they are
     * declared only the main parser can define positional args.  */
    ArgOption * desc = NULL;
    for (apn = ap->parsers; apn; apn = apn->next) {
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
    /* base is first positional arg we are passed, j is first descriptor for
     * positional arg.  */
    vprint("start pos: j=%d %s kind=%d basearg=%d\n", baseDestOffset,
           desc[baseDestOffset].longarg, desc[baseDestOffset].type, baseArg);

    j = 0;
    while ((desc[baseDestOffset + j].kind == KindPositional) &&
           ((baseArg + j) < argc)) {
        vprint("%d: %s\n", j, baseArg + desc[baseDestOffset + j].longarg);
        int32_t consumed = assignArg(desc + baseDestOffset + j,
                                     argv + baseArg + j, ap, 0, argv_end);
        j += consumed;
        desc[baseDestOffset + j].required |=
            (desc[baseDestOffset + j].required << 1);
    }
    /* check that we used all the arguments and don't have any extra.  */
    if (desc[baseDestOffset + j].type == (ArgType)KindPositional) {
        argdie(ap, "Expected more arguments, only given %d", j);
    }
    else if ((desc[baseDestOffset + j].type == (ArgType)KindEnd) &&
             ((baseArg + j) < argc)) {
        argdie(ap, "Too many arguments, given %d", j);
    }
    /* see if we have a variable number of args at end.  */
    if (desc[baseDestOffset + j].type == Rest && ((baseArg + j) < argc)) {
        int32_t consumed = assignArg(desc + baseDestOffset + j,
                                     argv + baseArg + j, ap, 0, argv_end);
        j += consumed;
        desc[baseDestOffset + j].required |=
            (desc[baseDestOffset + j].required << 1);
    }

    /* if user defined a post parsing function, call it - main prog called last.
     */
    for (apn = ap->parsers; apn; apn = apn->next) {
        if ((apn->main != 1) && (apn->parser->doneParsing != NULL)) {
            (*(apn->parser->doneParsing))();
        }
    }
    if (ap->mainProg->doneParsing) {
        (*(ap->mainProg->doneParsing))();
    }

    for (apn = ap->parsers; apn; apn = apn->next) {
        ArgOption * desc = apn->parser->args;
        for (j = 0; (desc[j].kind != KindEnd); ++j) {
            if (desc[j].required == 1) {
                argdie(ap, "Missing required argument\n\t%s\n",
                       arg2str(desc + j));
            }
        }
    }
    return 0;
}

int32_t
doParse(ArgDefs const * restrict argp, int argc, char * const * argv) {
    ArgParser * ap;
    int32_t     ret, orig_verbosity;

    orig_verbosity = get_verbosity();
    set_verbosity(argp_verbosity);

    ap  = createArgumentParser(argp);
    ret = parseArguments(ap, argc, argv);
    if (ret) {
        argdie(ap, "Missing required argument\n");
    }
    freeArgumentParser(ap);

    set_verbosity(orig_verbosity);
    return ret;
}

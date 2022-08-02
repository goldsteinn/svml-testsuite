#include "test/test-common.h"
#include "util/macro.h"

#include "lib/commonlib.h"
#define I_ONE  1
#define I_MANY 2
static int64_t count;
void
foo(int64_t i) {
    count += i;
}
void
foo2(int64_t i0, int64_t i1) {
    count += (i0 - i1);
}
#define dfoo2(...) foo2(DEPAREN(__VA_ARGS__))
#define I_TEST_MIN(x, y)                                                       \
    ({                                                                         \
        long _x = (x);                                                         \
        long _y = (y);                                                         \
        ((_x) < (_y) ? (_x) : (_y));                                           \
    })
#define I_MUL0(x)   ((x)*0L)
#define I_MUL1(x)   ((x)*1L)
#define I_MUL2(x)   ((x)*2L)
#define I_MUL3(x)   ((x)*3L)
#define I_MUL4(x)   ((x)*4L)
#define I_MUL5(x)   ((x)*5L)
#define I_MUL6(x)   ((x)*6L)
#define I_MUL7(x)   ((x)*7L)
#define I_MUL8(x)   ((x)*8L)
#define I_MUL9(x)   ((x)*9L)
#define I_MUL10(x)  ((x)*10L)
#define I_MUL11(x)  ((x)*11L)
#define I_MUL12(x)  ((x)*12L)
#define I_MUL13(x)  ((x)*13L)
#define I_MUL14(x)  ((x)*14L)
#define I_MUL15(x)  ((x)*15L)
#define I_MUL16(x)  ((x)*16L)
#define I_MUL17(x)  ((x)*17L)
#define I_MUL18(x)  ((x)*18L)
#define I_MUL19(x)  ((x)*19L)
#define I_MUL20(x)  ((x)*20L)
#define I_MUL21(x)  ((x)*21L)
#define I_MUL22(x)  ((x)*22L)
#define I_MUL23(x)  ((x)*23L)
#define I_MUL24(x)  ((x)*24L)
#define I_MUL25(x)  ((x)*25L)
#define I_MUL26(x)  ((x)*26L)
#define I_MUL27(x)  ((x)*27L)
#define I_MUL28(x)  ((x)*28L)
#define I_MUL29(x)  ((x)*29L)
#define I_MUL30(x)  ((x)*30L)
#define I_MUL31(x)  ((x)*31L)
#define I_MUL32(x)  ((x)*32L)
#define I_MUL33(x)  ((x)*33L)
#define I_MUL34(x)  ((x)*34L)
#define I_MUL35(x)  ((x)*35L)
#define I_MUL36(x)  ((x)*36L)
#define I_MUL37(x)  ((x)*37L)
#define I_MUL38(x)  ((x)*38L)
#define I_MUL39(x)  ((x)*39L)
#define I_MUL40(x)  ((x)*40L)
#define I_MUL41(x)  ((x)*41L)
#define I_MUL42(x)  ((x)*42L)
#define I_MUL43(x)  ((x)*43L)
#define I_MUL44(x)  ((x)*44L)
#define I_MUL45(x)  ((x)*45L)
#define I_MUL46(x)  ((x)*46L)
#define I_MUL47(x)  ((x)*47L)
#define I_MUL48(x)  ((x)*48L)
#define I_MUL49(x)  ((x)*49L)
#define I_MUL50(x)  ((x)*50L)
#define I_MUL51(x)  ((x)*51L)
#define I_MUL52(x)  ((x)*52L)
#define I_MUL53(x)  ((x)*53L)
#define I_MUL54(x)  ((x)*54L)
#define I_MUL55(x)  ((x)*55L)
#define I_MUL56(x)  ((x)*56L)
#define I_MUL57(x)  ((x)*57L)
#define I_MUL58(x)  ((x)*58L)
#define I_MUL59(x)  ((x)*59L)
#define I_MUL60(x)  ((x)*60L)
#define I_MUL61(x)  ((x)*61L)
#define I_MUL62(x)  ((x)*62L)
#define I_MUL63(x)  ((x)*63L)
#define I_MUL64(x)  ((x)*64L)
#define I_MUL65(x)  ((x)*65L)
#define I_MUL66(x)  ((x)*66L)
#define I_MUL67(x)  ((x)*67L)
#define I_MUL68(x)  ((x)*68L)
#define I_MUL69(x)  ((x)*69L)
#define I_MUL70(x)  ((x)*70L)
#define I_MUL71(x)  ((x)*71L)
#define I_MUL72(x)  ((x)*72L)
#define I_MUL73(x)  ((x)*73L)
#define I_MUL74(x)  ((x)*74L)
#define I_MUL75(x)  ((x)*75L)
#define I_MUL76(x)  ((x)*76L)
#define I_MUL77(x)  ((x)*77L)
#define I_MUL78(x)  ((x)*78L)
#define I_MUL79(x)  ((x)*79L)
#define I_MUL80(x)  ((x)*80L)
#define I_MUL81(x)  ((x)*81L)
#define I_MUL82(x)  ((x)*82L)
#define I_MUL83(x)  ((x)*83L)
#define I_MUL84(x)  ((x)*84L)
#define I_MUL85(x)  ((x)*85L)
#define I_MUL86(x)  ((x)*86L)
#define I_MUL87(x)  ((x)*87L)
#define I_MUL88(x)  ((x)*88L)
#define I_MUL89(x)  ((x)*89L)
#define I_MUL90(x)  ((x)*90L)
#define I_MUL91(x)  ((x)*91L)
#define I_MUL92(x)  ((x)*92L)
#define I_MUL93(x)  ((x)*93L)
#define I_MUL94(x)  ((x)*94L)
#define I_MUL95(x)  ((x)*95L)
#define I_MUL96(x)  ((x)*96L)
#define I_MUL97(x)  ((x)*97L)
#define I_MUL98(x)  ((x)*98L)
#define I_MUL99(x)  ((x)*99L)
#define I_MUL100(x) ((x)*100L)
#define I_MUL101(x) ((x)*101L)
#define I_MUL102(x) ((x)*102L)
#define I_MUL103(x) ((x)*103L)
#define I_MUL104(x) ((x)*104L)
#define I_MUL105(x) ((x)*105L)
#define I_MUL106(x) ((x)*106L)
#define I_MUL107(x) ((x)*107L)
#define I_MUL108(x) ((x)*108L)
#define I_MUL109(x) ((x)*109L)
#define I_MUL110(x) ((x)*110L)
#define I_MUL111(x) ((x)*111L)
#define I_MUL112(x) ((x)*112L)
#define I_MUL113(x) ((x)*113L)
#define I_MUL114(x) ((x)*114L)
#define I_MUL115(x) ((x)*115L)
#define I_MUL116(x) ((x)*116L)
#define I_MUL117(x) ((x)*117L)
#define I_MUL118(x) ((x)*118L)
#define I_MUL119(x) ((x)*119L)
#define I_MUL120(x) ((x)*120L)
#define I_MUL121(x) ((x)*121L)
#define I_MUL122(x) ((x)*122L)
#define I_MUL123(x) ((x)*123L)
#define I_MUL124(x) ((x)*124L)
#define I_MUL125(x) ((x)*125L)
#define I_MUL126(x) ((x)*126L)
#define I_MUL127(x) ((x)*127L)
#define I_MUL128(x) ((x)*128L)
#define I_MUL129(x) ((x)*129L)
#define I_MUL130(x) ((x)*130L)
#define I_MUL131(x) ((x)*131L)
#define I_MUL132(x) ((x)*132L)
#define I_MUL133(x) ((x)*133L)
#define I_MUL134(x) ((x)*134L)
#define I_MUL135(x) ((x)*135L)
#define I_MUL136(x) ((x)*136L)
#define I_MUL137(x) ((x)*137L)
#define I_MUL138(x) ((x)*138L)
#define I_MUL139(x) ((x)*139L)
#define I_MUL140(x) ((x)*140L)
#define I_MUL141(x) ((x)*141L)
#define I_MUL142(x) ((x)*142L)
#define I_MUL143(x) ((x)*143L)
#define I_MUL144(x) ((x)*144L)
#define I_MUL145(x) ((x)*145L)
#define I_MUL146(x) ((x)*146L)
#define I_MUL147(x) ((x)*147L)
#define I_MUL148(x) ((x)*148L)
#define I_MUL149(x) ((x)*149L)
#define I_MUL150(x) ((x)*150L)
#define I_MUL151(x) ((x)*151L)
#define I_MUL152(x) ((x)*152L)
#define I_MUL153(x) ((x)*153L)
#define I_MUL154(x) ((x)*154L)
#define I_MUL155(x) ((x)*155L)
#define I_MUL156(x) ((x)*156L)
#define I_MUL157(x) ((x)*157L)
#define I_MUL158(x) ((x)*158L)
#define I_MUL159(x) ((x)*159L)
#define I_MUL160(x) ((x)*160L)
#define I_MUL161(x) ((x)*161L)
#define I_MUL162(x) ((x)*162L)
#define I_MUL163(x) ((x)*163L)
#define I_MUL164(x) ((x)*164L)
#define I_MUL165(x) ((x)*165L)
#define I_MUL166(x) ((x)*166L)
#define I_MUL167(x) ((x)*167L)
#define I_MUL168(x) ((x)*168L)
#define I_MUL169(x) ((x)*169L)
#define I_MUL170(x) ((x)*170L)
#define I_MUL171(x) ((x)*171L)
#define I_MUL172(x) ((x)*172L)
#define I_MUL173(x) ((x)*173L)
#define I_MUL174(x) ((x)*174L)
#define I_MUL175(x) ((x)*175L)
#define I_MUL176(x) ((x)*176L)
#define I_MUL177(x) ((x)*177L)
#define I_MUL178(x) ((x)*178L)
#define I_MUL179(x) ((x)*179L)
#define I_MUL180(x) ((x)*180L)
#define I_MUL181(x) ((x)*181L)
#define I_MUL182(x) ((x)*182L)
#define I_MUL183(x) ((x)*183L)
#define I_MUL184(x) ((x)*184L)
#define I_MUL185(x) ((x)*185L)
#define I_MUL186(x) ((x)*186L)
#define I_MUL187(x) ((x)*187L)
#define I_MUL188(x) ((x)*188L)
#define I_MUL189(x) ((x)*189L)
#define I_MUL190(x) ((x)*190L)
#define I_MUL191(x) ((x)*191L)
#define I_MUL192(x) ((x)*192L)
#define I_MUL193(x) ((x)*193L)
#define I_MUL194(x) ((x)*194L)
#define I_MUL195(x) ((x)*195L)
#define I_MUL196(x) ((x)*196L)
#define I_MUL197(x) ((x)*197L)
#define I_MUL198(x) ((x)*198L)
#define I_MUL199(x) ((x)*199L)
#define I_MUL200(x) ((x)*200L)
#define I_MUL201(x) ((x)*201L)
#define I_MUL202(x) ((x)*202L)
#define I_MUL203(x) ((x)*203L)
#define I_MUL204(x) ((x)*204L)
#define I_MUL205(x) ((x)*205L)
#define I_MUL206(x) ((x)*206L)
#define I_MUL207(x) ((x)*207L)
#define I_MUL208(x) ((x)*208L)
#define I_MUL209(x) ((x)*209L)
#define I_MUL210(x) ((x)*210L)
#define I_MUL211(x) ((x)*211L)
#define I_MUL212(x) ((x)*212L)
#define I_MUL213(x) ((x)*213L)
#define I_MUL214(x) ((x)*214L)
#define I_MUL215(x) ((x)*215L)
#define I_MUL216(x) ((x)*216L)
#define I_MUL217(x) ((x)*217L)
#define I_MUL218(x) ((x)*218L)
#define I_MUL219(x) ((x)*219L)
#define I_MUL220(x) ((x)*220L)
#define I_MUL221(x) ((x)*221L)
#define I_MUL222(x) ((x)*222L)
#define I_MUL223(x) ((x)*223L)
#define I_MUL224(x) ((x)*224L)
#define I_MUL225(x) ((x)*225L)
#define I_MUL226(x) ((x)*226L)
#define I_MUL227(x) ((x)*227L)
#define I_MUL228(x) ((x)*228L)
#define I_MUL229(x) ((x)*229L)
#define I_MUL230(x) ((x)*230L)
#define I_MUL231(x) ((x)*231L)
#define I_MUL232(x) ((x)*232L)
#define I_MUL233(x) ((x)*233L)
#define I_MUL234(x) ((x)*234L)
#define I_MUL235(x) ((x)*235L)
#define I_MUL236(x) ((x)*236L)
#define I_MUL237(x) ((x)*237L)
#define I_MUL238(x) ((x)*238L)
#define I_MUL239(x) ((x)*239L)
#define I_MUL240(x) ((x)*240L)
#define I_MUL241(x) ((x)*241L)
#define I_MUL242(x) ((x)*242L)
#define I_MUL243(x) ((x)*243L)
#define I_MUL244(x) ((x)*244L)
#define I_MUL245(x) ((x)*245L)
#define I_MUL246(x) ((x)*246L)
#define I_MUL247(x) ((x)*247L)
#define I_MUL248(x) ((x)*248L)
#define I_MUL249(x) ((x)*249L)
#define I_MUL250(x) ((x)*250L)
#define I_MUL251(x) ((x)*251L)
#define I_MUL252(x) ((x)*252L)
#define I_MUL253(x) ((x)*253L)
#define I_MUL254(x) ((x)*254L)
#define I_MUL255(x) ((x)*255L)
#define I_MMUL(x)   CAT(I_MUL, x)
int32_t
test_generated_macros() {
#define INDIRECT_APPLY(m, op, ...) APPLY(m, op, __VA_ARGS__)
#define I_PLUS(x, y)               ((x) + (y))
    test_assert(CAT(I_, NOT_ONE_NARG()) == I_ONE);
    test_assert(IS_EMPTY() == 1);
    APPLY(a, b);
    APPLY_TOO(a, b);
    APPLY2(a, b);
    ARGP_APPLY(a, b);
    APPLY_PACKR(a);
    APPLY_PACKL(a);
    test_assert(PP_NARG(0) == 1);
    test_assert(CAT(I_, NOT_ONE_NARG(0)) == I_ONE);
    test_assert(IS_EMPTY(0) == 0);
    test_assert(APPLY(I_MUL2, +, 0) == 0);
    test_assert(APPLY_TOO(I_MUL2, +, 0) == 0);
    test_assert(APPLY2(I_MUL2, +, 0) == 0);
    test_assert(ARGP_APPLY(2, +, I_MUL0) == 0);
    test_assert(ARGP_APPLY(2, +, APPLY(I_MMUL, COMMA, 0)) == 0);
    test_assert(ARGP_APPLY(2, +, APPLY_TOO(I_MMUL, COMMA, 0)) == 0);
    test_assert(ARGP_APPLY(2, +, APPLY2(I_MMUL, COMMA, 0)) == 0);
    count = 0;
    APPLY(foo, ;, 0);
    test_assert(count == 0);
    count = 0;
    APPLY_TOO(foo, ;, 0);
    test_assert(count == 0);
    count = 0;
    APPLY2(foo, ;, 0);
    test_assert(count == 0);
    count = 0;
    APPLY(foo, ;, ARGP_APPLY(2, COMMA, I_MUL0));
    test_assert(count == 0);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKR(100L, 0));
    test_assert(count == -100);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKL(100L, 0));
    test_assert(count == 100);
    test_assert(PP_NARG(0, 1) == 2);
    test_assert(CAT(I_, NOT_ONE_NARG(0, 1)) == I_MANY);
    test_assert(IS_EMPTY(0, 1) == 0);
    test_assert(strcmp(V_TO_STR(CAT(0, 1)), "01") == 0);
    test_assert(APPLY_RECURSE(I_TEST_MIN, 0, 1) == 0);
    test_assert(APPLY_RECURSE(I_PLUS, 0, 1) == 1);
    test_assert(APPLY(I_MUL2, +, 0, 1) == 2);
    test_assert(APPLY_TOO(I_MUL2, +, 0, 1) == 2);
    test_assert(APPLY2(I_MUL2, +, 0, 1) == 2);
    test_assert(ARGP_APPLY(2, +, I_MUL0, I_MUL1) == 2);
    test_assert(ARGP_APPLY(2, +, APPLY(I_MMUL, COMMA, 0, 1)) == 2);
    test_assert(ARGP_APPLY(2, +, APPLY_TOO(I_MMUL, COMMA, 0, 1)) == 2);
    test_assert(ARGP_APPLY(2, +, APPLY2(I_MMUL, COMMA, 0, 1)) == 2);
    count = 0;
    APPLY(foo, ;, 0, 1);
    test_assert(count == 1);
    count = 0;
    APPLY_TOO(foo, ;, 0, 1);
    test_assert(count == 1);
    count = 0;
    APPLY2(foo, ;, 0, 1);
    test_assert(count == 1);
    count = 0;
    APPLY(foo, ;, ARGP_APPLY(2, COMMA, I_MUL0, I_MUL1));
    test_assert(count == 2);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKR(100L, 0, 1));
    test_assert(count == -199);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKL(100L, 0, 1));
    test_assert(count == 199);
    test_assert(PP_NARG(0, 1, 2) == 3);
    test_assert(CAT(I_, NOT_ONE_NARG(0, 1, 2)) == I_MANY);
    test_assert(IS_EMPTY(0, 1, 2) == 0);
    test_assert(strcmp(V_TO_STR(CAT(0, 1, 2)), "012") == 0);
    test_assert(APPLY_RECURSE(I_TEST_MIN, 0, 1, 2) == 0);
    test_assert(APPLY_RECURSE(I_PLUS, 0, 1, 2) == 3);
    test_assert(APPLY(I_MUL2, +, 0, 1, 2) == 6);
    test_assert(APPLY_TOO(I_MUL2, +, 0, 1, 2) == 6);
    test_assert(APPLY2(I_MUL2, +, 0, 1, 2) == 6);
    test_assert(ARGP_APPLY(2, +, I_MUL0, I_MUL1, I_MUL2) == 6);
    test_assert(ARGP_APPLY(2, +, APPLY(I_MMUL, COMMA, 0, 1, 2)) == 6);
    test_assert(ARGP_APPLY(2, +, APPLY_TOO(I_MMUL, COMMA, 0, 1, 2)) == 6);
    test_assert(ARGP_APPLY(2, +, APPLY2(I_MMUL, COMMA, 0, 1, 2)) == 6);
    count = 0;
    APPLY(foo, ;, 0, 1, 2);
    test_assert(count == 3);
    count = 0;
    APPLY_TOO(foo, ;, 0, 1, 2);
    test_assert(count == 3);
    count = 0;
    APPLY2(foo, ;, 0, 1, 2);
    test_assert(count == 3);
    count = 0;
    APPLY(foo, ;, ARGP_APPLY(2, COMMA, I_MUL0, I_MUL1, I_MUL2));
    test_assert(count == 6);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKR(100L, 0, 1, 2));
    test_assert(count == -297);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKL(100L, 0, 1, 2));
    test_assert(count == 297);
    test_assert(PP_NARG(0, 1, 2, 3) == 4);
    test_assert(CAT(I_, NOT_ONE_NARG(0, 1, 2, 3)) == I_MANY);
    test_assert(IS_EMPTY(0, 1, 2, 3) == 0);
    test_assert(strcmp(V_TO_STR(CAT(0, 1, 2, 3)), "0123") == 0);
    test_assert(APPLY_RECURSE(I_TEST_MIN, 0, 1, 2, 3) == 0);
    test_assert(APPLY_RECURSE(I_PLUS, 0, 1, 2, 3) == 6);
    test_assert(APPLY(I_MUL2, +, 0, 1, 2, 3) == 12);
    test_assert(APPLY_TOO(I_MUL2, +, 0, 1, 2, 3) == 12);
    test_assert(APPLY2(I_MUL2, +, 0, 1, 2, 3) == 12);
    test_assert(ARGP_APPLY(2, +, I_MUL0, I_MUL1, I_MUL2, I_MUL3) == 12);
    test_assert(ARGP_APPLY(2, +, APPLY(I_MMUL, COMMA, 0, 1, 2, 3)) == 12);
    test_assert(ARGP_APPLY(2, +, APPLY_TOO(I_MMUL, COMMA, 0, 1, 2, 3)) == 12);
    test_assert(ARGP_APPLY(2, +, APPLY2(I_MMUL, COMMA, 0, 1, 2, 3)) == 12);
    count = 0;
    APPLY(foo, ;, 0, 1, 2, 3);
    test_assert(count == 6);
    count = 0;
    APPLY_TOO(foo, ;, 0, 1, 2, 3);
    test_assert(count == 6);
    count = 0;
    APPLY2(foo, ;, 0, 1, 2, 3);
    test_assert(count == 6);
    count = 0;
    APPLY(foo, ;, ARGP_APPLY(2, COMMA, I_MUL0, I_MUL1, I_MUL2, I_MUL3));
    test_assert(count == 12);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKR(100L, 0, 1, 2, 3));
    test_assert(count == -394);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKL(100L, 0, 1, 2, 3));
    test_assert(count == 394);
    test_assert(PP_NARG(0, 1, 2, 3, 4) == 5);
    test_assert(CAT(I_, NOT_ONE_NARG(0, 1, 2, 3, 4)) == I_MANY);
    test_assert(IS_EMPTY(0, 1, 2, 3, 4) == 0);
    test_assert(strcmp(V_TO_STR(CAT(0, 1, 2, 3, 4)), "01234") == 0);
    test_assert(APPLY_RECURSE(I_TEST_MIN, 0, 1, 2, 3, 4) == 0);
    test_assert(APPLY_RECURSE(I_PLUS, 0, 1, 2, 3, 4) == 10);
    test_assert(APPLY(I_MUL2, +, 0, 1, 2, 3, 4) == 20);
    test_assert(APPLY_TOO(I_MUL2, +, 0, 1, 2, 3, 4) == 20);
    test_assert(APPLY2(I_MUL2, +, 0, 1, 2, 3, 4) == 20);
    test_assert(ARGP_APPLY(2, +, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4) == 20);
    test_assert(ARGP_APPLY(2, +, APPLY(I_MMUL, COMMA, 0, 1, 2, 3, 4)) == 20);
    test_assert(ARGP_APPLY(2, +, APPLY_TOO(I_MMUL, COMMA, 0, 1, 2, 3, 4)) ==
                20);
    test_assert(ARGP_APPLY(2, +, APPLY2(I_MMUL, COMMA, 0, 1, 2, 3, 4)) == 20);
    count = 0;
    APPLY(foo, ;, 0, 1, 2, 3, 4);
    test_assert(count == 10);
    count = 0;
    APPLY_TOO(foo, ;, 0, 1, 2, 3, 4);
    test_assert(count == 10);
    count = 0;
    APPLY2(foo, ;, 0, 1, 2, 3, 4);
    test_assert(count == 10);
    count = 0;
    APPLY(foo, ;, ARGP_APPLY(2, COMMA, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4));
    test_assert(count == 20);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKR(100L, 0, 1, 2, 3, 4));
    test_assert(count == -490);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKL(100L, 0, 1, 2, 3, 4));
    test_assert(count == 490);
    test_assert(PP_NARG(0, 1, 2, 3, 4, 5) == 6);
    test_assert(CAT(I_, NOT_ONE_NARG(0, 1, 2, 3, 4, 5)) == I_MANY);
    test_assert(IS_EMPTY(0, 1, 2, 3, 4, 5) == 0);
    test_assert(strcmp(V_TO_STR(CAT(0, 1, 2, 3, 4, 5)), "012345") == 0);
    test_assert(APPLY_RECURSE(I_TEST_MIN, 0, 1, 2, 3, 4, 5) == 0);
    test_assert(APPLY_RECURSE(I_PLUS, 0, 1, 2, 3, 4, 5) == 15);
    test_assert(APPLY(I_MUL2, +, 0, 1, 2, 3, 4, 5) == 30);
    test_assert(APPLY_TOO(I_MUL2, +, 0, 1, 2, 3, 4, 5) == 30);
    test_assert(APPLY2(I_MUL2, +, 0, 1, 2, 3, 4, 5) == 30);
    test_assert(
        ARGP_APPLY(2, +, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5) == 30);
    test_assert(ARGP_APPLY(2, +, APPLY(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5)) == 30);
    test_assert(ARGP_APPLY(2, +, APPLY_TOO(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5)) ==
                30);
    test_assert(ARGP_APPLY(2, +, APPLY2(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5)) ==
                30);
    count = 0;
    APPLY(foo, ;, 0, 1, 2, 3, 4, 5);
    test_assert(count == 15);
    count = 0;
    APPLY_TOO(foo, ;, 0, 1, 2, 3, 4, 5);
    test_assert(count == 15);
    count = 0;
    APPLY2(foo, ;, 0, 1, 2, 3, 4, 5);
    test_assert(count == 15);
    count = 0;
    APPLY(foo, ;, ARGP_APPLY(2, COMMA, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4,
                             I_MUL5));
    test_assert(count == 30);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKR(100L, 0, 1, 2, 3, 4, 5));
    test_assert(count == -585);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKL(100L, 0, 1, 2, 3, 4, 5));
    test_assert(count == 585);
    test_assert(PP_NARG(0, 1, 2, 3, 4, 5, 6) == 7);
    test_assert(CAT(I_, NOT_ONE_NARG(0, 1, 2, 3, 4, 5, 6)) == I_MANY);
    test_assert(IS_EMPTY(0, 1, 2, 3, 4, 5, 6) == 0);
    test_assert(strcmp(V_TO_STR(CAT(0, 1, 2, 3, 4, 5, 6)), "0123456") == 0);
    test_assert(APPLY_RECURSE(I_TEST_MIN, 0, 1, 2, 3, 4, 5, 6) == 0);
    test_assert(APPLY_RECURSE(I_PLUS, 0, 1, 2, 3, 4, 5, 6) == 21);
    test_assert(APPLY(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6) == 42);
    test_assert(APPLY_TOO(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6) == 42);
    test_assert(APPLY2(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6) == 42);
    test_assert(ARGP_APPLY(2, +, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5,
                           I_MUL6) == 42);
    test_assert(ARGP_APPLY(2, +, APPLY(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6)) ==
                42);
    test_assert(
        ARGP_APPLY(2, +, APPLY_TOO(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6)) == 42);
    test_assert(ARGP_APPLY(2, +, APPLY2(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6)) ==
                42);
    count = 0;
    APPLY(foo, ;, 0, 1, 2, 3, 4, 5, 6);
    test_assert(count == 21);
    count = 0;
    APPLY_TOO(foo, ;, 0, 1, 2, 3, 4, 5, 6);
    test_assert(count == 21);
    count = 0;
    APPLY2(foo, ;, 0, 1, 2, 3, 4, 5, 6);
    test_assert(count == 21);
    count = 0;
    APPLY(foo, ;, ARGP_APPLY(2, COMMA, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4,
                             I_MUL5, I_MUL6));
    test_assert(count == 42);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKR(100L, 0, 1, 2, 3, 4, 5, 6));
    test_assert(count == -679);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKL(100L, 0, 1, 2, 3, 4, 5, 6));
    test_assert(count == 679);
    test_assert(PP_NARG(0, 1, 2, 3, 4, 5, 6, 7) == 8);
    test_assert(CAT(I_, NOT_ONE_NARG(0, 1, 2, 3, 4, 5, 6, 7)) == I_MANY);
    test_assert(IS_EMPTY(0, 1, 2, 3, 4, 5, 6, 7) == 0);
    test_assert(strcmp(V_TO_STR(CAT(0, 1, 2, 3, 4, 5, 6, 7)), "01234567") == 0);
    test_assert(APPLY_RECURSE(I_TEST_MIN, 0, 1, 2, 3, 4, 5, 6, 7) == 0);
    test_assert(APPLY_RECURSE(I_PLUS, 0, 1, 2, 3, 4, 5, 6, 7) == 28);
    test_assert(APPLY(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7) == 56);
    test_assert(APPLY_TOO(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7) == 56);
    test_assert(APPLY2(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7) == 56);
    test_assert(ARGP_APPLY(2, +, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5,
                           I_MUL6, I_MUL7) == 56);
    test_assert(
        ARGP_APPLY(2, +, APPLY(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7)) == 56);
    test_assert(
        ARGP_APPLY(2, +, APPLY_TOO(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7)) ==
        56);
    test_assert(
        ARGP_APPLY(2, +, APPLY2(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7)) == 56);
    count = 0;
    APPLY(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7);
    test_assert(count == 28);
    count = 0;
    APPLY_TOO(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7);
    test_assert(count == 28);
    count = 0;
    APPLY2(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7);
    test_assert(count == 28);
    count = 0;
    APPLY(foo, ;, ARGP_APPLY(2, COMMA, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4,
                             I_MUL5, I_MUL6, I_MUL7));
    test_assert(count == 56);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKR(100L, 0, 1, 2, 3, 4, 5, 6, 7));
    test_assert(count == -772);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKL(100L, 0, 1, 2, 3, 4, 5, 6, 7));
    test_assert(count == 772);
    test_assert(PP_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8) == 9);
    test_assert(CAT(I_, NOT_ONE_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8)) == I_MANY);
    test_assert(IS_EMPTY(0, 1, 2, 3, 4, 5, 6, 7, 8) == 0);
    test_assert(strcmp(V_TO_STR(CAT(0, 1, 2, 3, 4, 5, 6, 7, 8)), "012345678") ==
                0);
    test_assert(APPLY_RECURSE(I_TEST_MIN, 0, 1, 2, 3, 4, 5, 6, 7, 8) == 0);
    test_assert(APPLY_RECURSE(I_PLUS, 0, 1, 2, 3, 4, 5, 6, 7, 8) == 36);
    test_assert(APPLY(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8) == 72);
    test_assert(APPLY_TOO(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8) == 72);
    test_assert(APPLY2(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8) == 72);
    test_assert(ARGP_APPLY(2, +, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5,
                           I_MUL6, I_MUL7, I_MUL8) == 72);
    test_assert(
        ARGP_APPLY(2, +, APPLY(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8)) ==
        72);
    test_assert(
        ARGP_APPLY(2, +, APPLY_TOO(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8)) ==
        72);
    test_assert(
        ARGP_APPLY(2, +, APPLY2(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8)) ==
        72);
    count = 0;
    APPLY(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8);
    test_assert(count == 36);
    count = 0;
    APPLY_TOO(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8);
    test_assert(count == 36);
    count = 0;
    APPLY2(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8);
    test_assert(count == 36);
    count = 0;
    APPLY(foo, ;, ARGP_APPLY(2, COMMA, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4,
                             I_MUL5, I_MUL6, I_MUL7, I_MUL8));
    test_assert(count == 72);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKR(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8));
    test_assert(count == -864);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKL(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8));
    test_assert(count == 864);
    test_assert(PP_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9) == 10);
    test_assert(CAT(I_, NOT_ONE_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)) == I_MANY);
    test_assert(IS_EMPTY(0, 1, 2, 3, 4, 5, 6, 7, 8, 9) == 0);
    test_assert(
        strcmp(V_TO_STR(CAT(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)), "0123456789") == 0);
    test_assert(APPLY_RECURSE(I_TEST_MIN, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9) == 0);
    test_assert(APPLY_RECURSE(I_PLUS, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9) == 45);
    test_assert(APPLY(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9) == 90);
    test_assert(APPLY_TOO(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9) == 90);
    test_assert(APPLY2(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9) == 90);
    test_assert(ARGP_APPLY(2, +, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5,
                           I_MUL6, I_MUL7, I_MUL8, I_MUL9) == 90);
    test_assert(
        ARGP_APPLY(2, +, APPLY(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9)) ==
        90);
    test_assert(ARGP_APPLY(2, +,
                           APPLY_TOO(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8,
                                     9)) == 90);
    test_assert(
        ARGP_APPLY(2, +, APPLY2(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9)) ==
        90);
    count = 0;
    APPLY(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    test_assert(count == 45);
    count = 0;
    APPLY_TOO(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    test_assert(count == 45);
    count = 0;
    APPLY2(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    test_assert(count == 45);
    count = 0;
    APPLY(foo, ;, ARGP_APPLY(2, COMMA, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4,
                             I_MUL5, I_MUL6, I_MUL7, I_MUL8, I_MUL9));
    test_assert(count == 90);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKR(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9));
    test_assert(count == -955);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKL(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9));
    test_assert(count == 955);
    test_assert(PP_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 11);
    test_assert(CAT(I_, NOT_ONE_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10)) ==
                I_MANY);
    test_assert(IS_EMPTY(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 0);
    test_assert(strcmp(V_TO_STR(CAT(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10)),
                       "012345678910") == 0);
    test_assert(APPLY_RECURSE(I_TEST_MIN, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) ==
                0);
    test_assert(APPLY_RECURSE(I_PLUS, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 55);
    test_assert(APPLY(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 110);
    test_assert(APPLY_TOO(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 110);
    test_assert(APPLY2(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 110);
    test_assert(ARGP_APPLY(2, +, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5,
                           I_MUL6, I_MUL7, I_MUL8, I_MUL9, I_MUL10) == 110);
    test_assert(ARGP_APPLY(2, +,
                           APPLY(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                 10)) == 110);
    test_assert(ARGP_APPLY(2, +,
                           APPLY_TOO(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8,
                                     9, 10)) == 110);
    test_assert(ARGP_APPLY(2, +,
                           APPLY2(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                  10)) == 110);
    count = 0;
    APPLY(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    test_assert(count == 55);
    count = 0;
    APPLY_TOO(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    test_assert(count == 55);
    count = 0;
    APPLY2(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    test_assert(count == 55);
    count = 0;
    APPLY(foo, ;, ARGP_APPLY(2, COMMA, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4,
                             I_MUL5, I_MUL6, I_MUL7, I_MUL8, I_MUL9, I_MUL10));
    test_assert(count == 110);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKR(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
    test_assert(count == -1045);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKL(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
    test_assert(count == 1045);
    test_assert(PP_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11) == 12);
    test_assert(CAT(I_, NOT_ONE_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11)) ==
                I_MANY);
    test_assert(IS_EMPTY(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11) == 0);
    test_assert(strcmp(V_TO_STR(CAT(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11)),
                       "01234567891011") == 0);
    test_assert(
        APPLY_RECURSE(I_TEST_MIN, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11) == 0);
    test_assert(APPLY_RECURSE(I_PLUS, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11) ==
                66);
    test_assert(APPLY(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11) == 132);
    test_assert(APPLY_TOO(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11) ==
                132);
    test_assert(APPLY2(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11) == 132);
    test_assert(ARGP_APPLY(2, +, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5,
                           I_MUL6, I_MUL7, I_MUL8, I_MUL9, I_MUL10,
                           I_MUL11) == 132);
    test_assert(ARGP_APPLY(2, +,
                           APPLY(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                 10, 11)) == 132);
    test_assert(ARGP_APPLY(2, +,
                           APPLY_TOO(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8,
                                     9, 10, 11)) == 132);
    test_assert(ARGP_APPLY(2, +,
                           APPLY2(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                  10, 11)) == 132);
    count = 0;
    APPLY(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
    test_assert(count == 66);
    count = 0;
    APPLY_TOO(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
    test_assert(count == 66);
    count = 0;
    APPLY2(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
    test_assert(count == 66);
    count = 0;
    APPLY(foo, ;
          , ARGP_APPLY(2, COMMA, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5,
                       I_MUL6, I_MUL7, I_MUL8, I_MUL9, I_MUL10, I_MUL11));
    test_assert(count == 132);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKR(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11));
    test_assert(count == -1134);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKL(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11));
    test_assert(count == 1134);
    test_assert(PP_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12) == 13);
    test_assert(CAT(I_, NOT_ONE_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                                     12)) == I_MANY);
    test_assert(IS_EMPTY(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12) == 0);
    test_assert(strcmp(V_TO_STR(CAT(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12)),
                       "0123456789101112") == 0);
    test_assert(APPLY_RECURSE(I_TEST_MIN, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                              12) == 0);
    test_assert(
        APPLY_RECURSE(I_PLUS, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12) == 78);
    test_assert(APPLY(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12) ==
                156);
    test_assert(
        APPLY_TOO(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12) == 156);
    test_assert(APPLY2(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12) ==
                156);
    test_assert(ARGP_APPLY(2, +, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5,
                           I_MUL6, I_MUL7, I_MUL8, I_MUL9, I_MUL10, I_MUL11,
                           I_MUL12) == 156);
    test_assert(ARGP_APPLY(2, +,
                           APPLY(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                 10, 11, 12)) == 156);
    test_assert(ARGP_APPLY(2, +,
                           APPLY_TOO(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8,
                                     9, 10, 11, 12)) == 156);
    test_assert(ARGP_APPLY(2, +,
                           APPLY2(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                  10, 11, 12)) == 156);
    count = 0;
    APPLY(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
    test_assert(count == 78);
    count = 0;
    APPLY_TOO(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
    test_assert(count == 78);
    count = 0;
    APPLY2(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
    test_assert(count == 78);
    count = 0;
    APPLY(foo, ;, ARGP_APPLY(2, COMMA, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4,
                             I_MUL5, I_MUL6, I_MUL7, I_MUL8, I_MUL9, I_MUL10,
                             I_MUL11, I_MUL12));
    test_assert(count == 156);
    count = 0;
    APPLY(dfoo2, ;
          , APPLY_PACKR(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12));
    test_assert(count == -1222);
    count = 0;
    APPLY(dfoo2, ;
          , APPLY_PACKL(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12));
    test_assert(count == 1222);
    test_assert(PP_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13) == 14);
    test_assert(CAT(I_, NOT_ONE_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                     13)) == I_MANY);
    test_assert(IS_EMPTY(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13) == 0);
    test_assert(
        strcmp(V_TO_STR(CAT(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13)),
               "012345678910111213") == 0);
    test_assert(APPLY_RECURSE(I_TEST_MIN, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                              12, 13) == 0);
    test_assert(APPLY_RECURSE(I_PLUS, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                              13) == 91);
    test_assert(
        APPLY(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13) == 182);
    test_assert(APPLY_TOO(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                          13) == 182);
    test_assert(
        APPLY2(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13) == 182);
    test_assert(ARGP_APPLY(2, +, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5,
                           I_MUL6, I_MUL7, I_MUL8, I_MUL9, I_MUL10, I_MUL11,
                           I_MUL12, I_MUL13) == 182);
    test_assert(ARGP_APPLY(2, +,
                           APPLY(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                 10, 11, 12, 13)) == 182);
    test_assert(ARGP_APPLY(2, +,
                           APPLY_TOO(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8,
                                     9, 10, 11, 12, 13)) == 182);
    test_assert(ARGP_APPLY(2, +,
                           APPLY2(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                  10, 11, 12, 13)) == 182);
    count = 0;
    APPLY(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
    test_assert(count == 91);
    count = 0;
    APPLY_TOO(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
    test_assert(count == 91);
    count = 0;
    APPLY2(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
    test_assert(count == 91);
    count = 0;
    APPLY(foo, ;, ARGP_APPLY(2, COMMA, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4,
                             I_MUL5, I_MUL6, I_MUL7, I_MUL8, I_MUL9, I_MUL10,
                             I_MUL11, I_MUL12, I_MUL13));
    test_assert(count == 182);
    count = 0;
    APPLY(dfoo2, ;
          , APPLY_PACKR(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13));
    test_assert(count == -1309);
    count = 0;
    APPLY(dfoo2, ;
          , APPLY_PACKL(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13));
    test_assert(count == 1309);
    test_assert(PP_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14) ==
                15);
    test_assert(CAT(I_, NOT_ONE_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                     13, 14)) == I_MANY);
    test_assert(IS_EMPTY(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14) ==
                0);
    test_assert(
        strcmp(V_TO_STR(CAT(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14)),
               "01234567891011121314") == 0);
    test_assert(APPLY_RECURSE(I_TEST_MIN, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                              12, 13, 14) == 0);
    test_assert(APPLY_RECURSE(I_PLUS, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                              13, 14) == 105);
    test_assert(APPLY(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                      14) == 210);
    test_assert(APPLY_TOO(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                          13, 14) == 210);
    test_assert(APPLY2(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                       14) == 210);
    test_assert(ARGP_APPLY(2, +, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5,
                           I_MUL6, I_MUL7, I_MUL8, I_MUL9, I_MUL10, I_MUL11,
                           I_MUL12, I_MUL13, I_MUL14) == 210);
    test_assert(ARGP_APPLY(2, +,
                           APPLY(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                 10, 11, 12, 13, 14)) == 210);
    test_assert(ARGP_APPLY(2, +,
                           APPLY_TOO(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8,
                                     9, 10, 11, 12, 13, 14)) == 210);
    test_assert(ARGP_APPLY(2, +,
                           APPLY2(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                  10, 11, 12, 13, 14)) == 210);
    count = 0;
    APPLY(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14);
    test_assert(count == 105);
    count = 0;
    APPLY_TOO(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14);
    test_assert(count == 105);
    count = 0;
    APPLY2(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14);
    test_assert(count == 105);
    count = 0;
    APPLY(foo, ;, ARGP_APPLY(2, COMMA, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4,
                             I_MUL5, I_MUL6, I_MUL7, I_MUL8, I_MUL9, I_MUL10,
                             I_MUL11, I_MUL12, I_MUL13, I_MUL14));
    test_assert(count == 210);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKR(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                13, 14));
    test_assert(count == -1395);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKL(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                13, 14));
    test_assert(count == 1395);
    test_assert(PP_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15) ==
                16);
    test_assert(CAT(I_, NOT_ONE_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                     13, 14, 15)) == I_MANY);
    test_assert(
        IS_EMPTY(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15) == 0);
    test_assert(strcmp(V_TO_STR(CAT(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                    13, 14, 15)),
                       "0123456789101112131415") == 0);
    test_assert(APPLY_RECURSE(I_TEST_MIN, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                              12, 13, 14, 15) == 0);
    test_assert(APPLY_RECURSE(I_PLUS, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                              13, 14, 15) == 120);
    test_assert(APPLY(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                      14, 15) == 240);
    test_assert(APPLY_TOO(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                          13, 14, 15) == 240);
    test_assert(APPLY2(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                       14, 15) == 240);
    test_assert(ARGP_APPLY(2, +, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5,
                           I_MUL6, I_MUL7, I_MUL8, I_MUL9, I_MUL10, I_MUL11,
                           I_MUL12, I_MUL13, I_MUL14, I_MUL15) == 240);
    test_assert(ARGP_APPLY(2, +,
                           APPLY(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                 10, 11, 12, 13, 14, 15)) == 240);
    test_assert(ARGP_APPLY(2, +,
                           APPLY_TOO(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8,
                                     9, 10, 11, 12, 13, 14, 15)) == 240);
    test_assert(ARGP_APPLY(2, +,
                           APPLY2(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                  10, 11, 12, 13, 14, 15)) == 240);
    count = 0;
    APPLY(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    test_assert(count == 120);
    count = 0;
    APPLY_TOO(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    test_assert(count == 120);
    count = 0;
    APPLY2(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    test_assert(count == 120);
    count = 0;
    APPLY(foo, ;, ARGP_APPLY(2, COMMA, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4,
                             I_MUL5, I_MUL6, I_MUL7, I_MUL8, I_MUL9, I_MUL10,
                             I_MUL11, I_MUL12, I_MUL13, I_MUL14, I_MUL15));
    test_assert(count == 240);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKR(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                13, 14, 15));
    test_assert(count == -1480);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKL(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                13, 14, 15));
    test_assert(count == 1480);
    test_assert(PP_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                        30, 31) == 32);
    test_assert(CAT(I_, NOT_ONE_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                     13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
                                     24, 25, 26, 27, 28, 29, 30, 31)) ==
                I_MANY);
    test_assert(IS_EMPTY(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                         16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                         30, 31) == 0);
    test_assert(
        strcmp(V_TO_STR(CAT(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                            15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
                            28, 29, 30, 31)),
               "012345678910111213141516171819202122232425262728293031") == 0);
    test_assert(APPLY_RECURSE(I_TEST_MIN, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                              12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
                              24, 25, 26, 27, 28, 29, 30, 31) == 0);
    test_assert(APPLY_RECURSE(I_PLUS, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                              13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
                              25, 26, 27, 28, 29, 30, 31) == 496);
    test_assert(APPLY(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                      14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
                      28, 29, 30, 31) == 992);
    test_assert(APPLY_TOO(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                          13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
                          26, 27, 28, 29, 30, 31) == 992);
    test_assert(APPLY2(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                       14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
                       28, 29, 30, 31) == 992);
    test_assert(ARGP_APPLY(2, +, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5,
                           I_MUL6, I_MUL7, I_MUL8, I_MUL9, I_MUL10, I_MUL11,
                           I_MUL12, I_MUL13, I_MUL14, I_MUL15, I_MUL16, I_MUL17,
                           I_MUL18, I_MUL19, I_MUL20, I_MUL21, I_MUL22, I_MUL23,
                           I_MUL24, I_MUL25, I_MUL26, I_MUL27, I_MUL28, I_MUL29,
                           I_MUL30, I_MUL31) == 992);
    test_assert(ARGP_APPLY(2, +,
                           APPLY(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
                                 22, 23, 24, 25, 26, 27, 28, 29, 30, 31)) ==
                992);
    test_assert(
        ARGP_APPLY(2, +,
                   APPLY_TOO(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                             11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
                             24, 25, 26, 27, 28, 29, 30, 31)) == 992);
    test_assert(
        ARGP_APPLY(2, +,
                   APPLY2(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                          12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
                          25, 26, 27, 28, 29, 30, 31)) == 992);
    count = 0;
    APPLY(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31);
    test_assert(count == 496);
    count = 0;
    APPLY_TOO(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                    17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31);
    test_assert(count == 496);
    count = 0;
    APPLY2(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31);
    test_assert(count == 496);
    count = 0;
    APPLY(foo, ;
          , ARGP_APPLY(2, COMMA, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5,
                       I_MUL6, I_MUL7, I_MUL8, I_MUL9, I_MUL10, I_MUL11,
                       I_MUL12, I_MUL13, I_MUL14, I_MUL15, I_MUL16, I_MUL17,
                       I_MUL18, I_MUL19, I_MUL20, I_MUL21, I_MUL22, I_MUL23,
                       I_MUL24, I_MUL25, I_MUL26, I_MUL27, I_MUL28, I_MUL29,
                       I_MUL30, I_MUL31));
    test_assert(count == 992);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKR(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
                                25, 26, 27, 28, 29, 30, 31));
    test_assert(count == -2704);
    count = 0;
    APPLY(dfoo2, ;, APPLY_PACKL(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
                                25, 26, 27, 28, 29, 30, 31));
    test_assert(count == 2704);
    test_assert(PP_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                        30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
                        44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
                        58, 59, 60, 61, 62, 63) == 64);
    test_assert(CAT(I_, NOT_ONE_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                     13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
                                     24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
                                     35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45,
                                     46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
                                     57, 58, 59, 60, 61, 62, 63)) == I_MANY);
    test_assert(IS_EMPTY(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                         16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                         30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
                         44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
                         58, 59, 60, 61, 62, 63) == 0);
    test_assert(
        strcmp(
            V_TO_STR(CAT(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                         16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                         30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
                         44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
                         58, 59, 60, 61, 62, 63)),
            "0123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263") ==
        0);
    test_assert(APPLY_RECURSE(I_TEST_MIN, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                              12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
                              24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
                              36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
                              48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
                              60, 61, 62, 63) == 0);
    test_assert(APPLY_RECURSE(I_PLUS, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                              13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
                              25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
                              37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
                              49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
                              61, 62, 63) == 2016);
    test_assert(APPLY(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                      14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
                      28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
                      42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55,
                      56, 57, 58, 59, 60, 61, 62, 63) == 4032);
    test_assert(APPLY_TOO(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                          13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
                          26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
                          39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
                          52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
                          63) == 4032);
    test_assert(APPLY2(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                       14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
                       28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
                       42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55,
                       56, 57, 58, 59, 60, 61, 62, 63) == 4032);
    test_assert(ARGP_APPLY(2, +, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5,
                           I_MUL6, I_MUL7, I_MUL8, I_MUL9, I_MUL10, I_MUL11,
                           I_MUL12, I_MUL13, I_MUL14, I_MUL15, I_MUL16, I_MUL17,
                           I_MUL18, I_MUL19, I_MUL20, I_MUL21, I_MUL22, I_MUL23,
                           I_MUL24, I_MUL25, I_MUL26, I_MUL27, I_MUL28, I_MUL29,
                           I_MUL30, I_MUL31, I_MUL32, I_MUL33, I_MUL34, I_MUL35,
                           I_MUL36, I_MUL37, I_MUL38, I_MUL39, I_MUL40, I_MUL41,
                           I_MUL42, I_MUL43, I_MUL44, I_MUL45, I_MUL46, I_MUL47,
                           I_MUL48, I_MUL49, I_MUL50, I_MUL51, I_MUL52, I_MUL53,
                           I_MUL54, I_MUL55, I_MUL56, I_MUL57, I_MUL58, I_MUL59,
                           I_MUL60, I_MUL61, I_MUL62, I_MUL63) == 4032);
    test_assert(
        ARGP_APPLY(2, +,
                   APPLY(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                         12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
                         26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
                         40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
                         54, 55, 56, 57, 58, 59, 60, 61, 62, 63)) == 4032);
    test_assert(
        ARGP_APPLY(
            2, +,
            APPLY_TOO(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                      13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
                      27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                      41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
                      55, 56, 57, 58, 59, 60, 61, 62, 63)) == 4032);
    test_assert(ARGP_APPLY(2, +,
                           APPLY2(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                                  21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
                                  32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                                  43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
                                  54, 55, 56, 57, 58, 59, 60, 61, 62, 63)) ==
                4032);
    count = 0;
    APPLY(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63);
    test_assert(count == 2016);
    count = 0;
    APPLY_TOO(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                    17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
                    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
                    47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,
                    62, 63);
    test_assert(count == 2016);
    count = 0;
    APPLY2(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63);
    test_assert(count == 2016);
    count = 0;
    APPLY(foo, ;
          , ARGP_APPLY(2, COMMA, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5,
                       I_MUL6, I_MUL7, I_MUL8, I_MUL9, I_MUL10, I_MUL11,
                       I_MUL12, I_MUL13, I_MUL14, I_MUL15, I_MUL16, I_MUL17,
                       I_MUL18, I_MUL19, I_MUL20, I_MUL21, I_MUL22, I_MUL23,
                       I_MUL24, I_MUL25, I_MUL26, I_MUL27, I_MUL28, I_MUL29,
                       I_MUL30, I_MUL31, I_MUL32, I_MUL33, I_MUL34, I_MUL35,
                       I_MUL36, I_MUL37, I_MUL38, I_MUL39, I_MUL40, I_MUL41,
                       I_MUL42, I_MUL43, I_MUL44, I_MUL45, I_MUL46, I_MUL47,
                       I_MUL48, I_MUL49, I_MUL50, I_MUL51, I_MUL52, I_MUL53,
                       I_MUL54, I_MUL55, I_MUL56, I_MUL57, I_MUL58, I_MUL59,
                       I_MUL60, I_MUL61, I_MUL62, I_MUL63));
    test_assert(count == 4032);
    count = 0;
    APPLY(dfoo2, ;
          , APPLY_PACKR(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                        29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                        43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
                        57, 58, 59, 60, 61, 62, 63));
    test_assert(count == -4384);
    count = 0;
    APPLY(dfoo2, ;
          , APPLY_PACKL(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                        29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                        43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
                        57, 58, 59, 60, 61, 62, 63));
    test_assert(count == 4384);
    test_assert(PP_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                        30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
                        44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
                        58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
                        72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
                        86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
                        100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                        111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
                        122, 123, 124, 125, 126, 127) == 128);
    test_assert(
        CAT(I_,
            NOT_ONE_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                         16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                         30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
                         44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
                         58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
                         72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
                         86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
                         100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                         111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
                         122, 123, 124, 125, 126, 127)) == I_MANY);
    test_assert(IS_EMPTY(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                         16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                         30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
                         44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
                         58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
                         72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
                         86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
                         100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                         111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
                         122, 123, 124, 125, 126, 127) == 0);
    test_assert(
        strcmp(
            V_TO_STR(CAT(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                         16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                         30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
                         44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
                         58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
                         72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
                         86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
                         100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                         111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
                         122, 123, 124, 125, 126, 127)),
            "0123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899100101102103104105106107108109110111112113114115116117118119120121122123124125126127") ==
        0);
    test_assert(
        APPLY_RECURSE(I_TEST_MIN, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                      14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
                      28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
                      42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55,
                      56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
                      70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83,
                      84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
                      98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
                      110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
                      121, 122, 123, 124, 125, 126, 127) == 0);
    test_assert(
        APPLY_RECURSE(I_PLUS, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                      15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                      29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                      43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
                      57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
                      71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
                      85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
                      99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                      111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
                      122, 123, 124, 125, 126, 127) == 8128);
    test_assert(APPLY(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                      14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
                      28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
                      42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55,
                      56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
                      70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83,
                      84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
                      98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
                      110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
                      121, 122, 123, 124, 125, 126, 127) == 16256);
    test_assert(APPLY_TOO(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                          13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
                          26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
                          39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
                          52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
                          65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77,
                          78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90,
                          91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
                          103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
                          114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124,
                          125, 126, 127) == 16256);
    test_assert(APPLY2(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                       14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
                       28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
                       42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55,
                       56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
                       70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83,
                       84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
                       98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
                       110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
                       121, 122, 123, 124, 125, 126, 127) == 16256);
    test_assert(
        ARGP_APPLY(
            2, +, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5, I_MUL6,
            I_MUL7, I_MUL8, I_MUL9, I_MUL10, I_MUL11, I_MUL12, I_MUL13, I_MUL14,
            I_MUL15, I_MUL16, I_MUL17, I_MUL18, I_MUL19, I_MUL20, I_MUL21,
            I_MUL22, I_MUL23, I_MUL24, I_MUL25, I_MUL26, I_MUL27, I_MUL28,
            I_MUL29, I_MUL30, I_MUL31, I_MUL32, I_MUL33, I_MUL34, I_MUL35,
            I_MUL36, I_MUL37, I_MUL38, I_MUL39, I_MUL40, I_MUL41, I_MUL42,
            I_MUL43, I_MUL44, I_MUL45, I_MUL46, I_MUL47, I_MUL48, I_MUL49,
            I_MUL50, I_MUL51, I_MUL52, I_MUL53, I_MUL54, I_MUL55, I_MUL56,
            I_MUL57, I_MUL58, I_MUL59, I_MUL60, I_MUL61, I_MUL62, I_MUL63,
            I_MUL64, I_MUL65, I_MUL66, I_MUL67, I_MUL68, I_MUL69, I_MUL70,
            I_MUL71, I_MUL72, I_MUL73, I_MUL74, I_MUL75, I_MUL76, I_MUL77,
            I_MUL78, I_MUL79, I_MUL80, I_MUL81, I_MUL82, I_MUL83, I_MUL84,
            I_MUL85, I_MUL86, I_MUL87, I_MUL88, I_MUL89, I_MUL90, I_MUL91,
            I_MUL92, I_MUL93, I_MUL94, I_MUL95, I_MUL96, I_MUL97, I_MUL98,
            I_MUL99, I_MUL100, I_MUL101, I_MUL102, I_MUL103, I_MUL104, I_MUL105,
            I_MUL106, I_MUL107, I_MUL108, I_MUL109, I_MUL110, I_MUL111,
            I_MUL112, I_MUL113, I_MUL114, I_MUL115, I_MUL116, I_MUL117,
            I_MUL118, I_MUL119, I_MUL120, I_MUL121, I_MUL122, I_MUL123,
            I_MUL124, I_MUL125, I_MUL126, I_MUL127) == 16256);
    test_assert(
        ARGP_APPLY(2, +,
                   APPLY(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                         12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
                         26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
                         40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
                         54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
                         68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
                         82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
                         96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107,
                         108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118,
                         119, 120, 121, 122, 123, 124, 125, 126, 127)) ==
        16256);
    test_assert(
        ARGP_APPLY(
            2, +,
            APPLY_TOO(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                      13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
                      27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                      41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
                      55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68,
                      69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
                      83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96,
                      97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108,
                      109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
                      120, 121, 122, 123, 124, 125, 126, 127)) == 16256);
    test_assert(
        ARGP_APPLY(
            2, +,
            APPLY2(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                   14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                   29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
                   44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58,
                   59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73,
                   74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88,
                   89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
                   103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114,
                   115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
                   127)) == 16256);
    count = 0;
    APPLY(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
                66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
                82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
                98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
                124, 125, 126, 127);
    test_assert(count == 8128);
    count = 0;
    APPLY_TOO(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                    17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
                    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
                    47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,
                    62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76,
                    77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
                    92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104,
                    105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
                    117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127);
    test_assert(count == 8128);
    count = 0;
    APPLY2(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
                 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
                 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
                 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
                 123, 124, 125, 126, 127);
    test_assert(count == 8128);
    count = 0;
    APPLY(foo, ;
          ,
          ARGP_APPLY(
              2, COMMA, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5, I_MUL6,
              I_MUL7, I_MUL8, I_MUL9, I_MUL10, I_MUL11, I_MUL12, I_MUL13,
              I_MUL14, I_MUL15, I_MUL16, I_MUL17, I_MUL18, I_MUL19, I_MUL20,
              I_MUL21, I_MUL22, I_MUL23, I_MUL24, I_MUL25, I_MUL26, I_MUL27,
              I_MUL28, I_MUL29, I_MUL30, I_MUL31, I_MUL32, I_MUL33, I_MUL34,
              I_MUL35, I_MUL36, I_MUL37, I_MUL38, I_MUL39, I_MUL40, I_MUL41,
              I_MUL42, I_MUL43, I_MUL44, I_MUL45, I_MUL46, I_MUL47, I_MUL48,
              I_MUL49, I_MUL50, I_MUL51, I_MUL52, I_MUL53, I_MUL54, I_MUL55,
              I_MUL56, I_MUL57, I_MUL58, I_MUL59, I_MUL60, I_MUL61, I_MUL62,
              I_MUL63, I_MUL64, I_MUL65, I_MUL66, I_MUL67, I_MUL68, I_MUL69,
              I_MUL70, I_MUL71, I_MUL72, I_MUL73, I_MUL74, I_MUL75, I_MUL76,
              I_MUL77, I_MUL78, I_MUL79, I_MUL80, I_MUL81, I_MUL82, I_MUL83,
              I_MUL84, I_MUL85, I_MUL86, I_MUL87, I_MUL88, I_MUL89, I_MUL90,
              I_MUL91, I_MUL92, I_MUL93, I_MUL94, I_MUL95, I_MUL96, I_MUL97,
              I_MUL98, I_MUL99, I_MUL100, I_MUL101, I_MUL102, I_MUL103,
              I_MUL104, I_MUL105, I_MUL106, I_MUL107, I_MUL108, I_MUL109,
              I_MUL110, I_MUL111, I_MUL112, I_MUL113, I_MUL114, I_MUL115,
              I_MUL116, I_MUL117, I_MUL118, I_MUL119, I_MUL120, I_MUL121,
              I_MUL122, I_MUL123, I_MUL124, I_MUL125, I_MUL126, I_MUL127));
    test_assert(count == 16256);
    count = 0;
    APPLY(dfoo2, ;
          , APPLY_PACKR(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                        29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                        43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
                        57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
                        71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
                        85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
                        99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
                        110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
                        121, 122, 123, 124, 125, 126, 127));
    test_assert(count == -4672);
    count = 0;
    APPLY(dfoo2, ;
          , APPLY_PACKL(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                        29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                        43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
                        57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
                        71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
                        85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
                        99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
                        110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
                        121, 122, 123, 124, 125, 126, 127));
    test_assert(count == 4672);
    test_assert(
        PP_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
                66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
                82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
                98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
                124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136,
                137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
                150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
                163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
                176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188,
                189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201,
                202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
                215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
                228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240,
                241, 242, 243, 244, 245) == 246);
    test_assert(
        CAT(I_,
            NOT_ONE_NARG(
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
                66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
                82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96,
                97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
                110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
                123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
                136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148,
                149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161,
                162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
                175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
                188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
                201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
                214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226,
                227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
                240, 241, 242, 243, 244, 245)) == I_MANY);
    test_assert(
        IS_EMPTY(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
                 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
                 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
                 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
                 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
                 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146,
                 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
                 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170,
                 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182,
                 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194,
                 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206,
                 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218,
                 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
                 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242,
                 243, 244, 245) == 0);
    test_assert(
        strcmp(
            V_TO_STR(CAT(
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
                66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
                82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
                98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
                124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136,
                137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
                150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
                163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
                176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188,
                189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201,
                202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
                215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
                228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240,
                241, 242, 243, 244, 245)),
            "0123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899100101102103104105106107108109110111112113114115116117118119120121122123124125126127128129130131132133134135136137138139140141142143144145146147148149150151152153154155156157158159160161162163164165166167168169170171172173174175176177178179180181182183184185186187188189190191192193194195196197198199200201202203204205206207208209210211212213214215216217218219220221222223224225226227228229230231232233234235236237238239240241242243244245") ==
        0);
    test_assert(
        APPLY_RECURSE(
            I_TEST_MIN, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
            50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66,
            67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83,
            84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,
            101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
            114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
            127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
            140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152,
            153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
            166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178,
            179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
            192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204,
            205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217,
            218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
            231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
            244, 245) == 0);
    test_assert(APPLY_RECURSE(
                    I_PLUS, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                    30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
                    45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
                    60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74,
                    75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
                    90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103,
                    104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115,
                    116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
                    128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
                    140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151,
                    152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163,
                    164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
                    176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
                    188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
                    200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211,
                    212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
                    224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235,
                    236, 237, 238, 239, 240, 241, 242, 243, 244, 245) == 30135);
    test_assert(
        APPLY(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
              16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
              32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
              48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
              64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
              80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
              96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
              110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
              123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
              136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148,
              149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161,
              162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
              175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
              188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
              201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
              214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226,
              227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
              240, 241, 242, 243, 244, 245) == 60270);
    test_assert(
        APPLY_TOO(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                  15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                  30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
                  45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
                  60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74,
                  75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
                  90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103,
                  104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115,
                  116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
                  128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
                  140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151,
                  152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163,
                  164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
                  176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
                  188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
                  200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211,
                  212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
                  224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235,
                  236, 237, 238, 239, 240, 241, 242, 243, 244, 245) == 60270);
    test_assert(
        APPLY2(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
               16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
               32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
               48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
               64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
               80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
               96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
               110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
               123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
               136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148,
               149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161,
               162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
               175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
               188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
               201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
               214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226,
               227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
               240, 241, 242, 243, 244, 245) == 60270);
    test_assert(
        ARGP_APPLY(
            2, +, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5, I_MUL6,
            I_MUL7, I_MUL8, I_MUL9, I_MUL10, I_MUL11, I_MUL12, I_MUL13, I_MUL14,
            I_MUL15, I_MUL16, I_MUL17, I_MUL18, I_MUL19, I_MUL20, I_MUL21,
            I_MUL22, I_MUL23, I_MUL24, I_MUL25, I_MUL26, I_MUL27, I_MUL28,
            I_MUL29, I_MUL30, I_MUL31, I_MUL32, I_MUL33, I_MUL34, I_MUL35,
            I_MUL36, I_MUL37, I_MUL38, I_MUL39, I_MUL40, I_MUL41, I_MUL42,
            I_MUL43, I_MUL44, I_MUL45, I_MUL46, I_MUL47, I_MUL48, I_MUL49,
            I_MUL50, I_MUL51, I_MUL52, I_MUL53, I_MUL54, I_MUL55, I_MUL56,
            I_MUL57, I_MUL58, I_MUL59, I_MUL60, I_MUL61, I_MUL62, I_MUL63,
            I_MUL64, I_MUL65, I_MUL66, I_MUL67, I_MUL68, I_MUL69, I_MUL70,
            I_MUL71, I_MUL72, I_MUL73, I_MUL74, I_MUL75, I_MUL76, I_MUL77,
            I_MUL78, I_MUL79, I_MUL80, I_MUL81, I_MUL82, I_MUL83, I_MUL84,
            I_MUL85, I_MUL86, I_MUL87, I_MUL88, I_MUL89, I_MUL90, I_MUL91,
            I_MUL92, I_MUL93, I_MUL94, I_MUL95, I_MUL96, I_MUL97, I_MUL98,
            I_MUL99, I_MUL100, I_MUL101, I_MUL102, I_MUL103, I_MUL104, I_MUL105,
            I_MUL106, I_MUL107, I_MUL108, I_MUL109, I_MUL110, I_MUL111,
            I_MUL112, I_MUL113, I_MUL114, I_MUL115, I_MUL116, I_MUL117,
            I_MUL118, I_MUL119, I_MUL120, I_MUL121, I_MUL122, I_MUL123,
            I_MUL124, I_MUL125, I_MUL126, I_MUL127, I_MUL128, I_MUL129,
            I_MUL130, I_MUL131, I_MUL132, I_MUL133, I_MUL134, I_MUL135,
            I_MUL136, I_MUL137, I_MUL138, I_MUL139, I_MUL140, I_MUL141,
            I_MUL142, I_MUL143, I_MUL144, I_MUL145, I_MUL146, I_MUL147,
            I_MUL148, I_MUL149, I_MUL150, I_MUL151, I_MUL152, I_MUL153,
            I_MUL154, I_MUL155, I_MUL156, I_MUL157, I_MUL158, I_MUL159,
            I_MUL160, I_MUL161, I_MUL162, I_MUL163, I_MUL164, I_MUL165,
            I_MUL166, I_MUL167, I_MUL168, I_MUL169, I_MUL170, I_MUL171,
            I_MUL172, I_MUL173, I_MUL174, I_MUL175, I_MUL176, I_MUL177,
            I_MUL178, I_MUL179, I_MUL180, I_MUL181, I_MUL182, I_MUL183,
            I_MUL184, I_MUL185, I_MUL186, I_MUL187, I_MUL188, I_MUL189,
            I_MUL190, I_MUL191, I_MUL192, I_MUL193, I_MUL194, I_MUL195,
            I_MUL196, I_MUL197, I_MUL198, I_MUL199, I_MUL200, I_MUL201,
            I_MUL202, I_MUL203, I_MUL204, I_MUL205, I_MUL206, I_MUL207,
            I_MUL208, I_MUL209, I_MUL210, I_MUL211, I_MUL212, I_MUL213,
            I_MUL214, I_MUL215, I_MUL216, I_MUL217, I_MUL218, I_MUL219,
            I_MUL220, I_MUL221, I_MUL222, I_MUL223, I_MUL224, I_MUL225,
            I_MUL226, I_MUL227, I_MUL228, I_MUL229, I_MUL230, I_MUL231,
            I_MUL232, I_MUL233, I_MUL234, I_MUL235, I_MUL236, I_MUL237,
            I_MUL238, I_MUL239, I_MUL240, I_MUL241, I_MUL242, I_MUL243,
            I_MUL244, I_MUL245) == 60270);
    test_assert(
        ARGP_APPLY(
            2, +,
            APPLY(
                I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
                47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
                63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
                79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94,
                95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108,
                109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
                122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
                135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147,
                148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
                161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173,
                174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
                187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
                200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212,
                213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225,
                226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238,
                239, 240, 241, 242, 243, 244, 245)) == 60270);
    test_assert(
        ARGP_APPLY(
            2, +,
            APPLY_TOO(
                I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
                47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
                63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
                79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94,
                95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108,
                109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
                122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
                135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147,
                148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
                161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173,
                174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
                187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
                200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212,
                213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225,
                226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238,
                239, 240, 241, 242, 243, 244, 245)) == 60270);
    test_assert(
        ARGP_APPLY(
            2, +,
            APPLY2(
                I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
                47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
                63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
                79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94,
                95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108,
                109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
                122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
                135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147,
                148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
                161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173,
                174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
                187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
                200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212,
                213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225,
                226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238,
                239, 240, 241, 242, 243, 244, 245)) == 60270);
    count = 0;
    APPLY(foo, ;, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
                66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
                82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
                98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
                124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136,
                137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
                150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
                163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
                176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188,
                189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201,
                202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
                215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
                228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240,
                241, 242, 243, 244, 245);
    test_assert(count == 30135);
    count = 0;
    APPLY_TOO(
        foo, ;
        , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
        38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55,
        56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73,
        74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
        92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107,
        108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
        122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
        136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
        150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163,
        164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177,
        178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
        192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205,
        206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
        220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233,
        234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245);
    test_assert(count == 30135);
    count = 0;
    APPLY2(foo, ;
           , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
           19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
           36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
           53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
           70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86,
           87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
           103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
           117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130,
           131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144,
           145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
           159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172,
           173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
           187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
           201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
           215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228,
           229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242,
           243, 244, 245);
    test_assert(count == 30135);
    count = 0;
    APPLY(foo, ;
          , ARGP_APPLY(
                2, COMMA, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5,
                I_MUL6, I_MUL7, I_MUL8, I_MUL9, I_MUL10, I_MUL11, I_MUL12,
                I_MUL13, I_MUL14, I_MUL15, I_MUL16, I_MUL17, I_MUL18, I_MUL19,
                I_MUL20, I_MUL21, I_MUL22, I_MUL23, I_MUL24, I_MUL25, I_MUL26,
                I_MUL27, I_MUL28, I_MUL29, I_MUL30, I_MUL31, I_MUL32, I_MUL33,
                I_MUL34, I_MUL35, I_MUL36, I_MUL37, I_MUL38, I_MUL39, I_MUL40,
                I_MUL41, I_MUL42, I_MUL43, I_MUL44, I_MUL45, I_MUL46, I_MUL47,
                I_MUL48, I_MUL49, I_MUL50, I_MUL51, I_MUL52, I_MUL53, I_MUL54,
                I_MUL55, I_MUL56, I_MUL57, I_MUL58, I_MUL59, I_MUL60, I_MUL61,
                I_MUL62, I_MUL63, I_MUL64, I_MUL65, I_MUL66, I_MUL67, I_MUL68,
                I_MUL69, I_MUL70, I_MUL71, I_MUL72, I_MUL73, I_MUL74, I_MUL75,
                I_MUL76, I_MUL77, I_MUL78, I_MUL79, I_MUL80, I_MUL81, I_MUL82,
                I_MUL83, I_MUL84, I_MUL85, I_MUL86, I_MUL87, I_MUL88, I_MUL89,
                I_MUL90, I_MUL91, I_MUL92, I_MUL93, I_MUL94, I_MUL95, I_MUL96,
                I_MUL97, I_MUL98, I_MUL99, I_MUL100, I_MUL101, I_MUL102,
                I_MUL103, I_MUL104, I_MUL105, I_MUL106, I_MUL107, I_MUL108,
                I_MUL109, I_MUL110, I_MUL111, I_MUL112, I_MUL113, I_MUL114,
                I_MUL115, I_MUL116, I_MUL117, I_MUL118, I_MUL119, I_MUL120,
                I_MUL121, I_MUL122, I_MUL123, I_MUL124, I_MUL125, I_MUL126,
                I_MUL127, I_MUL128, I_MUL129, I_MUL130, I_MUL131, I_MUL132,
                I_MUL133, I_MUL134, I_MUL135, I_MUL136, I_MUL137, I_MUL138,
                I_MUL139, I_MUL140, I_MUL141, I_MUL142, I_MUL143, I_MUL144,
                I_MUL145, I_MUL146, I_MUL147, I_MUL148, I_MUL149, I_MUL150,
                I_MUL151, I_MUL152, I_MUL153, I_MUL154, I_MUL155, I_MUL156,
                I_MUL157, I_MUL158, I_MUL159, I_MUL160, I_MUL161, I_MUL162,
                I_MUL163, I_MUL164, I_MUL165, I_MUL166, I_MUL167, I_MUL168,
                I_MUL169, I_MUL170, I_MUL171, I_MUL172, I_MUL173, I_MUL174,
                I_MUL175, I_MUL176, I_MUL177, I_MUL178, I_MUL179, I_MUL180,
                I_MUL181, I_MUL182, I_MUL183, I_MUL184, I_MUL185, I_MUL186,
                I_MUL187, I_MUL188, I_MUL189, I_MUL190, I_MUL191, I_MUL192,
                I_MUL193, I_MUL194, I_MUL195, I_MUL196, I_MUL197, I_MUL198,
                I_MUL199, I_MUL200, I_MUL201, I_MUL202, I_MUL203, I_MUL204,
                I_MUL205, I_MUL206, I_MUL207, I_MUL208, I_MUL209, I_MUL210,
                I_MUL211, I_MUL212, I_MUL213, I_MUL214, I_MUL215, I_MUL216,
                I_MUL217, I_MUL218, I_MUL219, I_MUL220, I_MUL221, I_MUL222,
                I_MUL223, I_MUL224, I_MUL225, I_MUL226, I_MUL227, I_MUL228,
                I_MUL229, I_MUL230, I_MUL231, I_MUL232, I_MUL233, I_MUL234,
                I_MUL235, I_MUL236, I_MUL237, I_MUL238, I_MUL239, I_MUL240,
                I_MUL241, I_MUL242, I_MUL243, I_MUL244, I_MUL245));
    test_assert(count == 60270);
    count = 0;
    APPLY(dfoo2, ;
          , APPLY_PACKR(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                        29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                        43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
                        57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
                        71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
                        85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
                        99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
                        110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
                        121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
                        132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
                        143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153,
                        154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
                        165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
                        176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
                        187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197,
                        198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208,
                        209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
                        220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
                        231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241,
                        242, 243, 244, 245));
    test_assert(count == 5535);
    count = 0;
    APPLY(dfoo2, ;
          , APPLY_PACKL(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                        29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                        43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
                        57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
                        71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
                        85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
                        99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
                        110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
                        121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
                        132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
                        143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153,
                        154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
                        165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
                        176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
                        187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197,
                        198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208,
                        209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
                        220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
                        231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241,
                        242, 243, 244, 245));
    test_assert(count == -5535);
    test_assert(
        PP_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
                66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
                82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
                98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
                124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136,
                137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
                150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
                163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
                176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188,
                189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201,
                202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
                215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
                228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240,
                241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
                253) == 254);
    test_assert(
        CAT(I_,
            NOT_ONE_NARG(
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
                66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
                82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96,
                97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
                110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
                123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
                136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148,
                149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161,
                162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
                175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
                188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
                201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
                214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226,
                227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
                240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
                253)) == I_MANY);
    test_assert(
        IS_EMPTY(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
                 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
                 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
                 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
                 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
                 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146,
                 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
                 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170,
                 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182,
                 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194,
                 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206,
                 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218,
                 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
                 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242,
                 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253) == 0);
    test_assert(
        strcmp(
            V_TO_STR(CAT(
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
                66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
                82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
                98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
                124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136,
                137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
                150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
                163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
                176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188,
                189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201,
                202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
                215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
                228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240,
                241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
                253)),
            "0123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899100101102103104105106107108109110111112113114115116117118119120121122123124125126127128129130131132133134135136137138139140141142143144145146147148149150151152153154155156157158159160161162163164165166167168169170171172173174175176177178179180181182183184185186187188189190191192193194195196197198199200201202203204205206207208209210211212213214215216217218219220221222223224225226227228229230231232233234235236237238239240241242243244245246247248249250251252253") ==
        0);
    test_assert(
        APPLY_RECURSE(
            I_TEST_MIN, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
            50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66,
            67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83,
            84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,
            101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
            114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
            127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
            140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152,
            153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
            166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178,
            179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
            192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204,
            205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217,
            218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
            231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
            244, 245, 246, 247, 248, 249, 250, 251, 252, 253) == 0);
    test_assert(
        APPLY_RECURSE(
            I_PLUS, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
            17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
            34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
            51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
            68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
            85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,
            101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
            114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
            127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
            140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152,
            153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
            166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178,
            179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
            192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204,
            205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217,
            218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
            231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
            244, 245, 246, 247, 248, 249, 250, 251, 252, 253) == 32131);
    test_assert(
        APPLY(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
              16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
              32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
              48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
              64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
              80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
              96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
              110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
              123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
              136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148,
              149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161,
              162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
              175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
              188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
              201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
              214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226,
              227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
              240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
              253) == 64262);
    test_assert(
        APPLY_TOO(
            I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
            17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
            34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
            51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
            68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
            85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,
            101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
            114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
            127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
            140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152,
            153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
            166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178,
            179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
            192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204,
            205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217,
            218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
            231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
            244, 245, 246, 247, 248, 249, 250, 251, 252, 253) == 64262);
    test_assert(
        APPLY2(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
               16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
               32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
               48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
               64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
               80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
               96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
               110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
               123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
               136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148,
               149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161,
               162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
               175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
               188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
               201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
               214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226,
               227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
               240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
               253) == 64262);
    test_assert(
        ARGP_APPLY(
            2, +, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5, I_MUL6,
            I_MUL7, I_MUL8, I_MUL9, I_MUL10, I_MUL11, I_MUL12, I_MUL13, I_MUL14,
            I_MUL15, I_MUL16, I_MUL17, I_MUL18, I_MUL19, I_MUL20, I_MUL21,
            I_MUL22, I_MUL23, I_MUL24, I_MUL25, I_MUL26, I_MUL27, I_MUL28,
            I_MUL29, I_MUL30, I_MUL31, I_MUL32, I_MUL33, I_MUL34, I_MUL35,
            I_MUL36, I_MUL37, I_MUL38, I_MUL39, I_MUL40, I_MUL41, I_MUL42,
            I_MUL43, I_MUL44, I_MUL45, I_MUL46, I_MUL47, I_MUL48, I_MUL49,
            I_MUL50, I_MUL51, I_MUL52, I_MUL53, I_MUL54, I_MUL55, I_MUL56,
            I_MUL57, I_MUL58, I_MUL59, I_MUL60, I_MUL61, I_MUL62, I_MUL63,
            I_MUL64, I_MUL65, I_MUL66, I_MUL67, I_MUL68, I_MUL69, I_MUL70,
            I_MUL71, I_MUL72, I_MUL73, I_MUL74, I_MUL75, I_MUL76, I_MUL77,
            I_MUL78, I_MUL79, I_MUL80, I_MUL81, I_MUL82, I_MUL83, I_MUL84,
            I_MUL85, I_MUL86, I_MUL87, I_MUL88, I_MUL89, I_MUL90, I_MUL91,
            I_MUL92, I_MUL93, I_MUL94, I_MUL95, I_MUL96, I_MUL97, I_MUL98,
            I_MUL99, I_MUL100, I_MUL101, I_MUL102, I_MUL103, I_MUL104, I_MUL105,
            I_MUL106, I_MUL107, I_MUL108, I_MUL109, I_MUL110, I_MUL111,
            I_MUL112, I_MUL113, I_MUL114, I_MUL115, I_MUL116, I_MUL117,
            I_MUL118, I_MUL119, I_MUL120, I_MUL121, I_MUL122, I_MUL123,
            I_MUL124, I_MUL125, I_MUL126, I_MUL127, I_MUL128, I_MUL129,
            I_MUL130, I_MUL131, I_MUL132, I_MUL133, I_MUL134, I_MUL135,
            I_MUL136, I_MUL137, I_MUL138, I_MUL139, I_MUL140, I_MUL141,
            I_MUL142, I_MUL143, I_MUL144, I_MUL145, I_MUL146, I_MUL147,
            I_MUL148, I_MUL149, I_MUL150, I_MUL151, I_MUL152, I_MUL153,
            I_MUL154, I_MUL155, I_MUL156, I_MUL157, I_MUL158, I_MUL159,
            I_MUL160, I_MUL161, I_MUL162, I_MUL163, I_MUL164, I_MUL165,
            I_MUL166, I_MUL167, I_MUL168, I_MUL169, I_MUL170, I_MUL171,
            I_MUL172, I_MUL173, I_MUL174, I_MUL175, I_MUL176, I_MUL177,
            I_MUL178, I_MUL179, I_MUL180, I_MUL181, I_MUL182, I_MUL183,
            I_MUL184, I_MUL185, I_MUL186, I_MUL187, I_MUL188, I_MUL189,
            I_MUL190, I_MUL191, I_MUL192, I_MUL193, I_MUL194, I_MUL195,
            I_MUL196, I_MUL197, I_MUL198, I_MUL199, I_MUL200, I_MUL201,
            I_MUL202, I_MUL203, I_MUL204, I_MUL205, I_MUL206, I_MUL207,
            I_MUL208, I_MUL209, I_MUL210, I_MUL211, I_MUL212, I_MUL213,
            I_MUL214, I_MUL215, I_MUL216, I_MUL217, I_MUL218, I_MUL219,
            I_MUL220, I_MUL221, I_MUL222, I_MUL223, I_MUL224, I_MUL225,
            I_MUL226, I_MUL227, I_MUL228, I_MUL229, I_MUL230, I_MUL231,
            I_MUL232, I_MUL233, I_MUL234, I_MUL235, I_MUL236, I_MUL237,
            I_MUL238, I_MUL239, I_MUL240, I_MUL241, I_MUL242, I_MUL243,
            I_MUL244, I_MUL245, I_MUL246, I_MUL247, I_MUL248, I_MUL249,
            I_MUL250, I_MUL251, I_MUL252, I_MUL253) == 64262);
    test_assert(
        ARGP_APPLY(
            2, +,
            APPLY(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                  14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                  29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
                  44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58,
                  59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73,
                  74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88,
                  89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
                  103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114,
                  115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
                  127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138,
                  139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
                  151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
                  163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
                  175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
                  187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198,
                  199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210,
                  211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222,
                  223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234,
                  235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246,
                  247, 248, 249, 250, 251, 252, 253)) == 64262);
    test_assert(
        ARGP_APPLY(
            2, +,
            APPLY_TOO(
                I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
                47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
                63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
                79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94,
                95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108,
                109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
                122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
                135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147,
                148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
                161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173,
                174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
                187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
                200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212,
                213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225,
                226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238,
                239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251,
                252, 253)) == 64262);
    test_assert(
        ARGP_APPLY(
            2, +,
            APPLY2(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                   14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                   29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
                   44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58,
                   59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73,
                   74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88,
                   89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
                   103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114,
                   115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
                   127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138,
                   139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
                   151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
                   163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
                   175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
                   187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198,
                   199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210,
                   211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222,
                   223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234,
                   235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246,
                   247, 248, 249, 250, 251, 252, 253)) == 64262);
    count = 0;
    APPLY(foo, ;
          , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
          19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
          36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
          53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
          70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86,
          87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
          103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
          117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130,
          131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144,
          145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
          159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172,
          173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
          187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
          201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
          215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228,
          229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242,
          243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253);
    test_assert(count == 32131);
    count = 0;
    APPLY_TOO(foo, ;
              , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
              18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
              34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
              50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
              66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
              82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
              98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
              111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
              124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136,
              137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
              150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
              163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
              176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188,
              189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201,
              202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
              215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
              228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240,
              241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253);
    test_assert(count == 32131);
    count = 0;
    APPLY2(foo, ;
           , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
           19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
           36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
           53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
           70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86,
           87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
           103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
           117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130,
           131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144,
           145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
           159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172,
           173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
           187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
           201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
           215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228,
           229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242,
           243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253);
    test_assert(count == 32131);
    count = 0;
    APPLY(foo, ;
          ,
          ARGP_APPLY(
              2, COMMA, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5, I_MUL6,
              I_MUL7, I_MUL8, I_MUL9, I_MUL10, I_MUL11, I_MUL12, I_MUL13,
              I_MUL14, I_MUL15, I_MUL16, I_MUL17, I_MUL18, I_MUL19, I_MUL20,
              I_MUL21, I_MUL22, I_MUL23, I_MUL24, I_MUL25, I_MUL26, I_MUL27,
              I_MUL28, I_MUL29, I_MUL30, I_MUL31, I_MUL32, I_MUL33, I_MUL34,
              I_MUL35, I_MUL36, I_MUL37, I_MUL38, I_MUL39, I_MUL40, I_MUL41,
              I_MUL42, I_MUL43, I_MUL44, I_MUL45, I_MUL46, I_MUL47, I_MUL48,
              I_MUL49, I_MUL50, I_MUL51, I_MUL52, I_MUL53, I_MUL54, I_MUL55,
              I_MUL56, I_MUL57, I_MUL58, I_MUL59, I_MUL60, I_MUL61, I_MUL62,
              I_MUL63, I_MUL64, I_MUL65, I_MUL66, I_MUL67, I_MUL68, I_MUL69,
              I_MUL70, I_MUL71, I_MUL72, I_MUL73, I_MUL74, I_MUL75, I_MUL76,
              I_MUL77, I_MUL78, I_MUL79, I_MUL80, I_MUL81, I_MUL82, I_MUL83,
              I_MUL84, I_MUL85, I_MUL86, I_MUL87, I_MUL88, I_MUL89, I_MUL90,
              I_MUL91, I_MUL92, I_MUL93, I_MUL94, I_MUL95, I_MUL96, I_MUL97,
              I_MUL98, I_MUL99, I_MUL100, I_MUL101, I_MUL102, I_MUL103,
              I_MUL104, I_MUL105, I_MUL106, I_MUL107, I_MUL108, I_MUL109,
              I_MUL110, I_MUL111, I_MUL112, I_MUL113, I_MUL114, I_MUL115,
              I_MUL116, I_MUL117, I_MUL118, I_MUL119, I_MUL120, I_MUL121,
              I_MUL122, I_MUL123, I_MUL124, I_MUL125, I_MUL126, I_MUL127,
              I_MUL128, I_MUL129, I_MUL130, I_MUL131, I_MUL132, I_MUL133,
              I_MUL134, I_MUL135, I_MUL136, I_MUL137, I_MUL138, I_MUL139,
              I_MUL140, I_MUL141, I_MUL142, I_MUL143, I_MUL144, I_MUL145,
              I_MUL146, I_MUL147, I_MUL148, I_MUL149, I_MUL150, I_MUL151,
              I_MUL152, I_MUL153, I_MUL154, I_MUL155, I_MUL156, I_MUL157,
              I_MUL158, I_MUL159, I_MUL160, I_MUL161, I_MUL162, I_MUL163,
              I_MUL164, I_MUL165, I_MUL166, I_MUL167, I_MUL168, I_MUL169,
              I_MUL170, I_MUL171, I_MUL172, I_MUL173, I_MUL174, I_MUL175,
              I_MUL176, I_MUL177, I_MUL178, I_MUL179, I_MUL180, I_MUL181,
              I_MUL182, I_MUL183, I_MUL184, I_MUL185, I_MUL186, I_MUL187,
              I_MUL188, I_MUL189, I_MUL190, I_MUL191, I_MUL192, I_MUL193,
              I_MUL194, I_MUL195, I_MUL196, I_MUL197, I_MUL198, I_MUL199,
              I_MUL200, I_MUL201, I_MUL202, I_MUL203, I_MUL204, I_MUL205,
              I_MUL206, I_MUL207, I_MUL208, I_MUL209, I_MUL210, I_MUL211,
              I_MUL212, I_MUL213, I_MUL214, I_MUL215, I_MUL216, I_MUL217,
              I_MUL218, I_MUL219, I_MUL220, I_MUL221, I_MUL222, I_MUL223,
              I_MUL224, I_MUL225, I_MUL226, I_MUL227, I_MUL228, I_MUL229,
              I_MUL230, I_MUL231, I_MUL232, I_MUL233, I_MUL234, I_MUL235,
              I_MUL236, I_MUL237, I_MUL238, I_MUL239, I_MUL240, I_MUL241,
              I_MUL242, I_MUL243, I_MUL244, I_MUL245, I_MUL246, I_MUL247,
              I_MUL248, I_MUL249, I_MUL250, I_MUL251, I_MUL252, I_MUL253));
    test_assert(count == 64262);
    count = 0;
    APPLY(dfoo2, ;
          ,
          APPLY_PACKR(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                      15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                      29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                      43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
                      57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
                      71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
                      85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
                      99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                      111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
                      122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132,
                      133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
                      144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154,
                      155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
                      166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176,
                      177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
                      188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198,
                      199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
                      210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220,
                      221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231,
                      232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242,
                      243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253));
    test_assert(count == 6731);
    count = 0;
    APPLY(dfoo2, ;
          ,
          APPLY_PACKL(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                      15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                      29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                      43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
                      57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
                      71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
                      85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
                      99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                      111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
                      122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132,
                      133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
                      144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154,
                      155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
                      166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176,
                      177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
                      188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198,
                      199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
                      210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220,
                      221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231,
                      232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242,
                      243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253));
    test_assert(count == -6731);
    test_assert(
        PP_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
                66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
                82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
                98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
                124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136,
                137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
                150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
                163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
                176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188,
                189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201,
                202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
                215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
                228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240,
                241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253,
                254) == 255);
    test_assert(
        CAT(I_,
            NOT_ONE_NARG(
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
                66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
                82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96,
                97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
                110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
                123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
                136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148,
                149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161,
                162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
                175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
                188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
                201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
                214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226,
                227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
                240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
                253, 254)) == I_MANY);
    test_assert(
        IS_EMPTY(
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
            19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
            36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
            53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
            70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86,
            87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
            103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115,
            116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128,
            129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141,
            142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154,
            155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167,
            168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180,
            181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193,
            194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206,
            207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
            220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232,
            233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245,
            246, 247, 248, 249, 250, 251, 252, 253, 254) == 0);
    test_assert(
        strcmp(
            V_TO_STR(CAT(
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
                66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
                82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
                98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
                124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136,
                137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
                150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
                163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
                176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188,
                189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201,
                202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
                215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
                228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240,
                241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253,
                254)),
            "0123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899100101102103104105106107108109110111112113114115116117118119120121122123124125126127128129130131132133134135136137138139140141142143144145146147148149150151152153154155156157158159160161162163164165166167168169170171172173174175176177178179180181182183184185186187188189190191192193194195196197198199200201202203204205206207208209210211212213214215216217218219220221222223224225226227228229230231232233234235236237238239240241242243244245246247248249250251252253254") ==
        0);
    test_assert(
        APPLY_RECURSE(
            I_TEST_MIN, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
            50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66,
            67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83,
            84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,
            101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
            114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
            127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
            140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152,
            153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
            166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178,
            179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
            192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204,
            205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217,
            218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
            231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
            244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254) == 0);
    test_assert(
        APPLY_RECURSE(
            I_PLUS, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
            17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
            34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
            51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
            68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
            85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,
            101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
            114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
            127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
            140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152,
            153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
            166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178,
            179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
            192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204,
            205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217,
            218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
            231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
            244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254) == 32385);
    test_assert(
        APPLY(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
              16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
              32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
              48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
              64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
              80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
              96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
              110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
              123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
              136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148,
              149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161,
              162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
              175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
              188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
              201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
              214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226,
              227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
              240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
              253, 254) == 64770);
    test_assert(
        APPLY_TOO(
            I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
            17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
            34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
            51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
            68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
            85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,
            101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
            114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
            127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
            140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152,
            153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
            166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178,
            179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
            192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204,
            205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217,
            218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
            231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
            244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254) == 64770);
    test_assert(
        APPLY2(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
               16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
               32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
               48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
               64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
               80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
               96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
               110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
               123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
               136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148,
               149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161,
               162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
               175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
               188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
               201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
               214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226,
               227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
               240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
               253, 254) == 64770);
    test_assert(
        ARGP_APPLY(
            2, +, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5, I_MUL6,
            I_MUL7, I_MUL8, I_MUL9, I_MUL10, I_MUL11, I_MUL12, I_MUL13, I_MUL14,
            I_MUL15, I_MUL16, I_MUL17, I_MUL18, I_MUL19, I_MUL20, I_MUL21,
            I_MUL22, I_MUL23, I_MUL24, I_MUL25, I_MUL26, I_MUL27, I_MUL28,
            I_MUL29, I_MUL30, I_MUL31, I_MUL32, I_MUL33, I_MUL34, I_MUL35,
            I_MUL36, I_MUL37, I_MUL38, I_MUL39, I_MUL40, I_MUL41, I_MUL42,
            I_MUL43, I_MUL44, I_MUL45, I_MUL46, I_MUL47, I_MUL48, I_MUL49,
            I_MUL50, I_MUL51, I_MUL52, I_MUL53, I_MUL54, I_MUL55, I_MUL56,
            I_MUL57, I_MUL58, I_MUL59, I_MUL60, I_MUL61, I_MUL62, I_MUL63,
            I_MUL64, I_MUL65, I_MUL66, I_MUL67, I_MUL68, I_MUL69, I_MUL70,
            I_MUL71, I_MUL72, I_MUL73, I_MUL74, I_MUL75, I_MUL76, I_MUL77,
            I_MUL78, I_MUL79, I_MUL80, I_MUL81, I_MUL82, I_MUL83, I_MUL84,
            I_MUL85, I_MUL86, I_MUL87, I_MUL88, I_MUL89, I_MUL90, I_MUL91,
            I_MUL92, I_MUL93, I_MUL94, I_MUL95, I_MUL96, I_MUL97, I_MUL98,
            I_MUL99, I_MUL100, I_MUL101, I_MUL102, I_MUL103, I_MUL104, I_MUL105,
            I_MUL106, I_MUL107, I_MUL108, I_MUL109, I_MUL110, I_MUL111,
            I_MUL112, I_MUL113, I_MUL114, I_MUL115, I_MUL116, I_MUL117,
            I_MUL118, I_MUL119, I_MUL120, I_MUL121, I_MUL122, I_MUL123,
            I_MUL124, I_MUL125, I_MUL126, I_MUL127, I_MUL128, I_MUL129,
            I_MUL130, I_MUL131, I_MUL132, I_MUL133, I_MUL134, I_MUL135,
            I_MUL136, I_MUL137, I_MUL138, I_MUL139, I_MUL140, I_MUL141,
            I_MUL142, I_MUL143, I_MUL144, I_MUL145, I_MUL146, I_MUL147,
            I_MUL148, I_MUL149, I_MUL150, I_MUL151, I_MUL152, I_MUL153,
            I_MUL154, I_MUL155, I_MUL156, I_MUL157, I_MUL158, I_MUL159,
            I_MUL160, I_MUL161, I_MUL162, I_MUL163, I_MUL164, I_MUL165,
            I_MUL166, I_MUL167, I_MUL168, I_MUL169, I_MUL170, I_MUL171,
            I_MUL172, I_MUL173, I_MUL174, I_MUL175, I_MUL176, I_MUL177,
            I_MUL178, I_MUL179, I_MUL180, I_MUL181, I_MUL182, I_MUL183,
            I_MUL184, I_MUL185, I_MUL186, I_MUL187, I_MUL188, I_MUL189,
            I_MUL190, I_MUL191, I_MUL192, I_MUL193, I_MUL194, I_MUL195,
            I_MUL196, I_MUL197, I_MUL198, I_MUL199, I_MUL200, I_MUL201,
            I_MUL202, I_MUL203, I_MUL204, I_MUL205, I_MUL206, I_MUL207,
            I_MUL208, I_MUL209, I_MUL210, I_MUL211, I_MUL212, I_MUL213,
            I_MUL214, I_MUL215, I_MUL216, I_MUL217, I_MUL218, I_MUL219,
            I_MUL220, I_MUL221, I_MUL222, I_MUL223, I_MUL224, I_MUL225,
            I_MUL226, I_MUL227, I_MUL228, I_MUL229, I_MUL230, I_MUL231,
            I_MUL232, I_MUL233, I_MUL234, I_MUL235, I_MUL236, I_MUL237,
            I_MUL238, I_MUL239, I_MUL240, I_MUL241, I_MUL242, I_MUL243,
            I_MUL244, I_MUL245, I_MUL246, I_MUL247, I_MUL248, I_MUL249,
            I_MUL250, I_MUL251, I_MUL252, I_MUL253, I_MUL254) == 64770);
    test_assert(
        ARGP_APPLY(
            2, +,
            APPLY(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                  14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                  29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
                  44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58,
                  59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73,
                  74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88,
                  89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
                  103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114,
                  115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
                  127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138,
                  139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
                  151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
                  163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
                  175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
                  187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198,
                  199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210,
                  211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222,
                  223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234,
                  235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246,
                  247, 248, 249, 250, 251, 252, 253, 254)) == 64770);
    test_assert(
        ARGP_APPLY(
            2, +,
            APPLY_TOO(
                I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
                47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
                63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
                79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94,
                95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108,
                109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
                122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
                135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147,
                148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
                161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173,
                174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
                187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
                200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212,
                213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225,
                226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238,
                239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251,
                252, 253, 254)) == 64770);
    test_assert(
        ARGP_APPLY(
            2, +,
            APPLY2(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                   14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                   29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
                   44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58,
                   59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73,
                   74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88,
                   89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
                   103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114,
                   115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
                   127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138,
                   139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
                   151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
                   163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
                   175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
                   187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198,
                   199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210,
                   211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222,
                   223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234,
                   235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246,
                   247, 248, 249, 250, 251, 252, 253, 254)) == 64770);
    count = 0;
    APPLY(foo, ;
          , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
          19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
          36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
          53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
          70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86,
          87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
          103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
          117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130,
          131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144,
          145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
          159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172,
          173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
          187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
          201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
          215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228,
          229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242,
          243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254);
    test_assert(count == 32385);
    count = 0;
    APPLY_TOO(
        foo, ;
        , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
        38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55,
        56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73,
        74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
        92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107,
        108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
        122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
        136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
        150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163,
        164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177,
        178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
        192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205,
        206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
        220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233,
        234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247,
        248, 249, 250, 251, 252, 253, 254);
    test_assert(count == 32385);
    count = 0;
    APPLY2(foo, ;
           , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
           19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
           36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
           53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
           70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86,
           87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
           103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
           117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130,
           131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144,
           145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
           159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172,
           173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
           187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
           201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
           215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228,
           229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242,
           243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254);
    test_assert(count == 32385);
    count = 0;
    APPLY(foo, ;
          , ARGP_APPLY(
                2, COMMA, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5,
                I_MUL6, I_MUL7, I_MUL8, I_MUL9, I_MUL10, I_MUL11, I_MUL12,
                I_MUL13, I_MUL14, I_MUL15, I_MUL16, I_MUL17, I_MUL18, I_MUL19,
                I_MUL20, I_MUL21, I_MUL22, I_MUL23, I_MUL24, I_MUL25, I_MUL26,
                I_MUL27, I_MUL28, I_MUL29, I_MUL30, I_MUL31, I_MUL32, I_MUL33,
                I_MUL34, I_MUL35, I_MUL36, I_MUL37, I_MUL38, I_MUL39, I_MUL40,
                I_MUL41, I_MUL42, I_MUL43, I_MUL44, I_MUL45, I_MUL46, I_MUL47,
                I_MUL48, I_MUL49, I_MUL50, I_MUL51, I_MUL52, I_MUL53, I_MUL54,
                I_MUL55, I_MUL56, I_MUL57, I_MUL58, I_MUL59, I_MUL60, I_MUL61,
                I_MUL62, I_MUL63, I_MUL64, I_MUL65, I_MUL66, I_MUL67, I_MUL68,
                I_MUL69, I_MUL70, I_MUL71, I_MUL72, I_MUL73, I_MUL74, I_MUL75,
                I_MUL76, I_MUL77, I_MUL78, I_MUL79, I_MUL80, I_MUL81, I_MUL82,
                I_MUL83, I_MUL84, I_MUL85, I_MUL86, I_MUL87, I_MUL88, I_MUL89,
                I_MUL90, I_MUL91, I_MUL92, I_MUL93, I_MUL94, I_MUL95, I_MUL96,
                I_MUL97, I_MUL98, I_MUL99, I_MUL100, I_MUL101, I_MUL102,
                I_MUL103, I_MUL104, I_MUL105, I_MUL106, I_MUL107, I_MUL108,
                I_MUL109, I_MUL110, I_MUL111, I_MUL112, I_MUL113, I_MUL114,
                I_MUL115, I_MUL116, I_MUL117, I_MUL118, I_MUL119, I_MUL120,
                I_MUL121, I_MUL122, I_MUL123, I_MUL124, I_MUL125, I_MUL126,
                I_MUL127, I_MUL128, I_MUL129, I_MUL130, I_MUL131, I_MUL132,
                I_MUL133, I_MUL134, I_MUL135, I_MUL136, I_MUL137, I_MUL138,
                I_MUL139, I_MUL140, I_MUL141, I_MUL142, I_MUL143, I_MUL144,
                I_MUL145, I_MUL146, I_MUL147, I_MUL148, I_MUL149, I_MUL150,
                I_MUL151, I_MUL152, I_MUL153, I_MUL154, I_MUL155, I_MUL156,
                I_MUL157, I_MUL158, I_MUL159, I_MUL160, I_MUL161, I_MUL162,
                I_MUL163, I_MUL164, I_MUL165, I_MUL166, I_MUL167, I_MUL168,
                I_MUL169, I_MUL170, I_MUL171, I_MUL172, I_MUL173, I_MUL174,
                I_MUL175, I_MUL176, I_MUL177, I_MUL178, I_MUL179, I_MUL180,
                I_MUL181, I_MUL182, I_MUL183, I_MUL184, I_MUL185, I_MUL186,
                I_MUL187, I_MUL188, I_MUL189, I_MUL190, I_MUL191, I_MUL192,
                I_MUL193, I_MUL194, I_MUL195, I_MUL196, I_MUL197, I_MUL198,
                I_MUL199, I_MUL200, I_MUL201, I_MUL202, I_MUL203, I_MUL204,
                I_MUL205, I_MUL206, I_MUL207, I_MUL208, I_MUL209, I_MUL210,
                I_MUL211, I_MUL212, I_MUL213, I_MUL214, I_MUL215, I_MUL216,
                I_MUL217, I_MUL218, I_MUL219, I_MUL220, I_MUL221, I_MUL222,
                I_MUL223, I_MUL224, I_MUL225, I_MUL226, I_MUL227, I_MUL228,
                I_MUL229, I_MUL230, I_MUL231, I_MUL232, I_MUL233, I_MUL234,
                I_MUL235, I_MUL236, I_MUL237, I_MUL238, I_MUL239, I_MUL240,
                I_MUL241, I_MUL242, I_MUL243, I_MUL244, I_MUL245, I_MUL246,
                I_MUL247, I_MUL248, I_MUL249, I_MUL250, I_MUL251, I_MUL252,
                I_MUL253, I_MUL254));
    test_assert(count == 64770);
    count = 0;
    APPLY(dfoo2, ;
          , APPLY_PACKR(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                        29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                        43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
                        57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
                        71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
                        85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
                        99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
                        110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
                        121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
                        132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
                        143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153,
                        154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
                        165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
                        176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
                        187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197,
                        198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208,
                        209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
                        220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
                        231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241,
                        242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
                        253, 254));
    test_assert(count == 6885);
    count = 0;
    APPLY(dfoo2, ;
          , APPLY_PACKL(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                        29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                        43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
                        57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
                        71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
                        85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
                        99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
                        110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
                        121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
                        132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
                        143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153,
                        154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
                        165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
                        176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
                        187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197,
                        198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208,
                        209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
                        220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
                        231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241,
                        242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
                        253, 254));
    test_assert(count == -6885);
    test_assert(
        PP_NARG(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
                66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
                82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
                98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
                124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136,
                137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
                150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
                163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
                176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188,
                189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201,
                202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
                215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
                228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240,
                241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253,
                254, 255) == 256);
    test_assert(
        CAT(I_,
            NOT_ONE_NARG(
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
                66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
                82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96,
                97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
                110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
                123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
                136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148,
                149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161,
                162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
                175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
                188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
                201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
                214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226,
                227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
                240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
                253, 254, 255)) == I_MANY);
    test_assert(
        IS_EMPTY(
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
            19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
            36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
            53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
            70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86,
            87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
            103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115,
            116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128,
            129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141,
            142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154,
            155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167,
            168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180,
            181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193,
            194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206,
            207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
            220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232,
            233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245,
            246, 247, 248, 249, 250, 251, 252, 253, 254, 255) == 0);
    test_assert(
        strcmp(
            V_TO_STR(CAT(
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
                66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
                82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
                98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
                124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136,
                137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
                150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
                163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
                176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188,
                189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201,
                202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
                215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
                228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240,
                241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253,
                254, 255)),
            "0123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899100101102103104105106107108109110111112113114115116117118119120121122123124125126127128129130131132133134135136137138139140141142143144145146147148149150151152153154155156157158159160161162163164165166167168169170171172173174175176177178179180181182183184185186187188189190191192193194195196197198199200201202203204205206207208209210211212213214215216217218219220221222223224225226227228229230231232233234235236237238239240241242243244245246247248249250251252253254255") ==
        0);
    test_assert(
        APPLY_RECURSE(
            I_TEST_MIN, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
            50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66,
            67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83,
            84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,
            101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
            114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
            127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
            140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152,
            153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
            166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178,
            179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
            192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204,
            205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217,
            218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
            231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
            244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255) == 0);
    test_assert(APPLY_RECURSE(
                    I_PLUS, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                    30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
                    45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
                    60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74,
                    75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
                    90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103,
                    104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115,
                    116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
                    128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
                    140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151,
                    152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163,
                    164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
                    176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
                    188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
                    200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211,
                    212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
                    224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235,
                    236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247,
                    248, 249, 250, 251, 252, 253, 254, 255) == 32640);
    test_assert(
        APPLY(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
              16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
              32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
              48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
              64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
              80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
              96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
              110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
              123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
              136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148,
              149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161,
              162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
              175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
              188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
              201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
              214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226,
              227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
              240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
              253, 254, 255) == 65280);
    test_assert(
        APPLY_TOO(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                  15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                  30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
                  45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
                  60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74,
                  75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
                  90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103,
                  104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115,
                  116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
                  128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
                  140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151,
                  152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163,
                  164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
                  176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
                  188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
                  200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211,
                  212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
                  224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235,
                  236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247,
                  248, 249, 250, 251, 252, 253, 254, 255) == 65280);
    test_assert(
        APPLY2(I_MUL2, +, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
               16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
               32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
               48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
               64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
               80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
               96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
               110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
               123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
               136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148,
               149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161,
               162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
               175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
               188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
               201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
               214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226,
               227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
               240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
               253, 254, 255) == 65280);
    test_assert(
        ARGP_APPLY(
            2, +, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5, I_MUL6,
            I_MUL7, I_MUL8, I_MUL9, I_MUL10, I_MUL11, I_MUL12, I_MUL13, I_MUL14,
            I_MUL15, I_MUL16, I_MUL17, I_MUL18, I_MUL19, I_MUL20, I_MUL21,
            I_MUL22, I_MUL23, I_MUL24, I_MUL25, I_MUL26, I_MUL27, I_MUL28,
            I_MUL29, I_MUL30, I_MUL31, I_MUL32, I_MUL33, I_MUL34, I_MUL35,
            I_MUL36, I_MUL37, I_MUL38, I_MUL39, I_MUL40, I_MUL41, I_MUL42,
            I_MUL43, I_MUL44, I_MUL45, I_MUL46, I_MUL47, I_MUL48, I_MUL49,
            I_MUL50, I_MUL51, I_MUL52, I_MUL53, I_MUL54, I_MUL55, I_MUL56,
            I_MUL57, I_MUL58, I_MUL59, I_MUL60, I_MUL61, I_MUL62, I_MUL63,
            I_MUL64, I_MUL65, I_MUL66, I_MUL67, I_MUL68, I_MUL69, I_MUL70,
            I_MUL71, I_MUL72, I_MUL73, I_MUL74, I_MUL75, I_MUL76, I_MUL77,
            I_MUL78, I_MUL79, I_MUL80, I_MUL81, I_MUL82, I_MUL83, I_MUL84,
            I_MUL85, I_MUL86, I_MUL87, I_MUL88, I_MUL89, I_MUL90, I_MUL91,
            I_MUL92, I_MUL93, I_MUL94, I_MUL95, I_MUL96, I_MUL97, I_MUL98,
            I_MUL99, I_MUL100, I_MUL101, I_MUL102, I_MUL103, I_MUL104, I_MUL105,
            I_MUL106, I_MUL107, I_MUL108, I_MUL109, I_MUL110, I_MUL111,
            I_MUL112, I_MUL113, I_MUL114, I_MUL115, I_MUL116, I_MUL117,
            I_MUL118, I_MUL119, I_MUL120, I_MUL121, I_MUL122, I_MUL123,
            I_MUL124, I_MUL125, I_MUL126, I_MUL127, I_MUL128, I_MUL129,
            I_MUL130, I_MUL131, I_MUL132, I_MUL133, I_MUL134, I_MUL135,
            I_MUL136, I_MUL137, I_MUL138, I_MUL139, I_MUL140, I_MUL141,
            I_MUL142, I_MUL143, I_MUL144, I_MUL145, I_MUL146, I_MUL147,
            I_MUL148, I_MUL149, I_MUL150, I_MUL151, I_MUL152, I_MUL153,
            I_MUL154, I_MUL155, I_MUL156, I_MUL157, I_MUL158, I_MUL159,
            I_MUL160, I_MUL161, I_MUL162, I_MUL163, I_MUL164, I_MUL165,
            I_MUL166, I_MUL167, I_MUL168, I_MUL169, I_MUL170, I_MUL171,
            I_MUL172, I_MUL173, I_MUL174, I_MUL175, I_MUL176, I_MUL177,
            I_MUL178, I_MUL179, I_MUL180, I_MUL181, I_MUL182, I_MUL183,
            I_MUL184, I_MUL185, I_MUL186, I_MUL187, I_MUL188, I_MUL189,
            I_MUL190, I_MUL191, I_MUL192, I_MUL193, I_MUL194, I_MUL195,
            I_MUL196, I_MUL197, I_MUL198, I_MUL199, I_MUL200, I_MUL201,
            I_MUL202, I_MUL203, I_MUL204, I_MUL205, I_MUL206, I_MUL207,
            I_MUL208, I_MUL209, I_MUL210, I_MUL211, I_MUL212, I_MUL213,
            I_MUL214, I_MUL215, I_MUL216, I_MUL217, I_MUL218, I_MUL219,
            I_MUL220, I_MUL221, I_MUL222, I_MUL223, I_MUL224, I_MUL225,
            I_MUL226, I_MUL227, I_MUL228, I_MUL229, I_MUL230, I_MUL231,
            I_MUL232, I_MUL233, I_MUL234, I_MUL235, I_MUL236, I_MUL237,
            I_MUL238, I_MUL239, I_MUL240, I_MUL241, I_MUL242, I_MUL243,
            I_MUL244, I_MUL245, I_MUL246, I_MUL247, I_MUL248, I_MUL249,
            I_MUL250, I_MUL251, I_MUL252, I_MUL253, I_MUL254,
            I_MUL255) == 65280);
    test_assert(
        ARGP_APPLY(
            2, +,
            APPLY(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                  14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                  29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
                  44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58,
                  59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73,
                  74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88,
                  89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
                  103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114,
                  115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
                  127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138,
                  139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
                  151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
                  163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
                  175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
                  187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198,
                  199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210,
                  211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222,
                  223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234,
                  235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246,
                  247, 248, 249, 250, 251, 252, 253, 254, 255)) == 65280);
    test_assert(
        ARGP_APPLY(
            2, +,
            APPLY_TOO(
                I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
                47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
                63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
                79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94,
                95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108,
                109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
                122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
                135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147,
                148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
                161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173,
                174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
                187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
                200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212,
                213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225,
                226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238,
                239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251,
                252, 253, 254, 255)) == 65280);
    test_assert(
        ARGP_APPLY(
            2, +,
            APPLY2(I_MMUL, COMMA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                   14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                   29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
                   44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58,
                   59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73,
                   74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88,
                   89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
                   103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114,
                   115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
                   127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138,
                   139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
                   151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
                   163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
                   175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
                   187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198,
                   199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210,
                   211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222,
                   223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234,
                   235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246,
                   247, 248, 249, 250, 251, 252, 253, 254, 255)) == 65280);
    count = 0;
    APPLY(foo, ;
          , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
          19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
          36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
          53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
          70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86,
          87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
          103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
          117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130,
          131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144,
          145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
          159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172,
          173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
          187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
          201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
          215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228,
          229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242,
          243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255);
    test_assert(count == 32640);
    count = 0;
    APPLY_TOO(
        foo, ;
        , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
        38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55,
        56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73,
        74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
        92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107,
        108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
        122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
        136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
        150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163,
        164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177,
        178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
        192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205,
        206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
        220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233,
        234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247,
        248, 249, 250, 251, 252, 253, 254, 255);
    test_assert(count == 32640);
    count = 0;
    APPLY2(foo, ;
           , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
           19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
           36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
           53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
           70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86,
           87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
           103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
           117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130,
           131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144,
           145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
           159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172,
           173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
           187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
           201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
           215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228,
           229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242,
           243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255);
    test_assert(count == 32640);
    count = 0;
    APPLY(foo, ;
          , ARGP_APPLY(
                2, COMMA, I_MUL0, I_MUL1, I_MUL2, I_MUL3, I_MUL4, I_MUL5,
                I_MUL6, I_MUL7, I_MUL8, I_MUL9, I_MUL10, I_MUL11, I_MUL12,
                I_MUL13, I_MUL14, I_MUL15, I_MUL16, I_MUL17, I_MUL18, I_MUL19,
                I_MUL20, I_MUL21, I_MUL22, I_MUL23, I_MUL24, I_MUL25, I_MUL26,
                I_MUL27, I_MUL28, I_MUL29, I_MUL30, I_MUL31, I_MUL32, I_MUL33,
                I_MUL34, I_MUL35, I_MUL36, I_MUL37, I_MUL38, I_MUL39, I_MUL40,
                I_MUL41, I_MUL42, I_MUL43, I_MUL44, I_MUL45, I_MUL46, I_MUL47,
                I_MUL48, I_MUL49, I_MUL50, I_MUL51, I_MUL52, I_MUL53, I_MUL54,
                I_MUL55, I_MUL56, I_MUL57, I_MUL58, I_MUL59, I_MUL60, I_MUL61,
                I_MUL62, I_MUL63, I_MUL64, I_MUL65, I_MUL66, I_MUL67, I_MUL68,
                I_MUL69, I_MUL70, I_MUL71, I_MUL72, I_MUL73, I_MUL74, I_MUL75,
                I_MUL76, I_MUL77, I_MUL78, I_MUL79, I_MUL80, I_MUL81, I_MUL82,
                I_MUL83, I_MUL84, I_MUL85, I_MUL86, I_MUL87, I_MUL88, I_MUL89,
                I_MUL90, I_MUL91, I_MUL92, I_MUL93, I_MUL94, I_MUL95, I_MUL96,
                I_MUL97, I_MUL98, I_MUL99, I_MUL100, I_MUL101, I_MUL102,
                I_MUL103, I_MUL104, I_MUL105, I_MUL106, I_MUL107, I_MUL108,
                I_MUL109, I_MUL110, I_MUL111, I_MUL112, I_MUL113, I_MUL114,
                I_MUL115, I_MUL116, I_MUL117, I_MUL118, I_MUL119, I_MUL120,
                I_MUL121, I_MUL122, I_MUL123, I_MUL124, I_MUL125, I_MUL126,
                I_MUL127, I_MUL128, I_MUL129, I_MUL130, I_MUL131, I_MUL132,
                I_MUL133, I_MUL134, I_MUL135, I_MUL136, I_MUL137, I_MUL138,
                I_MUL139, I_MUL140, I_MUL141, I_MUL142, I_MUL143, I_MUL144,
                I_MUL145, I_MUL146, I_MUL147, I_MUL148, I_MUL149, I_MUL150,
                I_MUL151, I_MUL152, I_MUL153, I_MUL154, I_MUL155, I_MUL156,
                I_MUL157, I_MUL158, I_MUL159, I_MUL160, I_MUL161, I_MUL162,
                I_MUL163, I_MUL164, I_MUL165, I_MUL166, I_MUL167, I_MUL168,
                I_MUL169, I_MUL170, I_MUL171, I_MUL172, I_MUL173, I_MUL174,
                I_MUL175, I_MUL176, I_MUL177, I_MUL178, I_MUL179, I_MUL180,
                I_MUL181, I_MUL182, I_MUL183, I_MUL184, I_MUL185, I_MUL186,
                I_MUL187, I_MUL188, I_MUL189, I_MUL190, I_MUL191, I_MUL192,
                I_MUL193, I_MUL194, I_MUL195, I_MUL196, I_MUL197, I_MUL198,
                I_MUL199, I_MUL200, I_MUL201, I_MUL202, I_MUL203, I_MUL204,
                I_MUL205, I_MUL206, I_MUL207, I_MUL208, I_MUL209, I_MUL210,
                I_MUL211, I_MUL212, I_MUL213, I_MUL214, I_MUL215, I_MUL216,
                I_MUL217, I_MUL218, I_MUL219, I_MUL220, I_MUL221, I_MUL222,
                I_MUL223, I_MUL224, I_MUL225, I_MUL226, I_MUL227, I_MUL228,
                I_MUL229, I_MUL230, I_MUL231, I_MUL232, I_MUL233, I_MUL234,
                I_MUL235, I_MUL236, I_MUL237, I_MUL238, I_MUL239, I_MUL240,
                I_MUL241, I_MUL242, I_MUL243, I_MUL244, I_MUL245, I_MUL246,
                I_MUL247, I_MUL248, I_MUL249, I_MUL250, I_MUL251, I_MUL252,
                I_MUL253, I_MUL254, I_MUL255));
    test_assert(count == 65280);
    count = 0;
    APPLY(dfoo2, ;
          , APPLY_PACKR(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                        29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                        43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
                        57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
                        71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
                        85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
                        99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
                        110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
                        121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
                        132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
                        143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153,
                        154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
                        165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
                        176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
                        187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197,
                        198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208,
                        209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
                        220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
                        231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241,
                        242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
                        253, 254, 255));
    test_assert(count == 7040);
    count = 0;
    APPLY(dfoo2, ;
          , APPLY_PACKL(100L, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                        29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                        43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
                        57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
                        71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
                        85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
                        99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
                        110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
                        121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
                        132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
                        143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153,
                        154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
                        165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
                        176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
                        187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197,
                        198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208,
                        209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
                        220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
                        231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241,
                        242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
                        253, 254, 255));
    test_assert(count == -7040);
    return 0;
}

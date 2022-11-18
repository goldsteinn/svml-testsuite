#include <stdint.h>

#include "svml-dbl-state.h"
#include "util/inline-math.h"
#include "util/memory-util.h"
#include "util/random.h"

static double dbls_must_test[] = {
    0x1.dffffb3488a4p-1,      0x1.6c05eb219ec46p-1,
    0x1.35d84799c86cap-1,     -0x1.0068b067c6feep-1,
    -0x1.0068b067c6feep-1,    -0x1.0068b067c6feep-1,
    -0x1.8d313198a2e03p-53,   0x1.35a0de2b038fep-1,
    0x1.ffc00159839aep-1,     0x1.35b9bac9f42c6p-1,
    0x1.0001ff6afc4bap+0,     0x1.018dfa697553p+0,
    0x1.20703c454bba3p+0,     0x1.0001ff6afc4bap+0,
    0x1.0001ff6afc4bap+0,     0x1.0001ff6afc4bap+0,
    0x1.00007fb3703ddp+0,     0x1.001ffe4f6d239p+0,
    0x1.007ff5e6aae25p+0,     0x1.0007ffe4f42f8p+0,
    -0x1.0000045b2c904p-3,    0x1.6c042a6378102p-1,
    -0x1.01bddfd228a2dp-1,    -0x1.004d1c5a9400bp-1,
    -0x1.004d1c5a9400bp-1,    -0x1.004d1c5a9400bp-1,
    0x1.eaeb8b58c0655p-2,     0x1.000c80481e7f9p-1,
    0x1.000fb59dbb7ffp-1,     -0x1.0312655c1d169p-1,
    -0x1.02657ff36d5f3p-2,    0x1.00038856b259ep-4,
    0x1.001c939e14315p+0,     -0x1.02657ff36d5f3p-2,
    -0x1.02657ff36d5f3p-2,    -0x1.0240f2bdb3f25p-2,
    -0x1.fdefd03df4cd7p-3,    -0x1.fff13ade9918p-12,
    0x1.fffdfee9d0656p-12,    -0x1.fff14d29165f4p-8,
    0x1.f9004c4fef9eap-4,     -0x1.ffff8020d3d1dp-7,
    -0x1.05deacb86c0dbp+0,    0x1.62ff6a1682c25p-1,
    0x1.62ff6a1682c25p-1,     0x1.62ff6a1682c25p-1,
    -0x1.13ff259eb9ca8p+1,    0x1.000a5e848c0dp-3,
    0x1.0029e0e2db7dp-3,      -0x1.0010aea41501p-3,
    0x1.f5805b28679f4p-4,     -0x1.e2cfb2667f17ep-9,
    -0x1.d7a5c53da7132p-2,    -0x1.f97fabc0650c4p-4,
    -0x1.f97fabc0650c4p-4,    -0x1.f8a404597baf4p-4,
    0x1.ffd834a270fp-10,      -0x1.fff0caf4c48dp-13,
    -0x1.ffe2abaa5690dp-13,   0x1.85cb7cc1e1318p-6,
    0x1.7a337e1ba1ec2p-257,   -0x1.f7af4893d1d51p-616,
    0x1.09806cdccbfa1p-748,   -0x1.00ddafe7d9deep-885,
    -0x1.13a5ccd87c9bbp+1008, -0x1.13a5ccd87c9bbp+1008,
    0x1.facf4856ce3c8p+491,   -0x0.bd54cbc41f0b9p-1022,
    0x0.bdf2e4b035cc5p-1022,  0x1.477fc84889eabp-511,
    -0x1.7120161c92674p+0,    -0x1.d19ebc5567dcdp+311,
    0x1.91b2e22984b85p-1,     -0x1.4ae182c1ab422p+21,
    -0x1.34e729fd08086p+21,   -0x1.34e729fd08086p+21,
    0x1.2f29eb4e99fa2p+7,     -0x1.13a5ccd87c9bbp+1008,
    0x1.852e715836081p+18,    -0x1.f5ec1ef4d1fb8p+3,
    -0x1.9a4f79002782p-6,     -0x1.633c654fee2bap+9,
    -0x1.5a364e6b98134p+9,    -0x1.ff1ecc8c7ea4fp+0,
    0x1.633cc2ae1c934p+9,     -0x1.6310ab92794a8p+9,
    -0x1.502bf5ad80729p+0,    -0x1.62dabd4848dc4p-2,
    -0x1.633c654fee2bap+9,    -0x1.633c654fee2bap+9,
    -0x1.2b3088f4a6e98p+4,    0x1.c332bde7ca515p-5,
    0x1.00b4cd58903b2p+2,     0x1.c332bde7ca515p-5,
    -0x1.c57541b55c8ebp-16,   -0x1.c57541b55c8ebp-16,
    -0x1.c57541b55c8ebp-16,   -0x1.e057e7a0e494cp-2,
    0x1.0000b7af4dcdp-8,      0x1.00005abf94234p-8,
    0x1.00001d2920fb7p-8,     0x1.3ff2d63705b29p+0,
    0x1.5d164509e8235p-1,     0x1.3ff2d63705b29p+0,
    0x1.5182d8799b84bp+0,     0x1.5182d8799b84bp+0,
    0x1.527f4fb0d9331p+0,     0x1.bba14dc3507ccp+1,
    0x1.78afff6f3044cp+4,     0x1.78affead86a26p+4,
    0x1.78afff9d452cp+4,      -0x1.49f33ad2c1c58p+9,
    0x1.fce66609f7428p+5,     0x1.b97dbeb2777ccp+5,
    0x1.2e8f20cf3cbe7p+8,     0x1.2e8f20cf3cbe7p+8,
    -0x1.18209ecd19a8cp+6,    -0x1.4133f4fd79c1cp-13,
    -0x1.205968aae119fp-8,    -0x1.2059763f8882bp-8,
    -0x1.205968a73d4abp-8,    0x1.334ab33a9aaep-2,
    -0x1.5cd9d94d49a85p+1,    0x1.facf4856ce3c8p+491,
    0x1.ce7ef793d4b0ap-2,     -0x1.fe8c27141c94ap+3,
    -0x1.c37443e446523p-16,   0x1.33f4082f47b74p+8,
    0x1.33f4082f47b74p+8,     0x1.33f4082f47b74p+8,
    -0x1.1a4ce073ea908p-5,    0x1.f3ffd85f33423p-1,
    0x1.3bedfcb8aba8dp-1,     -0x1.ff95ecb4e6331p-2,
    -0x1.ff1eb5acee46bp+9,    -0x1.1a4ce073ea908p-5,
    -0x1.b3d9b47ad1b2fp-13,   -0x1.4c31866f6d3bbp-6,
    -0x1.4c3c931a5de98p-6,    -0x1.8000e569a5545p-3,
    0x1.62f69d171fa65p-2,     -0x1.62fe464c64f65p-8,
    0x1.facf4856ce3c8p+491,   0x1.62ff47a01658fp-2,
    0x1.62ff47a01658fp-2,     0x1.62ff47a01658fp-2,
    0x1.e7f93188565ecp-5,     0x1.86f57e8de4a5p-9,
    0x1.856b41d86994cp-9,     0x1.c3b7c858f0575p-6,
    0x1.33d152e971b4p+1,      0x1.aff859518c846p+7,
    0x1.33d152e971b4p+1,      0x1.45f3067a0f4b2p+847,
    0x1.33d152e971b4p+1,      -0x1.33d152e971b4p+1,
    0x1.33d152e971b4p+1,      -0x1.ea75575af6f09p+1,
    -0x1.67b5541c7d8b7p+7,    -0x1.ea75575af6f09p+1,
    0x1.45f3066f80258p+325,   -0x1.ea75575af6f09p+1,
    0x1.ea75575af6f09p+1,     -0x1.ea75575af6f09p+1,
    -0x1.f613ab0969f81p+1,    -0x1.3f62c60e23b31p+2,
    -0x1.f613ab0969f81p+1,    -0x1.3a7fc9600f86cp+1,
    -0x1.3a7fc9600f86cp+1,    -0x1.3a7fc9600f86cp+1,
    -0x1.bffcbf76b86fp+2,     0x1.1211bef8f68e9p+0,
    0x1.008000db2e8bep+0,     0x1.0ffead33dce6fp+0,
    0x1.48ae5a67204f5p+0,     0x1.48ae5a67204f5p+0,
    0x1.dc0b586f2b26p-1,      0x1.490af72a25a81p-1,
    0x1.00e000c7fa1c3p+0,     0x1.002001ffaa4ap+0,
    0x1.001f01ac83b3p+0,      0x1.de02157073b31p-1,
    0x1.feda7b62c1033p-1,     0x1.10e857d0cb59dp+0,
    0x1.55535a0140a21p+0,     0x1.553e1cb579ee9p+0,
    0x1.55130bc4f0bddp+0,     0x1.2501ee5628b08p-1,
    0x1.00201204555c8p+0,     0x1.001fffbd3f495p+0,
    0x1.f03ebdaea826bp-1,     -0x1.2c10396268852p-2,
    0x1.000aee2a2757fp-9,     0x1.e004312b997bp-4,
    -0x1.2bf1de6b04a8ap-2,    -0x1.2bf1de6b04a8ap-2,
    -0x1.2bf32aaf122e2p-2,    -0x1.ffffff3fffffdp-28,
    0x1.000bcdec306p-11,      0x1.fff86f9b9acp-12,
    0x1.075745181aabp-6,      0x1.0b53197bd66c8p+0,
    0x1.00b0d7b252144p+0,     0x1.0b51de9e20b25p+0,
    0x1.68d778f076021p+0,     0x1.67eaf07ce24d1p+0,
    0x1.0b53197bd66c8p+0,     0x1.6b015f8d9a784p-1,
    0x1.002000d8e91c5p+0,     0x1.002003e5a80e3p+0,
    0x1.ede4ac763282bp-1,     -0x1.f8b791cafcdefp+4,
    -0x1.0e16eb809a35dp+944,  0x1.86485d1a8b19dp-1,
    -0x1.842d8ec8f752fp+21,   0x1.4d84db080b9fdp+21,
    0x1.4d84db080b9fdp+21,    -0x1.07e4c92b5349dp+4,
    -0x1.13a5ccd87c9bbp+1008, 0x1.a5a68e24971a3p+20,
    0x1.9977bea4253f1p+0,     -0x1.99631ed67b43fp+0,
    -0x1.633c654fee2bap+9,    -0x1.adc135eb544c1p-2,
    0x1.ffffffffffffep-26,    -0x1.633cae1335f26p+9,
    -0x1.63324af2fb5b7p-1,    -0x1.63324af2fb5b7p-1,
    0x1.d7131e11fc6b3p-2,     -0x1.c5c9440e9422dp-9,
    -0x1.633c654fee2bap+9,    -0x1.633c654fee2bap+9,
    0x1.fffffffffffffp-1,     0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1,     0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1,     0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1,     0x1.fffffffffffffp-1,
    0x1.fffffffffffffp-1,     -0x1.317cd745dd37cp+9,
    0x1.49adfd996a81dp+18,    -0x1.b5825e79ac164p+12,
    0x1.3f9605aaeb51bp+21,    0x1.3f9605aaeb51bp+21,
    0x1.3f9605aaeb51bp+21,    -0x1.a81d98fc58537p+6,
    -0x1.bab7afea4e624p+62,   0x1.72e90b4651593p+15,
    0x1.3fab696843fbfp+8,     -0x1.780c9aeca907cp+17,
    -0x1.e134557098e37p-3,    0x1.002629fd74484p+0,
    0x1.fd619059e2342p-1,     -0x1.e134557098e37p-3,
    -0x1.e134557098e37p-3,    -0x1.e134557098e37p-3,
    0x1.00cf9f273d84p+1,      -0x1.000a02c5a8b47p-2,
    -0x1.00010c3967f16p-2,    -0x1.001bf41f56582p-1,
    -0x1.202e0f30fe00cp+3,    -0x1.3e0001ad3bee3p+6,
    -0x1.202e0f30fe00cp+3,    -0x1.535175475cc8dp+7,
    -0x1.540b170c4e65ep+7,    -0x1.fc4b534c8eccp+2,
    -0x1.540b170c4e65ep+7,    0x1.c982eb8d417eap-1,
    0x1.4cdee58a47eddp-31,    0x1.c982eb8d417eap-1,
    0x1.c982eb8d417eap-1,     0x1.c982eb8d417eap-1,
    0x1.c982eb8d417eap-1,     0x1.c982eb8d417eap-1,
    0x1.193bed4dff243p+1,     0x1.c513c569fe78ep+0,
    0x1.193bed4dff243p+1,     0x1.193bed4dff243p+1,
    0x1.193bed4dff243p+1,     0x1.193bed4dff243p+1,
    0x1.193bed4dff243p+1,     0x1.fcd5742999ab8p-1,
    -0x1.ee2b43286db75p-1,    -0x1.f692ba202abcp-4,
    -0x1.9915e876fc062p-1,    -0x1.fd7d13f1663afp-1,
    0x1.16c08b622e36p-1,      0x1.f63845056f35ep-1,
    0x1.fffff3634acd6p-1,     -0x1.cb3b399d747f3p-55,
    0x1.1a62633145c07p-54,    0x1.800010834fed5p-1,
    -0x1.80052ddfdd07cp-1,    0x1p+0,
    0x1.499c0c2050962p-997,   -0x1.010e23e83c2a7p-997,
    -0x1.08b9e4381190dp-999,  0x1.dd113d1bb494bp-998,
    -0x1.ffffab1933322p-32,   0x1.fffbbb0468123p-32,
};

const_assert(sizeof(dbls_must_test) + 8 * sizeof(double) < k_dbl_arr_sz);


#define dbl_bit_mask(b) ((1UL << 63) | ((1UL << (b)) - 1))

void
dbl_state_init(dbl_state_t * dbl_state) {
    uint8_t * buf = dbl_state->buf_;
    uint32_t  i;
    double    dbl_v;


    __builtin_memcpy(buf, dbls_must_test, sizeof(dbls_must_test));
    dbl_v = DBL_MIN;
    __builtin_memcpy(buf + sizeof(dbls_must_test) + sizeof(double) * 0, &dbl_v,
                     sizeof(double));
    dbl_v = DBL_MAX;
    __builtin_memcpy(buf + sizeof(dbls_must_test) + sizeof(double) * 1, &dbl_v,
                     sizeof(double));
    dbl_v = FLT_MIN;
    __builtin_memcpy(buf + sizeof(dbls_must_test) + sizeof(double) * 2, &dbl_v,
                     sizeof(double));
    dbl_v = FLT_MAX;
    __builtin_memcpy(buf + sizeof(dbls_must_test) + sizeof(double) * 3, &dbl_v,
                     sizeof(double));


    seed_rand(12345);
    for (i = sizeof(dbls_must_test) + sizeof(double) * 4; i < k_dbl_arr_sz;
         i += sizeof(double)) {
        uint64_t iv;
        switch (rand32() & 0x7) {
            case 0: {
                dbl_v = (double)rand64();
                __builtin_memcpy(buf + i, &dbl_v, sizeof(double));
            } break;
            case 1: {
                iv = rand64() & dbl_bit_mask(52);
                __builtin_memcpy(buf + i, &iv, sizeof(double));
            } break;
            case 2: {
                iv = rand64() & dbl_bit_mask(55);
                __builtin_memcpy(buf + i, &iv, sizeof(double));
            } break;
            case 3: {
                iv = rand64();
                __builtin_memcpy(buf + i, &iv, sizeof(double));
            } break;
            case 4: {
                do {
                    dbl_v = ((double)rand64());
                } while (dbl_v == 0);
                dbl_v = ((double)rand64()) / dbl_v;
                __builtin_memcpy(buf + i, &dbl_v, sizeof(double));
            } break;
            case 5: {
                iv = rand32() | (rand64() & (1UL << 63));
                __builtin_memcpy(buf + i, &iv, sizeof(double));
            } break;
            case 6: {
                iv = rand64() | ((CAST(uint64_t, rand32()) << 53) >> 1);
                __builtin_memcpy(buf + i, &iv, sizeof(double));
            } break;
            case 7: {
                iv = rand64() | (0x7ffUL << 52);
                __builtin_memcpy(buf + i, &iv, sizeof(double));
            } break;
        }
    }

    dbl_state->mulv_ = (__m512i)_mm512_set_pd(
        .010203040506, 1.13234243, -.03242342341, -1.2312332342, .001123131,
        1.0 + DBL_MIN, .0012232, .123456);
    dbl_state->offset_ = 0;
    dbl_state->state2_ = 0;
    dbl_state->ge_     = 0;

    flt_state_init(&(dbl_state->flt_state_));
}

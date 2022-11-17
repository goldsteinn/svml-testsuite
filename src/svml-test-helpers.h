#ifndef SRC_D_SVML_TEST_HELPERS_H_
#define SRC_D_SVML_TEST_HELPERS_H_

#define run_TT_64(src, dst, func, T)                                           \
    {                                                                          \
        uint8_t * I_tmp_src_ = (src);                                          \
        uint8_t * I_tmp_dst_ = (dst);                                          \
        uint64_t  I_tmp_i_   = 0;                                              \
        for (; i < 64; i += sizeof(T)) {                                       \
            T I_tmp_arg_;                                                      \
            __builtin_memcpy(&I_tmp_arg_, I_tmp_src_ + i, sizeof(T));          \
            I_tmp_arg_ = func(I_tmp_arg_);                                     \
            __builtin_memcpy(I_tmp_dst_ + i, &I_tmp_arg_, sizeof(T));          \
        }                                                                      \
    }



#endif

#include "xhash/md4.h"
#include <stdio.h>
#include <string>
#include <iostream>

static std::string md4_str(std::string str)
{
    xhash_context ctx;
    uint8_t buf[16];
    char out[3];
    std::string ret;

    xhash_init(&ctx, XH_ALGO_MD4);
    xhash_update(&ctx, (void*)str.c_str(), str.length());
    xhash_final(&ctx, (void*)buf, 16, NULL);
    xhash_free(&ctx);

    for (size_t i=0; i<16; i++) 
    {
        snprintf(out, 3, "%02x", buf[i]);
        ret += out;
    }
    return ret;
}

#define EXPECT_EQ(exp, value) do{ \
    auto r = exp; \
    auto v = value; \
    if (r != v) { \
        std::cout << r << " << Not equal >> " << v << std::endl; \
    } else { \
        std::cout << r << " << equal >> " << v << std::endl; \
    } \
}while(0);

void test_md4()
{
    EXPECT_EQ(md4_str("abc"), "a448017aaf21d8525fc10ae87aa6729d");
}


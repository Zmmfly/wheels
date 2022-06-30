#include "xhash/xhash.h"

int xhash_init(xhash_context *ctx, xhash_algo algo)
{
    if (ctx == NULL || algo == NULL) return XH_EARGS;
    ctx->algo = algo;
    return algo(ctx, XH_OP_INIT, NULL);
}

int xhash_update(xhash_context *ctx, void *ptr, size_t len)
{
    xhash_args_update arg;
    if (ctx == NULL || ptr == NULL || len == 0) return XH_EARGS;
    if (ctx->algo == NULL) return XH_ECTX;
    arg.ptr = ptr;
    arg.len = len;
    return ctx->algo(ctx, XH_OP_UPDATE, &arg);
}

int xhash_final(xhash_context *ctx, void *out, size_t max, size_t *len)
{
    xhash_args_final arg;
    if (ctx == NULL || out == NULL || max == 0) return XH_EARGS;
    if (ctx->algo == NULL) return XH_ECTX;
    arg.out = out;
    arg.max = max;
    arg.len = len;
    return ctx->algo(ctx, XH_OP_FINAL, &arg);
}

int xhash_reset(xhash_context *ctx)
{
    int ret;
    if (ctx == NULL) return XH_ENULL;
    if (ctx->algo == NULL) return XH_ECTX;
    return ctx->algo(ctx, XH_OP_FREE, NULL);
}

int xhash_free(xhash_context *ctx)
{
    int ret;
    if (ctx == NULL) return XH_ENULL;
    if (ctx->algo == NULL) return XH_ECTX;
    ret = ctx->algo(ctx, XH_OP_FREE, NULL);
    ctx->ctx = NULL;
    return ret;
}

size_t xhash_min(size_t a, size_t b)
{
    return a > b ? b : a;
}

size_t xhash_max(size_t a, size_t b)
{
    return a > b ? a : b;
}

int xhash_is_little()
{
    union {
        uint8_t u8[2];
        uint16_t u16;
    }un16;
    un16.u16 = 0x0001;
    return un16.u8[0];
}

void xhash_swap8(uint8_t *a, uint8_t *b)
{
    uint8_t tmp = 0;
    if (a == NULL || b == NULL) return;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

uint32_t xhash_order_swap32(uint32_t u32)
{
    union {
        uint32_t u32;
        uint16_t u16[2];
        uint8_t u8[4];
    } un32;
    un32.u32 = u32;
    xhash_swap8(&un32.u8[0], &un32.u8[3]);
    xhash_swap8(&un32.u8[1], &un32.u8[2]);
    return un32.u32;
}

uint64_t xhash_order_swap64(uint64_t u64)
{
    union {
        uint32_t u32[2];
        uint64_t u64;
    } un64;
    un64.u64 = u64;
    uint32_t tmp = xhash_order_swap32(un64.u32[0]);
    un64.u32[0] = xhash_order_swap32(un64.u32[1]);
    un64.u32[1] = tmp;
    return un64.u64;
}

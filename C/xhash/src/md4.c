#include "xhash/md4.h"

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define lshift(x, n) ((((x) << (n)) | ((x) >> (32-(n)))) & 0xffffffff)

#define FF(a, b, c, d, x, s) { \
    (a) += F ((b), (c), (d)) + (x); \
    (a) = lshift ((a), (s)); \
  }

#define GG(a, b, c, d, x, s) { \
    (a) += G ((b), (c), (d)) + (x) + (uint32_t)0x5a827999; \
    (a) = lshift ((a), (s)); \
  }
#define HH(a, b, c, d, x, s) { \
    (a) += H ((b), (c), (d)) + (x) + (uint32_t)0x6ed9eba1; \
    (a) = lshift ((a), (s)); \
  }

#pragma pack(push,1)
typedef struct md4_context
{
    union {
        uint8_t buf[64];
        uint32_t u32[16];
    };
    uint8_t pos;
    size_t total;
    union {
        uint32_t digest[4];
        uint8_t digu8[16];
    };
} md4_context;
#pragma pack(pop)

static const uint8_t md4_padding[] = {
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static int md4_ctx_reset(md4_context *ctx)
{
    if (ctx == NULL) return XH_ENULL;

    ctx->digest[0] = 0x67452301;
    ctx->digest[1] = 0xefcdab89;
    ctx->digest[2] = 0x98badcfe;
    ctx->digest[3] = 0x10325476;

    ctx->pos   = 0;
    ctx->total = 0;
    memset(ctx->buf, 0, 64);
    return XH_EOK;
};

static int md4_calc(md4_context *ctx)
{
    uint32_t a = ctx->digest[0];
    uint32_t b = ctx->digest[1];
    uint32_t c = ctx->digest[2];
    uint32_t d = ctx->digest[3];
    uint32_t tmp = 0;

    uint32_t *x = ctx->u32;

    if (!xhash_is_little()) {
        for (uint8_t i=0; i<16; i++)
        {
            x[i] = xhash_order_swap32(x[i]);
        }
    }

    //Round 1
    FF(a, b, c, d, x[0],  3);
    FF(d, a, b, c, x[1],  7);
    FF(c, d, a, b, x[2],  11);
    FF(b, c, d, a, x[3],  19);
    FF(a, b, c, d, x[4],  3);
    FF(d, a, b, c, x[5],  7);
    FF(c, d, a, b, x[6],  11);
    FF(b, c, d, a, x[7],  19);
    FF(a, b, c, d, x[8],  3);
    FF(d, a, b, c, x[9],  7);
    FF(c, d, a, b, x[10], 11);
    FF(b, c, d, a, x[11], 19);
    FF(a, b, c, d, x[12], 3);
    FF(d, a, b, c, x[13], 7);
    FF(c, d, a, b, x[14], 11);
    FF(b, c, d, a, x[15], 19);
  
    //Round 2
    GG(a, b, c, d, x[0],  3);
    GG(d, a, b, c, x[4],  5);
    GG(c, d, a, b, x[8],  9);
    GG(b, c, d, a, x[12], 13);
    GG(a, b, c, d, x[1],  3);
    GG(d, a, b, c, x[5],  5);
    GG(c, d, a, b, x[9],  9);
    GG(b, c, d, a, x[13], 13);
    GG(a, b, c, d, x[2],  3);
    GG(d, a, b, c, x[6],  5);
    GG(c, d, a, b, x[10], 9);
    GG(b, c, d, a, x[14], 13);
    GG(a, b, c, d, x[3],  3);
    GG(d, a, b, c, x[7],  5);
    GG(c, d, a, b, x[11], 9);
    GG(b, c, d, a, x[15], 13);
  
    //Round 3
    HH(a, b, c, d, x[0],  3);
    HH(d, a, b, c, x[8],  9);
    HH(c, d, a, b, x[4],  11);
    HH(b, c, d, a, x[12], 15);
    HH(a, b, c, d, x[2],  3);
    HH(d, a, b, c, x[10], 9);
    HH(c, d, a, b, x[6],  11);
    HH(b, c, d, a, x[14], 15);
    HH(a, b, c, d, x[1],  3);
    HH(d, a, b, c, x[9],  9);
    HH(c, d, a, b, x[5],  11);
    HH(b, c, d, a, x[13], 15);
    HH(a, b, c, d, x[3],  3);
    HH(d, a, b, c, x[11], 9);
    HH(c, d, a, b, x[7],  11);
    HH(b, c, d, a, x[15], 15);

    ctx->digest[0] += a;
    ctx->digest[1] += b;
    ctx->digest[2] += c;
    ctx->digest[3] += d;
}

static int md4_update(md4_context *ctx, void *ptr, size_t len)
{
    size_t rd = 0;
    while(len) {
        rd = xhash_min(len, 64 - ctx->pos);
        memcpy(&ctx->buf[ctx->pos], ptr, rd);

        ctx->pos += rd;
        ctx->total += rd;
        ptr = (uint8_t*)ptr + rd;
        len -= rd;
        if (ctx->pos == 64) {
            md4_calc(ctx);
            ctx->pos = 0;
        }
    }
    return XH_EOK;
}

static int md4_final(md4_context *ctx, void *ptr, size_t len, size_t *out)
{
    union {
        uint8_t u8[8];
        uint64_t u64;
        uint32_t u32[2];
    } un64;
    size_t pad_len;
    uint32_t tmp;

    un64.u64 = ctx->total * 8;
    pad_len = ((ctx->pos < 56) ? 0 : 65) + 56 - ctx->pos;
    md4_update(ctx, md4_padding, pad_len);

    if (!xhash_is_little()) {
        un64.u64 = xhash_order_swap64(un64.u64);
    }
    md4_update(ctx, un64.u8, 8);
    
    if (!xhash_is_little()) {
        ctx->digest[0] = xhash_order_swap32(ctx->digest[0]);
        ctx->digest[1] = xhash_order_swap32(ctx->digest[1]);
        ctx->digest[2] = xhash_order_swap32(ctx->digest[2]);
        ctx->digest[3] = xhash_order_swap32(ctx->digest[3]);
    }

    memcpy(ptr, ctx->digu8, xhash_min(len, 16));
    return XH_EOK;
}

int XH_ALGO_MD4(xhash_context *xhctx, xhash_op op, void *arg)
{
    if (xhctx == NULL) return XH_ENULL;
    switch(op) {
        case XH_OP_INIT:
            xhctx->ctx = calloc(1, sizeof(md4_context));
            if (xhctx->ctx == NULL) return XH_ENOMEM;
            return md4_ctx_reset((md4_context *)xhctx->ctx);

        case XH_OP_UPDATE:
            if (arg == NULL) return XH_EARGS;
            return md4_update((md4_context *)xhctx->ctx, ((xhash_args_update*)arg)->ptr, ((xhash_args_update*)arg)->len);
            break;

        case XH_OP_FINAL:
            if (arg == NULL) return XH_EARGS;
            return md4_final((md4_context *)xhctx->ctx, ((xhash_args_final*)arg)->out, ((xhash_args_final*)arg)->max, ((xhash_args_final*)arg)->len);

        case XH_OP_RESET:
            if (xhctx->ctx == NULL) return XH_ECTX;
            return md4_ctx_reset((md4_context *)xhctx->ctx);

        case XH_OP_FREE:
            if (xhctx->ctx == NULL) return XH_ECTX;
            free(xhctx->ctx);
            break;

        default:
            return XH_EARGS;
    }
}

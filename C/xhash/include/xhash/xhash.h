#ifndef __XHASH_H__
#define __XHASH_H__

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum xhash_op
{
    XH_OP_INIT = 0,
    XH_OP_UPDATE,
    XH_OP_FINAL,
    XH_OP_RESET,
    XH_OP_FREE,
    XH_OP_MAX,
} xhash_op;

typedef enum xhash_ec
{
    XH_EOK    = 0,
    XH_ENULL  = -8,
    XH_EARGS,
    XH_ECTX,
    XH_ENOMEM,
    XH_ENOTENOUGH,
} xhash_ec;

typedef struct xhash_args_update
{
    void *ptr;
    size_t len;
} xhash_args_update;

typedef struct xhash_args_final
{
    void *out;
    size_t max;
    size_t *len;
} xhash_args_final;


struct xhash_context;
typedef struct xhash_context xhash_context;

typedef int (*xhash_algo)(xhash_context *ctx, xhash_op op, void *args);

typedef struct xhash_context
{
    void *ctx;
    xhash_algo algo;
} xhash_context;

/**
 * @brief Init context with hash algorithm
 * 
 * @param ctx 
 * @param algo XH_ALGO_*
 * @return int 
 */
extern int xhash_init(xhash_context *ctx, xhash_algo algo);

/**
 * @brief Update hash data
 * 
 * @param ctx 
 * @param ptr 
 * @param len 
 * @return int 
 */
extern int xhash_update(xhash_context *ctx, void *ptr, size_t len);

/**
 * @brief End for hash and get result
 * 
 * @param ctx 
 * @param out buffer ptr for result out
 * @param max max result
 * @param len result len out, could be NULL for not out
 * @return int 
 */
extern int xhash_final(xhash_context *ctx, void *out, size_t max, size_t *len);

extern int xhash_reset(xhash_context *ctx);

/**
 * @brief Release algorithm context, not xhash_context
 * 
 * @param ctx 
 * @return int 
 */
extern int xhash_free(xhash_context *ctx);

extern size_t xhash_min(size_t a, size_t b);
extern size_t xhash_max(size_t a, size_t b);
extern int xhash_is_little();
extern void xhash_swap8(uint8_t *a, uint8_t *b);
extern uint32_t xhash_order_swap32(uint32_t u32);
extern uint64_t xhash_order_swap64(uint64_t u64);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XHASH_H__ */

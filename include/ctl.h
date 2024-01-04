#ifndef CTL_H
#define CTL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CTL_STR_MAX_LEN 256

enum ctl_return_values {
    CTL_OK           =  0,
    CTL_ERROR        = -1,
    CTL_INVALID_JSON = -3
};

struct ctl_cfg {
    char     dir[CTL_STR_MAX_LEN+1];
    uint32_t bit_mask;
    int      use_internal;
};

/*
 * Parse the following JSON into a struct ctl_cfg:
 *   {
 *       "dir": String,
 *       "bitMask": Number,
 *       "useInternal": true|false
 *   }
 */
int ctl_parse_config(const char* json, struct ctl_cfg* cfg);

#ifdef __cplusplus
}
#endif

#endif /* CTL_H */

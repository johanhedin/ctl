
#include <stdio.h>
#include <string.h>

#include <jansson.h>

#include "ctl.h"

int ctl_parse_config(const char* json, struct ctl_cfg* cfg) {
    int           ret = CTL_OK;
    json_t       *json_root;
    json_error_t  json_error;
    size_t        json_flags;
    json_t       *json_dir;
    const char   *dir;

    cfg->dir[0] = 0;
    cfg->bit_mask = 0;
    cfg->use_internal = 0;

    json_flags = 0;
    json_root = json_loads(json, json_flags, &json_error);
    if (!json_root) {
        fprintf(stderr, "ctl error on line %d: %s\n", json_error.line, json_error.text);
        goto error;
    }

    if (!json_is_object(json_root)) {
        fprintf(stderr, "ctl error: Document is not of type Object\n");
        goto error;
    }

    json_dir = json_object_get(json_root, "dir");
    if (!json_dir || !json_is_string(json_dir)) {
        fprintf(stderr, "ctl error: Key \"dir\" is missing or is not of type String\n");
        goto error;
    }

    dir = json_string_value(json_dir);
    strncpy(cfg->dir, dir, CTL_STR_MAX_LEN);
    cfg->dir[CTL_STR_MAX_LEN] = 0;

    ret = CTL_OK;
    goto done;

error:
    ret = CTL_INVALID_JSON;

done:
    if (json_root) json_decref(json_root);

    return ret;
} /* ctl_parse_config */

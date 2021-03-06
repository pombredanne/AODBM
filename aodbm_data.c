/*  
    Copyright (C) 2011 aodbm authors,
    
    This file is part of aodbm.
    
    aodbm is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    aodbm is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>

#define ntohll(x) ( ( (uint64_t)(ntohl( (uint32_t)((x << 32) >> 32) )) << 32) |\
    ntohl( ((uint32_t)(x >> 32)) ) )                                        
#define htonll(x) ntohll(x)

#include "aodbm.h"
#include "aodbm_data.h"
#include "aodbm_rope.h"

aodbm_data *aodbm_construct_data(const char *dat, size_t sz) {
    aodbm_data *ptr = malloc(sizeof(aodbm_data));
    ptr->sz = sz;
    ptr->dat = malloc(sz);
    memcpy(ptr->dat, dat, sz);
    return ptr;
}

aodbm_data *aodbm_cat_data(aodbm_data *a, aodbm_data *b) {
    aodbm_data *dat = malloc(sizeof(aodbm_data));
    dat->sz = a->sz + b->sz;
    dat->dat = malloc(dat->sz);
    memcpy(dat->dat, a->dat, a->sz);
    memcpy(dat->dat + a->sz, b->dat, b->sz);
    return dat;
}

aodbm_data *aodbm_cat_data_di(aodbm_data *a, aodbm_data *b) {
    aodbm_data *res = aodbm_cat_data(a, b);
    aodbm_free_data(a);
    aodbm_free_data(b);
    return res;
}

aodbm_data *aodbm_data_from_str(const char *dat) {
    return aodbm_construct_data(dat, strlen(dat));
}

aodbm_data *aodbm_data_from_32(uint32_t n) {
    n = htonl(n);
    return aodbm_construct_data((const char*)&n, 4);
}

aodbm_data *aodbm_data_from_64(uint64_t n) {
    n = htonll(n);
    return aodbm_construct_data((const char*)&n, 8);
}

aodbm_data *aodbm_data_empty() {
    aodbm_data *dat = malloc(sizeof(aodbm_data));
    dat->sz = 0;
    dat->dat = NULL;
    return dat;
}

bool aodbm_data_lt(aodbm_data *a, aodbm_data *b) {
    int p;
    if (a->sz < b->sz) {
        return true;
    } else if (a->sz > b->sz) {
        return false;
    } else {
        for (p = 0; p < a->sz; ++p) {
            if (a->dat[p] < b->dat[p])
                return true;
            if (a->dat[p] > b->dat[p])
                return false;
        }
        return false;
    }
}

bool aodbm_data_gt(aodbm_data *a, aodbm_data *b) {
    return aodbm_data_lt(b, a);
}

bool aodbm_data_le(aodbm_data *a, aodbm_data *b) {
    return !aodbm_data_gt(a, b);
}

bool aodbm_data_ge(aodbm_data *a, aodbm_data *b) {
    return !aodbm_data_lt(a, b);
}

bool aodbm_data_eq(aodbm_data *a, aodbm_data *b) {
    if (a->sz != b->sz) {
        return false;
    }
    uint32_t i;
    for (i = 0; i < a->sz; ++i) {
        if (a->dat[i] != b->dat[i]) {
            return false;
        }
    }
    return true;
}

int aodbm_data_cmp(aodbm_data *a, aodbm_data *b) {
    if (aodbm_data_eq(a, b)) {
        return 0;
    }
    if (aodbm_data_lt(a, b)) {
        return -1;
    }
    return 1;
}

aodbm_data *aodbm_data_dup(aodbm_data *v) {
    return aodbm_construct_data(v->dat, v->sz);
}

void aodbm_free_data(aodbm_data *dat) {
    free(dat->dat);
    free(dat);
}

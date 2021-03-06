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

#ifndef AODBM_INTERNAL_H
#define AODBM_INTERNAL_H

#include "aodbm.h"
#include "aodbm_data.h"
#include "aodbm_rope.h"
#include "aodbm_rwlock.h"
#include "aodbm_stack.h"

struct aodbm {
    uint64_t file_size;
    FILE *fd;
    pthread_mutex_t rw;
    volatile uint64_t cur;
    pthread_mutex_t version;
    #ifdef AODBM_USE_MMAP
    volatile void *mapping;
    volatile uint64_t mapping_size;
    aodbm_rwlock_t mmap_mut;
    #endif
};

void print_hex(unsigned char);
void annotate_data(const char *name, aodbm_data *);
void annotate_rope(const char *name, aodbm_rope *);

aodbm_rope *make_block(aodbm_data *);
aodbm_rope *make_block_di(aodbm_data *);
aodbm_rope *make_record(aodbm_data *, aodbm_data *);
aodbm_rope *make_record_di(aodbm_data *, aodbm_data *);

bool aodbm_read_bytes(aodbm *, void *, size_t);
void aodbm_seek(aodbm *, int64_t, int);
uint64_t aodbm_tell(aodbm *);
void aodbm_write_bytes(aodbm *, void *, size_t);
void aodbm_truncate(aodbm *, uint64_t);

void aodbm_write_data_block(aodbm *db, aodbm_data *data);
void aodbm_write_version(aodbm *db, uint64_t ver);
void aodbm_read(aodbm *db, uint64_t off, size_t sz, void *ptr);
uint32_t aodbm_read32(aodbm *db, uint64_t off);
uint64_t aodbm_read64(aodbm *db, uint64_t off);
aodbm_data *aodbm_read_data(aodbm *db, uint64_t off);

/* returns the offset of the leaf node that the key belongs in */
uint64_t aodbm_search(aodbm *, aodbm_version, aodbm_data *);

struct aodbm_path_node {
    aodbm_data *key;
    uint64_t node;
};

typedef struct aodbm_path_node aodbm_path_node;

aodbm_stack *aodbm_search_path(aodbm *, aodbm_version, aodbm_data *);

#endif

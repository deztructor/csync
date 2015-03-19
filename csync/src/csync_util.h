/*
 * libcsync -- a library to sync a directory with another
 *
 * Copyright (c) 2008-2013 by Andreas Schneider <asn@cryptomilk.org>
 * Copyright (c) 2012-2013 by Klaas Freitag <freitag@owncloud.com>wie
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef _CSYNC_UTIL_H
#define _CSYNC_UTIL_H

#include <stdint.h>

#include "csync_private.h"

const char *csync_instruction_str(enum csync_instructions_e instr);

void csync_memstat_check(void);

int csync_merge_file_trees(CSYNC *ctx);

int csync_unix_extensions(CSYNC *ctx);

/* Convert a csync_file_stat_t to csync_vio_file_stat_t */
csync_vio_file_stat_t *csync_vio_convert_file_stat(csync_file_stat_t *st);

#endif /* _CSYNC_UTIL_H */

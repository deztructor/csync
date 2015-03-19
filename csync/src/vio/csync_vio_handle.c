/*
 * libcsync -- a library to sync a directory with another
 *
 * Copyright (c) 2008-2013 by Andreas Schneider <asn@cryptomilk.org>
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

#include "c_lib.h"

#include "vio/csync_vio_handle_private.h"

csync_vio_handle_t *csync_vio_handle_new(const char *uri, csync_vio_method_handle_t *method_handle) {
  csync_vio_handle_t *new = NULL;

  if (uri == NULL || method_handle == NULL) {
    return NULL;
  }

  new = c_malloc(sizeof(csync_vio_handle_t));
  if (new == NULL) {
    return NULL;
  }

  new->uri = c_strdup(uri);
  new->method_handle = method_handle;

  return new;
}

void csync_vio_handle_destroy(csync_vio_handle_t *handle) {
  if (handle == NULL || handle->uri == NULL) {
    return;
  }

  SAFE_FREE(handle->uri);
  SAFE_FREE(handle);
}

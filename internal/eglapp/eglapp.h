/*
 * Copyright © Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * under the terms of the GNU General Public License version 2 or 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __EGLAPP_H__
#define __EGLAPP_H__

#include <memory>
#include <vector>
#include "miregl.h"

extern float mir_eglapp_background_opacity;

std::vector<std::shared_ptr<MirEglSurface>> mir_eglapp_init(struct wl_display* display);
#endif

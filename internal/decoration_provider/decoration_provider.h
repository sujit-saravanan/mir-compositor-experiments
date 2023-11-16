/*
 * Copyright © Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
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

#ifndef MIRAL_SHELL_DECORATION_PROVIDER_H
#define MIRAL_SHELL_DECORATION_PROVIDER_H


#include <miral/window_manager_tools.h>
#include <mir/fd.h>

#include <mutex>

class DecorationProvider
{
public:
    DecorationProvider();
    ~DecorationProvider();

    void operator()(struct wl_display* display);
    void operator()(std::weak_ptr<mir::scene::Session> const& session);

    auto session() const -> std::shared_ptr<mir::scene::Session>;

    void stop();

    bool is_decoration(miral::Window const& window) const;

private:
    mir::Fd const shutdown_signal;

    std::mutex mutable mutex;
    std::weak_ptr<mir::scene::Session> weak_session;
};


#endif //MIRAL_SHELL_DECORATION_PROVIDER_H

#pragma once
#include <memory>
#include <miral/minimal_window_manager.h>
#include "splash_session.h"

class SpinnerSplash {
public:
        SpinnerSplash();
        ~SpinnerSplash();
        
        void operator()(struct wl_display* display);
        void operator()(std::weak_ptr<mir::scene::Session> const& session);
        
        operator std::shared_ptr<SplashSession>() const;

private:
        struct Self;
        std::shared_ptr<Self> const self;
};
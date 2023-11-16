#ifndef MIR_SPLASH_SESSION_H
#define MIR_SPLASH_SESSION_H

#include <memory>

namespace mir { class Server; namespace scene { class Session; }}

class SplashSession
{
public:
        virtual auto session() const -> std::shared_ptr<mir::scene::Session> = 0;
        
        SplashSession() = default;
        virtual ~SplashSession() = default;
        SplashSession(SplashSession const&) = delete;
        SplashSession& operator=(SplashSession const&) = delete;
};

#endif //MIR_SPLASH_SESSION_H
#include "spinner.h"
#include "render_pipelines.h"
#include "glad.h"
#include "texture.h"
#include "buffer.h"
#include <chrono>

#include "eglapp.h"

#define LIGHT_AUBERGINE 0.466666667f, 0.297297297f, 0.435294118f

using time_point = typeof(std::chrono::high_resolution_clock::now());

class Animation {
public:
        Animation(GLfloat angle = 0, GLfloat fadeBackground = 0, GLfloat fadeLogo = 0, GLfloat fadeGlow = 0) {
                creation_time = std::chrono::high_resolution_clock::now();
                current_time = std::chrono::high_resolution_clock::now();
        }
        
        bool run() {
                auto new_time = std::chrono::high_resolution_clock::now();
                const float dela_time = std::chrono::duration<float>(new_time - current_time).count();
                const float elapsed = std::chrono::duration<float>(new_time - creation_time).count();
                
                current_time = new_time;
                
                // step 1.)
                if (elapsed < 0.6f)
                        fadeLogo += 1.6f * dela_time;
                
                // step 2.)
                angle -= (0.017453292519943f * 360.0f / 18.0f) * dela_time;
                
                // step 3.) glow
                if (elapsed > 6.0f && elapsed < 6.833f)
                        fadeGlow += 1.2f * dela_time;
                
                // step 3.) background
                if (elapsed > 0.6f && elapsed < 6.833f) { if (fadeBackground > 0) fadeBackground -= 0.15f * dela_time; else fadeBackground = 0; }
                
                // step 4.) glow
                if (elapsed > 7.0f)
                        fadeGlow -= 0.6f * dela_time;
                
                // step 5.)
                if (elapsed > 6.833f)
                        fadeLogo -= 1.6f * dela_time;
                
                return elapsed < 8.266f;
        }
        
        time_point creation_time;
        time_point current_time;
        
        GLfloat angle{0};
        GLfloat fadeBackground{0};
        GLfloat fadeLogo{0};
        GLfloat fadeGlow{0};
};

struct SpinnerSplash::Self : SplashSession {
        std::mutex mutable mutex;
        std::weak_ptr<mir::scene::Session> session_;
        
        std::shared_ptr<mir::scene::Session> session() const override {
                std::lock_guard lock{mutex};
                return session_.lock();
        }
};

SpinnerSplash::SpinnerSplash() : self{std::make_shared<Self>()} {}

SpinnerSplash::~SpinnerSplash() = default;

void SpinnerSplash::operator()(std::weak_ptr<mir::scene::Session> const& session) {
        std::lock_guard lock{self->mutex};
        self->session_ = session;
}

SpinnerSplash::operator std::shared_ptr<SplashSession>() const {
        return self;
}

void SpinnerSplash::operator()(struct wl_display* display)
try {
        WindowManager::Rasterization glow_shader("assets/shaders/spinner.vert", "assets/shaders/glow.frag");
        WindowManager::Rasterization logo_shader("assets/shaders/spinner.vert", "assets/shaders/logo.frag");
        
        WindowManager::Texture glow_spinner("assets/textures/spinner-glow.png");
        WindowManager::Texture logo_spinner("assets/textures/spinner-logo.png");
        
        WindowManager::VertexAttributeObject glow_spinner_quad;
        glow_spinner_quad.addAttribute<GL_FLOAT, 2>();
        WindowManager::VertexAttributeObject logo_spinner_quad;
        logo_spinner_quad.addAttribute<GL_FLOAT, 2>();
        
        uint32_t indices[] = {  // note that we start from 0!
                0, 1, 3,  // first Triangle
                1, 2, 3   // second Triangle
        };
        WindowManager::HostBuffer ibo;
        ibo.append<uint32_t>(&(indices[0]), 6);
        
        glow_spinner_quad.indexBuffer(ibo.id());
        logo_spinner_quad.indexBuffer(ibo.id());
        
        auto const windows = mir_eglapp_init(display);
        
        if (!windows.size()) return;
        
        double pixelSize = 10 * 11.18;
        const GLfloat texCoordsSpinner[] =
                {
                        -0.5f, 0.5f,
                        -0.5f, -0.5f,
                        0.5f, 0.5f,
                        0.5f, -0.5f,
                };
        
        WindowManager::HostBuffer initial_vbo;
        initial_vbo.append<GLfloat>((void*) &(texCoordsSpinner[0]), 8);
        
        glow_spinner_quad.vertexBuffer(initial_vbo.id());
        logo_spinner_quad.vertexBuffer(initial_vbo.id());
        
        // setup proper GL-blending and gamma correction
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glEnable(GL_FRAMEBUFFER_SRGB);
        
        // bunch of shader-attributes to enable
        Animation anim{0.0, 1.0, 0.0, 0.0};

        // This could be done in a single draw call using texture arrays and persistently mapped buffers.
        do {
                for (auto const& surface: windows)
                        surface->paint([&](unsigned int width, unsigned int height) {
                                GLfloat halfRealWidth = ((2.0 / width) * pixelSize) / 2.0;
                                GLfloat halfRealHeight = ((2.0 / height) * pixelSize) / 2.0;
                                
                                const GLfloat vertices[] =
                                        {
                                                halfRealWidth, halfRealHeight,
                                                halfRealWidth, -halfRealHeight,
                                                -halfRealWidth, halfRealHeight,
                                                -halfRealWidth, -halfRealHeight,
                                        };
                                WindowManager::HostBuffer vbo;
                                vbo.append<GLfloat>((void*) &(vertices[0]), 8);
                                
                                glow_spinner_quad.vertexBuffer(vbo.id());
                                logo_spinner_quad.vertexBuffer(vbo.id());
                                
                                glViewport(0, 0, width, height);
                                
                                GLfloat color[] = {LIGHT_AUBERGINE};
                                for (auto& c: color) { c *= anim.fadeBackground; }
                                
                                glClearColor(color[0], color[1], color[2], anim.fadeBackground);
                                glClear(GL_COLOR_BUFFER_BIT);
                                
                                // draw glow
                                glow_shader.use();
                                glow_spinner.bind(0);
                                glUniform1i(glow_shader["uSampler"], 0);
                                glUniform1f(glow_shader["theta"], anim.angle);
                                glUniform1f(glow_shader["uFadeGlow"], anim.fadeGlow);
                                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                                
                                // draw logo
                                logo_shader.use();
                                logo_spinner.bind(0);
                                glUniform1i(logo_shader["uSampler"], 0);
                                glUniform1f(logo_shader["theta"], anim.angle);
                                glUniform1f(logo_shader["uFadeLogo"], anim.fadeLogo);
                                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                        });
        } while (anim.run());
} catch (std::exception const& x) {
        printf("%s\n", x.what());
}
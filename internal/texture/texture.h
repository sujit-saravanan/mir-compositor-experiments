#pragma once
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <filesystem>
#include "glad/glad.h"

namespace WindowManager {
        class Texture {
        public:  // public constructors/destructors/overloads
                Texture() = default;
                explicit Texture(uint8_t* data, int32_t width, int32_t height, int32_t channels, GLenum internalformat = GL_SRGB_ALPHA, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE, GLint min_filter = GL_LINEAR_MIPMAP_LINEAR, GLint mag_filter = GL_LINEAR, GLint wrap_s = GL_REPEAT, GLint wrap_t = GL_REPEAT);
                explicit Texture(const std::filesystem::path& texture_filepath, GLenum internalformat = GL_SRGB_ALPHA, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE, GLint min_filter = GL_LINEAR_MIPMAP_LINEAR, GLint mag_filter = GL_LINEAR, GLint wrap_s = GL_REPEAT, GLint wrap_t = GL_REPEAT);
                explicit Texture(int32_t width, int32_t height, int32_t channels, GLenum internalformat = GL_SRGB_ALPHA, GLint min_filter = GL_LINEAR_MIPMAP_LINEAR, GLint mag_filter = GL_LINEAR, GLint wrap_s = GL_REPEAT, GLint wrap_t = GL_REPEAT);
                
                Texture(const Texture&) = delete;
                void operator=(const Texture&) = delete;
                
                Texture(Texture&& other) noexcept;
                void operator=(Texture&& other) noexcept;
                
                ~Texture();
        public:  // public member functions
                [[nodiscard]] int32_t width() const;
                [[nodiscard]] int32_t height() const;
                [[nodiscard]] int32_t channels() const;
                [[nodiscard]] uint32_t id() const;
                
                void set(void* data, GLenum format, GLenum type, int32_t width, int32_t height, int32_t x_offset = 0, int32_t y_offset = 0) const;
                void set(void* data, GLenum format, GLenum type) const;
                void bind(int32_t texture_unit) const;
                void bind_image(int32_t texture_unit, GLenum access, GLenum format) const;
        public:  // public member variables
        private: // private member functions
        private: // private member variables
                int32_t m_width = 0;
                int32_t m_height = 0;
                int32_t m_channels = 0;
                uint32_t m_id = 0;
        };
}
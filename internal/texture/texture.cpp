#include "texture.h"

#include "glad.h"
#include "stb_image.h"

WindowManager::Texture::Texture(uint8_t* data, int32_t width, int32_t height, int32_t channels, GLenum internalformat, GLenum format, GLenum type, GLint min_filter, GLint mag_filter, GLint wrap_s, GLint wrap_t) : m_width(width), m_height(height), m_channels(channels) {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
        
        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, min_filter);
        glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, mag_filter);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, wrap_s);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, wrap_t);
        
        glTextureStorage2D(m_id, 1, internalformat, m_width, m_height);
        glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, format, type, data);
        glGenerateTextureMipmap(m_id);
}
WindowManager::Texture::Texture(int32_t width, int32_t height, int32_t channels, GLenum internalformat, GLint min_filter, GLint mag_filter, GLint wrap_s, GLint wrap_t) : m_width(width), m_height(height), m_channels(channels) {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
        
        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, min_filter);
        glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, mag_filter);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, wrap_s);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, wrap_t);
        
        glTextureStorage2D(m_id, 1, internalformat, m_width, m_height);
        glGenerateTextureMipmap(m_id);
}
WindowManager::Texture::Texture(const std::filesystem::path& texture_filepath, GLenum internalformat, GLenum format, GLenum type, GLint min_filter, GLint mag_filter, GLint wrap_s, GLint wrap_t) {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
        
        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, min_filter);
        glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, mag_filter);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, wrap_s);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, wrap_t);
        
        glTextureStorage2D(m_id, 1, internalformat, m_width, m_height);
        unsigned char* data = stbi_load(texture_filepath.string().c_str(), &m_height, &m_height, &m_channels, 0);
        glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, format, type, data);
        stbi_image_free(data);
        glGenerateTextureMipmap(m_id);
}
WindowManager::Texture::~Texture() {
        glDeleteTextures(1, &m_id);
        m_id = 0;
        m_width = 0;
        m_height = 0;
        m_channels = 0;
}

WindowManager::Texture::Texture(Texture&& other) noexcept {
        this->m_id = other.id();
        this->m_height = other.height();
        this->m_width = other.width();
        this->m_channels = other.channels();
        
        other.m_id = 0;
        other.m_width = 0;
        other.m_height = 0;
        other.m_channels = 0;
}
void WindowManager::Texture::operator=(Texture&& other) noexcept {
        this->m_id = other.id();
        this->m_height = other.height();
        this->m_width = other.width();
        this->m_channels = other.channels();
        
        other.m_id = 0;
        other.m_width = 0;
        other.m_height = 0;
        other.m_channels = 0;
}

int32_t WindowManager::Texture::width() const {
        return m_width;
}
int32_t WindowManager::Texture::height() const {
        return m_height;
}
int32_t WindowManager::Texture::channels() const {
        return m_channels;
}
uint32_t WindowManager::Texture::id() const {
        return m_id;
}

void WindowManager::Texture::bind(int32_t texture_unit) const {
        glBindTextureUnit(texture_unit, m_id);
}
void WindowManager::Texture::bind_image(int32_t texture_unit, GLenum access, GLenum format) const {
        glBindImageTexture(texture_unit, m_id, 0, GL_FALSE, 0, access, format);
}
void WindowManager::Texture::set(void* data, GLenum format, GLenum type, int32_t width, int32_t height, int32_t x_offset, int32_t y_offset) const {
        glTextureSubImage2D(m_id, 0, x_offset, y_offset, width, height, format, type, data);
}
void WindowManager::Texture::set(void* data, GLenum format, GLenum type) const {
        glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, format, type, data);
}

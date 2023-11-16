#pragma once
#include <cstddef>
#include <cstring>
#include <iostream>
#include <vector>
#include "glad.h"

namespace WindowManager {
        template<GLenum T>
        static inline consteval uint32_t glTypeSize() {
                switch (T) {
                        case GL_BYTE:
                                return sizeof(GLbyte);
                        case GL_UNSIGNED_BYTE:
                                return sizeof(GLubyte);
                        case GL_SHORT:
                                return sizeof(GLshort);
                        case GL_UNSIGNED_SHORT:
                                return sizeof(GLfloat);
                        case GL_INT:
                                return sizeof(GLint);
                        case GL_UNSIGNED_INT:
                                return sizeof(GLuint);
                        case GL_FIXED:
                                return sizeof(GLfixed);
                        case GL_HALF_FLOAT:
                                return sizeof(GLhalf);
                        case GL_FLOAT:
                                return sizeof(GLfloat);
                        case GL_DOUBLE:
                                return sizeof(GLdouble);
                        default:
                                return 0;
                }
        }
        
        
        class VertexAttributeObject {
        public:  // public constructors/destructors/overloads
                VertexAttributeObject();
        public:  // public member functions
                template<GLenum T, size_t count>
                inline void addAttribute() {
                        glEnableVertexArrayAttrib(m_id, m_attribute_count);
                        glVertexArrayAttribBinding(m_id, m_attribute_count, 0);
                        glVertexArrayAttribFormat(m_id, m_attribute_count, count, T, GL_FALSE, m_stride);
                        m_stride += glTypeSize<T>() * count;
                        m_attribute_count++;
                }
                
                void vertexBuffer(uint32_t vbo_id, uint32_t binding_index = 0, uint32_t offset = 0) const;
                
                void indexBuffer(uint32_t ibo_id) const;
                
                void bind() const;
                
                [[nodiscard]] uint32_t id() const;
        public:  // public member variables
        private: // private member functions
        private: // private member variables
                uint32_t m_id = 0;
                int32_t m_stride = 0;
                uint32_t m_attribute_count = 0;
        };
        
        
        class HostBuffer {
        public:  // public constructors/destructors/overloads
                explicit HostBuffer(uint32_t size = 1, GLenum usage = GL_STATIC_DRAW);
                explicit HostBuffer(void* data, uint32_t size = 1, GLenum usage = GL_STATIC_DRAW);
                
                HostBuffer(const HostBuffer&) = delete;
                void operator=(const HostBuffer&) = delete;
                
                HostBuffer(HostBuffer&& other) noexcept;
                void operator=(HostBuffer&& other) noexcept;
                
                ~HostBuffer();
        public:  // public member functions
        public:  // public member variables
                template<typename T>
                void write(void* data, uint32_t count, uint32_t buffer_offset = 0, uint32_t source_offset = 0) const {
                        glNamedBufferSubData(m_id, buffer_offset, sizeof(T) * count, static_cast<uint8_t*>(data) + source_offset);
                }
                template<typename T>
                void append(void* data, uint32_t count) {
                        while (m_size + sizeof(T) * count >= m_capacity)
                                expand();
                        
                        write<T>(data, count, m_size);
                        m_size += sizeof(T) * count;
                }
                [[nodiscard]] uint32_t id() const;
                void expand();
                [[nodiscard]] uint32_t capacity() const;
                [[nodiscard]] uint32_t size() const;
        private: // private member functions
        private: // private member variables
                uint32_t m_id = 0;
                uint32_t m_size = 0;
                uint32_t m_capacity = 0;
                GLenum m_usage = GL_STATIC_DRAW;
        };
        
        
        class MappedBuffer {
                static constexpr GLbitfield mapping_flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT;
                static constexpr GLbitfield storage_flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_CLIENT_STORAGE_BIT;
        public:  // public constructors/destructors/overloads
                explicit MappedBuffer(uint32_t size = 1, GLenum usage = GL_STATIC_DRAW);
                
                MappedBuffer(const MappedBuffer&) = delete;
                void operator=(const MappedBuffer&) = delete;
                
                MappedBuffer(MappedBuffer&& other) noexcept;
                void operator=(MappedBuffer&& other) noexcept;
                
                ~MappedBuffer();
        public:  // public member functions
        public:  // public member variables
                template<typename T>
                void write(void* data, uint32_t count, uint32_t buffer_offset = 0, uint32_t source_offset = 0) {
                        memcpy(static_cast<uint8_t*>(m_data) + buffer_offset, static_cast<uint8_t*>(data) + source_offset, sizeof(T) * count);
                        m_size = std::max(m_size, buffer_offset + uint32_t(sizeof(T)) * count);
                }
                template<typename T>
                void append(void* data, uint32_t count) {
                        while (m_size + sizeof(T) * count >= m_capacity)
                                expand();
                        
                        write<T>(data, count, m_size);
                }
                void push(uint32_t size, uint32_t client_offset = 0, uint32_t host_offset = 0) const;
                void expand();
                [[nodiscard]] uint32_t host_buffer() const;
                [[nodiscard]] uint32_t client_buffer() const;
                [[nodiscard]] void* data() const;
                [[nodiscard]] uint32_t capacity() const;
                [[nodiscard]] uint32_t size() const;
        
        
        private: // private member functions
        private: // private member variables
                void* m_data = nullptr;
                uint32_t m_client_staging_buffer_id = 0;
                uint32_t m_host_target_buffer_id = 0;
                uint32_t m_size = 0;
                uint32_t m_capacity = 0;
                GLenum m_usage = GL_STATIC_DRAW;
        };
}
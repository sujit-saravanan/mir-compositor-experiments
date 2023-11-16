#include "buffer.h"

// VertexAttributeObject
WindowManager::VertexAttributeObject::VertexAttributeObject() {
        glCreateVertexArrays(1, &m_id);
}
void WindowManager::VertexAttributeObject::vertexBuffer(uint32_t vbo_id, uint32_t binding_index, uint32_t offset) const {
        glVertexArrayVertexBuffer(m_id, binding_index, vbo_id, offset, m_stride);
}
void WindowManager::VertexAttributeObject::indexBuffer(uint32_t ibo_id) const {
        glVertexArrayElementBuffer(m_id, ibo_id);
}
void WindowManager::VertexAttributeObject::bind() const {
        glBindVertexArray(m_id);
}
uint32_t WindowManager::VertexAttributeObject::id() const {
        return m_id;
}



// HostBuffer
WindowManager::HostBuffer::HostBuffer(uint32_t size, GLenum usage) : m_capacity(size), m_usage(usage) {
        glCreateBuffers(1, &m_id);
        glNamedBufferData(m_id, size, nullptr, usage);
}
WindowManager::HostBuffer::HostBuffer(void* data, uint32_t size, GLenum usage) : m_capacity(size), m_usage(usage) {
        glCreateBuffers(1, &m_id);
        glNamedBufferData(m_id, size, data, usage);
}
WindowManager::HostBuffer::HostBuffer(HostBuffer&& other) noexcept {
        if (this->m_id == other.m_id)
                return;
        this->m_id = other.m_id;
        this->m_capacity = other.m_capacity;
        this->m_usage = other.m_usage;
        
        other.m_id = 0;
        other.m_capacity = 0;
        other.m_usage = GL_STATIC_DRAW;
}
void WindowManager::HostBuffer::operator=(HostBuffer&& other) noexcept {
        if (this->m_id == other.m_id)
                return;
        this->m_id = other.m_id;
        this->m_capacity = other.m_capacity;
        this->m_usage = other.m_usage;
        
        other.m_id = 0;
        other.m_capacity = 0;
        other.m_usage = GL_STATIC_DRAW;
}
WindowManager::HostBuffer::~HostBuffer() {
        glDeleteBuffers(1, &m_id);
}
uint32_t WindowManager::HostBuffer::id() const {
        return m_id;
}
void WindowManager::HostBuffer::expand() {
        const uint32_t new_capacity = std::max(1u, m_capacity * 2);
        
        // Create new host buffer and copy everything over;
        uint32_t new_buffer;
        glCreateBuffers(1, &new_buffer);
        glNamedBufferData(new_buffer, new_capacity, nullptr, m_usage);
        glCopyNamedBufferSubData(m_id, new_buffer, 0, 0, m_capacity);
        
        // Delete old buffers
        glDeleteBuffers(1, &m_id);
        
        // Update values
        m_id = new_buffer;
        m_capacity = new_capacity;
}
uint32_t WindowManager::HostBuffer::capacity() const {
        return m_capacity;
}
uint32_t WindowManager::HostBuffer::size() const {
        return m_size;
}


// MappedBuffer
WindowManager::MappedBuffer::MappedBuffer(uint32_t size, GLenum usage) : m_capacity(size), m_usage(usage) {
        glCreateBuffers(1, &m_client_staging_buffer_id);
        glNamedBufferStorage(m_client_staging_buffer_id, size, nullptr, storage_flags);
        m_data = glMapNamedBufferRange(m_client_staging_buffer_id, 0, size, mapping_flags);
        
        glCreateBuffers(1, &m_host_target_buffer_id);
        glNamedBufferData(m_host_target_buffer_id, size, nullptr, usage);
}
WindowManager::MappedBuffer::MappedBuffer(MappedBuffer&& other) noexcept {
        this->m_client_staging_buffer_id = other.m_client_staging_buffer_id;
        this->m_host_target_buffer_id = other.m_host_target_buffer_id;
        this->m_data = other.m_data;
        this->m_capacity = other.m_capacity;
        this->m_usage = other.m_usage;
        
        other.m_client_staging_buffer_id = 0;
        other.m_host_target_buffer_id = 0;
        other.m_data = nullptr;
        other.m_capacity = 0;
        other.m_usage = GL_STATIC_DRAW;
}
void WindowManager::MappedBuffer::operator=(MappedBuffer&& other) noexcept {
        this->m_client_staging_buffer_id = other.m_client_staging_buffer_id;
        this->m_host_target_buffer_id = other.m_host_target_buffer_id;
        this->m_data = other.m_data;
        this->m_capacity = other.m_capacity;
        this->m_usage = other.m_usage;
        
        other.m_client_staging_buffer_id = 0;
        other.m_host_target_buffer_id = 0;
        other.m_data = nullptr;
        other.m_capacity = 0;
        other.m_usage = GL_STATIC_DRAW;
}
WindowManager::MappedBuffer::~MappedBuffer() {
        glUnmapNamedBuffer(m_client_staging_buffer_id);
        glDeleteBuffers(1, &m_client_staging_buffer_id);
        glDeleteBuffers(1, &m_host_target_buffer_id);
}
void WindowManager::MappedBuffer::push(uint32_t size, uint32_t client_offset, uint32_t host_offset) const {
        glCopyNamedBufferSubData(m_client_staging_buffer_id, m_host_target_buffer_id, client_offset, host_offset, size);
}
void WindowManager::MappedBuffer::expand() {
        const uint32_t new_capacity = std::max(1u, m_capacity * 2);
        
        // Create new staging buffer and copy everything over
        uint32_t new_client_staging_buffer;
        glCreateBuffers(1, &new_client_staging_buffer);
        glNamedBufferStorage(new_client_staging_buffer, new_capacity, nullptr, storage_flags);
        void* new_data = glMapNamedBufferRange(new_client_staging_buffer, 0, new_capacity, mapping_flags);
        memcpy(static_cast<uint8_t*>(new_data), static_cast<uint8_t*>(m_data), m_capacity);
        
        // Create new host buffer and copy everything over;
        uint32_t new_host_target_buffer;
        glCreateBuffers(1, &new_host_target_buffer);
        glNamedBufferData(new_host_target_buffer, new_capacity, nullptr, m_usage);
        glCopyNamedBufferSubData(m_host_target_buffer_id, new_host_target_buffer, 0, 0, m_capacity);
        
        // Delete old buffers
        glUnmapNamedBuffer(m_client_staging_buffer_id);
        glDeleteBuffers(1, &m_client_staging_buffer_id);
        glDeleteBuffers(1, &m_host_target_buffer_id);
        
        // Update values
        m_client_staging_buffer_id = new_client_staging_buffer;
        m_host_target_buffer_id = new_host_target_buffer;
        m_data = new_data;
        m_capacity = new_capacity;
}
uint32_t WindowManager::MappedBuffer::host_buffer() const {
        return m_host_target_buffer_id;
}
uint32_t WindowManager::MappedBuffer::client_buffer() const {
        return m_client_staging_buffer_id;
}
void* WindowManager::MappedBuffer::data() const {
        return m_data;
}
uint32_t WindowManager::MappedBuffer::capacity() const {
        return m_capacity;
}
uint32_t WindowManager::MappedBuffer::size() const {
        return m_size;
}

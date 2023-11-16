#pragma once
#include <string>
#include <cstdint>
#include <unordered_map>

namespace WindowManager {
        enum class PipelineType {
                Rasterization,
                Compute
        };
        
        namespace details {
                class Pipeline {
                public: // Static Variables
                        static constexpr PipelineType m_pipeline_type = PipelineType::Rasterization;
                
                public:  // public constructors/destructors/overloads
                        int32_t operator[] (const std::string& uniform_name);
                public:  // public member functions
                        void use() const;
                        [[nodiscard]] int32_t getUniformLocation(const std::string& uniform_name) const;
                        [[nodiscard]] uint32_t id() const;
                
                public:  // public member variables
                private: // private member functions
                protected: // private member variables
                        uint32_t m_shader_program = 0;
                        std::unordered_map<std::string, int32_t> m_uniforms{};
                };
        }
        
        template<class T>
        concept pipeline = std::is_base_of_v<details::Pipeline, std::remove_cvref_t<T>>;
        
        
        class Rasterization : public details::Pipeline {
        public: // Static Variables
                static constexpr PipelineType m_pipeline_type = PipelineType::Rasterization;
                struct CreateInfo {
                        const char* vertex_shader_filepath;
                        const char* fragment_shader_filepath;
                };
        
        public:  // public constructors/destructors/overloads
                Rasterization() = default;
                Rasterization(const char* vertex_shader_filepath, const char* fragment_shader_filepath);
        public:  // public member functions
        
        public:  // public member variables
        private: // private member functions
        private: // private member variables
        };
        
        class Compute : public details::Pipeline {
        public: // Static Variables
                static constexpr PipelineType m_pipeline_type = PipelineType::Compute;
                struct CreateInfo {
                        const char* compute_shader_filepath;
                };
        public:  // public constructors/destructors/overloads
                Compute(const char* compute_shader_filepath);
        public:  // public member functions
        public:  // public member variables
        private: // private member functions
        private: // private member variables
        };
}


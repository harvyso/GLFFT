/* Copyright (C) 2015 Hans-Kristian Arntzen <maister@archlinux.us>
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef GLFFT_GL_INTERFACE_HPP__
#define GLFFT_GL_INTERFACE_HPP__

#include "glfft_interface.hpp"

// Implement this header somewhere in your include path and include relevant GL/GLES API headers.
#include "glfft_gl_api_headers.hpp"

#ifndef GLFFT_GLSL_LANG_STRING
#error GLFFT_GLSL_LANG_STRING must be defined to e.g. "#version 310 es\n" or "#version 430 core\n".
#endif

#ifndef GLFFT_LOG_OVERRIDE
// Implement this.
void glfft_log(const char *fmt, ...);
#else
#define glfft_log GLFFT_LOG_OVERRIDE
#endif

#ifndef GLFFT_TIME_OVERRIDE
// Implement this.
void glfft_time();
#else
#define glfft_time GLFFT_TIME_OVERRIDE
#endif

namespace GLFFT
{
    class GLContext;

    class GLTexture : public Texture
    {
        public:
            friend class GLContext;
            friend class GLCommandBuffer;
            ~GLTexture();

        private:
            GLTexture(const void *initial_data,
                    unsigned width, unsigned height, unsigned levels,
                    Format format, WrapMode wrap_s, WrapMode wrap_t,
                    Filter min_filter, Filter mag_filter);
            GLuint name;
    };

    class GLSampler : public Sampler
    {
        public:
            friend class GLContext;
            friend class GLCommandBuffer;
            ~GLSampler();

        private:
            GLSampler(GLuint name);
            GLuint name;
    };

    class GLBuffer : public Buffer
    {
        public:
            friend class GLContext;
            friend class GLCommandBuffer;
            ~GLBuffer();

        private:
            GLuint name;
            GLBuffer(const void *initial_data, size_t size, AccessMode access);
    };

    class GLProgram : public Program
    {
        public:
            friend class GLContext;
            friend class GLCommandBuffer;
            ~GLProgram();

        private:
            GLProgram(GLuint name);
            GLuint name;
    };

    class GLCommandBuffer : public CommandBuffer
    {
        public:
            ~GLCommandBuffer() = default;
            GLCommandBuffer() = default;

            void bind_program(Program *program) override;
            void bind_storage_texture(unsigned binding, Texture *texture, Format format) override;
            void bind_texture(unsigned binding, Texture *texture) override;
            void bind_sampler(unsigned binding, Sampler *sampler) override;
            void bind_storage_buffer(unsigned binding, Buffer *texture) override;
            void bind_storage_buffer_range(unsigned binding, size_t offset, size_t length, Buffer *texture) override;
            void dispatch(unsigned x, unsigned y, unsigned z) override;

            void barrier(Buffer *buffer) override;
            void barrier(Texture *buffer) override;
            void barrier() override;

            void uniform1ui(unsigned location, unsigned v) override;
            void uniform2f(unsigned location, float v0, float v1) override;
    };

    class GLContext : public Context
    {
        public:
            std::unique_ptr<Texture> create_texture(const void *initial_data,
                    unsigned width, unsigned height, unsigned levels,
                    Format format, WrapMode wrap_s, WrapMode wrap_t,
                    Filter min_filter, Filter mag_filter) override;

            std::unique_ptr<Buffer> create_buffer(const void *initial_data, size_t size, AccessMode access) override;
            std::unique_ptr<Program> compile_compute_shader(const char *source) override;

            CommandBuffer* request_command_buffer() override;
            void submit_command_buffer(CommandBuffer *cmd) override;
            void wait_idle() override;

            const char* get_renderer_string() override;
            void log(const char *fmt, ...) override;
            double get_time() override;

            unsigned get_max_work_group_threads() override;

            const void* map(Buffer *buffer, size_t offset, size_t size) override;
            void unmap(Buffer *buffer) override;

        private:
            static GLCommandBuffer static_command_buffer;
    };
}

#endif

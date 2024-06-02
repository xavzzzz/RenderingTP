#include "opengl-framework/opengl-framework.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
#include <glm/ext/matrix_transform.hpp>

int main()
{
    // Initialisation
    gl::init("TPs de Rendering");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE);
    glEnable(GL_DEPTH_TEST);

    auto camera = gl::Camera{};
    gl::set_events_callbacks({
        camera.events_callbacks(),
        {
            .on_mouse_pressed = [&](gl::MousePressedEvent const& e) {
                std::cout << "Mouse pressed at " << e.position.x << " " << e.position.y << '\n';
            },
        },
        });

    auto const shader = gl::Shader{ {
        .vertex = gl::ShaderSource::File{"res/vertex.glsl"},
        .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
    } };

    auto const cube_mesh = gl::Mesh{ {
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position3D{0}, gl::VertexAttribute::UV{1}},
            .data = {
            // Define the 8 vertices of the cube with UV coordinates
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Vertex 0
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // Vertex 1
            0.5f, 0.5f, -0.5f,   1.0f, 1.0f, // Vertex 2
            -0.5f, 0.5f, -0.5f,  0.0f, 1.0f, // Vertex 3
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, // Vertex 4
            0.5f, -0.5f, 0.5f,   1.0f, 0.0f, // Vertex 5
            0.5f, 0.5f, 0.5f,    1.0f, 1.0f, // Vertex 6
            -0.5f, 0.5f, 0.5f,    0.0f, 1.0f // Vertex 7
        },
    }},
    .index_buffer = {
            // Define the 12 triangles (2 per face) using the indices of the vertices
            0, 1, 2,  0, 2, 3,  // Face 1 (bottom)
            4, 5, 6,  4, 6, 7,  // Face 2 (top)
            0, 1, 5,  0, 5, 4,  // Face 3 (front)
            2, 3, 7,  2, 7, 6,  // Face 4 (back)
            0, 3, 7,  0, 7, 4,  // Face 5 (left)
            1, 2, 6,  1, 6, 5   // Face 6 (right)
        },
    } };


    // Load the texture
    auto const texture = gl::Texture{
        gl::TextureSource::File{
            .path = "res/texture.png",
            .flip_y = true,
            .texture_format = gl::InternalFormat::RGBA8,
        },
        gl::TextureOptions{
            .minification_filter = gl::Filter::Linear,
            .magnification_filter = gl::Filter::Linear,
            .wrap_x = gl::Wrap::Repeat,
            .wrap_y = gl::Wrap::Repeat,
        }
    };

    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.bind();
        shader.set_uniform("_view_projection", glm::infinitePerspective(glm::radians(45.f), gl::framebuffer_aspect_ratio(), 0.1f) * camera.view_matrix());
        shader.set_uniform("_transform", glm::translate(glm::rotate(glm::mat4{ 1.f }, gl::time_in_seconds(), glm::vec3{ 0.f, 0.f, 1.f }), glm::vec3{ 0.f, 1.f, 0.f }));

        shader.set_uniform("myTexture", texture);

        cube_mesh.draw();
    }

    return 0;
}

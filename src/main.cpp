#include "opengl-framework/opengl-framework.hpp"// Inclue la librairie qui va nous servir à faire du rendu
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
#include <glm/ext/matrix_transform.hpp>

int main()
{
    // Initialisation
    gl::init("TPs de Rendering"); // On crée une fenêtre et on choisit son nom
    gl::maximize_window(); // On peut la maximiser si on veut
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE); // On peut configurer l'équation qui mélange deux couleurs, comme pour faire différents blend mode dans Photoshop. Cette équation-ci donne le blending "normal" entre pixels transparents.
    // À mettre dans l'initialisation
    glEnable(GL_DEPTH_TEST);

    // Dans l'initialisation
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
        .layout = {gl::VertexAttribute::Position3D{0}},
        .data = {
            // Define the 8 vertices of the cube
            -0.5f, -0.5f, -0.5f,  // Vertex 0
            0.5f, -0.5f, -0.5f,   // Vertex 1
            0.5f, 0.5f, -0.5f,    // Vertex 2
            -0.5f, 0.5f, -0.5f,   // Vertex 3
            -0.5f, -0.5f, 0.5f,   // Vertex 4
            0.5f, -0.5f, 0.5f,    // Vertex 5
            0.5f, 0.5f, 0.5f,     // Vertex 6
            -0.5f, 0.5f, 0.5f     // Vertex 7
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

    auto const texture = gl::Texture{
    gl::TextureSource::File{ // Peut être un fichier, ou directement un tableau de pixels
        .path = "res/texture.png",
        .flip_y = true, // Il n'y a pas de convention universelle sur la direction de l'axe Y. Les fichiers (.png, .jpeg) utilisent souvent une direction différente de celle attendue par OpenGL. Ce booléen flip_y est là pour inverser la texture si jamais elle n'apparaît pas dans le bon sens.
        .texture_format = gl::InternalFormat::RGBA8, // Format dans lequel la texture sera stockée. On pourrait par exemple utiliser RGBA16 si on voulait 16 bits par canal de couleur au lieu de 8. (Mais ça ne sert à rien dans notre cas car notre fichier ne contient que 8 bits par canal, donc on ne gagnerait pas de précision). On pourrait aussi stocker en RGB8 si on ne voulait pas de canal alpha. On utilise aussi parfois des textures avec un seul canal (R8) pour des usages spécifiques.
    },
    gl::TextureOptions{
        .minification_filter = gl::Filter::Linear, // Comment on va moyenner les pixels quand on voit l'image de loin ?
        .magnification_filter = gl::Filter::Linear, // Comment on va interpoler entre les pixels quand on zoom dans l'image ?
        .wrap_x = gl::Wrap::Repeat,   // Quelle couleur va-t-on lire si jamais on essaye de lire en dehors de la texture ?
        .wrap_y = gl::Wrap::Repeat,   // Idem, mais sur l'axe Y. En général on met le même wrap mode sur les deux axes.
    }
    };




    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 1.f, 1.f); // Choisis la couleur à utiliser. Les paramètres sont R, G, B, A avec des valeurs qui vont de 0 à 1
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Vient remplacer glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT); // Exécute concrètement l'action d'appliquer sur tout l'écran la couleur choisie au-dessus
        // Rendu à chaque frame
        shader.bind(); // On a besoin qu'un shader soit bind (i.e. "actif") avant de draw(). On en reparle dans la section d'après.
        shader.set_uniform("_uniforme", glm::vec2{ 1.f, 3.f });
        shader.set_uniform("_aspectRatio", gl::framebuffer_aspect_ratio());
        //shader.set_uniform("_time", gl::time_in_seconds());
        //shader.set_uniform("_trans", gl::time_in_seconds());
        //gl::Mesh trail = triangle_mesh;

        glm::mat4 const view_matrix = camera.view_matrix();
        glm::mat4 const projection_matrix = glm::infinitePerspective(glm::radians(45.f), gl::framebuffer_aspect_ratio() /*aspect ratio*/, .1f /*near plane*/);

        glm::mat4 const rotation = glm::rotate(glm::mat4{ 1.f }, gl::time_in_seconds() /*angle de la rotation*/, glm::vec3{ 0.f, 0.f, 1.f } /* axe autour duquel on tourne */);
        glm::mat4 const translation = glm::translate(glm::mat4{ 1.f }, glm::vec3{ 0.f, 1.f, 0.f } /* déplacement */);
        glm::mat4 const transform = translation * rotation;

        shader.set_uniform("_view_projection", (projection_matrix * view_matrix));

        cube_mesh.draw(); // C'est ce qu'on appelle un "draw call" : on envoie l'instruction à la carte graphique de dessiner notre mesh.
    }

}
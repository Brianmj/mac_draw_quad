#include "app.hpp"
#include <string>
#include <iostream>
#include <OpenGL/glu.h>

using namespace knu::math;

void App::general_setup()
{
	load_shaders();
	glEnable(GL_DEPTH_TEST);
    
    std::vector<knu::math::vector4f> vecs { {-15.0f, 15.0f, 0.0f, 1.0f},
        {-15.0f, -15.0f, 0.0f, 1.0f},
        {15.0f, -15.0f, 0.0f, 1.0f},
        {15.0f, 15.0f, 0.0f, 1.0f}
    };
    
    std::vector<unsigned short> indices {0, 1, 3, 3, 1, 2};
    
    glGenBuffers(1, &index_buf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);
    
    glGenBuffers(1, &vertex_buf);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
    glBufferData(GL_ARRAY_BUFFER, vecs.size() * sizeof(vector4f), vecs.data(), GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, (void*)0);
    glEnableVertexAttribArray(0);
    
}

void App::draw_scene()
{
	glClearBufferfv(GL_COLOR, 0, clear_color);
	glClearBufferfv(GL_DEPTH, 0, &clear_depth);
    
    matrix4f modelview = matrix4f::translation_matrix(0.0f, 0.0f, -5.0f);
    matrix4f mvp =  modelview * perspective_matrix;
    glProgramUniformMatrix4fv(quad_program.obj(), mvp_uniform, 1, true, mvp.data());
    
    quad_program.bind();
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buf);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
    
}

void App::process_inputs()
{
    int returned_keys_count = 0;
    auto ptr = SDL_GetKeyboardState(&returned_keys_count);
    std::vector<std::uint8_t> keyboard_state(ptr, ptr + returned_keys_count);
    
    
}

void App::update(KNU_TIME seconds)
{

}

void App::load_shaders()
{
    std::string vertex_shader {"shaders/quad.vert"};
    std::string frag_shader {"shaders/quad.frag"};
    
    quad_program.add_vertex_file(vertex_shader);
    quad_program.add_fragment_file(frag_shader);
    quad_program.build();
    
    model_uniform = quad_program.uniform("modelview_mat");
    proj_uniform = quad_program.uniform("projection_mat");
    mvp_uniform = quad_program.uniform("mvp");
}

void App::process_messages(SDL_Event *event)
{
	switch (event->type)
	{
	case SDL_WINDOWEVENT:
		{
			switch (event->window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				{
					resize(event->window.data1, event->window.data2);
				}break;

			case SDL_WINDOWEVENT_CLOSE:
				{
					window.set_quit(true);
				}break;
			}
		}break;

	case SDL_KEYDOWN:
		{
			if (event->key.keysym.sym == SDLK_ESCAPE)
				window.set_quit(true);
            
		}break;
            
        case SDL_QUIT:
        {
            window.set_quit(true);
        }break;
	
	}
}

void App::resize(int w, int h)
{
	glViewport(0, 0, w, h);
    
    float half_width = w / 2.0f;
    float half_height = h / 2.0f;
    
    float left = -half_width;
    float right = half_width;
    
    float bottom = -half_height;
    float top = half_height;
    
    float near = 0.10f;
    float far = 50.0f;
    
    orthographic_matrix = matrix4f{ 2.0f / (right - left), 0.0f, 0.0f, 0.0f,
                                    0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
                                    0.0f, 0.0f, 2.0f / (far - near), 0.0f,
        (left + right)/(left - right), (bottom + top) / (bottom - top), (near + far) / (far - near), 1.0f};
    
    float a = (2 * near) / (right - left);
    float b = (2 * near) / (top - bottom);
    float c = (right + left) / (right - left);
    float d = (top + bottom) / (top - bottom);
    float e = (near + far) / (near - far);
    float f = (2 * near * far) / (near - far);
    
    perspective_matrix = matrix4f { a, 0.0f, 0.0f, 0.0f,
                                    0.0f, b, 0.0f, 0.0f,
                                    c, d, e, -1.0f,
                                    0.0f, 0.0f, f, 0.0f
    };
    
    //perspective_matrix = make_frustrum<float>(-15, 15, -15, 15, 0.1f, 100.0f);
    perspective_matrix = make_ortho2<float>(-15, 15, -15, 15, 0.1f, 100.0f);
}

void App::get_window_size(int &w, int &h)
{
    window.get_window_size(w, h);
}

int App::window_width()
{
	int w, h;
	get_window_size(w, h);
	return w;
}

int App::window_height()
{
	int w, h;
	get_window_size(w, h);
	return h;
}
App::App():
	window(1024, 768, MAJOR_VERSION, MINOR_VERSION, false, 24, 0)
{
	window.set_event_callback(std::bind(&App::process_messages, this, std::placeholders::_1));
	clear_color[0] = 0.1f; clear_color[1] = 0.2f; clear_color[2] = 0.4f; clear_color[3] = 1.0f;
	clear_depth = 1.0f;
}

int App::run()
{
	last_time = current_time = std::chrono::steady_clock::now();
	initialize_graphics();
    general_setup();

	while (window.is_active())
	{
		current_time = std::chrono::steady_clock::now();
		window.poll_events();

        process_inputs();
		update(current_time - last_time);
		draw_scene();

        last_time = current_time;
		window.swap_buffers();
	}

	return 0;
}

void App::initialize_graphics()
{
#ifdef WIN32
	glDebugMessageCallback(&debug_output1, nullptr);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif
	int w, h; window.get_window_size(w, h);	resize(w, h);
    
    std::cout << glGetString(GL_VERSION) << "\n";
    int ext = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &ext);
    
    for(int i = 0; i < ext; ++i) { std::cout << glGetStringi(GL_EXTENSIONS, i) << "\n";};
}

void APIENTRY debug_output1(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char *message, const void *userParam)
{
	std::string msg = std::string("source: ") + std::to_string(source) + "\n type: " + std::to_string(type)
		+ "\n id: " + std::to_string(id) + "\n severity: " + std::to_string(severity)
		+ "\n " + std::string(message) + "\n";

#ifdef WIN32
	OutputDebugStringA(msg.c_str());
#endif
	std::cerr << msg << std::endl;
}

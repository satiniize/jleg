#include "visuals.hpp"

const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;
const unsigned int SCALE = 2;
const unsigned int FRAME_RATE = 60;

jleg::shader jleg::shader_program;
jleg::sprite_drawer jleg::drawer;
std::vector<jleg::sprite*> jleg::sprites;
GLFWwindow* jleg::window;
jleg::camera jleg::cam;
// jleg::node* jleg::root_node;




void jleg::register_sprite(jleg::sprite* _sprite){
    jleg::sprites.push_back(_sprite);
};


void jleg::start_loop(jleg::scene_graph _graph){
    float prev_time = 0.0;
    float delta;
    unsigned int framecount = 0;

    // jleg::node* last_node = jleg::root_node;
    std::vector<jleg::node*> node_queue;
    std::vector<jleg::node*> node_buffer;
    // std::vector<jleg::sprite*> sprite_queue;

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        float cur_time = glfwGetTime();
        delta = cur_time - prev_time;
        framecount++;

        jleg::game_render_loop();

        if (delta >= jleg::time_step){
            jleg::step();

            jleg::game_physics_loop();

            std::string title = "Jleg Window " + std::to_string(1.0 / delta);
            glfwSetWindowTitle(window, title.c_str());

            prev_time = cur_time;

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shader_program.activate();
            cam.matrix(shader_program, "proj");

            for (sprite* _sprite : sprites){
                drawer.draw(_sprite);
            };

            // node_queue.push_back(_graph.get_root_node()); //root
            // node_buffer.push_back(_graph.get_root_node()); //root
            //
            //
            // while (node_buffer.size() > 0){
            //     for (node* _child : node_buffer.back()->get_children()){
            //         node_queue.push_back(_child);
            //         node_buffer.insert(node_buffer.begin(), _child);
            //     };
            //     node_buffer.pop_back();
            // };

            // if (_child->renderable()){
            //     sprite* tmp = _child;
            //     sprite_queue.push_back(tmp);
            // };
            // node_queue.clear();
            // node_buffer.clear();

    		glfwSwapBuffers(window);
        };
    };
    shader_program.remove();
    glfwDestroyWindow(window);
    glfwTerminate();
};


int jleg::create_window(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "jleg window", NULL, NULL);

    if (window == NULL){
        glfwTerminate();
        return -1;
    };

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // shader shader_program("res/shaders/sprite.vert", "res/shaders/sprite.frag");
    shader_program = shader("res/shaders/sprite.vert", "res/shaders/sprite.frag");
    cam = camera(SCREEN_WIDTH / SCALE, SCREEN_HEIGHT / SCALE, vec2(0.0f, 0.0f));//mvoe to scene?

    // sprite_drawer drawer(&shader_program);
    drawer = sprite_drawer(&shader_program);

    return 0;
};
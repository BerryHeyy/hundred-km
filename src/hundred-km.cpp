#include <iostream>
#include <stdexcept>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>

#include "player.hpp"
#include "scene.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLFWwindow* window;

glm::mat4 projection;

int WIDTH = 800, HEIGHT = 600;

double delta_time = 0;

void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void init_glfw()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Hundred Kilometers", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL((GLADloadfunc) glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        WIDTH = width;
        HEIGHT = height;
        projection = glm::perspective(glm::radians(45.0f), ((float) WIDTH) / ((float) HEIGHT), 0.1f, 100.0f);
    });

    glfwSetCursorPosCallback(window, player::handle_mouse);
    glfwSetInputMode(window,  GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Opengl settings
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main()
{
    init_glfw();  

    stbi_set_flip_vertically_on_load(true);  

    Shader shader("resources/shader/test.vert", "resources/shader/test.frag");

    Scene world;
    world.set_position(0.0f, -1.0f, 0.0f);

    Model* test_model = new Model("test_cube.obj", &shader);
    Model* test_model2 = new Model("car.obj",  &shader);
    Model* road = new Model("road_curve_90deg_20m.obj", &shader);
    Model* grass_trees = new Model("grass_trees_1.obj", &shader);

    world.add_model(test_model);
    world.add_model(test_model2);
    world.add_model(road);
    world.add_model(grass_trees);

    test_model->set_position(0.0f, 2.0f, 0.0f);
    test_model2->set_position(0.0f, 0.5f, 0.0f);

    player::init(WIDTH, HEIGHT);
    
    projection = glm::perspective(glm::radians(45.0f), ((float) WIDTH) / ((float) HEIGHT), 0.1f, 100.0f);
    
    double last_frame = 0;
    while (!glfwWindowShouldClose(window))
    {
        // Poll and process events
        glfwPollEvents();
        process_input(window);

        // Clear color and depth buffers
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Drawin stuff
        test_model->set_rotation(test_model->get_rotation() + glm::vec3(0, glm::radians(-10.0f) * delta_time, 0));

        shader.use();
        shader.set_mat4("view", player::get_view_matrix());
        shader.set_mat4("projection", projection); // Find a way to only update projection matrix when window size changes
        shader.set_vec2("screen_size", glm::vec2(WIDTH, HEIGHT));

        player::update(window, delta_time);
        world.draw_scene();

        // Swap buffers
        glfwSwapBuffers(window);

        // Calculate delta time
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
    }

    delete test_model, test_model2, road, grass_trees;

    glfwTerminate();
    return 0;
}

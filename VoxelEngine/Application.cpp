#include "Application.h"



//static functions
static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    glm::vec2 mousePos((float)xpos, (float)-ypos);

    Camera::getInstance().updateMouse(mousePos);

}


Application::Application() {
	//constructor
}

Application::~Application() {
	//destructor
}

bool Application::initialize() {
    if (!glfwInit()) {
        return false;
    }

    //initialize glfw
    window = glfwCreateWindow(Camera::SCR_WIDTH, Camera::SCR_HEIGHT, "VoxelEngine", NULL, NULL);
    if (!window) {
        print("Failed to initialize GLFW");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    //initailize glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        print("Failed to initialize GLAD");
        glfwTerminate();
        exit(-1);
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Set clear color to gray

    //initialize block dictionaries
    Block::Initialize();

    //chunks
    Chunk chunk(glm::vec2(0, 0));
    chunk.loadBlocks();
    chunk.loadMesh();
    chunks.push_back(chunk);

    //setup shader
    shader.setShader("vertex.glsl", "fragment.glsl");

    //load textures
    if (texture.load("assets/atlas.png")) {
        print("Successfully loaded atlas");
    }
    else {
        print("ERROR::Cannot load atlas");
    }

    //setup mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    //glEanble
    glEnable(GL_DEPTH_TEST);

    return true;
}

void Application::run() {
    print("Running app...");
    if (!initialize())
        return;

    mainLoop();
    cleanup();

    print("Done");
}

void Application::mainLoop() {
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double dt = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(dt);


        shader.setInt("texture1", 0);
        shader.setVec3("viewPos", Camera::getInstance().camPos);


        std::cout << "Chunks in memory: " << chunks.size() << std::endl;
        //render new chunks
        int xPos = Camera::getInstance().camPos.x / Chunk::CHUNK_SIZE;
        int yPos = Camera::getInstance().camPos.z / Chunk::CHUNK_SIZE;
        bool chunkAdded = false;
        for (int x = -Chunk::CHUNK_RENDER_RADIUS + xPos; x <= Chunk::CHUNK_RENDER_RADIUS + xPos; x++) {
            for (int y = -Chunk::CHUNK_RENDER_RADIUS + yPos; y <= Chunk::CHUNK_RENDER_RADIUS + yPos; y++) {
                bool found = false;
                for (Chunk chunk : chunks) {
                    if (chunk.position.x == x && chunk.position.y == y)
                        found = true;
                }

                //add chunk & render, if it doesnt exist yet
                if (!found && !chunkAdded) {
                    Chunk chunk(glm::vec2(x, y));
                    chunk.loadBlocks();
                    chunk.loadMesh();
                    chunks.push_back(chunk);
                    chunkAdded = true;
                }
            }
        }

        for (Chunk chunk : chunks) {
            chunk.draw();
        }
        

        //draw
        shader.use();

        shader.setMat4("view", Camera::getInstance().getViewMatrix());
        shader.setMat4("projection", Camera::getInstance().projectionMatrix);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::print(const char* str) {
    std::cout << "APP::" << str << std::endl;
}
void Application::sprint(const char* str) {
    std::cout << "APP::" << str << std::endl;
}

void Application::cleanup() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::processInput(double dt)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    glm::vec3 moveDir(0, 0, 0);

    //movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        moveDir.z -= 1; // Move forward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        moveDir.z += 1; // Move backward
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        moveDir.x -= 1; // Move left
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        moveDir.x += 1; // Move right
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        moveDir.y += 1;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        moveDir.y -= 1;

    if (length(moveDir) > 0)
        Camera::getInstance().updatePos(normalize(moveDir), dt);
}

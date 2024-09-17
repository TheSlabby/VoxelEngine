#include "Application.h"
#include "Entity.h"



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

    glClearColor(0.529f, 0.808f, 0.922f, 1.0f); // Set clear color to sky blue

    //initialize block dictionaries
    Block::Initialize();

    srand(Chunk::SEED);


    //CLIENT SETUP
    //client.start();

    // setup player entity
    player.setPosition(glm::vec3(0, 100, 0));



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
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);


    //setup shader
    shader.setShader("vertex.glsl", "fragment.glsl");
    shadowShader.setShader("shadowVertex.glsl", "shadowFragment.glsl");
    shadowShader.setupShadowMap();

    //UI IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();


    return true;
}

void Application::run() {
    print("Running app...");
    if (!initialize())
        return;
    
    print("Initialized, starting main loop...");
    mainLoop();
    print("Cleaning up...");
    cleanup();

    print("Done");
}

void Application::mainLoop() {
    double lastTime = glfwGetTime();
    double lastFPS = 0;
    double lastChunkUpdate = glfwGetTime();
    double lastUIUpdate = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {


        double dt = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        // update all entities
        player.update(chunks, dt);

        // set camera pos to player entity position
        Camera::getInstance().camPos = player.getPosition();



        //prepare ui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        processInput(dt);



        //setup shaders
        shader.use();
        shader.setInt("texture1", 0);
        shader.setVec3("viewPos", Camera::getInstance().camPos);



        //std::cout << "Chunks in memory: " << chunks.size() << std::endl;
        //render new chunks
        int xPos = Camera::getInstance().camPos.x / Chunk::CHUNK_SIZE;
        int yPos = Camera::getInstance().camPos.z / Chunk::CHUNK_SIZE;
        if (glfwGetTime() - lastChunkUpdate > CHUNK_LOAD_RATE) 
        {
            lastChunkUpdate = glfwGetTime();
            bool chunkAdded = false;
            for (int x = -Chunk::CHUNK_RENDER_RADIUS + xPos; x <= Chunk::CHUNK_RENDER_RADIUS + xPos; x++) {
                for (int y = -Chunk::CHUNK_RENDER_RADIUS + yPos; y <= Chunk::CHUNK_RENDER_RADIUS + yPos; y++) {
                    bool found = false;
                    for (const auto& chunk : chunks) {
                        if (chunk->position.x == x && chunk->position.y == y)
                            found = true;
                    }

                    //add chunk & render, if it doesnt exist yet
                    if (!found && !chunkAdded) {
                        auto chunk = std::make_unique<Chunk>(glm::vec2(x, y));
                        chunk->loadAsync();
                        //chunk->loadMesh();
                        chunks.push_back(std::move(chunk));
                        chunkAdded = true;
                        break;
                    }
                }
            }
        }

        // check if chunks need rendering
        for (auto& chunk : chunks) {
            if (chunk->renderReady) {
                chunk->loadMesh();
                chunk->renderReady = false;
                //std::cout << "Render is ready" << std::endl;
            }
        }

        //clear unneeded chunks
        for (auto it = chunks.begin(); it != chunks.end();) {
            if (it->get()->position.x < xPos - Chunk::CHUNK_RENDER_RADIUS || it->get()->position.x > xPos + Chunk::CHUNK_RENDER_RADIUS ||
                it->get()->position.y < yPos - Chunk::CHUNK_RENDER_RADIUS || it->get()->position.y > yPos + Chunk::CHUNK_RENDER_RADIUS) {

                it = chunks.erase(it);
            }
            else {
                // Move to the next element
                ++it;
            }

            
        }

        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glCullFace(GL_BACK);

        //light space matrix
        glm::vec3 lightInvDir = glm::vec3(0.5f, 1.5, 1);
        glm::vec3 lightPos = Camera::getInstance().camPos + (lightInvDir * glm::vec3(10));
        // Compute the MVP matrix from the light's point of view
        glm::mat4 depthProjectionMatrix = glm::ortho<float>(-50, 50, -50, 50, -10, 50);
        glm::mat4 depthViewMatrix = glm::lookAt(lightPos, Camera::getInstance().camPos, glm::vec3(0, 1, 0));
        glm::mat4 depthModelMatrix = glm::mat4(1.0);
        glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;


        shadowShader.use();
        shadowShader.setMat4("lightSpaceMatrix", depthMVP);
        glViewport(0, 0, shadowShader.shadowWidth, shadowShader.shadowHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowShader.depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        // render to shadowmap
        for (auto& chunk : chunks) {
			chunk->draw();
		}

        shader.use();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_DEPTH_BUFFER_BIT);
        //glCullFace(GL_FRONT);
        glViewport(0, 0, Camera::SCR_WIDTH, Camera::SCR_HEIGHT);
        glScissor(0, 0, Camera::SCR_WIDTH, Camera::SCR_HEIGHT);
        shader.setInt("shadowMap", 1);
        shader.setInt("texture1", 0);
        shader.setMat4("view", Camera::getInstance().getViewMatrix());
        shader.setMat4("projection", Camera::getInstance().projectionMatrix);
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("lightDir", glm::normalize(- lightInvDir));
        shader.setMat4("lightSpaceMatrix", depthMVP);

        //render to screen now
        for (auto& chunk : chunks) {
            chunk->draw();
        }
        
        //render ui
        if (glfwGetTime() - lastUIUpdate > 1) {
            lastUIUpdate = glfwGetTime();
            lastFPS = 1.0 / dt;
        }
        glm::vec3 cameraPos = Camera::getInstance().camPos;

        if (!isMouseLocked)
        {
            ImGui::Begin("Camera Info", NULL, ImGuiWindowFlags_AlwaysAutoResize); // Begin a new window named "Camera Info"
            ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", cameraPos.x, cameraPos.y, cameraPos.z);
            ImGui::Text("Chunk Position: (%i, %i)", (int)cameraPos.x / Chunk::CHUNK_SIZE, (int)cameraPos.z / Chunk::CHUNK_SIZE);
            ImGui::Text("Chunks Loaded: (%i)", chunks.size());
            ImGui::Text("FPS: %.2f", lastFPS);

            // Add "FLY" button
            if (ImGui::Button("FLY")) {
                print("FLY button clicked!");
                // Implement your fly action here, for example, toggle a flying mode
                // player.toggleFlyMode(); // Example function call
            }


            ImGui::End();
        }


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        checkErrors();

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
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::checkErrors() {
    //check for errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        glfwSetWindowShouldClose(window, true);
        std::string errorString;
        switch (error) {
        case GL_INVALID_ENUM:
            errorString = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            errorString = "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            errorString = "GL_INVALID_OPERATION";
            break;
        case GL_STACK_OVERFLOW:
            errorString = "GL_STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            errorString = "GL_STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
            errorString = "GL_OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        default:
            errorString = "Unknown Error";
            break;
        }
        std::cout << "OpenGL Error: " << errorString << std::endl;
    }
}

// this should be in separate class but whatever
void Application::processInput(double dt)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == 1)
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
    /*if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        moveDir.y += 1;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)*/
        //moveDir.y -= 1;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        player.jump();

    if (length(moveDir) > 0)
        player.attemptMove(normalize(moveDir), Camera::getInstance().getCameraYaw(), chunks, dt);
        //Camera::getInstance().updatePos(normalize(moveDir), dt);


    //unlock mouse
    static bool keyWasPressed = false;
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        if (!keyWasPressed) {
            isMouseLocked = !isMouseLocked; // Toggle the state
            glfwSetInputMode(window, GLFW_CURSOR, isMouseLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
            //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            keyWasPressed = true;
        }
    }
    else {
        keyWasPressed = false;
    }

}

uint8_t Application::getBlockAt(int x, int y, int z) {
    for (auto& chunk : chunks) {
        int chunkX = chunk->position.x;
        int chunkY = chunk->position.y;
        if (x / chunk->CHUNK_SIZE == chunkX && z / chunk->CHUNK_SIZE == chunkY) {
            return chunk->blocks[x % chunk->CHUNK_SIZE][y][z % chunk->CHUNK_SIZE];
        }
    }

    return 0;
}


Application& Application::getInstance()
{
    static Application instance;
    return instance;

}
//static Application& Application::getInstance() {
//    static Application instance;
//    return instance;
//}
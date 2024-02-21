#include "OS.h"

namespace OS
{
    Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
    float lastX;
    float lastY;
    bool firstMouse;

    // timing
    float deltaTime;
    float lastFrame;

    unsigned int display_width = 800;
    unsigned int display_height = 600;

    glm::vec3 mouse_position = glm::vec3(0.0f, 0.0f, 0.0f);

	GLFWwindow *init()
	{
        // window settings
        display_width = 800;
        display_height = 600;

        // camera
        lastX = display_width / 2.0;
        lastY = display_height / 2.0;
        firstMouse = true;

        // timing
        deltaTime = 0.0f;
        lastFrame = 0.0f;

		// Initialize the OS
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(OS::display_width, OS::display_height, "LearnOpenGL", NULL, NULL);

    #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return nullptr;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Build atlas
        unsigned char* tex_pixels = nullptr;
        int tex_w, tex_h;
        io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_w, &tex_h);

        // Setup Dear ImGui style
        ImGui::StyleColorsLight();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return nullptr;
        }

        return window;
	}
    
    void update(GLFWwindow *window)
    {
        processInput(window);
        // per-frame time logic
        // --------------------
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ImGuiIO& io = ImGui::GetIO(); (void)io;

        mouse_position = glm::vec3(io.MousePos.x, io.MousePos.y, 0.0f);

        io.DisplaySize = ImVec2(1920, 1080);
        io.DeltaTime = 1.0f / 60.0f;
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        // render gui
        // ------
        gui_render();

        ImGui::Render();

        glfwGetFramebufferSize(window, (int*) & display_width, (int*) & display_height);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

bool isMouseDisabled = true;
bool oldMouseDisabled = false;

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (!isMouseDisabled) return;
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (OS::firstMouse)
    {
        OS::lastX = xpos;
        OS::lastY = ypos;
        OS::firstMouse = false;
    }

    float xoffset = xpos - OS::lastX;
    float yoffset = OS::lastY - ypos; // reversed since y-coordinates go from bottom to top

    OS::lastX = xpos;
    OS::lastY = ypos;

    OS::camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    OS::camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        OS::camera.ProcessKeyboard(FORWARD, OS::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        OS::camera.ProcessKeyboard(BACKWARD, OS::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        OS::camera.ProcessKeyboard(LEFT, OS::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        OS::camera.ProcessKeyboard(RIGHT, OS::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        if (oldMouseDisabled == isMouseDisabled) isMouseDisabled = !isMouseDisabled;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
    {
        if (oldMouseDisabled != isMouseDisabled)
		{
            oldMouseDisabled = isMouseDisabled;
			glfwSetInputMode(window, GLFW_CURSOR, isMouseDisabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		}
	}
}
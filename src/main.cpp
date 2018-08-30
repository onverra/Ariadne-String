#include "AlgSimple.h"
#include "Preview.h"
#include "SetupView.h"
#include "Solution.h"
#include "GenerationConfig.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <stdio.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <opencv2/opencv.hpp>

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

int main(int argc, char** argv)
{
    if(argc != 2) {
        std::cout << "Usage:\n\t" << argv[0] << " IMAGE" << std::endl;
        return 1;
    }
    // Setup window
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return 1;

#if __APPLE__
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac;
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Ariadne String", NULL, NULL);
	if (window == NULL)
		return 1;

    glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

						 // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
	bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
	bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
	bool err = gladLoadGL() != 0;
#endif
	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}

    // Setup ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();


    ImVec4 clear_color = ImColor(39, 40, 34);

    cv::Mat image = cv::imread(argv[1]);

    Solution solution(image);
    AlgSimple simple;
	Preview preview{ solution };
	SetupView setupView{solution};

    while (!glfwWindowShouldClose(window)) {
        if(!glfwGetWindowAttrib(window, GLFW_ICONIFIED) && glfwGetWindowAttrib(window, GLFW_VISIBLE)) {
            glfwPollEvents();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

            // Settings
            ImGui::BeginMainMenuBar();
            float menuHeight = ImGui::GetTextLineHeightWithSpacing();
            if(ImGui::BeginMenu("File")) {
                if(ImGui::MenuItem("Quit")) {
                    glfwSetWindowShouldClose(window, GL_TRUE);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();

            bool open = false;
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
            ImGui::SetNextWindowPos(ImVec2(0, menuHeight));
            ImGui::SetNextWindowSize(ImVec2(400, ImGui::GetIO().DisplaySize.y - menuHeight));
            if (ImGui::Begin("###main", &open, ImVec2(0, 0), 0.5f,
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
                             ImGuiWindowFlags_NoSavedSettings))
            {
                if (ImGui::TreeNodeEx("Solution", ImGuiTreeNodeFlags_DefaultOpen)) {
                    solution.draw();
                    ImGui::TreePop();
                }
                if (ImGui::TreeNodeEx("Algorithm", ImGuiTreeNodeFlags_DefaultOpen)) {
                    static int currentAlgorithm = 0;
                    ImGui::Combo("###AlgorithmCombo", &currentAlgorithm, "simple\0empty\0\0");
                    simple.draw();
                    ImGui::TreePop();
                }
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
                if (ImGui::Button("Calculate Pattern", ImVec2(ImGui::GetContentRegionAvailWidth(), 20))) {
                    simple.calculate(solution);
                }
            }
            ImGui::End();
            ImGui::PopStyleVar();

            // Preview
			preview.draw(solution);
			setupView.draw(solution);

            // Rendering
            int display_w, display_h;
            ImGui::Render();
			glfwMakeContextCurrent(window);
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			glfwMakeContextCurrent(window);
			glfwSwapBuffers(window);
        } else {
            glfwWaitEvents();
        }
    }

    // Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

    return 0;
}

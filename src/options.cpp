#include "options.hpp"


namespace Options{
    void init(Window* window){
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window->getGLFWWindowPtr(), true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    void drawData(){
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void render(){
            glGetError();
            glCreateShader(GL_VERTEX_SHADER);
            int error = glGetError();
            std::cout << error << std::endl;

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();

            ImGui::NewFrame();
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5)); 
            ImGui::SetNextWindowPos(ImVec2(10, 10));
            ImGui::Begin(
                "Options", 
                nullptr, 
                ImGuiWindowFlags_NoMove |          
                ImGuiWindowFlags_AlwaysAutoResize 
            );
            ImGui::Text("Select the generation and display options");
            ImGui::SliderFloat("Scale", &scale, 0.01f, 1.0f);
            ImGui::SliderFloat("Detaliztion", &detailiztion, 1.0f, 80.0f);
            ImGui::PushItemWidth(80); 
            ImGui::SliderInt("Heigh", &heigh, 5, 75);
            ImGui::SameLine();
            ImGui::PushItemWidth(80); 
            ImGui::SliderInt("Width", &width, 5, 75);
            ImGui::Checkbox("Draw only lines", &isOnlyLines);
            if (ImGui::Button("Regenerate")){
                isShouldRegen = true;
            }
            ImGui::End();
            ImGui::PopStyleVar(); 
            ImGui::Render();
    }

    void destroy(){
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }   

    float scale = 0.2f;
    float detailiztion = 4.0f;
    int heigh = 10;
    int width = 10;

    bool isOnlyLines = false;
    bool isShouldRegen = true;
}
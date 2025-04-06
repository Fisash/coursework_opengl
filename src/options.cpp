#include "options.hpp"
#include<iostream>

void Options::init(Window window){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.getGLFWWindowPtr(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void Options::drawData(){
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Options::render(){
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

void Options::destroy(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}   
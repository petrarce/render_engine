#define GLM_FORCE_RADIANS
#include "RenderContext.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

RenderContext::RenderContext() {


    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);

    m_Window = SDL_CreateWindow("Penis",
            0, 0,
            800, 600,
            SDL_WINDOW_OPENGL /*| SDL_WINDOW_RESIZABLE*/);

    SDL_GL_SetSwapInterval(1); // v-sync
    auto gl_context = SDL_GL_CreateContext(m_Window);
    //SDL_SetRelativeMouseMode(SDL_TRUE);

    glewExperimental = true;
    glewInit();

    std::vector<float> quad = {
       -1.0f,  1.0f,  0.0f, 1.0f,
       -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

       -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
     };

    m_Quad.createVertexArray<float>(quad);
    m_Quad.describeVertexArray(0,2,GlTypes::Float, 4, GlBool::False, 0);
    m_Quad.describeVertexArray(1,2,GlTypes::Float, 4, GlBool::False, 2);
}

int
mapToGl( PrimitiveType p) {
    const int lut[] = {
        GL_POINTS, GL_TRIANGLE_STRIP, GL_TRIANGLES
    };
    return lut[ static_cast<int>( p )];
}

int
mapToGl( StateType s) {
    const int lut[] = {
            GL_DEPTH_TEST, GL_STENCIL, GL_BLEND, GL_CULL_FACE };
    return lut[ static_cast<int>( s )];
}
int mapToGl ( DepthFunction d ) {
    const int lut[] = {
        GL_ALWAYS, GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL,
        GL_GREATER, GL_NOTEQUAL, GL_GEQUAL
    };
    return lut[ static_cast<int>( d )];
}

void 
RenderContext::enableSRGB(){
    glEnable(GL_FRAMEBUFFER_SRGB); 
}
void
RenderContext::enableDepthTest() {
    glEnable( GL_DEPTH_TEST );
}

void
RenderContext::disableDepthTest() {
    glDisable(GL_DEPTH_TEST );
}

void
RenderContext::writeToDepthBuffer(DepthFunction d) {
    glDepthMask(GL_TRUE);
    glDepthFunc(mapToGl(d));
}

void
RenderContext::readOnlyDepthBuffer() {
    glDepthMask(GL_FALSE); //wrong
}

void
RenderContext::setDepthFunction(DepthFunction d) {
    glDepthFunc ( mapToGl( d  ) );
}

void
RenderContext::clearColor(float r, float g, float b, float a) {
    glClearColor( r, g, b, a );
}
void
RenderContext::clearColorBuffer() {
    glClear(GL_COLOR_BUFFER_BIT);
}
void
RenderContext::clearDepthBuffer() {
    glClear(GL_DEPTH_BUFFER_BIT);
}

void
RenderContext::draw( VertexArray & VertexArray,
                     PrimitiveType p) {
    glDrawArrays( mapToGl( p ), 0, VertexArray.getSize() );
    glBindVertexArray( 0 );
}

void RenderContext::initGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(getWindow(), getContext());
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);

}

void RenderContext::updateGui(struct ProgramState& state)
{
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplSDL2_NewFrame(getWindow());
      ImGui::NewFrame();
      ImGui::ShowDemoWindow();
      ShowStatisticsWindow(state);
      ShowConfigsWidget(state);
      ImGui::Render();
}
void RenderContext::drawGui()
{
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void
RenderContext::drawIndex(PrimitiveType p, int size) {
    glDrawElements(mapToGl( p ), size, GL_UNSIGNED_INT, 0);
    glBindVertexArray( 0 );

}

/*void
RenderContext::drawDepthMap(Shader & shader, Texture & texture) {
    glViewport(0,0,800,600);
    shader.activate();
    texture.activate(0);
    clearDepthBuffer();
    m_Quad.bindVertexArray();
    draw(m_Quad,PrimitiveType::Triangle);
}
*/
void
RenderContext::swapBuffers() {
    SDL_GL_SwapWindow(m_Window);
}

void RenderContext::ShowStatisticsWindow(struct ProgramState& state)
{
      ImGui::Begin("Statistics");
      {
        static int latestTime = SDL_GetTicks();
        int dT = SDL_GetTicks() - latestTime;

        ImGui::BulletText("Framerate: %3.1f fps\n", (1000.0f * float(state.frame) / dT));
        if(dT > 1000){
          latestTime = SDL_GetTicks();
          state.frame = 0;
        }
      }
      ImGui::End();
}

void RenderContext::ShowConfigsWidget(struct ProgramState& state)
{
    ImGui::Begin("Configurations");
        ImGui::Text("Edit background");
        ImGui::SameLine(); HelpMarker("Configure background color"); ImGui::SameLine();
        ImGui::ColorEdit4("MyColor##3", (float*)&state.window.color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    
        ImGui::SliderFloat("Camera velocity", &state.camera.velocity, 0.001f, 0.01f, "%.3f m/s");

        ImGui::SliderFloat("Mouse sensitivity", &state.camera.sensitivity, 0.1f, 5.0f, "%.3f");

    ImGui::End();
}

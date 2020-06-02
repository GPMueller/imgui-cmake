#include <imgui/imgui.h>
#include <imgui_impl/glfw.h>
#include <imgui_impl/opengl3.h>
#include <imgui_impl/fonts.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <fmt/format.h>

#include <stdio.h>
#include <cmath>

static void glfw_error_callback( int error, const char * description )
{
    fmt::print( "Glfw Error {}: {}\n", error, description );
}

GLFWwindow * g_window;
ImVec4 clear_color       = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );
bool show_demo_window    = true;
bool show_another_window = false;

#ifdef __EMSCRIPTEN__
EM_JS( int, canvas_get_width, (), { return Module.canvas.width; } );

EM_JS( int, canvas_get_height, (), { return Module.canvas.height; } );

EM_JS( void, resizeCanvas, (), { js_resizeCanvas(); } );
#endif

static void show_menu_bar( GLFWwindow * window )
{
    if( ImGui::BeginMainMenuBar() )
    {
        if( ImGui::BeginMenu( "File" ) )
        {
            if( ImGui::MenuItem( "Open" ) )
            {
            }
            ImGui::EndMenu();
        }
        if( ImGui::BeginMenu( "Edit" ) )
        {
            if( ImGui::MenuItem( "Undo", "CTRL+Z" ) )
            {
            }
            if( ImGui::MenuItem( "Redo", "CTRL+Y", false, false ) )
            {
            } // Disabled item
            ImGui::Separator();
            if( ImGui::MenuItem( "Cut", "CTRL+X" ) )
            {
            }
            if( ImGui::MenuItem( "Copy", "CTRL+C" ) )
            {
            }
            if( ImGui::MenuItem( "Paste", "CTRL+V" ) )
            {
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void apply_charcoal_style( ImGuiStyle * dst = NULL )
{
    ImGuiStyle * style = dst ? dst : &ImGui::GetStyle();
    ImVec4 * colors    = style->Colors;

    colors[ImGuiCol_Text]                 = ImVec4( 1.000f, 1.000f, 1.000f, 1.000f );
    colors[ImGuiCol_TextDisabled]         = ImVec4( 0.500f, 0.500f, 0.500f, 1.000f );
    colors[ImGuiCol_WindowBg]             = ImVec4( 0.180f, 0.180f, 0.180f, 1.000f );
    colors[ImGuiCol_ChildBg]              = ImVec4( 0.280f, 0.280f, 0.280f, 0.000f );
    colors[ImGuiCol_PopupBg]              = ImVec4( 0.313f, 0.313f, 0.313f, 1.000f );
    colors[ImGuiCol_Border]               = ImVec4( 0.266f, 0.266f, 0.266f, 1.000f );
    colors[ImGuiCol_BorderShadow]         = ImVec4( 0.000f, 0.000f, 0.000f, 0.000f );
    colors[ImGuiCol_FrameBg]              = ImVec4( 0.160f, 0.160f, 0.160f, 1.000f );
    colors[ImGuiCol_FrameBgHovered]       = ImVec4( 0.200f, 0.200f, 0.200f, 1.000f );
    colors[ImGuiCol_FrameBgActive]        = ImVec4( 0.280f, 0.280f, 0.280f, 1.000f );
    colors[ImGuiCol_TitleBg]              = ImVec4( 0.148f, 0.148f, 0.148f, 1.000f );
    colors[ImGuiCol_TitleBgActive]        = ImVec4( 0.148f, 0.148f, 0.148f, 1.000f );
    colors[ImGuiCol_TitleBgCollapsed]     = ImVec4( 0.148f, 0.148f, 0.148f, 1.000f );
    colors[ImGuiCol_MenuBarBg]            = ImVec4( 0.195f, 0.195f, 0.195f, 1.000f );
    colors[ImGuiCol_ScrollbarBg]          = ImVec4( 0.160f, 0.160f, 0.160f, 1.000f );
    colors[ImGuiCol_ScrollbarGrab]        = ImVec4( 0.277f, 0.277f, 0.277f, 1.000f );
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4( 0.300f, 0.300f, 0.300f, 1.000f );
    colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4( 1.000f, 0.391f, 0.000f, 1.000f );
    colors[ImGuiCol_CheckMark]            = ImVec4( 1.000f, 1.000f, 1.000f, 1.000f );
    colors[ImGuiCol_SliderGrab]           = ImVec4( 0.391f, 0.391f, 0.391f, 1.000f );
    colors[ImGuiCol_SliderGrabActive]     = ImVec4( 1.000f, 0.391f, 0.000f, 1.000f );
    colors[ImGuiCol_Button]               = ImVec4( 1.000f, 1.000f, 1.000f, 0.000f );
    colors[ImGuiCol_ButtonHovered]        = ImVec4( 1.000f, 1.000f, 1.000f, 0.156f );
    colors[ImGuiCol_ButtonActive]         = ImVec4( 1.000f, 1.000f, 1.000f, 0.391f );
    colors[ImGuiCol_Header]               = ImVec4( 0.313f, 0.313f, 0.313f, 1.000f );
    colors[ImGuiCol_HeaderHovered]        = ImVec4( 0.469f, 0.469f, 0.469f, 1.000f );
    colors[ImGuiCol_HeaderActive]         = ImVec4( 0.469f, 0.469f, 0.469f, 1.000f );
    colors[ImGuiCol_Separator]            = colors[ImGuiCol_Border];
    colors[ImGuiCol_SeparatorHovered]     = ImVec4( 0.391f, 0.391f, 0.391f, 1.000f );
    colors[ImGuiCol_SeparatorActive]      = ImVec4( 1.000f, 0.391f, 0.000f, 1.000f );
    colors[ImGuiCol_ResizeGrip]           = ImVec4( 1.000f, 1.000f, 1.000f, 0.250f );
    colors[ImGuiCol_ResizeGripHovered]    = ImVec4( 1.000f, 1.000f, 1.000f, 0.670f );
    colors[ImGuiCol_ResizeGripActive]     = ImVec4( 1.000f, 0.391f, 0.000f, 1.000f );
    colors[ImGuiCol_PlotLines]            = ImVec4( 0.469f, 0.469f, 0.469f, 1.000f );
    colors[ImGuiCol_PlotLinesHovered]     = ImVec4( 1.000f, 0.391f, 0.000f, 1.000f );
    colors[ImGuiCol_PlotHistogram]        = ImVec4( 0.586f, 0.586f, 0.586f, 1.000f );
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4( 1.000f, 0.391f, 0.000f, 1.000f );
    colors[ImGuiCol_TextSelectedBg]       = ImVec4( 1.000f, 1.000f, 1.000f, 0.156f );
    colors[ImGuiCol_DragDropTarget]       = ImVec4( 1.000f, 0.391f, 0.000f, 1.000f );
    colors[ImGuiCol_NavHighlight]         = ImVec4( 1.000f, 0.391f, 0.000f, 1.000f );

    style->ChildRounding     = 4.0f;
    style->FrameBorderSize   = 1.0f;
    style->FrameRounding     = 2.0f;
    style->GrabMinSize       = 7.0f;
    style->PopupRounding     = 2.0f;
    style->ScrollbarRounding = 12.0f;
    style->ScrollbarSize     = 13.0f;
    style->WindowRounding    = 4.0f;
}

void loop()
{
#ifdef __EMSCRIPTEN__
    int width  = canvas_get_width();
    int height = canvas_get_height();

    glfwSetWindowSize( g_window, width, height );
#endif

    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    show_menu_bar( g_window );

    // 1. Show a simple window.
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
    {
        static float f     = 0.0f;
        static int counter = 0;
        ImGui::Text( "Hello, world!" );                // Display some text (you can use a format string too)
        ImGui::SliderFloat( "float", &f, 0.0f, 1.0f ); // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3( "clear color", (float *)&clear_color ); // Edit 3 floats representing a color

        ImGui::Checkbox( "Demo Window", &show_demo_window ); // Edit bools storing our windows open/close state
        ImGui::Checkbox( "Another Window", &show_another_window );

        if( ImGui::Button(
                "Button" ) ) // Buttons return true when clicked (NB: most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text( "counter = %d", counter );

        ImGui::Text(
            "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
            ImGui::GetIO().Framerate );
    }

    // 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
    if( show_another_window )
    {
        ImGui::Begin( "Another Window", &show_another_window );
        ImGui::Text( "Hello from another window!" );
        if( ImGui::Button( "Close Me" ) )
            show_another_window = false;
        ImGui::End();
    }

    // 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more
    // about Dear ImGui!
    if( show_demo_window )
    {
        ImGui::SetNextWindowPos(
            ImVec2( 650, 20 ), ImGuiCond_FirstUseEver ); // Normally user code doesn't need/want to call this because
                                                         // positions are saved in .ini file anyway. Here we just want
                                                         // to make the demo initial state a bit more friendly!
        ImGui::ShowDemoWindow( &show_demo_window );
    }

    ImGui::Render();

    int display_w, display_h;
    glfwMakeContextCurrent( g_window );
    glfwGetFramebufferSize( g_window, &display_w, &display_h );
    glViewport( 0, 0, display_w, display_h );
    glClearColor( clear_color.x, clear_color.y, clear_color.z, clear_color.w );
    glClear( GL_COLOR_BUFFER_BIT );

    ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

    glfwMakeContextCurrent( g_window );
    glfwSwapBuffers( g_window );
}

int init()
{
    glfwSetErrorCallback( glfw_error_callback );

    if( !glfwInit() )
    {
        fmt::print( "Failed to initialize GLFW\n" );
        return 1;
    }

    // glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); // We don't want the old OpenGL
    // glfwWindowHint( GLFW_DECORATED, false );
    // glfwWindowHint( GLFW_RESIZABLE, true );
#if __APPLE__
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
#endif

    // Open a window and create its OpenGL context
    int canvasWidth  = 800;
    int canvasHeight = 600;
    g_window         = glfwCreateWindow( canvasWidth, canvasHeight, "WebGui Demo", NULL, NULL );
    glfwMakeContextCurrent( g_window );
    glfwSwapInterval( 1 ); // Enable vsync

    if( g_window == NULL )
    {
        fmt::print( "Failed to open GLFW window.\n" );
        glfwTerminate();
        return -1;
    }

    gladLoadGL( (GLADloadfunc)glfwGetProcAddress ); // Initialize GLAD

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO & io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL( g_window, false );
    ImGui_ImplOpenGL3_Init();

    // Setup style
    // ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();
    apply_charcoal_style();

    // Load Fonts
    auto font_cousine = fonts::cousine_regular();
    // ImGui::PushFont( font_cousine );

    // Cursor callbacks
    glfwSetMouseButtonCallback( g_window, ImGui_ImplGlfw_MouseButtonCallback );
    glfwSetScrollCallback( g_window, ImGui_ImplGlfw_ScrollCallback );
    glfwSetKeyCallback( g_window, ImGui_ImplGlfw_KeyCallback );
    glfwSetCharCallback( g_window, ImGui_ImplGlfw_CharCallback );

#ifdef __EMSCRIPTEN__
    resizeCanvas();
#endif

    return 0;
}

void quit()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow( g_window );
    glfwTerminate();
}

extern "C" int main( int argc, char ** argv )
{
    if( init() != 0 )
        return 1;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop( loop, 0, 1 );
#else
    while( !glfwWindowShouldClose( g_window ) )
    {
        loop();
    }
#endif

    quit();

    return 0;
}
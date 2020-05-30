#include <imgui/imgui.h>
#include <imgui_impl/glfw.h>
#include <imgui_impl/opengl3.h>
#include <imgui_impl/fonts.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <stdio.h>
#include <cmath>
#include <iostream>

static void glfw_error_callback( int error, const char * description )
{
    fprintf( stderr, "Glfw Error %d: %s\n", error, description );
}

static bool drag_main_window = false;
static double wx_start, wy_start;
static double cx_start, cy_start;

static void show_menu_bar( GLFWwindow * window )
{
    if( !ImGui::IsMouseDown( 0 ) )
        drag_main_window = false;

    if( ImGui::BeginMainMenuBar() )
    {
        if( ImGui::BeginMenu( "File" ) )
        {
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

        ImGui::SameLine( ImGui::GetWindowContentRegionMax().x - 20 );
        if( ImGui::Button( "X" ) )
        {
            glfwSetWindowShouldClose( window, true );
        }

        if( ImGui::GetIO().WantCaptureMouse )
        {
            int wx_current, wy_current;
            glfwGetWindowPos( window, &wx_current, &wy_current );

            double cx_current, cy_current;
            glfwGetCursorPos( window, &cx_current, &cy_current );
            cx_current += wx_current;
            cy_current += wy_current;

            if( ImGui::IsWindowHovered( ImGuiHoveredFlags_None ) )
            {
                if( ImGui::IsMouseClicked( 0 ) )
                {
                    drag_main_window = true;
                    wx_start         = wx_current;
                    wy_start         = wy_current;
                    cx_start         = cx_current;
                    cy_start         = cy_current;
                }
            }

            if( ImGui::IsMouseDragging( 0 ) )
            {
                if( drag_main_window )
                {
                    glfwSetWindowPos(
                        window, int( wx_start + cx_current - cx_start ), int( wy_start + cy_current - cy_start ) );
                }
            }
        }

        ImGui::EndMainMenuBar();
    }
}

void ApplyCharcoalStyle( ImGuiStyle * dst = NULL )
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

int main()
{
    // Setup window
    glfwSetErrorCallback( glfw_error_callback );
    if( !glfwInit() )
        return 1;
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_DECORATED, false );
    glfwWindowHint( GLFW_RESIZABLE, true );
#if __APPLE__
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
#endif
    GLFWwindow * window = glfwCreateWindow( 1280, 720, "ImGui GLFW+OpenGL3 example", NULL, NULL );
    glfwMakeContextCurrent( window );
    glfwSwapInterval( 1 ); // Enable vsync

    gladLoadGL( (GLADloadfunc)glfwGetProcAddress );

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO & io = ImGui::GetIO();
    (void)io;

    ImGui_ImplGlfw_InitForOpenGL( window, true );
    ImGui_ImplOpenGL3_Init();

    // Setup style
    ImGui::StyleColorsLight();
    ApplyCharcoalStyle();

    auto font_cousine = fonts::cousine_regular();

    bool show_demo_window    = true;
    bool show_another_window = false;

    ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );

    // Main loop
    while( !glfwWindowShouldClose( window ) )
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        show_menu_bar( window );

        ImGui::PushFont( font_cousine );

        {
            static float f     = 0.0f;
            static int counter = 0;
            ImGui::Text( "Hello, world!" );
            ImGui::SliderFloat( "float", &f, 0.0f, 1.0f );
            ImGui::ColorEdit3( "clear color", (float *)&clear_color );

            ImGui::Text( "Windows" );
            ImGui::Checkbox( "Demo Window", &show_demo_window );
            ImGui::Checkbox( "Another Window", &show_another_window );

            ImGui::Text( "Font Samples" );
            ImGui::PushFont( font_cousine );
            ImGui::Text( "Font Render Test - Cousine: Bit Test.123" );
            ImGui::Text( "Font Render Test - Cousine: XXXXXXXXXXXX" );
            ImGui::PopFont();

            if( ImGui::Button( "Button" ) )
                counter++;
            ImGui::SameLine();
            ImGui::Text( "counter = %d", counter );

            ImGui::Text(
                "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate );
        }

        if( show_another_window )
        {
            ImGui::Begin( "Another Window", &show_another_window );
            ImGui::Text( "Hello from another window!" );
            if( ImGui::Button( "Close Me" ) )
                show_another_window = false;
            ImGui::End();
        }

        if( show_demo_window )
        {
            ImGui::SetNextWindowPos( ImVec2( 650, 20 ), ImGuiCond_FirstUseEver );
            ImGui::ShowDemoWindow( &show_demo_window );
        }

        ImGui::PopFont();

        // OpenGL rendering
        ImGui::Render();
        int display_w, display_h;
        glfwMakeContextCurrent( window );
        glfwGetFramebufferSize( window, &display_w, &display_h );
        glViewport( 0, 0, display_w, display_h );
        glClearColor( clear_color.x, clear_color.y, clear_color.z, clear_color.w );
        glClear( GL_COLOR_BUFFER_BIT );
        ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

        glfwMakeContextCurrent( window );
        glfwSwapBuffers( window );
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow( window );
    glfwTerminate();

    return 0;
}

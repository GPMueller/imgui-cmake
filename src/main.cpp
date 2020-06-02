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

GLFWwindow * g_window;
static ImVec4 clear_color       = ImVec4( 0.4f, 0.4f, 0.4f, 1.f );
static bool show_demo_window    = true;
static bool show_another_window = false;
static int selected_mode        = 1;

static bool dark_mode = true;

static bool drag_main_window = false;
static double wx_start, wy_start;
static double cx_start, cy_start;
static bool main_window_maximized = false;

static ImFont * font_14 = nullptr;
static ImFont * font_16 = nullptr;
static ImFont * font_18 = nullptr;

#ifdef __EMSCRIPTEN__
EM_JS( int, canvas_get_width, (), { return Module.canvas.width; } );
EM_JS( int, canvas_get_height, (), { return Module.canvas.height; } );
EM_JS( void, resizeCanvas, (), { js_resizeCanvas(); } );
#endif

static void glfw_error_callback( int error, const char * description )
{
    fmt::print( "Glfw Error {}: {}\n", error, description );
}

static void draw_gl( int display_w, int display_h )
{
    glViewport( 0, 0, display_w, display_h );
    glClearColor( clear_color.x, clear_color.y, clear_color.z, clear_color.w );
    glClear( GL_COLOR_BUFFER_BIT );
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
    colors[ImGuiCol_Tab]                  = colors[ImGuiCol_Header];
    colors[ImGuiCol_TabHovered]           = colors[ImGuiCol_HeaderHovered];
    colors[ImGuiCol_TabActive]            = colors[ImGuiCol_HeaderActive];
    colors[ImGuiCol_TabUnfocused]         = colors[ImGuiCol_Tab];
    colors[ImGuiCol_TabUnfocusedActive]   = colors[ImGuiCol_TabActive];
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

static void show_menu_bar( GLFWwindow * window )
{
    ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 7.f, 7.f ) );
    ImGui::PushFont( font_16 );

    if( ImGui::BeginMainMenuBar() )
    {
        // mainmenu_height = ImGui::GetWindowSize().y;
        ImGui::PopStyleVar();

        // ImGui::SameLine();
        if( ImGui::BeginMenu( "File" ) )
        {
            if( ImGui::MenuItem( "New" ) )
            {
            }
            if( ImGui::MenuItem( "Open", "Ctrl+O" ) )
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
        if( ImGui::BeginMenu( "View" ) )
        {
            if( ImGui::MenuItem( "Something" ) )
            {
            }
            ImGui::Separator();
            if( ImGui::MenuItem( "Fullscreen", "CTRL+SHIFT+F" ) )
            {
            }
            ImGui::EndMenu();
        }
        if( ImGui::BeginMenu( "Help" ) )
        {
            if( ImGui::MenuItem( "Keybindings", "F1" ) )
            {
            }
            if( ImGui::MenuItem( "About" ) )
            {
            }
            ImGui::EndMenu();
        }

        auto io            = ImGui::GetIO();
        auto & style       = ImGui::GetStyle();
        float font_size_px = 14;
        float right_edge   = ImGui::GetWindowContentRegionMax().x;
        float bar_height   = ImGui::GetWindowContentRegionMax().y + 2 * style.FramePadding.y;
        float width;


        width = 2.5 * font_size_px;
        ImGui::SameLine( right_edge - width );
        if( dark_mode )
        {
            if( ImGui::Button( ICON_FA_SUN, ImVec2( width, bar_height ) ) )
            {
                ImGui::StyleColorsLight();
                clear_color = ImVec4( 0.7f, 0.7f, 0.7f, 1.f );
                dark_mode   = false;
            }
        }
        else
        {
            if( ImGui::Button( ICON_FA_MOON, ImVec2( width, bar_height ) ) )
            {
                apply_charcoal_style();
                clear_color = ImVec4( 0.4f, 0.4f, 0.4f, 1.f );
                dark_mode   = true;
            }
        }
        right_edge -= ( width + style.FramePadding.x );

        std::string label;
        static int N = 4;
        for( int n = N; n > 0; n-- )
        {
            label = fmt::format( "Mode {}", n );
            width = label.length() * font_size_px;
            ImGui::SameLine( right_edge - width );
            if( ImGui::Selectable( label.c_str(), selected_mode == n, 0, ImVec2( width, bar_height ) ) )
                selected_mode = n;
            right_edge -= ( width + style.FramePadding.x );
        }

        ImGui::EndMainMenuBar();
    }
    ImGui::PopFont();
}

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.txt)
static void help_marker( const char * description )
{
    ImGui::TextDisabled( "(?)" );
    if( ImGui::IsItemHovered() )
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos( ImGui::GetFontSize() * 35.0f );
        ImGui::TextUnformatted( description );
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

static void show_overlay( bool * p_open )
{
    const float DISTANCE = 50.0f;
    static int corner    = 0;
    ImGuiIO & io         = ImGui::GetIO();
    if( corner != -1 )
    {
        ImVec2 window_pos = ImVec2(
            ( corner & 1 ) ? io.DisplaySize.x - DISTANCE : DISTANCE,
            ( corner & 2 ) ? io.DisplaySize.y - DISTANCE : DISTANCE );
        ImVec2 window_pos_pivot = ImVec2( ( corner & 1 ) ? 1.0f : 0.0f, ( corner & 2 ) ? 1.0f : 0.0f );
        ImGui::SetNextWindowPos( window_pos, ImGuiCond_Always, window_pos_pivot );
    }
    ImGui::SetNextWindowBgAlpha( 0.35f ); // Transparent background
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize
                                    | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing
                                    | ImGuiWindowFlags_NoNav;
    if( corner != -1 )
        window_flags |= ImGuiWindowFlags_NoMove;
    if( ImGui::Begin( "Example: Simple overlay", p_open, window_flags ) )
    {
        ImGui::Text( "Simple overlay\n"
                     "in the corner of the screen.\n"
                     "(right-click to change position)" );
        ImGui::Separator();
        if( ImGui::IsMousePosValid() )
            ImGui::Text( "Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y );
        else
            ImGui::Text( "Mouse Position: <invalid>" );
        if( ImGui::BeginPopupContextWindow() )
        {
            if( ImGui::MenuItem( "Custom", NULL, corner == -1 ) )
                corner = -1;
            if( ImGui::MenuItem( "Top-left", NULL, corner == 0 ) )
                corner = 0;
            if( ImGui::MenuItem( "Top-right", NULL, corner == 1 ) )
                corner = 1;
            if( ImGui::MenuItem( "Bottom-left", NULL, corner == 2 ) )
                corner = 2;
            if( ImGui::MenuItem( "Bottom-right", NULL, corner == 3 ) )
                corner = 3;
            if( p_open && ImGui::MenuItem( "Close" ) )
                *p_open = false;
            ImGui::EndPopup();
        }
    }
    ImGui::End();
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

    ImGui::PushFont( font_14 );

    show_menu_bar( g_window );
    bool p_open = true;
    show_overlay( &p_open );

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

    {
        static float f     = 0.0f;
        static int counter = 0;
        ImGui::Text( "Hello, world!" );
        ImGui::SliderFloat( "float", &f, 0.0f, 1.0f );
        ImGui::ColorEdit3( "clear color", (float *)&clear_color );

        ImGui::Text( "Windows" );
        ImGui::Checkbox( "Demo Window", &show_demo_window );
        ImGui::Checkbox( "Another Window", &show_another_window );


        if( ImGui::Button( "Button" ) )
            counter++;
        ImGui::SameLine();
        ImGui::Text( "counter = %d", counter );

        ImGui::Text(
            "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
            ImGui::GetIO().Framerate );
    }

    ImGui::PopFont();

    ImGui::Render();

    int display_w, display_h;
    glfwMakeContextCurrent( g_window );
    glfwGetFramebufferSize( g_window, &display_w, &display_h );

    draw_gl( display_w, display_h );

    ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

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

    glfwWindowHint( GLFW_SAMPLES, 4 ); // 4x antialiasing
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); // We don't want the old OpenGL
    // glfwWindowHint( GLFW_DECORATED, false );
    // glfwWindowHint( GLFW_RESIZABLE, true );
#if __APPLE__
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
#endif

    // Open a window and create its OpenGL context
    int canvasWidth  = 1280;
    int canvasHeight = 720;
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
    ImGuiIO & io   = ImGui::GetIO();
    io.IniFilename = "imgui_state.ini";

    ImGui_ImplGlfw_InitForOpenGL( g_window, false );
    ImGui_ImplOpenGL3_Init();

    // Setup style
    // ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();
    apply_charcoal_style();

    // Load Fonts
    font_14 = fonts::font_combined( 14 );
    font_16 = fonts::font_combined( 16 );
    font_18 = fonts::font_combined( 18 );

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
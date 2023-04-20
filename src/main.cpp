#define SDL_MAIN_HANDLED

#define GAME_NAME "[RHYTHM_GAME_TEST]"
#define GAME_VERSION "[0.8.0]"

#define FONT_PATH "assets/fonts/"
#define IMG_PATH "assets/images/"
#define SND_PATH "assets/sounds/"
#define SONG_PATH "assets/songs/"
#define SOUND_PATH "assets/sounds/"
#define NOTESKIN_PATH "assets/noteskins/"
#define SHADER_PATH "assets/shaders/"
#define MENU_PATH "assets/menu/"

#define NOTESKIN "Default"
#define PI 3.14159265
#define VSYNC true

#include <default/libs.h>
#include <default/variables.h>

#include <default/logger.cpp>
Logger *logger = new Logger;

#include <default/velocity.cpp>
Velocity *vel = new Velocity;

#include <default/functions.cpp>
#include <default/text.cpp>
#include <default/shaders.cpp>
#include <default/easing.cpp>

#include <rhythm/receptor.cpp>
#include <rhythm/note.cpp>
#include <rhythm/lane.cpp>

#include <rhythm/playfield.cpp>
std::vector<Playfield *> playfields;

#include <rhythm/inputs.cpp>
auto L = luaL_newstate();

#include <rhythm/conductor.cpp>
Conductor *conductor = new Conductor;

#include <modchart/mod.cpp>
#include <modchart/manager.cpp>

ModManager *manager = new ModManager;
#include <default/luabinds.cpp>

#include <states/State.cpp>
#include <states/GameState.cpp>
#include <states/CrashState.cpp>
#include <states/MainMenuState.cpp>
#include <states/SongSelectState.cpp>

State *state = NULL;

#include <objects/FPS.cpp>
#include <objects/Debug.cpp>

int main(int argc, char *argv[])
{
    // Clear the log file
    logger->Clear();
    logger->Log(TraceP, con("Started process: ", currentDateTime()));

    FPS *fps = new FPS;
    fps->fpsOriginRect = new GPU_Rect;
    fps->fpsOriginRect->x = 4;
    fps->fpsOriginRect->y = 4;
    fps->fpsOriginRect->w = 0;
    fps->fpsOriginRect->h = 0;

    Debug *debugTab = new Debug;
    debugTab->debugOriginRect = new GPU_Rect;
    debugTab->debugOriginRect->x = 4;
    debugTab->debugOriginRect->y = 4;
    debugTab->debugOriginRect->w = 0;
    debugTab->debugOriginRect->h = 0;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        // SDL failed to initialize
        logger->Log(FatalP, SDL_GetError());
    }
    else
    {
        if (TTF_Init() < 0)
        {
            // TTF failed to initialize
            logger->Log(FatalP, TTF_GetError());
        }

        window = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, flags);
        if (window == NULL)
        {
            // Window failed to initialize
            logger->Log(FatalP, SDL_GetError());
        }
        else
        {
            // Window initialized successfully
            GPU_SetInitWindow(SDL_GetWindowID(window));

            // Set window width and height
            window_width = WIDTH;
            window_height = HEIGHT;

            // MakeWindowTransparent(window, 0x00FF0000);
            GPU_SetPreInitFlags(GPU_INIT_DISABLE_VSYNC);

            // Create renderer
            renderer = GPU_InitRenderer(GPU_RENDERER_OPENGL_4, WIDTH, HEIGHT, NULL);
            GPU_SetShapeBlendMode(GPU_BLEND_NORMAL);
            GPU_ResetProjection(renderer);

            // Create a framebuffer
            framebuffer = GPU_CreateImage(WIDTH, HEIGHT, GPU_FORMAT_RGBA);
            GPU_LoadTarget(framebuffer);
            GPU_Clear(framebuffer->target);

            // Set display mode
            SDL_DisplayMode DM;
            SDL_GetCurrentDisplayMode(0, &DM);

            fps->Init();
            debugTab->Init();

            // Set screen width and height
            screen_width = DM.w;
            screen_height = DM.h;

            // Load keybinds
            std::ifstream file("assets/keybinds.txt");
            std::string str;

            while (std::getline(file, str))
            {
                keybinds.push_back(str);
            }

            // Load songs
            songs = getFolders(to_string(con(SONG_PATH, "/")));
        }
    }

    // Create temp & state variables
    temp = GPU_CopyImageFromTarget(framebuffer->target);

    bool switched = false;

    while (!quit)
    {
        if (curState < 0)
            curState = 0;

        switched = false;

        if (curState != prevState)
        {
            // Destroy previous state
            prevState = curState;
            if (state != NULL)
                state->Destroy();

            logger->Log(TraceP, "State changed");
            switched = true;

            // Create new state
            switch (curState)
            {
            case 0:
                state = static_cast<State *>(new MainMenuState);
                break;
            case 1:
                state = static_cast<State *>(new SongSelectState);
                break;
            case 2:
                state = static_cast<State *>(new GameState);
                break;
            case 3:
                state = static_cast<State *>(new CrashState);
                break;
            case 4:
                // state = static_cast<State *>(new OptionsState);
                break;
            case 5:
                quit = true;
                break;
            default:
                state = static_cast<State *>(new MainMenuState);
                break;
            }

            // Initialize state
            logger->Log(TraceP, con("State: ", state->GetName()));
            state->Initialize();
        }

        // Update deltatime
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = ((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
        deltaTime = round(deltaTime * 100) / 100;

        // Event handling
        SDL_Event e;
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                break;
            else if (!switched)
                state->HandleEvent(e);
        }

        // Reset renderer and framebuffer
        GPU_ClearRGBA(renderer, 0, 0, 0, 255);
        GPU_ClearRGBA(framebuffer->target, 0, 0, 0, 255);

        // Update ticks
        t = SDL_GetTicks() / 1000.0f;

        // Update state & fps counter
        if (!switched)
            state->Update();

        fps->Update();

        debugTab->debugOriginRect->x = fps->fpsOriginRect->x;
        debugTab->debugOriginRect->y = fps->fpsOriginRect->y + fps->fpsOriginRect->h + 4;

        debugTab->Update();

        // Fetch & render framebuffer
        temp = GPU_CopyImageFromTarget(framebuffer->target);
        GPU_SetAnchor(temp, 0.0f, 0.0f);

        auto [x, y, z] = framebufferRot; // NOT IMPLEMENTED

        GPU_Blit(temp, NULL, renderer, 0, 0);

        if (!switched)
            state->AfterBuffer();

        GPU_Flip(renderer);
        GPU_FreeImage(temp);

        // Update FPS counter data
        /*while (framerateUpdater >= 60)
        {
            GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc));

            char *game_name = con(con(GAME_NAME, " - "), GAME_VERSION);
            char *fps_text = con("FPS: ", to_string(ceil(framerate - 0.5)).c_str());
            char *memory_usage = con("Memory: ", to_string(pmc.WorkingSetSize / 1024 / 1024).c_str());
            char *delta_time = con("Delta time: ", to_string(deltaTime).c_str());
            char *current_state = con("Current state: ", state->GetName());

            char *combined_str = con(con(game_name, "\n\n"), con(con(fps_text, "\n"), con(memory_usage, "mb")));
            combined_str = con(con(combined_str, "\n"), delta_time);
            combined_str = con(con(combined_str, "\n"), current_state);

            // Add GPU info if debug mode is enabled
            if (debug)
            {
                char *debugTitle = "\n\n[DEBUG MODE]";

                char *gpu_info = con("\nVendor: ", (char *)vendorGl);
                char *renderer_info = con("\nGPU: ", (char *)rendererGl);

                combined_str = con(combined_str, debugTitle);
                combined_str = con(combined_str, gpu_info);
                combined_str = con(combined_str, renderer_info);
            }

            // Update counter & call for fps counter update on states
            fps->ChangeText(combined_str);
            if (!switched)
                state->FPSUpdate();

            framerateUpdater -= 60;
        }*/
    }

    // Clear variables
    GPU_FreeTarget(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL
    GPU_Quit();
    IMG_Quit();
    SDL_Quit();
}

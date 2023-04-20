bool debug = true;

float audio_pos = 0;
Mix_Music *backgroundMusic = NULL;
GPU_Image *framebuffer;
std::tuple<float, float, float> framebufferRot = {45, 45, 45};

int screen_width, screen_height;
float window_width, window_height;
int w, h;

float WIDTH = 1280.;
float HEIGHT = 720.;

std::vector<const char *> frags;
std::vector<const char *> verts;

Uint64 NOW = SDL_GetPerformanceCounter();
Uint64 LAST = 0;

double deltaTime = 0;
double BPM = 0;
float offset = 0;

SDL_Window *window = NULL;
GPU_Target *renderer = NULL;
GPU_Image *background = NULL;

GPU_Rect texr = {0, 0, WIDTH, HEIGHT};
GPU_Rect bgRect = {0, 0, WIDTH, HEIGHT};

float progress = 0.5;
GPU_Image *temp = NULL;

Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS;

std::vector<std::string> keybinds;
std::vector<std::string> songs;

float framerate = 0;
float framerateUpdater = 0;

float res[2] = {WIDTH, HEIGHT};
double t;

int curState = -1;
int prevState = -1;
std::string SONG_NAME = "Analys";

int randomSong = 0;
int menuSelected = 0;
std::vector<std::string> menuAudios;

bool quit = false;
bool isPaused = false;
int curJudgement = 0;

int notesHit = 0;
int totalNotes = 0;

std::vector<int> judgementCount = {0, 0, 0, 0, 0};
std::vector<int> judgementWindows = {22, 45, 90, 135, 180};
std::vector<std::string> judgementNames = {"Marvelous", "Perfect", "Great", "Good", "Bad", "Miss"};
std::vector<SDL_Color> judgementColors = {{255, 255, 255, 255}, {245, 209, 66, 255}, {69, 245, 66, 255}, {66, 138, 245, 255}, {245, 66, 66, 255}, {255, 0, 0, 255}};

double accuracy = 1.;
int misses = 0;
float ms = 0;
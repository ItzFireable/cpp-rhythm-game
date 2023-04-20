#include <sstream>
typedef uint32_t Uint32;

template <typename T>
std::string to_string(T);
Uint32 get_time();
float get_time_seconds();
Uint32 get_time_from_seconds(float);
float get_seconds_from_time(Uint32);
char *float_to_char(float);
char *con(const char *, const char *);
int get_rotation(int);
char *getTimer(double, double);
bool loadAudio(Mix_Music *, const char *, bool, int, int);
void audioFinished();
unsigned int power_two_floor(unsigned int);
bool MakeWindowTransparent(SDL_Window *, COLORREF);
float get_memory_usage();
char *currentDateTime();
std::vector<std::vector<std::string>> parse_chart(char *);
std::vector<std::vector<std::string>> parse_data(char *);
std::vector<std::string> getFolders(std::string);
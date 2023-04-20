#include <default/functions.h>
template <typename T>

// custom cpp tostring
std::string to_string(T value)
{
	std::ostringstream os;
	os << value;
	return os.str();
}

// get current time in milliseconds
Uint32 get_time()
{
	return SDL_GetTicks();
}

// get current time in seconds
float get_time_seconds()
{
	return get_time() / 1000.0f;
}

// get time in milliseconds
Uint32 get_time_from_seconds(float seconds)
{
	return seconds * 1000.0f;
}

// get time in seconds
float get_seconds_from_time(Uint32 time)
{
	return time / 1000.0f;
}

// convert float to char*
char *float_to_char(float f)
{
	std::string s = to_string(f);
	char *cstr = new char[s.length() + 1];
	strcpy(cstr, s.c_str());
	return cstr;
}

char *con(const char *first, const char *second)
{
	int l1 = 0, l2 = 0;
	const char *f = first, *l = second;

	while (*f++)
		++l1;
	while (*l++)
		++l2;

	char *result = new char[l1 + l2 + 1];

	for (int i = 0; i < l1; i++)
		result[i] = first[i];
	for (int i = l1; i < l1 + l2; i++)
		result[i] = second[i - l1];

	result[l1 + l2] = '\0';
	return result;
}

int get_rotation(int i)
{
	switch (i)
	{
	case 0:
		return 90;
		break;
	case 1:
		return 0;
		break;
	case 2:
		return 180;
		break;
	case 3:
		return -90;
		break;
	default:
		return 0;
	}
}

char *getTimer(double timePos, double timeLength)
{
	int timePosInt = (int)timePos;
	int timeLengthInt = (int)timeLength;

	int timePosMin = timePosInt / 60;
	int timePosSec = timePosInt % 60;

	int timeLengthMin = timeLengthInt / 60;
	int timeLengthSec = timeLengthInt % 60;

	char *timePosMinStr = new char[3];
	char *timePosSecStr = new char[3];
	char *timeLengthMinStr = new char[3];
	char *timeLengthSecStr = new char[3];

	if (timePosMin < 10)
		sprintf(timePosMinStr, "0%d", timePosMin);
	else
		sprintf(timePosMinStr, "%d", timePosMin);

	if (timePosSec < 10)
		sprintf(timePosSecStr, "0%d", timePosSec);
	else
		sprintf(timePosSecStr, "%d", timePosSec);

	if (timeLengthMin < 10)
		sprintf(timeLengthMinStr, "0%d", timeLengthMin);
	else
		sprintf(timeLengthMinStr, "%d", timeLengthMin);

	if (timeLengthSec < 10)
		sprintf(timeLengthSecStr, "0%d", timeLengthSec);
	else
		sprintf(timeLengthSecStr, "%d", timeLengthSec);

	char *result = con(con(timePosMinStr, ":"), con(timePosSecStr, con("/", con(timeLengthMinStr, con(":", timeLengthSecStr)))));

	return result;
}

void onSongEnd()
{
	switch (curState)
	{
	case 0:
	{
		logger->Log(TraceP, "Song ended");
		int curSongRand = randomSong;
		randomSong = rand() % menuAudios.size();

		// repeat until random song is different
		while (randomSong == curSongRand)
			randomSong = rand() % menuAudios.size();

		logger->Log(TraceP, "Playing song");
		break;
	}
	case 2:
		curState = 1;
		break;
	}
}

bool loadAudio(Mix_Music *var, const char *path, bool playAudio, int loops, int delay)
{
	if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	var = Mix_LoadMUS(path);
	if (var == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	if (playAudio)
	{
		if (delay)
			SDL_Delay(delay);
		Mix_PlayMusic(var, loops);
		Mix_HookMusicFinished(onSongEnd);
	}

	return true;
}

unsigned int power_two_floor(unsigned int val)
{
	unsigned int power = 2, nextVal = power * 2;
	while ((nextVal *= 2) <= val)
		power *= 2;
	return power * 2;
}

bool MakeWindowTransparent(SDL_Window *window, COLORREF colorKey)
{
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version); // Initialize wmInfo
	SDL_GetWindowWMInfo(window, &wmInfo);
	HWND hWnd = wmInfo.info.win.window;

	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	return SetLayeredWindowAttributes(hWnd, colorKey, 0, LWA_COLORKEY);
}

float get_memory_usage()
{
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	SIZE_T virtualMemUsedByMe = pmc.WorkingSetSize;
	return virtualMemUsedByMe / 1024.0 / 1024.0;
}

char *currentDateTime()
{
	std::time_t now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	return (char *)std::ctime(&now_time);
}

std::vector<std::vector<std::string>> parse_chart(char *filePath)
{
	std::ifstream file(filePath);
	std::string str;

	std::vector<std::vector<std::string>> notesTemp;

	while (std::getline(file, str))
	{
		// split string with comma
		// 0 is lane, 1 is strumTime

		std::vector<std::string> result;
		std::istringstream iss(str);
		std::string delim = ",";

		for (std::string token; std::getline(iss, token, ',');)
		{
			result.push_back(token);
		}

		notesTemp.push_back(result);
	}
	return notesTemp;
}

std::vector<std::vector<std::string>> parse_data(char *filePath)
{
	std::ifstream file(filePath);
	std::string str;

	std::vector<std::vector<std::string>> dataTemp;

	while (std::getline(file, str))
	{
		// split string with comma
		// 0 is lane, 1 is strumTime

		std::vector<std::string> result;
		std::istringstream iss(str);

		for (std::string token; std::getline(iss, token, '=');)
		{
			result.push_back(token);
		}

		dataTemp.push_back(result);
	}
	return dataTemp;
}

std::vector<std::string> getFolders(std::string folder)
{
	// do not use std::filesystem

	std::vector<std::string> folders;
	std::string path = folder;

	HANDLE dir;
	WIN32_FIND_DATA file_data;

	if ((dir = FindFirstFile((path + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return folders; /* No files found */

	do
	{
		const std::string file_name = file_data.cFileName;
		const std::string full_file_name = path + "/" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (is_directory && file_name[0] != '.')
		{
			folders.push_back(file_name);
		}
	} while (FindNextFile(dir, &file_data));

	FindClose(dir);
	return folders;
}
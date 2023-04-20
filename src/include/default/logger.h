enum LogPriority
{
	TraceP,
	DebugP,
	InfoP,
	WarnP,
	ErrorP,
	FatalP
};

class Logger
{
private:
	LogPriority verbosity = TraceP;
	const char *filepath = "logs/log.txt";

public:
	void SetVerbosity(LogPriority new_priority);
	void Clear();
	void Log(LogPriority priority, const char *message);
};
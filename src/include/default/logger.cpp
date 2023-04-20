#include <default/logger.h>

void Logger::SetVerbosity(LogPriority new_priority)
{
	verbosity = new_priority;
}
void Logger::Clear()
{
	std::ofstream ofs;
	ofs.open(filepath, std::ofstream::out | std::ofstream::trunc);
	ofs.close();
}
void Logger::Log(LogPriority priority, const char *message)
{
	if (!debug)
		return;
	if (priority >= verbosity)
	{
		std::ofstream FILE(filepath, std::ios_base::app);
		switch (priority)
		{
		case TraceP:
			FILE << "Trace:\t";
			break;
		case DebugP:
			FILE << "Debug:\t";
			break;
		case InfoP:
			FILE << "Info:\t";
			break;
		case WarnP:
			FILE << "Warn:\t";
			break;
		case ErrorP:
			FILE << "Error:\t";
			break;
		case FatalP:
			FILE << "Fatal:\t";
			break;
		}
		FILE << message << "\n";
		FILE.close();
	}
}
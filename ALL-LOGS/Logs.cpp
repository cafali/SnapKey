#include <iostream>
#include <fstream>
#include <ctime>
#include <filesystem>
#define _CRT_SECURE_NO_WARNINGS

char* CurrentTime()
{
    time_t rawTime;
    time(&rawTime);
    struct tm* localTime = localtime(&rawTime);
    char* timeString = asctime(localTime);

    // Remove newline from asctime result
    timeString[strcspn(timeString, "\n")] = '\0';

    return timeString;
}

bool WriteLog(std::string ErrorReason);

bool WriteToFile(std::string String, std::string FileName)
{
    char* CT = CurrentTime();

    std::fstream File(FileName, std::ios::app);
    if (!File.is_open())
    {
        std::cout << "Failed to open file" << std::endl;
        return false;
    }

    File << String << " : " << CT << std::endl;
    File.close();
    return true;
}

bool CheckLogFile()
{
    return std::filesystem::exists("SnapKeyLogs.txt");
}

bool CreateLogFile()
{
    std::ofstream CreateLog("SnapKeyLogs.txt");
    return CreateLog.is_open();
}

bool WriteLog(std::string ErrorReason)
{
    try
    {
        if (!CheckLogFile())   // FIXED — calling the function
        {
            CreateLogFile();
        }

        WriteToFile(ErrorReason, "SnapKeyLogs.txt");
        return true;
    }
    catch (const std::exception&)
    {
        std::cout << "FAILED TO WRITE LOG" << std::endl;
        return false;
    }
}


#include <windows.h>
#include <fstream>
#include <iostream>
#include <ctime>

void log(const char file[], int line, const char* text, ...)
{
    static char tmp_string[256];
    static char tmp_string2[256];
    static va_list ap;
    static bool is_first_call = true;


    const char* filename = strrchr(file, '\\') + 1;


    va_start(ap, text);
    vsprintf_s(tmp_string, 256, text, ap);
    va_end(ap);


    sprintf_s(tmp_string2, 256, "%s(%d): %s\n", filename, line, tmp_string);

    //enviar mensages por consola

    //abrir archivos
    std::ofstream logfile;
    if (is_first_call)
    {

        logfile.open("log.txt", std::ios_base::trunc);
        is_first_call = false;

        time_t rawtime;
        std::tm timeinfo;
        char buffer[128];

        std::time(&rawtime);
        localtime_s(&timeinfo, &rawtime);

        std::strftime(buffer, sizeof(buffer), "Project executed on %d-%m-%Y at %H:%M:%S.", &timeinfo);
        logfile << buffer << std::endl;
    }
    else
    {

        logfile.open("log.txt", std::ios_base::app);
    }
    if (!logfile.is_open())
    {

        std::cerr << "Failed to open log file!" << std::endl;
        return;
    }


    logfile << tmp_string2;
    logfile.close();
}
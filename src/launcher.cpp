#define JAVAPATH       L"jre\\bin\\java.exe"
#define ARGS           L"--class-path lib/*;* com.example.MyApplication"

#include <iostream>
#include <filesystem>
#include <vector>


#define BUFSIZE 512
wchar_t buf[BUFSIZE];


#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
    #include <unistd.h>

    #define POSIX_VERSION _POSIX_VERSION
    #define character_t char_t
    #define string_t std::string
#else
    #include <windows.h>
    #include <stdio.h>
    #include <shlwapi.h>
    #include <comdef.h>

    #define POSIX_VERSION 0
    #define character_t wchar_t
    #define string_t std::wstring
#endif

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"



void print(string_t string) {
    std::wcout << string << std::endl;
}

void print(character_t array[]) {
    auto string = string_t{array};
    spdlog::trace(array);
    spdlog::trace(string);
}

void goToExeDirectory() {
    // create path array with windows MAX_PATH size
    character_t path[MAX_PATH];
    // populate path with this exe path and returns path array populated size
    int len = GetModuleFileNameW(NULL, path, MAX_PATH);
    print(path);
    if (len > 0 && len < MAX_PATH) {
        string_t pathStr{path};
        string_t subpath = pathStr.substr(0, pathStr.find_last_of('\\'));
        std::filesystem::current_path(subpath.c_str());
    }
}

string_t cwd() {
    string_t cwd = std::filesystem::current_path();
    print(cwd);
    return cwd;
}

void assureExeDirectory() {
    cwd();
    goToExeDirectory();
    cwd();
}

int main(int argc, char* argv[], char* envp[]) {
    auto console = spdlog::stdout_color_mt("console");
    spdlog::set_default_logger(console);
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%n] [%^%l%$] [%s#%!:%#] [%P(%t)] - %v");

    SPDLOG_INFO("loggers can be retrieved from a global registry using the spdlog::get(logger_name)");
    SPDLOG_INFO("argc: {}", argc);
    for(int i = 0; i < argc; i++) {
        SPDLOG_TRACE("argv[{}]: {}", i, argv[i]);
    }
    for(size_t i = 0; envp[i] != NULL; i++) {
        SPDLOG_TRACE("envp[{}]: {}", i, envp[i]);
    }

    SPDLOG_WARN("HERE");
    assureExeDirectory();
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    LPCWSTR cmd = JAVAPATH;
    WCHAR args[] = JAVAPATH L" " ARGS;
    if (CreateProcessW(cmd, args, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi) == 0) {
        swprintf_s( buf, BUFSIZE, L"Could not run %s\nerror code: %lu\ncommand line: %s", cmd, GetLastError(), args );
        MessageBoxExW(NULL, buf, L"Message", MB_OK | MB_ICONERROR, 0);
        return 0;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD exitCode = 0;
    if (GetExitCodeProcess(pi.hProcess, &exitCode) != 0) {
        if (exitCode != 0) {
            swprintf_s( buf, BUFSIZE, L"%s\nfailed with exit code %lu\ncommand line: %s", cmd, exitCode, args );
            MessageBoxExW(NULL, buf, L"Message", MB_OK | MB_ICONERROR, 0);
        }
    }
    else {
        swprintf_s( buf, BUFSIZE, L"%s\nterminated with unknown exit code\ncommand line: %s", cmd, args );
        MessageBoxExW(NULL, buf, L"Message", MB_OK | MB_ICONERROR, 0);
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    // FIXME return exit code from java
    return exitCode;
}

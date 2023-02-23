#define JAVAPATH       L"jre\\bin\\java.exe"
#define ARGS           L"--class-path lib/*;* com.example.MyApplication"

#include <iostream>
#include <filesystem>

#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>
#define BUFSIZE 512
wchar_t buf[BUFSIZE];


void print(std::wstring string) {
    std::wcout << string << std::endl;
}

void print(wchar_t array[]) {
    std::wstring string{array};
    std::wcout << string << std::endl;
}


void cdToApplication() {
    // create path array with windows MAX_PATH size
	wchar_t path[MAX_PATH];
    // populate path with this exe path and returns path array populated size
	int len = GetModuleFileNameW(NULL, path, MAX_PATH);
    print(path);
	if (len > 0 && len < MAX_PATH) {
		std::wstring pathStr{path};
        std::wstring subpath = pathStr.substr(0, pathStr.find_last_of('\\'));
        print(subpath);
        SetCurrentDirectoryW(subpath.c_str());
	}
    std::wcout << std::filesystem::current_path();
}

int main() {
	cdToApplication();
	/*STARTUPINFOW si;
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
	CloseHandle(pi.hThread);*/
    // FIXME return exit code from java
	return 0;
}

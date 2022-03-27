// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "resource.h"

#include <memory>
#include <string>
#include <optional>
#include <cassert>

struct ThreadData {
    HMODULE hModule;
};

typedef std::basic_string<WCHAR> STDWSTRING;

class FileHandleWrapper {
public:
    FileHandleWrapper(HANDLE handle) noexcept : _handle{ handle } {}

    HANDLE operator *() const {
        return _handle;
    }

    BOOL operator !() const {
        return !_handle;
    }

    FileHandleWrapper(FileHandleWrapper &&wrapper) noexcept :
            _handle{ wrapper._handle } {
        wrapper._handle = NULL;
    }

    void reset() {
        if (!_handle) {
            return;
        }
        CloseHandle(_handle);
        _handle = NULL;
    }

    ~FileHandleWrapper() {
        reset();
    }

private:
    HANDLE _handle;

    FileHandleWrapper(const FileHandleWrapper &) = delete;
    FileHandleWrapper &operator=(const FileHandleWrapper &) = delete;
};

class FileData {
public:
    FileData(FileHandleWrapper &&hFile, STDWSTRING &&filename) :
            _hFile{ std::move(hFile) },
            _filename{ std::move(filename) } {}

    ~FileData() {
        if (!_hFile) {
            // If we get here, it means this object's data has been moved to
            // another object using move semantics.
            return;
        }
        _hFile.reset();
        DeleteFile(getFilenameAsLpcwstr());
    }

    LPCWSTR getFilenameAsLpcwstr() const {
        return _filename.c_str();
    }

    FileData(FileData &&) noexcept = default;
    FileData(const FileData &) = delete;
    FileData &operator=(const FileData &) = delete;

private:
    FileHandleWrapper _hFile;
    STDWSTRING _filename;
};

std::optional<FileData> WriteExe(HMODULE hModule, LPVOID pExe, DWORD szExe) {
    WCHAR wcModuleFilename[MAX_PATH + 1];
    const DWORD szModuleFilename = GetModuleFileName(
            hModule, wcModuleFilename, MAX_PATH);
    if (szModuleFilename == 0) {
        return std::nullopt;
    }
    const STDWSTRING moduleFilename{ wcModuleFilename };
    const size_t lastSlash = moduleFilename.find_last_of(L'\\');
    const STDWSTRING modulePath = moduleFilename.substr(0, lastSlash + 1);
    WCHAR tempFile[MAX_PATH + 1];
    const UINT uUnique = GetTempFileName(
            modulePath.c_str(), L"fps", 0, tempFile);
    if (uUnique == 0) {
        return std::nullopt;
    }
    FileHandleWrapper hFile{ CreateFile(tempFile, GENERIC_WRITE,
            FILE_SHARE_READ, NULL, CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL, NULL) };
    if (*hFile == INVALID_HANDLE_VALUE) {
        return std::nullopt;
    }
    DWORD nBytesWritten;
    const BOOL bSuccess = WriteFile(*hFile, pExe, szExe, &nBytesWritten, NULL);
    if (!bSuccess || (nBytesWritten != szExe)) {
        return std::nullopt;
    }
    FileHandleWrapper hFileRead{ CreateFile(tempFile, GENERIC_READ,
            FILE_SHARE_READ, NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, NULL) };
    if (!hFileRead) {
        return std::nullopt;
    }
    return FileData{ std::move(hFileRead), std::move(tempFile) };
}

DWORD ExecuteAndWaitForExe(const FileData &&exeData) {
    STARTUPINFO startupInfo;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);

    PROCESS_INFORMATION processInformation;
    ZeroMemory(&processInformation, sizeof(processInformation));

    // The string used to hold the command line cannot be const because the
    // CreateProcess function reserves the right to modify this string.
    WCHAR wcCommandLine[]{ L"module.exe /q" };

    const BOOL bSuccess = CreateProcess(
            exeData.getFilenameAsLpcwstr(),
            wcCommandLine,
            NULL,  // lpProcessAttributes
            NULL,  // lpThreadAttributes
            FALSE, // bInheritHandles
            0,     // dwCreationFlags
            NULL,  // lpEnvironment
            NULL,  // lpCurrentDirectory
            &startupInfo,
            &processInformation);
    if (!bSuccess) {
        return 1;
    }

    WaitForSingleObject(processInformation.hProcess, INFINITE);
    CloseHandle(processInformation.hProcess);
    CloseHandle(processInformation.hThread);
    return 0;
}

DWORD WINAPI ThreadProc(LPVOID lpParameter) {
    const std::unique_ptr<ThreadData> pData{
            reinterpret_cast<ThreadData *>(lpParameter)};
    const HRSRC hRes = FindResource(pData->hModule,
            MAKEINTRESOURCE(IDR_RCDATA1), RT_RCDATA);
    if (!hRes) {
        return 1;
    }
    const DWORD szExe = SizeofResource(pData->hModule, hRes);
    if (szExe == 0) {
        return 1;
    }
    const HGLOBAL hData = LoadResource(pData->hModule, hRes);
    if (!hData) {
        return 1;
    }
    const LPVOID pExe = LockResource(hData);
    if (!pExe) {
        return 1;
    }
    auto &&exeData = WriteExe(pData->hModule, pExe, szExe);
    if (!exeData) {
        return 1;
    }
    return ExecuteAndWaitForExe(std::move(*exeData));
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        ThreadData *const pData = new ThreadData{ hModule };
        const HANDLE hThread = CreateThread(
                NULL, 0, ThreadProc, pData, 0, NULL /*lpThreadId*/);
        if (!hThread) {
            return FALSE;
        }
    } break;
    case DLL_PROCESS_DETACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

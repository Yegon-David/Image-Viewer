@echo off
cmake --build --preset msvc-debug && (
    D:\pjojects\c++\projects\wx\Viewer\build\msvc\debug\src\Debug\viewer.exe
    ) || @echo build failed
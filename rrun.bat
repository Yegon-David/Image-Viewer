@echo off
rmdir /s /q build && (cmake --preset msvc-debug && (cmake --build --preset msvc-debug && (D:\pjojects\c++\projects\wx\Viewer\build\msvc\debug\src\Debug\viewer.exe) || @echo build failed) || @echo configure failed) || @echo rm build failed

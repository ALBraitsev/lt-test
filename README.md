# lt-test


# Сборка для Windows
cmake -S ./client -B ./build/client -A Win32 -DCMAKE_CONFIGURATION_TYPES=Release
cmake --build ./build/client --config Release
windeployqt ./build/client/Release

# lt-test

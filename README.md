# lt-test

# Сборка server
cmake -S server -B build/server 
cmake --build build/server


# Сборка client для Windows
cmake -S client -B build/client -A Win32 -DCMAKE_CONFIGURATION_TYPES=Release
cmake --build build/client --config Release
windeployqt build/client/Release

# lt-test

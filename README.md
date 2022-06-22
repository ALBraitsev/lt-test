# lt-test

# Сборка server
cmake -S server -B build/server 
cmake --build build/server

# Сборка client
cmake -S client -B build/client
cmake --build build/client

# Сборка qml-client
cmake -S qml-client -B build/qml-client
cmake --build build/qml-client

# Сборка client для Windows
cmake -S client -B build/client -A Win32 -DCMAKE_CONFIGURATION_TYPES=Release
cmake --build build/client --config Release
windeployqt build/client/Release


# настройка Linux
sudo apt-get install qt5-default
sudo apt-get install assistant
sudo apt-get install qtcreator
sudo apt-get install libclang-common-8-dev
sudo apt-get install qml-module-qtquick-controls
sudo apt-get install qml-module-qtquick-dialogs
sudo apt-get install -y qml-module-qt-labs-folderlistmodel
sudo apt-get install -y qml-module-qt-labs-settings

# lt-test

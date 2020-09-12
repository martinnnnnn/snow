if not exist "build" mkdir build
cd build
cmake -G "Visual Studio 16 2019" ..
cd ..
if not exist "snow.sln" mklink snow.sln build\snow.sln
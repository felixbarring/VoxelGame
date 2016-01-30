cd ..
cd ..
rm -r Eclipse
mkdir Eclipse
cd Eclipse
cmake -G "Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug ../librecraft
cd ../librecraft/scripts

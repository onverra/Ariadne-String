if not exist "..\3rdparty\glfw\build\" mkdir "..\3rdparty\glfw\build\"

pushd .
cd "..\3rdparty\glfw\build\"

cmake ..

popd

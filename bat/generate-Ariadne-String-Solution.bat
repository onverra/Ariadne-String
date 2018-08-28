if not exist "..\build\" mkdir "..\build"

pushd .
cd "..\build"

cmake -DOpenCV_DIR="3rdparty/opencv/build/"  ..

popd
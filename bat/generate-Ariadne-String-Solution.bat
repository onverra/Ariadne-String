if not exist "..\build\" mkdir "..\build"

pushd .
cd "..\build"

cmake -DOpenCV_DIR="../opencv/build/"  ..

popd

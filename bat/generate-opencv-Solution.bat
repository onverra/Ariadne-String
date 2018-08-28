if not exist "..\3rdparty\opencv\build\" mkdir "..\3rdparty\opencv\build\"

pushd .
cd "..\3rdparty\opencv\build\"

cmake ..

popd
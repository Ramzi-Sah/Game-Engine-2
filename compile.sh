cd build/make/

echo ---------------- Compiling ----------------
cmake ../../ || exit 1
make || exit 1
echo ------------ build successfull ------------

cd ../..
build/make/SAHEngine

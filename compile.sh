# create build directory
mkdir -p build
cd build

# cmake project
echo ---------------- Compiling ----------------
cmake .. || exit 1

# make project
make || exit 1
echo ------------ build successfull ------------

# execute program
cd ..
build/SAHEngine

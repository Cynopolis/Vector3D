# be in the root folder of this project when you run this
cd build/
ninja matrix-tests
echo "Running tests. This will take a while."
./unit-tests/matrix-tests -n "Timing Tests" -d yes > ../unit-tests/matrix-test-timings-temp.txt
cd ../unit-tests/
python3 test-timing-post-process.py
# Introduction
This matrix math library is focused on embedded development and avoids any heap memory allocation unless you explicitly ask for it.
It uses templates to pre-allocate matrices on the stack.

# Building
1. Initialize the repositiory with the command:
```bash
cmake -S . -B build -G Ninja
```

2. Go into the build folder and run `ninja`
3. That's it. You can test out the build by running `./unit-tests/matrix-tests`
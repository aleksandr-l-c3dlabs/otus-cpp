# OTUS C++ course homeworks

## Requirement

```shell
sudo apt-get install googletest clang-tidy clang-format python3-pip
pip install pre-commit
pre-commit install
```

## Build

```shell
cmake -S . -B build
cd build
cmake --build .
# cmake --build . -- -j4
```

## Test

```shell
ctest --test-dir build
```


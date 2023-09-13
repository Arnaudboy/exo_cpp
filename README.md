# Record reader

## How to build

```sh
cmake .
cmake --build .
```

For debug build:
```sh
cmake -DCMAKE_BUILD_TYPE=Debug .
cmake --build .
```

## Build and run

```sh
cmake --build . && ./record_reader -j 0 -f record_file_name
```
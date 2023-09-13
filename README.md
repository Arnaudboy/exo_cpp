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

To record data
```sh
cmake --build . && ./record_reader -r 1 -f record_file_name
```

To replay record
```sh
cmake --build . && ./record_reader -p 1 -j 0 -f record_file_name
```

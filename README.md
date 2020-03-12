# test_2

## srv
* `cd srv/build`
* `CC=clang CXX=clang++ cmake ../src`
* `cmake --build .`
* `./srv /tmp/socket`

## cli
* `cd cli/build`
* `CC=clang CXX=clang++ cmake ../src`
* `cmake --build .`
* `echo '{"key_1": "value_1", "key_2": []}' | ./cli /tmp/socket`

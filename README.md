Implemented basic operations (including `+-*/<<>>%`) for long integers.

## Usage

### `judge.py`

```bash
python3 judge.py --n ... --case_num ... --length ... --timeout ... --maxsize ...
```

See `judge.py` for detailed usage.

### cpp code

```bash
mkdir build
g++ main.cpp SuperLong.cpp --std=c++17 -o build/superlong
./build/superlong
```

## Cpp Code

`SuperLong.hpp`: Declaration of the class `SuperLong`;
`SuperLong.cpp`: Implementation of the class.

## Future Plan

- add divide and conquer method to accelerate `*`;
- add FFT implementation for `*`, and potentially for `/` and `%`;
- add benchmark.
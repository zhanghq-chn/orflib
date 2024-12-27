# ORFLIB

Michael G Sotiropoulos, 1-Sep-2024  
Haoqian Zhang, 17-May-2025

This repository contains the source code for the **orflib** quant library and its interfaces (Python, and Excel on Windows).

## Build

From the `orflib` project directory (the one that contains the top-level `CMakeLists.txt`):

```bash
mkdir -p build && cd build
cmake -DPLATFORM_TARGET=arm64 .. -DCMAKE_BUILD_TYPE=Release && cmake --build .
```

- On **Apple Silicon** use `-DPLATFORM_TARGET=arm64`; on **Intel macOS** use `-DPLATFORM_TARGET=x64`.
- On **Windows** use `-DPLATFORM_TARGET=x64` (or `x86` for 32-bit) from a build directory configured for your IDE or toolchain.

## Examples

Examples are provided under the **examples** folder:

- **Python**: `examples/Python/orflib-examples.py` and `examples/Python/orflib-examples.ipynb`

Run the Python examples after building the `pyorflib` extension and installing or placing the module so your Python environment can import `orflib`.

## Acknowledgments

Special thanks to **Prof. Michael Sotiropoulos** — ORF531 was one of the best courses I had at Princeton. 🎉🎉🎉

This is a wasm entrypoint for null0.

Put main.wasm along with main.py in your cart zip-file, and it will run it.

Here is an example:

```py
def load():
  print("Hello from python!")

def update(deltaTime):
  clear(0, RAYWHITE)
  draw_line(0, 10, 10, 100, 100, 2, RED)
```

## build

You don't need to compile it yourself, but if you want to, I used wasi-sdk, like this:

```
cmake -G Ninja -B build -DCMAKE_TOOLCHAIN_FILE=$WASI_SDK/share/cmake/wasi-sdk.cmake
cmake --build build

wasmtime --dir .::/ build/main.wasm
```

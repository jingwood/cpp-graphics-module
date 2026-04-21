# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

`libugm.a` — a static C++ library of 2D/3D graphics math primitives (vectors, matrices, colors, images, space partitioning trees). Consumed by sibling projects in `~/Projects/jingwood/`, notably `raygen-renderer`. All code lives under `src/ugm/` in namespace `ugm`.

## Build

Per-platform makefiles live in `build/{linux,mac-m,mac-nintel,windows}/`. There is no top-level build target — `cd` into the platform dir and run `make` / `make clean`. Output is `libugm.a` in the same directory.

```sh
cd build/mac-m && make          # Apple Silicon (arm64, clang++, -std=c++14)
cd build/mac-nintel && make     # Intel macOS
cd build/linux && make          # Linux (-std=c++11)
# build/windows/cpp-graphics-module/ holds the MSVC solution
```

There are **no tests, no linter config, and no CI**. Validate changes by rebuilding `libugm.a` *and* rebuilding a consumer (typically `../raygen-renderer/build/mac-m/`), since the library is header-heavy and template-heavy — many bugs only surface at instantiation sites in consumers.

## External dependency

Every makefile adds `-I../../../cpp-common-class/src`. The sibling repo `~/Projects/jingwood/cpp-common-class` (namespace `ucm`, library `libucm.a`) provides `ucm/types.h` (defines `byte`, `uint`, etc.) and `ucm/exception.h`. Pulled in by `basefun.h` and `image.h`. If a file says "ucm/types.h not found" or "undeclared identifier `byte`", the include path to `cpp-common-class` is the issue — **not** a problem with this repo.

`inc/` bundles `libpng` and `libjpeg-6b` headers used by `imgcodec.*`; the actual `.a` files are expected to be supplied by the consumer's link step (this library only references them).

## Architecture notes worth knowing up front

**Templated component types with unions.** `_vec2<T>`, `_vec3<T>`, `_color3<T>`, `_color4<T>` all use anonymous unions to alias `{x,y,z,w}` / `{r,g,b,a}` / `arr[N]` / sub-vector views onto the same storage. Concrete aliases are `vec3 = _vec3<float>`, `color4b = _color4<byte>`, etc. **Critical invariant:** every union member must have the same width as `T` for the given instantiation. Mixing a fixed-width member (e.g., the concrete `vec4` struct, which is float-only) into a template union silently inflates `sizeof` for other instantiations and breaks pointer arithmetic in `Image::getPixel` / `Image::setPixel`, which cast `buffer` to `color4b*` / `color4f*` and index by element. When touching these unions, verify `sizeof(color3b)==3`, `sizeof(color4b)==4`, `sizeof(color4f)==16`.

**Note:** `vec4` is a concrete `struct` (float-only), not a `_vec4<T>` template — unlike `vec3`. Don't assume symmetry between 3- and 4-component types.

**Umbrella header.** `ugm.h` re-exports everything and does `using namespace ugm;`. Consumers typically include individual headers (`vector.h`, `color.h`, etc.) rather than `ugm.h`.

**Space partitioning.** `kdtree.h`, `octree.h`, `spacetree.h`, `boxtree.h` are header-only templates (their `.cpp` files are empty — `ranlib` warns about this during build, which is expected). Used by `raygen-renderer` for ray/triangle acceleration.

**Image pipeline.** `Image` (raw buffer) → `imgcodec` (PNG/JPEG I/O) → `imgfilter` (blur/resize/compose). `Image` stores pixels as either `color3b`/`color3f`/`color4b`/`color4f` depending on `components` × `bitDepth`; `getPixel`/`setPixel` branch on that combination and cast the buffer accordingly.

# JWUtil

A collection of personal utilities used to ease C++ development, released as a small header-only library for C++.

## Overview

Currently the following modules are bundled with the library:

| Name     | File                      | Description      |
| -------- | ------------------------- | ---------------- |
| Logging  | `include/jw/logging.hpp`  | Fancy log output |
| ArgParse | `include/jw/argparse.hpp` | CLI arguments    |

## Usage

Add the library to the relevant search path. To use all the modules, use `#include <jw.hpp>`. To include single modules, use e.g. `#include <jw/logging.hpp>`.

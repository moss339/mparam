# MOSS Parameter System (mparam)

Type-safe parameter management system with shared memory storage and change notifications.

## Features

- **Type Safety**: Compile-time type checking for parameters
- **Namespace Support**: Hierarchical parameter organization
- **Change Notification**: Callback on parameter updates
- **Shared Memory Storage**: Persistent storage across processes
- **JSON Persistence**: Save/load parameters to/from JSON files

## Architecture

```
mparam/
├── include/mparam/
│   ├── mparam.h           # Main include
│   ├── param_store.h      # Parameter storage
│   ├── param_server.h     # Parameter server
│   └── param_client.h     # Parameter client
├── src/
│   ├── param_store.cpp
│   ├── param_server.cpp
│   └── param_client.cpp
└── test/                   # Unit tests
```

## Dependencies

- mlog (Logging)
- mshm (Shared Memory)

## Building

```bash
cd mparam
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DMPARAM_BUILD_TESTS=ON
make -j4
```

## Running Tests

```bash
ctest
```

## Usage Example

### Parameter Server

```cpp
#include <mparam/mparam.h>

int main() {
    moss::param::ParamServer server;

    // Declare parameters with types
    server.declare<int>("robot.max_speed", 100);
    server.declare<double>("robot.acceleration", 0.5);
    server.declare<std::string>("robot.name", "my_robot");

    // Set parameter
    server.set("robot.max_speed", 150);

    // Get parameter
    auto speed = server.get<int>("robot.max_speed");

    // Persist to JSON
    server.save("/path/to/params.json");

    server.start();
    return 0;
}
```

### Parameter Client

```cpp
#include <mparam/mparam.h>

int main() {
    moss::param::ParamClient client;

    // Get parameter
    auto speed = client.get<int>("robot.max_speed");

    // Set parameter (synchronously)
    client.set("robot.max_speed", 200);

    // Watch for changes
    client.watch("robot.max_speed", [](int new_value) {
        printf("max_speed changed to %d\n", new_value);
    });

    client.start();
    return 0;
}
```

## Parameter Types

Supported parameter types:
- `int`, `int64_t`
- `unsigned`, `uint64_t`
- `float`, `double`
- `std::string`
- `std::vector<uint8_t>` (binary data)

## Shared Memory

Parameters are stored in shared memory under `/mparam` namespace, allowing:
- Cross-process parameter access
- Atomic updates
- Watch callbacks via mshm notifications

## License

MIT License

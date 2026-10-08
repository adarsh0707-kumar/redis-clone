# Redis-Style TCP Server Prototype

A small **C++17 TCP server prototype** built from scratch to explore socket programming, client handling, and the foundations of a Redis-like network service.

> **Status:** Early educational prototype — **not Redis-compatible** and not a functional key-value database yet.

## What is implemented

The current code intentionally focuses on the networking layer:

- Creates a TCP server with POSIX sockets.
- Listens on port `6379`.
- Accepts multiple client connections.
- Handles each connected client on a detached `std::thread`.
- Reads incoming bytes from the socket.
- Logs received data to stdout.
- Sends a fixed `OK\\n` response.
- Detects client disconnects and closes the socket.

### Current request flow

```text
TCP Client
    │
    ▼
socket / bind / listen
    │
    ▼
accept()
    │
    ▼
per-client std::thread
    │
    ▼
read() incoming bytes
    │
    ▼
log request
    │
    ▼
send("OK\\n")
```

This is a **networking foundation**, not yet a Redis protocol implementation.

## What is not implemented

The repository does **not** currently provide:

- RESP/RESP2 protocol parsing
- `PING`, `GET`, `SET`, `DEL`, `EXISTS`, or other Redis commands
- An in-memory key-value data structure
- Expiration or TTL handling
- Persistence, snapshots, or append-only logging
- Redis-compatible clients
- Authentication or TLS
- Request limits or rate limiting
- Graceful server shutdown
- Automated tests
- Performance benchmarks

The repository should therefore not be described as a Redis replacement or Redis-compatible server.

## Architecture

The implementation is deliberately small:

| Layer | Current implementation |
|---|---|
| Transport | POSIX TCP sockets |
| Listener | `socket()` → `bind()` → `listen()` → `accept()` |
| Concurrency | One detached `std::thread` per client |
| Request handling | Raw bytes read with `read()` |
| Response | Fixed `OK\\n` payload |
| Storage | None |
| Protocol | Raw TCP bytes; no RESP framing/parsing |
| Language | C++17 |

### Concurrency model

Each accepted connection is handed to a detached thread. This keeps the prototype simple and demonstrates basic concurrent client handling, but it is not an appropriate scalability strategy for a production Redis-style server.

## Build and run

Requirements:

- Linux or another POSIX-like environment
- g++
- C++17
- pthread support
- GNU Make

Build:

```bash
make
```

Run:

```bash
./redis
```

The server listens on TCP port `6379`.

### Basic socket check

With the server running, use netcat from another terminal:

```bash
printf 'PING\\n' | nc 127.0.0.1 6379
```

Expected response:

```text
OK
```

This only verifies TCP connectivity and the prototype response path. It does **not** demonstrate Redis `PING` semantics.

Clean the build:

```bash
make clean
```

## Project structure

```text
redis-clone/
├── include/
│   └── server.h
├── src/
│   ├── main.cpp
│   └── server.cpp
├── Makefile
└── README.md
```

## Engineering focus

This project is useful as a compact exercise in:

- POSIX socket lifecycle
- TCP client/server programming
- Blocking I/O
- Connection handling
- Thread-per-client concurrency
- Incrementally building a network protocol
- Separating transport concerns from future command/storage layers

The small codebase also makes it a good starting point for experimenting with protocol parsing, concurrent data structures, and benchmark-driven optimization.

## Limitations and safety

The server currently binds to `INADDR_ANY`, so it may accept connections from network interfaces beyond localhost depending on host firewall configuration.

**Do not expose this prototype to an untrusted network.** It has no authentication, encryption, request limits, or production-grade error handling.

The current implementation also does not check the return values of operations such as `bind()` and `listen()`, so startup failures are not handled robustly.

## Roadmap

If this project is continued, the implementation path is:

1. Add explicit connection/error handling.
2. Implement RESP2 parsing and response encoding.
3. Add a command dispatcher.
4. Implement `PING`.
5. Add thread-safe in-memory `SET` / `GET` storage.
6. Add `DEL`, `EXISTS`, and TTL/expiration.
7. Add request framing and protocol-level validation.
8. Add automated unit and integration tests.
9. Add persistence and recovery experiments.
10. Replace thread-per-client with an event-driven I/O model if benchmarks justify it.
11. Add reproducible benchmarks for throughput, latency, and concurrent clients.
12. Compare behavior and performance against a real Redis instance.

## Why this project exists

The goal is to build the system **from the socket layer upward** rather than starting with a framework or existing server implementation.

The current milestone is intentionally small: establish reliable TCP connectivity first, then grow the protocol, storage engine, concurrency model, and persistence layer incrementally.

## License

See [LICENSE](LICENSE) if a license file is added to the repository.

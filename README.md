# TW-Mailer

A socket-based client-server email application implemented in C++ for Linux systems as part of the class "Grundlagen verteilter Systeme".

The project consists of a client application (`twmailer-client`) and an iterative server application (`twmailer-server`) that communicate over TCP stream sockets using a custom plain-text protocol.

## Features

- **Socket Communication:** Reliable TCP client-server messaging.
- **Mail Spooler:** File-based persistence directory for managing inbox messages per user.
- **Custom Text Protocol:** Line-delimited and multi-line message passing ending with `.\n`.
- **Iterative Server:** Synchronous request handling and robust error logging.
- **Resource Management:** Clean memory management and network socket lifecycle.

## Building the Project

The project includes a `Makefile` supporting standard build targets.

```bash
# Build both client and server executables
make

# Clean build artifacts
make clean
```

This generates two executable binaries: `twmailer-client` and `twmailer-server`.

## Usage

### 1. Starting the Server

Run the server by providing a listening port and a path to the mail spool directory:

```bash
./twmailer-server <port> <mail-spool-directoryname>
```

*Example:*
```bash
./twmailer-server 8080 ./mail_spool
```

### 2. Running the Client

Connect to the server by providing its IP address and port:

```bash
./twmailer-client <ip> <port>
```

*Example:*
```bash
./twmailer-client 127.0.0.1 8080
```

---

## Protocol Specification

Communication uses plain text delimited by `\n`. Multi-line content ends with a dot on a single line (`.\n`).

### Supported Commands

| Command | Request Format | Server Response | Description |
| :--- | :--- | :--- | :--- |
| **SEND** | `SEND\n<Sender>\n<Receiver>\n<Subject>\n<Message>\n.\n` | `OK\n` or `ERR\n` | Sends a message to a user. Subject max 80 chars; user max 8 chars. |
| **LIST** | `LIST\n<Username>\n` | `<count>\n<subject 1>\n...` | Lists the number of inbox messages and their subjects for a user. |
| **READ** | `READ\n<Username>\n<Message-Number>\n` | `OK\n<Message Content>` or `ERR\n` | Fetches and displays a specific message by index. |
| **DEL**  | `DEL\n<Username>\n<Message-Number>\n`  | `OK\n` or `ERR\n` | Deletes a specific message by index. |
| **QUIT** | `QUIT\n` | *(No response)* | Closes the connection. |

## 👥 Authors

Developed as part of a two-person group project at **FH Technikum Wien**.

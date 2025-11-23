<div align="center">

# 🐚 oh-my-shells

The offline, CLI-first swiss army knife for shell payloads! 🚀

[![Version](https://img.shields.io/badge/version-v0.0.4-blue.svg)](#)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](#)
[![Language](https://img.shields.io/badge/language-C-cyan)](#)
[![Downloads](https://img.shields.io/github/downloads/ordinary-hacker/oh-my-shells/total.svg)](#)

</div>

---

## ❓ Why, What, How?

For a quick overview of what the tool does, how it works, and why it exists, see [this blog post](https://0rd1n4ry.bearblog.dev/oh-my-shells).

---

## ✨ Features

- 🔍 **Search** and filter hundreds of shell payloads by OS, type, protocol, and language
- 📝 **Generate** ready-to-use payloads and listeners for your target
- 📋 **Show** detailed info for each shell
- 🖨️ **Colorful output** (with `--no-color` option for scripts)
- 🏷️ **Offline**: No internet required
- 🛠️ **Easy to extend** with your own payloads (just add TOML files!)
- 🆓 **Open Source** and hackable

---

## 🚦 Usage

```sh
./oh-my-shell <command> [OPTIONS]
```

### Commands

- `list` (alias: `l`) - List payloads with optional filters
- `search <term>` (alias: `sr`) - Search for payloads
- `show <shell_id>` (alias: `sh`) - Show details about a specific payload
- `generate <shell_id>` (alias: `g`) - Generate a payload

### Options

- `-i, --id <shell_id>`        Shell identifier
- `-s, --search <term>`        Search term
- `-H, --lhost <host>`         Local host for payload generation
- `-P, --lport <port>`         Local port (default: 4444)
- `-S, --shell <shell>`        Shell to use for payloads
- `-e, --encoding <type>`      Output encoding (url, base64, etc.)
- `-l, --listener`             Output only the recommended listener
- `-p, --payload`              Output only the generated payload
- `--no-color`                 Disable ANSI color output
- `--version`                  Show version information
- `-h, --help`                 Show help message

### Filters (for `list`)

- `--os <os>`                  Filter by OS (bsd, linux, osx, unix, windows)
- `--type <type>`              Filter by type (reverse, bind)
- `--proto <proto>`            Filter by protocol (tcp, udp, http)
- `--lang <lang>`              Filter by language (ruby, go, python, etc.)

---

## 🧑‍💻 Examples

```sh
./oh-my-shell list --os linux --type reverse
./oh-my-shell l --os linux --type reverse    # Using alias
./oh-my-shell search "netcat"
./oh-my-shell sr "netcat"                    # Using alias
./oh-my-shell show bash_read_line
./oh-my-shell sh bash_read_line              # Using alias
./oh-my-shell generate bash_i -H 192.168.1.100 -P 4444
./oh-my-shell g bash_i -H 192.168.1.100 -P 4444  # Using alias
./oh-my-shell --version
```

---

## 📦 Adding Your Own Payloads

Just drop a new `.toml` file in the appropriate `shells/` subdirectory. The tool will pick it up automatically!

---

## 🛠️ Building

**Requires**: `make` and any C compiler (e.g. `gcc`, `clang`, `cc`, ...).

```sh
make
```

---

## 📥 Installing & Updating

### Installer Script

Just run the following command, note you need to have `make` and a C compiler installed:

```sh
curl -fsSL https://raw.githubusercontent.com/ordinary-hacker/oh-my-shells/trunk/scripts/install.sh | sudo bash
```

### Prebuilt Binaries

Or, otherwise you can use one of the precompiled binaries available at `release/builds/` and in the latest release.
Just make sure you have both the binary and the `shells/` directory in the same folder. The latest release also contains a `.zip` and `.tar.gz` of the `shells/` directory for easier install.

### Verify installation

Afterwards, no matter the method of installation,  you can just run the tool as:

```sh
oh-my-shells -h
```

---

## 🧪 Testing

The project includes a full suite of unit, integration, and black-box tests.

This comprehensive amount of tests is available in `tests/`, all of them using the [Unity Test Framework](https://github.com/ThrowTheSwitch/Unity) at `lib/unity/`.

Here's a look at the structure of tests:
* **White Box Tests (Unit Tests)**
  - `test_cli.c`: Tests CLI arguments parsing and command structure initialization
  - `test_module_loader.c`: Tests module loading, registry management, and TOML parsing
* **Integration Tests**
  - `test_integration.c`: Tests complete workflows and component interactions
* **Black Box Tests**
  - `test_blackbox.c`: Tests the binary as an external user would, testing all commands and edge cases

### Running Tests

Running all tests:
```bash
make test
```

Running individual test suites:
```bash
make run-test-cli
make run-test-module-loader
make run-test-integration
make run-test-blackbox
```

Building tests without running them:
```bash
make build-tests
```

### Tests Coverage

The whole test suite covers:
- CLI argument parsing and validation
- Module registry initialization and management
- TOML file loading and parsing
- Shell module search and filtering
- Payload generation functionality
- All command-line commands (list, search, show, generate)
- Error handling and edge cases
- Command aliases and flag combinations
- Integration between components

### Expected Results

These tests should always pass with 0 failures. In total the test suite includes:
- 8 CLI tests
- 5 Module loader tests
- 6 Integration tests
- 13 Black box tests

Giving us a **total of 32 tests**.

---

## ©️ Credits

**Author and maintainer**: [ordinary-hacker](https://github.com/ordinary-hacker)

This tool depends on and includes a vendored version of [tomlc17](https://github.com/cktan/tomlc17) by [cktan](https://github.com/cktan).

There's also a vendored version of the [Unity Test Framework](https://github.com/ThrowTheSwitch/Unity) by [ThrowTheSwitch](https://github.com/ThrowTheSwitch) used for testing.

---

## 📝 License

MIT License. See [LICENSE](LICENSE) for details.

---

<div align="center">

Made with ❤️ by hackers, for hackers.

</div>

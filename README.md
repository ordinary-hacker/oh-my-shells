<div align="center">

# 🐚 oh-my-shells

The offline, CLI-first swiss army knife for shell payloads! 🚀

[![Version](https://img.shields.io/badge/version-v0.0.4-blue.svg)](#)

</div>

---

## ❓ Why, What, How?

To learn about why use this tool, and overview of what it is, and how it works, you can read my little article about it [here](https://0rd1n4ry.bearblog.dev/oh-my-shells).

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

- `list` (alias: `l`) — List payloads with optional filters
- `search <term>` (alias: `sr`) — Search for payloads
- `show <shell_id>` (alias: `sh`) — Show details about a specific payload
- `generate <shell_id>` (alias: `g`) — Generate a payload

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

```sh
make
```

---

## 📥 Installing & Updating

Just run the following command, note you need to have `make` installed:

```sh
curl -fsSL https://raw.githubusercontent.com/ordinary-hacker/oh-my-shells/trunk/scripts/install.sh | sudo bash
```

Or, otherwise you can use one of the precompiled binaries available at `release/builds/` and in the latest release.
Just make sure you have both the binary and the `shells/` directory in the same directory. The latest release also contains a `.zip` and `.tar.gz` of the `shells/` directory for easier install.

Afterwards you can just run the tool as:

```sh
oh-my-shells -h
```

---

## ©️ Credits

**Author and maintainer**: [ordinary-hacker](https://github.com/ordinary-hacker)

This tool depends on and includes a vendored version of [tomlc17](https://github.com/cktan/tomlc17) by [cktan](https://github.com/cktan).

---

## 📝 License

MIT License. See [LICENSE](LICENSE) for details.

---

<div align="center">

Made with ❤️ by hackers, for hackers.

</div>

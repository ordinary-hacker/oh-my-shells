<div align="center">

# 🐚 oh-my-shells

The offline, CLI-first swiss army knife for shell payloads! 🚀

[![Version](https://img.shields.io/badge/version-v0.0.1-blue.svg)](#)

</div>

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

- `list` — List payloads with optional filters
- `search <term>` — Search for payloads
- `show <shell_id>` — Show details about a specific payload
- `generate <shell_id>` — Generate a payload

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
./oh-my-shell search "netcat"
./oh-my-shell show bash_read_line
./oh-my-shell generate bash_i -H 192.168.1.100 -P 4444
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
curl -fsSL https://raw.githubusercontent.com/ordinary-hacker/oh-my-shells/trunk/install.sh | sudo bash
```

Afterwards you can just run the tool as:

```sh
oh-my-shells -h
```

---

## 📝 License

MIT License. See [LICENSE](LICENSE) for details.

---

<div align="center">

Made with ❤️ by hackers, for hackers.

</div>

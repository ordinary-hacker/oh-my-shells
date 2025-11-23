# Changelog

All notable changes to this project will be documented in this file.

This format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## [Unreleased]

### Added

- A proper testing suite using Unity framework which replaces the earlier `test.sh` script
- Github actions which automatically: run tests, package `shells/`, and generate precompiled binaries
- A dedicated tests section in `README.md`
- This changelog (`CHANGELOG.md`)
- A contribution guide (`CONTRIBUTING.md`)
- A simple CoC (`CODE_OF_CONDUCT.md`)
- Issue templates for: bug reports, enhancement requests, and feature requests
- A pull request template

### Changed

- Polished the `README.md` with more badges and better wording
- Slightly changed the project structure to organize things better, this includes new `docs/`, `scripts/`, and `tests/` dirs

### Removed

- Removed the now deprecated `test.sh` script

## [0.0.4] - 2025-10-14

### Added

- Shorter aliases for subcommands

### Fixed

- Improved URL encoding, by encoding ALL special characters ensuring that the payload works

## [0.0.3] - 2025-09-17

### Added

- Webshell support with a new valid TOML key which can be used to set an specific payload as a webshell
- LHOST/LPORT validation
- Support for choosing a network interface as LHOST and internally resolving the IP
- Ensured cross-compatibility across the following targets: x86_64/ARM/aarch64 Linux, x86_64/aarch64 macOS, x86_64 FreeBSD, and x86_64 NetBSD.
- Both precompiled binaries and packaged `shells/` directory are provided
- Scripts to generate both the packaged `shells/` and precompiled binaries are provided

## [0.0.2] - 2025-08-31 [YANKED]

### Added

- A testing script (`test.sh`) which quickly runs the CLI testing different arg combinations

### Fixed

- Tool being unable to find `shells/` directory if it was symlinked somewhere
- General unstability issues
- Really the first actual working version, 0.0.1 was actually barely tested even for simple stuff

## [0.0.1] - 2025-08-31

### Added

- Initial commit!
- Over 80 payloads to choose from in `shells/`
- Base project structure
- A (at least if the binary is in the same dir) working product

[unreleased]: https://github.com/ordinary-hacker/oh-my-shells/compare/v0.0.4...HEAD
[0.0.4]: https://github.com/ordinary-hacker/oh-my-shells/compare/v0.0.3...v0.0.4
[0.0.3]: https://github.com/ordinary-hacker/oh-my-shells/compare/v0.0.2...v0.0.3
[0.0.2]: https://github.com/ordinary-hacker/oh-my-shells/compare/v0.0.1...v0.0.2
[0.0.1]: https://github.com/ordinary-hacker/oh-my-shells/releases/tag/v0.0.1

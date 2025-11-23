# Contributing to oh-my-shells

## Introduction

Thanks a lot for taking the time to contribute to **oh-my-shells**. Even though this project started as a solo effort, it’s people like you who make open-source awesome.

Following the standards and guidelines here helps maintainers keep things moving smoothly. It makes it easier to review issues and PRs quickly, and in return it helps you get your contribution addressed as soon as possible.

Any contribution is valuable, writing tutorials or blog posts, improving documentation, filing bug reports or feature requests, adding code, or even just spreading the word that this tool exists.

Please avoid using the issue tracker for general usage questions unless your question directly relates to improving the usage documentation (for example, updating the `README.md` or the help message).

## Ground Rules

Beyond the obvious things like being respectful and considerate, here are a few responsibilities depending on how you're contributing:

* Make sure your changes work across the platforms this project aims to support. If something is marked as experimental, full compatibility isn’t required yet.
* For AI-generated code, please test it thoroughly. “AI code” here refers to code generated entirely by AI or cases where AI contributed more to the final code than humans. Make sure the final result is solid and maintainable.
* Even though GitHub Actions runs tests automatically, please run the tests locally before submitting a PR.
* If your code introduces new features, try to include tests for them.
* When opening issues, please follow the provided templates.
* Try to avoid adding new dependencies. If one is truly needed, place it in the `lib/` directory and include only the *minimum* required source files (e.g., for header-only libraries). Discuss dependency additions in the relevant issue.
* Aim for clean, maintainable code.
* Unless a change is *really* small (like fixing a typo), create an issue first to describe your idea in detail so progress stays organized.
* Be welcoming to everyone.
* All contributors are expected to follow the [oh-my-shells Code of Conduct](CODE_OF_CONDUCT.md).
* In return, I’ll do my best to respond to issues and PRs as quickly as I can.

## Your First Contribution

Not sure where to start? There are plenty of places to jump in.

Check out issues labeled `good first issue` or `help wanted`.

* **good first issue**: simple changes, usually only a few lines of code and maybe one or two tests.
* **help wanted**: a bit more involved, but intentionally open for anyone to tackle.

If this is your first PR ever, resources like [First Contributions](https://github.com/firstcontributions/first-contributions#first-contributions) can help you get a feel for the workflow.

## Getting Started

Contributing follows a pretty standard workflow:

1. Fork the repository.
2. Make your changes in your fork.
3. Create a branch with a descriptive name. **Allowed prefixes** are:

   * `feature/`: new features
   * `fix/`: bug fixes
   * `docs/`: documentation updates
   * `enhancement/`: improvements to existing features
   * `small/`: really small changes (see below)
4. Open a PR using the PR template.
5. Wait for review and feedback!

“Really small” changes don’t require a dedicated issue and should use the `small/` prefix. These include:

* Spelling and grammar fixes
* Typos, whitespace, or formatting tweaks
* Comment cleanup
* Anything that doesn’t meaningfully affect functionality or style

## How to Report a Bug

1. Check the issue tracker to make sure the bug hasn’t already been reported.
2. Use the bug report template when opening your issue.
3. Wait for discussion to progress, ideally ending in a fix!

## How to Suggest a Feature or Enhancement

Before proposing something, make sure it aligns with the project’s goals.

**oh-my-shells** aims to provide a fast, simple way to generate, search, and view details about web/reverse/bind shells, while keeping things lightweight: a single binary plus supporting files (like the `shells/` directory). Contributions should fit that philosophy.

When proposing an idea:

1. Think through the scope, purpose, and how it might work.
2. Open a new issue using the relevant feature/enhancement template.
3. Wait for conversation and feedback to develop around it.

## Code Review Process

For now, I’m the only one reviewing PRs. If something needs adjustments before merging, I’ll let you know. Once everything looks good and I’ve tested it, I’ll merge it.

Response times vary since I do this in my free time. Sometimes I reply the same day; other times it might take a few days. I try to keep responses within about two weeks, if it takes longer, feel free to mention me in a comment.

For reference, I’m in CST/UTC-6 year-round.

As the project grows, I’ll put together a plan for contributors to eventually gain commit access and for building a maintainer/contributor list.

## Commit Message and Labeling Conventions

There’s no strict commit message format, just keep them clear and descriptive so the changes are easy to understand.

For labels, these are available:

* `feature`: new features
* `bug`: something isn’t working
* `documentation`: docs updates
* `duplicate`: issue/PR already exists
* `enhancement`: improvements to existing features
* `good first issue`: great starting points
* `help wanted`: extra attention needed
* `invalid`: not applicable or incorrect
* `wontfix`: won’t be worked on

The labels `feature`, `bug`, `documentation`, and `enhancement` are **mutually exclusive** and should be applied when the PR/issue is created. The rest are added by maintainers as needed.

## Versioning Scheme

Even though the standard is SemVer I straight up just didn't feel like using it.
The versioning scheme used by this project is fairly simple, simply the biggest the version number, the newer the release.

E.g. 0.0.3 < 0.0.4, 0.1.2 > 0.0.9, etc.

The version number has no further meaning, and breaking-changes, bug fixes, and other stuff is separately documented in the release notes.

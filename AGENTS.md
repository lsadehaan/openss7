# Repository Guidelines

## Project Structure & Module Organization
`src/` contains the implementation: `src/kernel/` for STREAMS kernel code, `src/modules/` for loadable modules, `src/lib/` for libraries, and bindings under `src/perl/`, `src/python/`, `src/tcl/`, and `src/java/`. Public headers live in `src/include/`. `tests/` holds the GNU Autotest suite (`testsuite.at`, `testsuite-*.at`, `local.at`, `kernel.at`). `doc/` contains manuals, man pages, specs, and project papers. Autotools inputs are rooted at `configure.ac`, `Makefile.am`, `m4/`, and `am/`.

## Build, Test, and Development Commands
Run `./configure` for a standard build, or `./configure.sh` for the maintainer-oriented local setup used in this tree. Build with `make -j$(nproc)`. Run `make check` for pre-install checks and `make installcheck` for install-path validation through the generated `tests/testsuite` harness. Use `make distcheck` before large changes that touch packaging, generated files, or recursive make logic. Use `make V=1` when debugging build rules.

## Coding Style & Naming Conventions
Follow the existing Vim modelines: most `Makefile.am`, `.at`, and `configure.ac` files use tabs with `sw=4 noet`; shell helpers generally use 4 spaces. Keep naming aligned with the subsystem: `str*` for STREAMS core files, `testsuite-*.at` for focused test groups, and lowercase hyphenated config scripts in `configs/`. Preserve license headers and avoid drive-by reformatting. Generated SNMP stubs reference `indent -nhnl`; do not restyle them by hand.

## Testing Guidelines
Tests are written with GNU Autotest, not xUnit. Add coverage in the closest existing `tests/testsuite-*.at` file, or create a similarly named file and register it in `tests/Makefile.am` via `TESTSUITE_DEPS`. Run `make check` for logic that does not require installed artifacts; use `make installcheck` for runtime or packaging behavior. Kernel-facing tests may require module insertion and elevated privileges, so document prerequisites in the change description.

## Commit & Pull Request Guidelines
Recent history uses short, imperative subjects such as `remove owner before kern_(un)mount` and `break dependency cycle`. Keep subjects concise, lowercase, and focused on one change. Pull requests should describe the affected subsystem, list exact build/test commands run, and call out kernel, distro, or packaging assumptions. Include logs or screenshots only when they clarify a user-visible tool or documentation change.

## Security & Configuration Notes
Do not commit machine-specific edits from `configure.sh` or `configs/configure.*` unless they are intentionally portable. Review changes under `src/kernel/` and install paths for privilege boundaries, module loading behavior, and distro-specific side effects.

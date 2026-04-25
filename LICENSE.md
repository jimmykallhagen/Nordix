# Nordix Software License


## License

[![License: GPL v3+](https://img.shields.io/badge/License-GPLv3%2B-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

Nordix is licensed under the **GNU General Public License, version 3 or later**
(`SPDX-License-Identifier: GPL-3.0-or-later`).

Copyright (C) 2025-2026 Jimmy Källhagen

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the [GNU General Public License](LICENSE) for
more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <https://www.gnu.org/licenses/>.

### Previous licensing

Versions of Nordix released before **2026-04-21** were distributed under the
[PolyForm Noncommercial License 1.0.0](https://polyformproject.org/licenses/noncommercial/1.0.0).
Those releases remain available under that license; the relicense to GPLv3+
applies only to commits from that date forward. See [CHANGELOG.md](CHANGELOG.md)
for the relicense notice.

### Trademarks

**Nordix** and **Yggdrasil** are trademarks of Jimmy Källhagen. The GPLv3+
license grants you freedom to use, modify, and redistribute the code, but
does not grant any rights to use these trademarks. You may not use the names
"Nordix" or "Yggdrasil" to identify forks or derivative works in a way that
implies endorsement by or affiliation with the original project. Forks must
be renamed.

### Third-party components

Nordix installs and configures third-party software, including OpenZFS
(licensed under [CDDL-1.0](https://opensource.org/license/cddl-1-0))
and various GPL-licensed kernel modules and userspace tools. Each component
retains its own license; Nordix's GPLv3+ license applies only to the scripts,
configuration files, and documentation authored as part of this project.

The combination of CDDL-licensed OpenZFS with GPL-licensed Linux kernel code
is a well-known license-interaction area. See
<https://openzfs.github.io/openzfs-docs/License.html> for OpenZFS's own
analysis, and note that Nordix builds the ZFS module via DKMS on the end
user's system rather than shipping pre-linked binaries.

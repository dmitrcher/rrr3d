# Task-owned static MSVC2008 SP1 extraction

The original x86 compiler (15.00.30729.01), linker, PDB support, VC headers/libraries, and Windows SDK6.0A x86 headers/libraries were extracted from already-owned read-only installer media. Original MSI File/Component/Directory tables determine paths. No installer or extracted compiler was executed.

- VC/bin: compiler tools, with English resources under1033.
- VC/include and VC/lib: VC/CRT/STL headers and x86 libraries.
- Common7/IDE: original mspdb80/core/srv and msobj80 dependencies.
- WindowsSDK/Include and WindowsSDK/Lib: SDK6.0A headers and x86 import libraries.
- runtime/Microsoft.VC90.CRT: authentic SP1 release CRT9.0.30729.1; adjacent publisher policy redirects9.0.21022.8 to it.

The compiler manifests request Microsoft.VC90.CRT9.0.21022.8. The included authentic SP1 publisher policy must be activated correctly, or an independently owned matching original CRT used. Merely copying the9.0.30729.1 private assembly does not prove version binding. No assembly identities were changed. Runtime activation and compilation remain to be qualified by the parent task in its isolated prefix.

`environment.cmd` supplies only PATH/INCLUDE/LIB for an isolated Windows compatibility environment. It has not been executed. No user prefix, original SDK, game DLL, archive, repository source, or build was changed.

`provenance.json` records source hashes, CAB members, MSI mappings, file sizes, SHA256 hashes, and key binary versions/architecture. `binary-dependencies.json` is a static import/embedded-manifest inventory. `read-msi.py` is a task-local native Python CFB/MSI metadata reader. Its schema comes from the original MSI Columns table; every selected output size matches the MSI File table.

Only relevant x86 toolchain/header/library members were extracted. Derived selected CABs retain original compressed data while restricting their own file tables. Original CABs remain unchanged. These proprietary files remain local and must not be published.

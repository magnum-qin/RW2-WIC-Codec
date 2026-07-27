# Security policy

## Supported versions

Security fixes are provided for the latest published release.

| Version | Supported |
| --- | --- |
| 1.1.x | Yes |
| 1.0.x | No |

## Reporting a vulnerability

Please do not disclose a suspected vulnerability in a public issue.

Use GitHub's **Report a vulnerability** feature in the repository's Security tab. Include:

- the affected version;
- a concise description and impact;
- reproduction steps or a proof of concept;
- relevant Windows and camera/file details;
- any suggested mitigation.

If private vulnerability reporting is unavailable, open a public issue containing no exploit details and ask the maintainer for a private contact channel.

You should receive an initial acknowledgement within seven days. Confirmed issues will be assessed, fixed, and disclosed in coordination with the reporter.

## Scope

Security-sensitive areas include COM registration, DLL loading, malformed RW2 parsing, memory safety at the WIC boundary, and installer/uninstaller behavior.

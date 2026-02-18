# Security Policy

## Reporting a Vulnerability

If you discover a security vulnerability in NUXP, please report it responsibly.

**Do NOT open a public GitHub issue for security vulnerabilities.**

Instead, please use one of these methods:

1. **GitHub Security Advisories** (preferred): [Report a vulnerability](https://github.com/ByteBard97/nuxp/security/advisories/new)
2. **Email**: Send details to the maintainers via the email listed in the repository profile

### What to Include

- Description of the vulnerability
- Steps to reproduce
- Potential impact
- Suggested fix (if any)

### What to Expect

- Acknowledgment within 48 hours
- A plan for resolution within 7 days
- Credit in the fix commit (unless you prefer anonymity)

## Scope

NUXP runs a local HTTP server on `localhost`. The primary security considerations are:

- **Local network exposure**: The HTTP server binds to localhost by default. Binding to `0.0.0.0` would expose the Illustrator SDK to the local network.
- **Input validation**: All HTTP endpoints should validate input before passing to SDK calls.
- **No authentication**: The local server does not implement authentication. This is by design for a localhost-only development tool, but users should be aware of this if changing the bind address.

## Supported Versions

Security fixes are applied to the latest release only.

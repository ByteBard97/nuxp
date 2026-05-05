#!/usr/bin/env python3
"""
create_pipl.py — Generate binary PiPL for Illustrator plugins.

Produces a PiPL binary matching the format used by working Illustrator
plugins on disk. In particular, the mi32 property is emitted with an
empty payload rather than a 4-byte null block.

Usage:
  python3 create_pipl.py -input '[{"name":"MyPlugin","entry_point":"PluginMain"}]'

Produces plugin.pipl in the current working directory.
"""

import argparse
import json
import os
import struct


def pad4(data: bytes) -> bytes:
    """Pad byte string to a multiple of 4 bytes."""
    remainder = len(data) % 4
    if remainder:
        data += b'\x00' * (4 - remainder)
    return data


def make_property(vendor: bytes, key: bytes, prop_id: int, value: bytes) -> bytes:
    return vendor + key + struct.pack('>I', prop_id) + struct.pack('>I', len(value)) + value


def make_pipl(name: str) -> bytes:
    props = []
    props.append(make_property(b'ADBE', b'kind', 0, b'SPEA'))
    props.append(make_property(b'ADBE', b'ivrs', 0, struct.pack('>I', 2)))
    # Working Illustrator plugins on this machine encode mi32 with a
    # zero-length payload. A 4-byte null payload looks parseable but
    # does not match the known-good bundles we compared against.
    props.append(make_property(b'ADBE', b'mi32', 0, b''))
    props.append(make_property(b'ADBE', b'pinm', 0, pad4(name.encode('ascii'))))

    header = struct.pack('>II', 0, len(props))  # version=0, property_count
    return header + b''.join(props)


def main():
    parser = argparse.ArgumentParser(description="Generate Illustrator PiPL")
    parser.add_argument("-input", required=True, help="JSON array of plugin descriptors")
    args = parser.parse_args()

    plugins = json.loads(args.input)
    pipls = []
    for desc in plugins:
        name = desc.get("name", "Plugin")
        pipls.append(make_pipl(name))

    output_path = os.path.join(os.getcwd(), "plugin.pipl")
    with open(output_path, 'wb') as f:
        f.write(struct.pack('>I', len(pipls)))  # pipl count
        for pipl_data in pipls:
            f.write(pipl_data)

    print(f"Generated PiPL: {output_path} ({os.path.getsize(output_path)} bytes)")


if __name__ == "__main__":
    main()

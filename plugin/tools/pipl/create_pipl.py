#!/usr/bin/env python3

import argparse
import json
import os
import sys


SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
if SCRIPT_DIR not in sys.path:
    sys.path.insert(0, SCRIPT_DIR)

from pipl_gen import pipl  # noqa: E402


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("-input", required=True, help="JSON plugin descriptor list")
    parser.add_argument(
        "-output",
        default=os.path.join(os.getcwd(), "plugin.pipl"),
        help="Output path for the generated PiPL binary",
    )
    args = parser.parse_args()

    descriptors = json.loads(args.input)
    if not isinstance(descriptors, list) or not descriptors:
        raise ValueError("Expected a non-empty JSON array of plugin descriptors")

    descriptor = descriptors[0]
    plugin_name = descriptor.get("name", "Plugin")
    entry_point = descriptor.get("entry_point", "PluginMain")

    plugin = pipl()
    plugin.add_plugin_name(plugin_name)
    plugin.add_plugin_entry(entry_point)
    plugin.generate_pipl_bin(args.output)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

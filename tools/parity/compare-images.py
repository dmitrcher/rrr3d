#!/usr/bin/env python3
"""Compare game menu/race captures without requiring a physics trace (Pillow)."""
import argparse
import json
from pathlib import Path
from compare import image_difference

p = argparse.ArgumentParser(description=__doc__)
p.add_argument('left'); p.add_argument('right'); p.add_argument('--out',required=True)
a = p.parse_args()
try:
    Path(a.out).mkdir(parents=True,exist_ok=True)
    print(json.dumps(image_difference(a.left,a.right,a.out),indent=2))
except (OSError,ValueError) as e:
    p.exit(2,str(e)+'\n')

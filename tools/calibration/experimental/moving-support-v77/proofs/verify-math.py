from pathlib import Path
import subprocess,tempfile
here=Path(__file__).resolve().parent
with tempfile.TemporaryDirectory(prefix="motorrock-v77-math-") as folder:
 for opt in ("-O0","-O2"):
  binary=Path(folder)/("verify"+opt)
  subprocess.run(["clang++","-std=c++17",opt,"-ffp-contract=off",str(here/"verify.cpp"),"-o",str(binary)],check=True)
  subprocess.run([str(binary)],check=True)
subprocess.run(["python3",str(here/"replay-wheel-order.py")],check=True)

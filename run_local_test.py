import subprocess
import sys
import os
import glob

GCC_PATH = r"C:\Program Files\CodeBlocks\MinGW\bin\gcc.exe"

def run_test(target):
    day_matches = glob.glob('Day129*')
    day_dir = day_matches[0] if day_matches else 'Day129'
    src_file = os.path.join(day_dir, f"{target}.c")
    exe_file = os.path.join(day_dir, f"{target}.exe")

    if not os.path.exists(src_file):
        print(f"File not found: {src_file}")
        return

    print(f"Compiling {src_file} with GCC...")
    compile_cmd = [GCC_PATH, "-Wall", "-Wextra", src_file, "-o", exe_file]
    res = subprocess.run(compile_cmd, capture_output=True, text=True)

    if res.returncode != 0:
        print("Compile Error:")
        print(res.stderr)
        return

    if res.stderr:
        print("Warnings:")
        print(res.stderr)

    print(f"Running {exe_file}...\n" + "="*50)
    run_res = subprocess.run([exe_file], capture_output=True, text=True)
    print(run_res.stdout)
    if run_res.stderr:
        print("STDERR:")
        print(run_res.stderr)
    print("="*50)

if __name__ == "__main__":
    target = sys.argv[1].lower() if len(sys.argv) > 1 else 'p2'
    run_test(target)

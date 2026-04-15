import os
import subprocess

base_dir = os.path.dirname(os.path.abspath(__file__))

def compile(filename):
    print(f"Compiling {filename}...")
    subprocess.run(
        " ".join(['/opt/homebrew/bin/g++-14', f"{filename}.cpp", '-o', f"build/{filename}"]),
        shell=True,
        check=True,
        cwd=base_dir
    )
    print(f"Compile {filename} succeeded.")
    
def run(filename, In=None, Out=None):
    command = [f"./build/{filename}"]
    if In != None: command += ['<', In]
    if Out != None: command += ['>', Out]
    # print(f"running command {" ".join(command)}...")
    subprocess.run(
        " ".join(command),
        shell=True,
        check=True,
        cwd=base_dir
    )
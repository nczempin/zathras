import os
import subprocess
import sys


def main():
    exe = 'zathras.exe' if os.name == 'nt' else './zathras'
    proc = subprocess.run(
        exe,
        input=b'quit\n',
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        shell=True,
    )
    output = proc.stdout.decode('utf-8', errors='ignore')
    if proc.returncode != 0:
        print(output)
        sys.exit(proc.returncode)
    assert 'Welcome to Zathras' in output
    print('Smoke test passed')


if __name__ == '__main__':
    main()

import os
import subprocess
import sys


def main():
    # Look for executable in different locations
    if os.name == 'nt':
        exe_paths = [
            os.path.join('x64', 'Release', 'zathras.exe'),
            os.path.join('Release', 'zathras.exe'),
            'zathras.exe'
        ]
    else:
        exe_paths = ['./zathras', 'zathras']
    
    exe = None
    for exe_path in exe_paths:
        if os.path.exists(exe_path):
            exe = exe_path
            break
    
    if not exe:
        print('Error: zathras executable not found')
        sys.exit(1)
    
    print(f'Running smoke test with: {exe}')
    proc = subprocess.run(
        exe,
        input=b'quit\n',
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        shell=True,
    )
    output = proc.stdout.decode('utf-8', errors='ignore')
    if proc.returncode != 0:
        print(f'Process failed with return code {proc.returncode}')
        print('Output:')
        print(output)
        sys.exit(proc.returncode)
    
    print('Output:')
    print(output)
    assert 'Welcome to Zathras' in output
    print('Smoke test passed')


if __name__ == '__main__':
    main()

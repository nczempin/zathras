import os
import shutil
import subprocess
import sys


def run(cmd):
    print(f"+ {cmd}")
    result = subprocess.run(cmd, shell=True)
    if result.returncode != 0:
        sys.exit(result.returncode)


def build_and_test():
    if shutil.which('make'):
        run('make clean')
        run('make')
        run('make test')
        if shutil.which('cppcheck'):
            run('cppcheck --enable=warning --inconclusive --quiet src zathras_lib/src tests/unit')
    elif shutil.which('msbuild'):
        sln = 'msvc/zathras.sln'
        run(f'msbuild {sln} /p:Configuration=Release /p:Platform=x64')
        # Check for test executable in different possible locations
        test_exe_paths = [
            os.path.join('msvc', 'x64', 'Release', 'run_tests.exe'),
            os.path.join('msvc', 'Release', 'run_tests.exe'),
            os.path.join('x64', 'Release', 'run_tests.exe'),
            os.path.join('Release', 'run_tests.exe'),
            'run_tests.exe'
        ]
        test_exe = None
        for path in test_exe_paths:
            if os.path.exists(path):
                test_exe = path
                break
        if test_exe:
            run(test_exe)
        else:
            print('Warning: Test executable not found, skipping tests')
    else:
        print('No suitable build tool found.')
        sys.exit(1)


def smoke_test():
    run(f"{sys.executable} tests/smoke/test_basic.py")


def package():
    dist = 'dist'
    if os.path.exists(dist):
        shutil.rmtree(dist)
    os.makedirs(dist, exist_ok=True)
    
    # Look for executables in different locations
    if os.name == 'nt':
        exe_paths = [
            os.path.join('msvc', 'x64', 'Release', 'zathras.exe'),
            os.path.join('msvc', 'Release', 'zathras.exe'),
            os.path.join('x64', 'Release', 'zathras.exe'),
            os.path.join('Release', 'zathras.exe'),
            'zathras.exe'
        ]
        lib_paths = [
            os.path.join('msvc', 'x64', 'Release', 'zathras_lib.lib'),
            os.path.join('msvc', 'Release', 'zathras_lib.lib'),
            os.path.join('x64', 'Release', 'zathras_lib.lib'),
            os.path.join('Release', 'zathras_lib.lib'),
            'zathras_lib.lib'
        ]
    else:
        exe_paths = ['zathras']
        lib_paths = ['libzathras.a']
    
    # Copy executable
    for exe_path in exe_paths:
        if os.path.exists(exe_path):
            shutil.copy(exe_path, dist)
            break
    
    # Copy library
    for lib_path in lib_paths:
        if os.path.exists(lib_path):
            shutil.copy(lib_path, dist)
            break
    
    shutil.make_archive('zathras', 'zip', dist)


def main():
    build_and_test()
    smoke_test()
    package()


if __name__ == '__main__':
    main()

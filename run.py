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
        sln = 'zathras.sln'
        run(f'msbuild {sln} /p:Configuration=Release')
        exe = os.path.join('Release', 'run_tests.exe')
        if os.path.exists(exe):
            run(exe)
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
    exe = 'zathras.exe' if os.name == 'nt' else 'zathras'
    if os.path.exists(exe):
        shutil.copy(exe, dist)
    if os.path.exists('libzathras.a'):
        shutil.copy('libzathras.a', dist)
    shutil.make_archive('zathras', 'zip', dist)


def main():
    build_and_test()
    smoke_test()
    package()


if __name__ == '__main__':
    main()

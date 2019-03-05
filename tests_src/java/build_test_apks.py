#!/usr/bin/env python

import os
import contextlib
import tempfile
import shutil
import subprocess


self_dir = os.path.dirname(os.path.realpath(__file__))


@contextlib.contextmanager
def _tempdir(prefix='/tmp/pysoot_tmp', delete=True):
    tmpdir = tempfile.mkdtemp(prefix=prefix)
    try:
        yield tmpdir
    finally:
        if delete:
            shutil.rmtree(tmpdir)


def exec_cmd(args, cwd=None, shell=False, debug=False):
    # debug = True
    if debug:
        print "EXECUTING:", repr(args), cwd, shell

    pipe = subprocess.PIPE
    p = subprocess.Popen(args, cwd=cwd, shell=shell, stdout=pipe, stderr=pipe)
    std = p.communicate()
    retcode = p.poll()
    res = (std[0], std[1], retcode)

    if debug:
        print "RESULT:", repr(res)

    return res


if __name__ == "__main__":
    code_path = os.path.join(self_dir, "android1_code")
    out_path1 = os.path.join(code_path, "app", "build", "outputs", "apk", "app-debug.apk")
    out_path2 = os.path.join(self_dir, "..", "..", "tests", "java", "android1.apk")
    print out_path2
    try:
        os.unlink(out_path1)
    except OSError:
        pass
    try:
        os.unlink(out_path2)
    except OSError:
        pass
    exec_cmd(["./gradlew", "clean"], cwd=code_path)
    exec_cmd(["./gradlew", "assembleDebug"], cwd=code_path)
    shutil.copy2(out_path1, out_path2)

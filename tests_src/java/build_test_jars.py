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
    code_path = os.path.join(self_dir, "simple1_code")
    out_path = os.path.join(self_dir, "..", "..", "tests", "java", "simple1.jar")
    try:
        os.unlink(out_path)
    except OSError:
        pass
    with _tempdir() as td:
        jar_dir = os.path.join(td, "jar")
        os.mkdir(jar_dir)
        exec_cmd(['javac -d ' + jar_dir + ' $(find . -name "*.java")'], shell=True, cwd=code_path)
        exec_cmd(["jar cvf ../out.jar **"], cwd=jar_dir, shell=True)
        shutil.copy2(os.path.join(td, "out.jar"), out_path)

    code_path = os.path.join(self_dir, "simple2_code")
    out_path = os.path.join(self_dir, "..", "..", "tests", "java", "simple2.jar")
    try:
        os.unlink(out_path)
    except OSError:
        pass
    with _tempdir() as td:
        jar_dir = os.path.join(td, "jar")
        os.mkdir(jar_dir)
        exec_cmd(['javac -d ' + jar_dir + ' $(find . -name "*.java")'], shell=True, cwd=code_path)
        exec_cmd(["jar cvf ../out.jar **"], cwd=jar_dir, shell=True)
        shutil.copy2(os.path.join(td, "out.jar"), out_path)

    code_path = os.path.join(self_dir, "exceptions1_code")
    out_path = os.path.join(self_dir, "..", "..", "tests", "java", "exceptions1.jar")
    try:
        os.unlink(out_path)
    except OSError:
        pass
    with _tempdir() as td:
        jar_dir = os.path.join(td, "jar")
        os.mkdir(jar_dir)
        exec_cmd(['javac -d ' + jar_dir + ' $(find . -name "*.java")'], shell=True, cwd=code_path)
        exec_cmd(["jar cvf ../out.jar **"], cwd=jar_dir, shell=True)
        shutil.copy2(os.path.join(td, "out.jar"), out_path)


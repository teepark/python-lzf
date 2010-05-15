import errno
import os
from setuptools import Extension

from paver.easy import *
from paver.path import path
from paver.setuputils import setup


setup(
    name="python-lzf",
    description="C Extension for liblzf",
    version="0.1",
    author="Travis Parker",
    author_email="travis.parker@gmail.com",
    url="http://github.com/teepark/python-lzf",
    license="BSD",
    ext_modules=[Extension(
        'lzf',
        ['lzf_module.c', 'lzf_c.c', 'lzf_d.c'],
        include_dirs=('.',),
        extra_compile_args=['-Wall'])],
    classifiers = [
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: BSD License",
        "Natural Language :: English",
        "Programming Language :: C",
        "Topic :: System :: Archiving :: Compression",
    ]
)

MANIFEST = (
    "setup.py",
    "paver-minilib.zip",
    "lzf_module.c",
    "lzf_c.c",
    "lzf_d.c",
    "lzf.h",
    "lzfP.h",
)

@task
def manifest():
    path('MANIFEST.in').write_lines('include %s' % x for x in MANIFEST)

@task
@needs('generate_setup', 'minilib', 'manifest', 'setuptools.command.sdist')
def sdist():
    pass

@task
def clean():
    for p in map(path, ('lzf.egg-info', 'dist', 'build', 'MANIFEST.in')):
        if p.exists():
            if p.isdir():
                p.rmtree()
            else:
                p.remove()
    for p in path(__file__).abspath().parent.walkfiles():
        if p.endswith(".pyc") or p.endswith(".pyo"):
            try:
                p.remove()
            except OSError, exc:
                if exc.args[0] == errno.EACCES:
                    continue
                raise

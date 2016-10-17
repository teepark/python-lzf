from setuptools import setup, Extension


VERSION = (0, 2, 4)

setup(
    name="python-lzf",
    description="C Extension for liblzf",
    version=".".join(filter(None, map(str, VERSION))),
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
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: BSD License",
        "Natural Language :: English",
        "Programming Language :: C",
        "Topic :: System :: Archiving :: Compression",
    ]
)

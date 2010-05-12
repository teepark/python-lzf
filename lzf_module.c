#include "Python.h"

#include "lzf.h"


static PyObject *
python_compress(PyObject *self, PyObject *args) {
    char *input, *output;
    Py_ssize_t inlen;
    long outlen = 0;
    PyObject *result;

    if (!PyArg_ParseTuple(args, "s#|l", &input, &inlen, &outlen))
        return NULL;

    if (!outlen) outlen = inlen - 2;

    output = (char *)malloc(outlen);
    outlen = lzf_compress(input, inlen, output, outlen + 1);

    if (outlen)
        result = PyString_FromStringAndSize(output, outlen);
    else {
        Py_XINCREF(Py_None);
        result = Py_None;
    }
    free(output);

    return result;
}


static PyObject *
python_decompress(PyObject *self, PyObject *args) {
    char *input, *output;
    Py_ssize_t inlen;
    long outlen;
    PyObject *result;

    if (!PyArg_ParseTuple(args, "s#l", &input, &inlen, &outlen))
        return NULL;

    output = (char *)malloc(outlen);
    outlen = lzf_decompress(input, inlen, output, outlen);

    if (outlen)
        result = PyString_FromStringAndSize(output, outlen);
    else {
        Py_XINCREF(Py_None);
        result = Py_None;
    }
    free(output);

    return result;
}


static PyMethodDef methods[] = {
    {"compress", python_compress, METH_VARARGS,
"return the compressed string, \
or None if it doesn't compress to smaller than the original"},
    {"decompress", python_decompress, METH_VARARGS, ""},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initlzf(void) {
    Py_InitModule("lzf", methods);
}

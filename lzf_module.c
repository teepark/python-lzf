#define PY_SSIZE_T_CLEAN

#include "Python.h"
#include <errno.h>
#include "lzf.h"

#if PY_MAJOR_VERSION >= 3
    #define PYBYTES_FSAS PyBytes_FromStringAndSize
    #define PyInt_CheckExact(pyobj) 0
    #define PyInt_AsLong(pyobj) 0
#else
    #define PYBYTES_FSAS PyString_FromStringAndSize
#endif

#if PY_MAJOR_VERSION == 2 && PY_MINOR_VERSION <= 4
    #define Py_ssize_t ssize_t
#endif

static PyObject *
python_compress(PyObject *self, PyObject *args) {
    char *input, *output;
    Py_ssize_t inlen;
    PyObject *pyoutlen = Py_None;
    long outlen;
    PyObject *result;

    if (!PyArg_ParseTuple(args, "s#|O", &input, &inlen, &pyoutlen))
        return NULL;

    if (pyoutlen == Py_None)
        outlen = inlen - 1;
    else if (PyInt_CheckExact(pyoutlen))
        outlen = PyInt_AsLong(pyoutlen);
    else if (PyLong_CheckExact(pyoutlen))
        outlen = PyLong_AsLong(pyoutlen);
    else {
        PyErr_SetString(PyExc_TypeError, "max_len must be an integer");
        return NULL;
    }

    if (inlen == 1) outlen++; /* work around for what looks like a liblzf bug */
    if (outlen <= 0) {
        PyErr_SetString(PyExc_ValueError, "max_len must be > 0");
        return NULL;
    }

    output = (char *)malloc(outlen + 1);
    if (output == NULL) {
        PyErr_SetString(PyExc_MemoryError, "out of memory");
        return NULL;
    }
    outlen = lzf_compress(input, inlen, output, outlen + 1);

    if (outlen)
        result = PYBYTES_FSAS(output, outlen);
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

    if (outlen < 0) {
        PyErr_SetString(PyExc_ValueError, "max_len cannot be less than 0");
        return NULL;
    }

    output = (char *)malloc(outlen);
    outlen = lzf_decompress(input, inlen, output, outlen);

    if (outlen)
        result = PYBYTES_FSAS(output, outlen);
    else {
        if (errno == EINVAL) {
            PyErr_SetString(PyExc_ValueError, "error in compressed data");
            free(output);
            return NULL;
        }
        Py_XINCREF(Py_None);
        result = Py_None;
    }
    free(output);

    return result;
}


static PyMethodDef methods[] = {
    {"compress", python_compress, METH_VARARGS,
"compress(input, max_length=None)\n\
\n\
return the compressed string, or None if it doesn't compress to smaller \n\
than the original\n\
\n\
takes an optional second parameter, for specifying a maximum compressed \n\
length (default is one less than the length of the original)"},
    {"decompress", python_decompress, METH_VARARGS,
"decompress(input, max_length)\n\
\n\
return the decompressed string\n\
\n\
will return None if the string doesn't decompress to within max_length bytes"},
    {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef lzfmodule = {
    PyModuleDef_HEAD_INIT,
    "lzf", "", -1, methods,
    NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_lzf(void) {
    PyObject *module = PyModule_Create(&lzfmodule);
    return module;
}
#else
PyMODINIT_FUNC
initlzf(void) {
    Py_InitModule("lzf", methods);
}
#endif

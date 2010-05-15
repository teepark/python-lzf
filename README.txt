python-lzf: liblzf python bindings

This package is just a straight translation of the C api of liblzf to python.
It has two functions: compress() and decompress().

compress() accepts a string to compress and an optional maximum length for the
output. If it cannot compress the string down to the requested size is to
return None. The default behavior if no size is explicitly requested is to use
one less than the length of the input, so the caller must always be prepared
for a return value of None.

decompress() accepts a string and a size in which the uncompressed result must
fit. If the uncompressed version doesn't fit in the specified size then the
function returns None.

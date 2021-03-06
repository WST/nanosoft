
/**
* Размер блока файлового буфера
*/
#define FDBUFFER_BLOCK_SIZE 1024

/**
* Размер файлового буфера в блоках
*/
#define FDBUFFER_DEFAULT_SIZE 16

/**
* Размер буфера чтения
*/
#define FD_READ_CHUNK_SIZE 4096

/**
* Поддержка zlib сконфигурирована?
*/
#undef HAVE_LIBZ

/**
* Уроверь компресси в zlib
*/
#define ZLIB_COMPRESS_LEVEL 6

/**
* Размер блока-буфера компрессии zlib
*/
#define ZLIB_DEFLATE_CHUNK_SIZE 4096

/**
* Размер блока-буфера для декомпрессии zlib
*/
#define ZLIB_INFLATE_CHUNK_SIZE (FD_READ_CHUNK_SIZE * 8)

/**
* Поддержка GnuTLS
*/
#define HAVE_GNUTLS

/**
* Поддержка GNU SASL library
*/
#define HAVE_GSASL


#define USE_GETADDRINFO

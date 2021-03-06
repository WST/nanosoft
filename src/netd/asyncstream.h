#ifndef NANOSOFT_ASYNCSTREAM_H
#define NANOSOFT_ASYNCSTREAM_H

#include <sys/types.h>

#include "asyncobject.h"
#include "config.h"

#ifdef HAVE_LIBZ
#include <zlib.h>
#endif // HAVE_LIBZ

#ifdef HAVE_GNUTLS
#include <gnutls/gnutls.h>
#endif // HAVE_GNUTLS

/**
* Название метода компрессии
*/
typedef const char *compression_method_t;

/**
* Класс для асинхронной работы с потоками
*/
class AsyncStream: public AsyncObject
{
#ifdef HAVE_GNUTLS
public:
	/**
	* Контекст TLS
	*/
	struct tls_ctx
	{
		gnutls_certificate_credentials_t x509_cred;
		gnutls_priority_t priority_cache;
		gnutls_dh_params_t dh_params;
	};
#endif // HAVE_GNUTLS
private:
	/**
	* Флаги
	*/
	int flags;
	
#ifdef HAVE_LIBZ
	/**
	* Флаг компрессии zlib
	*
	* TRUE - компрессия включена
	* FALSE - компрессия отключена
	*/
	bool compression;
	
	/**
	* Контекст компрессора zlib исходящего трафика
	*/
	z_stream strm_tx;
	
	/**
	* Контекст декомпрессора zlib входящего трафика
	*/
	z_stream strm_rx;
	
	/**
	* Обработка поступивших сжатых данных
	*/
	void handleInflate(const char *data, size_t len);
	
	/**
	* Записать данные со сжатием zlib deflate
	*
	* Данные сжимаются и записываются в файловый буфер. Данная функция
	* пытается принять все данные и возвращает TRUE если это удалось.
	*
	* TODO В случае неудачи пока не возможно определить какая часть данных
	* была записана, а какая утеряна.
	*
	* @param data указатель на данные
	* @param len размер данных
	* @return TRUE данные приняты, FALSE произошла ошибка
	*/
	bool putDeflate(const char *data, size_t len);
#endif // HAVE_LIBZ
	
#ifdef HAVE_GNUTLS
	/**
	* Флаг TLS
	*
	* tls_off        - TLS отключен
	* tls_handshake  - TLS включен, идет handshake
	* tls_on         - TLS вкючен и активен
	*/
	enum { tls_off, tls_handshake, tls_on } tls_status;
	
	/**
	* SSL
	*/
	gnutls_session_t tls_session;
	
	/**
	* Push (write) function для GnuTLS
	*/
	static ssize_t tls_push(gnutls_transport_ptr_t ptr, const void *data, size_t len);
	
	/**
	* Pull (read) function для GnuTLS
	*/
	static ssize_t tls_pull(gnutls_transport_ptr_t ptr, void *data, size_t len);
#endif // HAVE_GNUTLS
	
	/**
	* Обработка поступивших данных
	* 
	* Данные читаются из сокета и при необходимости обрабатываются
	* (сжатие, шифрование и т.п.). Обработанные данные затем передаются
	* виртуальному методу onRead()
	*/
	void handleRead();
	
	/**
	* Передать полученные данные в декомпрессор
	*
	* Если компрессия поддерживается и включена, то данные распаковываются
	* и передаются нижележащему уровню
	*/
	void putInDecompressor(const char *data, size_t len);
	
	/**
	* Передать данные обработчику onRead()
	*/
	void putInReadEvent(const char *data, size_t len);
	
	/**
	* Отправка накопленных данных
	*
	* Эта функция вызывается когда сокет готов принять данные и производит
	* отправку данных накопленных в файловом буфере
	*/
	void handleWrite();
	
	/**
	* Обработка обрыва связи
	*/
	void handlePeerDown();
protected:
	
	/**
	* Вернуть маску ожидаемых событий
	*/
	virtual uint32_t getEventsMask();
	
	/**
	* Обработчик события
	*/
	virtual void onEvent(uint32_t events);
	
	/**
	* Обработчик прочитанных данных
	*/
	virtual void onRead(const char *data, size_t len) = 0;
	
	/**
	* Пир (peer) закрыл поток.
	*
	* Мы уже ничего не можем отправить в ответ,
	* можем только корректно закрыть соединение с нашей стороны.
	*/
	virtual void onPeerDown() = 0;
	
	/**
	* Передать данные компрессору
	*
	* Если сжатие поддерживается и включено, то сжать данные
	* и передать на нижележащий уровень (TLS).
	*
	* @param data указатель на данные
	* @param len размер данных
	* @return TRUE данные приняты, FALSE данные не приняты - нет места
	*/
	bool putInCompressor(const char *data, size_t len);
	
	/**
	* Передать данные в TLS
	*
	* Если TLS поддерживается и включено, то данные шифруются
	* и передаются на нижележащий уровень (файловый буфер)
	*
	* @param data указатель на данные
	* @param len размер данных
	* @return TRUE данные приняты, FALSE данные не приняты - нет места
	*/
	bool putInTLS(const char *data, size_t len);
	
	/**
	* Передать данные в файловый буфер
	*
	* Данные записываются сначала в файловый буфер и только потом отправляются.
	* Для обеспечения целостности переданный блок либо записывается целиком
	* и функция возвращает TRUE, либо ничего не записывается и функция
	* возвращает FALSE
	*
	* @param data указатель на данные
	* @param len размер данных
	* @return TRUE данные приняты, FALSE данные не приняты - нет места
	*/
	bool putInBuffer(const char *data, size_t len);
public:
	
	/**
	* Для shutdown()
	*/
	enum { READ = 1, WRITE = 2 };
	
	/**
	* Конструктор
	*/
	AsyncStream(int afd);
	
	/**
	* Деструктор
	*/
	virtual ~AsyncStream();
	
	/**
	* Проверить поддерживается ли компрессия
	* @return TRUE - компрессия поддерживается, FALSE - компрессия не поддерживается
	*/
	bool canCompression();
	
	/**
	* Проверить поддерживается ли компрессия конкретным методом
	* @param method метод компрессии
	* @return TRUE - компрессия поддерживается, FALSE - компрессия не поддерживается
	*/
	bool canCompression(const char *method);
	
	/**
	* Вернуть список поддерживаемых методов компрессии
	*/
	const compression_method_t* getCompressionMethods();
	
	/**
	* Вернуть флаг компрессии
	* @return TRUE - компрессия включена, FALSE - компрессия отключена
	*/
	bool isCompressionEnable();
	
	/**
	* Вернуть текущий метод компрессии
	* @return имя метода компрессии или NULL если компрессия не включена
	*/
	compression_method_t getCompressionMethod();
	
	/**
	* Включить компрессию
	* @param method метод компрессии
	* @return TRUE - компрессия включена, FALSE - компрессия не включена
	*/
	bool enableCompression(compression_method_t method);
	
	/**
	* Отключить компрессию
	* @return TRUE - компрессия отключена, FALSE - произошла ошибка
	*/
	bool disableCompression();
	
	/**
	* Проверить поддерживается ли TLS
	* @return TRUE - TLS поддерживается, FALSE - TLS не поддерживается
	*/
	bool canTLS();
	
	/**
	* Вернуть флаг TLS
	* @return TRUE - TLS включен, FALSE - TLS отключен
	*/
	bool isTLSEnable();
	
	/**
	* Включить TLS
	* @param ctx контекст TLS
	* @return TRUE - TLS включен, FALSE - произошла ошибка
	*/
	bool enableTLS(tls_ctx *ctx);
	
	/**
	* Отключить TLS
	* @return TRUE - TLS отключен, FALSE - произошла ошибка
	*/
	bool disableTLS();
	
	/**
	* Записать данные
	*
	* При необходимости данные обрабатываются (сжимаются, шифруются и т.п.)
	* и записываются в файловый буфер. Фактическая отправка данных будет
	* когда сокет готов будет принять очередную порцию данных. Эта функция
	* старается принять все переданные данные и возвращает TRUE если это удалось.
	* Если принять данные не удалось, то возвращается FALSE. В случае
	* использования сжатия и/или компрессии невозможно точно установить
	* какая часть данных была записана в буфер.
	*
	* @param data указатель на данные
	* @param len размер данных
	* @return TRUE данные приняты, FALSE данные не приняты - нет места
	*/
	bool put(const char *data, size_t len);
	
	/**
	* Завершить чтение/запись
	* @note только для сокетов
	*/
	bool shutdown(int how);
	
	/**
	* Закрыть поток
	*/
	void close();
};

#endif // NANOSOFT_ASYNCSTREAM_H

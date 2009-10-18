#include <sys/types.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <nanosoft/asyncstream.h>
#include <nanosoft/error.h>
#include <sys/socket.h>

using namespace std;

/**
* Конструктор
*/
AsyncStream::AsyncStream(int afd): AsyncObject(afd), flags(0)
{
}

/**
* Деструктор
*/
AsyncStream::~AsyncStream()
{
	close();
}

/**
* Вернуть маску ожидаемых событий
*/
uint32_t AsyncStream::getEventsMask()
{
	return EPOLLIN | EPOLLRDHUP | EPOLLONESHOT | EPOLLHUP | EPOLLERR | EPOLLET;
}

/**
* Обработчик события
*/
void AsyncStream::onEvent(uint32_t events)
{
	if ( events & EPOLLERR ) onError("epoll report some error in stream...");
	if ( events & EPOLLIN ) onRead();
	if ( (events & EPOLLRDHUP) || (events & EPOLLHUP) ) onShutdown();
}

/**
* Неблокирующее чтение из потока
*/
ssize_t AsyncStream::read(void *buf, size_t count)
{
	ssize_t r = ::read(fd, buf, count);
	if ( r < 0 ) stderror();
	return r;
}

/**
* Неблокирующая запись в поток
*/
ssize_t AsyncStream::write(const void *buf, size_t count)
{
	ssize_t r = ::write(fd, buf, count);
	if ( r < 0 ) stderror();
	return r;
}

/**
* Приостановить чтение из потока
*/
bool AsyncStream::suspend()
{
	onError("TODO AsyncStream::suspend()");
}

/**
* Возобновить чтение из потока
*/
bool AsyncStream::resume()
{
	onError("TODO AsyncStream::resume()");
}

enum { READ = 1, WRITE = 2 };

/**
* Завершить чтение/запись
* @note только для сокетов
*/
bool AsyncStream::shutdown(int how)
{
	if ( how & READ & ~ flags ) {
		if ( ::shutdown(fd, SHUT_RD) != 0 ) stderror();
		flags |= READ;
	}
	if ( how & WRITE & ~ flags ) {
		if ( ::shutdown(fd, SHUT_WR) != 0 ) stderror();
		flags |= WRITE;
	}
}

/**
* Закрыть поток
*/
void AsyncStream::close()
{
	if ( fd )
	{
		int r = ::close(fd);
		fd = 0;
		if ( r < 0 ) stderror();
	}
}

/**
* Событие ошибки
*
* Вызывается в случае возникновения какой-либо ошибки.
* По умолчанию выводит все ошибки в stderr
*/
void AsyncStream::onError(const char *message)
{
	cerr << "[AsyncStream]: " << message << endl;
}

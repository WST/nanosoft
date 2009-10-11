#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <exception>
#include <nanosoft/netdaemon.h>
#include <nanosoft/asyncxmlstream.h>
#include <nanosoft/asyncserver.h>
#include <nanosoft/xmlwriter.h>

using namespace std;

class NetDaemon;

class TestStream: public AsyncXMLStream, private nanosoft::XMLWriter
{
private:
	/**
	* Ссылка на демона
	*/
	NetDaemon *daemon;
protected:
	void onWriteXML(const char *data, size_t len)
	{
		cout << string(data, len);
	}
public:
	TestStream(NetDaemon *d, int afd): AsyncXMLStream(afd), daemon(d), XMLWriter(80) { }
	
	/**
	* Событие готовности к чтению
	*
	* Вызывается когда в потоке есть данные,
	* которые можно прочитать без блокирования
	*/
	/*
	void onRead()
	{
		char buf[400];
		int r = read(buf, sizeof(buf)-1);
		if ( r > 0 )
		{
			cout << string(buf, r);
		}
		if ( r <= 0 )
		{
			//daemon->terminate(r < 0 ? 1 : 0);
			return;
		}
		
	}*/
	
	/**
	* Событие готовности к записи
	*
	* Вызывается, когда в поток готов принять
	* данные для записи без блокировки
	*/
	void onWrite()
	{
		std::cerr << "not implemented TestStream::onWrite()" << std::endl;
	}
	
	/**
	* Событие закрытие соединения
	*
	* Вызывается если peer закрыл соединение
	*/
	void onShutdown()
	{
		AsyncXMLStream::onShutdown();
		XMLWriter::flush();
		//std::cerr << "[TestStream]: peer shutdown connection" << std::endl;
		if ( shutdown(fd, SHUT_RDWR) != 0 )
		{
			stderror();
		}
		daemon->removeObject(this);
		delete this;
	}
	
	/**
	* Обработчик открытия тега
	*/
	virtual void onStartElement(const std::string &name, const attributtes_t &attributes)
	{
		startElement(name);
		for(attributtes_t::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
		{
			setAttribute(iter->first, iter->second);
		}
	}
	
	/**
	* Обработчик символьных данных
	*/
	virtual void onCharacterData(const std::string &cdata)
	{
		characterData(cdata);
	}
	
	/**
	* Обработчик закрытия тега
	*/
	virtual void onEndElement(const std::string &name)
	{
		endElement(name);
	}
};

class MyServer: public AsyncServer
{
private:
	/**
	* Ссылка на демона
	*/
	NetDaemon *daemon;
	
	int count;
public:
	MyServer(NetDaemon *d): daemon(d), count(0) { }
	
	AsyncObject* onAccept()
	{
		int sock = accept();
		if ( sock )
		{
			AsyncObject *client = new TestStream(daemon, sock);
			daemon->addObject(client);
			return client;
		}
		return 0;
	}
};

class MyDaemon: public NetDaemon
{
private:
	/**
	* Поток стандартного ввода
	*/
	TestStream *stdin;
	
	/**
	* Демо сервер
	*/
	MyServer *server;
	
public:
	MyDaemon(): NetDaemon(10000)
	{
		stdin = new TestStream(this, STDIN_FILENO);
		if ( ! addObject(stdin) )
		{
			onError("add stdin fault");
			throw exception();
		}
		
		server = new MyServer(this);
		server->bind(4000);
		server->listen(10);
		addObject(server);
	}
	
	~MyDaemon()
	{
		removeObject(stdin);
		delete stdin;
		delete server;
	}
};

int main()
{
	MyDaemon daemon;
	daemon.setWorkerCount(2);
	return daemon.run();
}

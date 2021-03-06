
#include <map>
#include <string>

#include "xmlnsmanager.h"
#include "error.h"

using namespace std;

namespace nanosoft
{
	/**
	* Регистрация стандартного префикса
	* @param prefix префикс
	* @param ns простанство имен
	*/
	void XMLNSManager::registerPrefix(const std::string &prefix, const std::string &ns)
	{
		p2ns_map::iterator p = p2ns.find(prefix);
		if ( p == p2ns.end() )
		{ // новый префикс
			if ( ns2p.find(ns) != ns2p.end() ) fatal_error("[XMLNSManager] prefix conflict: " + prefix + "=" + ns);
			p2ns[prefix].ns = ns;
			ns2p[ns] = prefix;
		}
		else
		{ // дубликат, увеличить счетчик ссылок
			if ( p->second.ns != ns  ) fatal_error("[XMLNSManager] prefix conflict: " + prefix + "=" + ns);
			p2ns[prefix].counter ++;
		}
	}
	
	/**
	* Дерегистрация стандартного префикса
	* @param prefix префикс
	*/
	void XMLNSManager::unregisterPrefix(const std::string &prefix)
	{
		p2ns_map::iterator p = p2ns.find(prefix);
		if ( p == p2ns.end() || p->second.counter == 0 ) fatal_error("[XMLNSManager] unable unregister prefix, not found: " + prefix);
		p->second.counter--;
	}
	
	/**
	* Вернуть простанство имен связанное со стандартным префиксом
	* @param prefix префикс
	* @return простраство имен
	*/
	std::string XMLNSManager::prefixToNS(const std::string &prefix) const
	{
		p2ns_map::const_iterator p = p2ns.find(prefix);
		if ( p == p2ns.end() ) fatal_error("[XMLNSManager] unknown prefix:" + prefix);
	}
	
	/**
	* Вернуть стандартный префикс простанства имен
	*
	* Возвращает стандартный префикс который определен для данного NS
	* если стандартного нет, то возвращает '#' + defaultValue,
	* где defaultValue - это то, что предложил парсер, по первому
	* символу # мы отличаем, что это неизвестный нам namespace
	*/
	std::string XMLNSManager::NSToPrefix(const string &ns, const string &defaultValue) const
	{
		ns2p_map::const_iterator p = ns2p.find(ns);
		if ( p == ns2p.end() )
		{ // неизвестное простанство имен
			return "#" + defaultValue;
		}
		else
		{
			return p->second;
		}
	}
};

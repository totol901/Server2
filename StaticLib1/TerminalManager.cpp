#include "stdafx.h"
#include "TerminalManager.h"

TerminalManager::TerminalManager()
{
}

TerminalManager::~TerminalManager()
{
}

//여기부터 따라 들어가면 됨
void TerminalManager::initialize(xml_t *config)
{
	xmlNode_t *root = config->FirstChildElement("App")->FirstChildElement("Terminal");
	xmlHandle_t terminalNode = TiXmlHandle(root);
	if (!root) 
	{
		SLog(L"@ not exist terminal setting");
		return;
	}

	xmlNode_t *elem = terminalNode.FirstChildElement().Element();
	while (elem)
	{
		std::array<WCHAR, _MAX_PATH> terminalName;
		StrConvA2W((char *)elem->Value(), terminalName.data(), terminalName.max_size());

		Terminal *terminal = new Terminal(server_, terminalName.data());
		terminal->initialize(elem);
		this->put(terminalName.data(), terminal);

		elem = elem->NextSiblingElement();
	}

	SLog(L"### Terminal set ###");
}

void TerminalManager::release()
{
	for (auto itr : terminalPool_)
	{
		auto terminal = itr.second;
		SAFE_DELETE(terminal);
	}

	SLog(L"### Terminal release ###");
}

void TerminalManager::put(wstr_t serverName, Terminal *terminal)
{
	terminalPool_.insert(std::make_pair(serverName, terminal));
}

Terminal* TerminalManager::get(wstr_t name)
{
	auto itr = terminalPool_.find(name);
	if (itr == terminalPool_.end()) 
	{
		return nullptr;
	}
	return terminalPool_.at(name);
}

bool TerminalManager::isTerminal(const char *ip)
{
	for (auto terminal : terminalPool_) 
	{
		if (!::strcmp(terminal.second->ip(), ip))
		{
			return true;
		}
	}
	return false;
}

void TerminalManager::run(Server *server)
{
	server_ = server;

	xml_t config;
	if (!loadConfig(&config)) 
	{
		return;
	}
	this->initialize(&config);
}

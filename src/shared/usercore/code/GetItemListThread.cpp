/*
Copyright (C) 2011 Mark Chandler (Desura Net Pty Ltd)
Copyright (C) 2014 Bad Juju Games, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA.

Contact us at legal@badjuju.com.

*/

#include "Common.h"
#include "GetItemListThread.h"
#include "InstallInfo.h"

//This is also defined in CIPGrid in uicore
#define CREATE_CIPITEMINFO "create table cipiteminfo(internalid INTEGER PRIMARY KEY, name TEXT);"
#define COUNT_CIPITEMINFO "select count(*) from sqlite_master where name='cipiteminfo';"

#include "sqlite3x.hpp"
#include "sql/CustomInstallPathSql.h"

using namespace UserCore::Thread;

GetItemListThread::GetItemListThread() : InstalledWizardThread()
{
	m_bTriggerNewEvent = false;
}

void GetItemListThread::doRun()
{
	m_szDbName = getCIBDb(getUserCore()->getAppDataPath());

	XML::gcXMLDocument doc;
	getWebCore()->getInstalledItemList(doc);

	int ver = doc.ProcessStatus("itemwizard");

	auto infoNode = doc.GetRoot("itemwizard");

	if (!infoNode.IsValid())
		throw gcException(ERR_BADXML);

	if (isStopped())
		return;

	auto wMng = gcRefPtr<WildcardManager>::create();

	if (ver == 1)
		parseItems1(infoNode, wMng);
	else
		parseItems2(infoNode, wMng);

	try
	{
		createCIPDbTables(getUserCore()->getAppDataPath());

		sqlite3x::sqlite3_connection db(m_szDbName.c_str());
		sqlite3x::sqlite3_command cmd(db, "REPLACE INTO cipiteminfo (internalid, name) VALUES (?,?);");

		for (size_t x = 0; x<m_vGameList.size(); x++)
		{
			cmd.bind(1, (long long int)m_vGameList[x].getId().toInt64());
			cmd.bind(2, std::string(m_vGameList[x].getName()));
			cmd.executenonquery();
		}
	}
	catch (std::exception &e)
	{
		Warning("Failed to update cip item list: {0}\n", e.what());
	}

	uint32 prog = 0;
	onCompleteEvent(prog);
}

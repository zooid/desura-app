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
#include "ItemHandle.h"

#include "User.h"

DesuraId g_ElevatedGames [] =
{
	DesuraId(196, DesuraId::TYPE_GAME),
	DesuraId(),
};

using namespace UserCore::Item;


void ItemHandle::doLaunch(gcRefPtr<Helper::ItemLaunchHelperI> helper)
{
	preLaunchCheck();

	bool needElevation = false;

	size_t x=0;
	while (g_ElevatedGames[x].isOk())
	{
		if (getItemInfo()->getId() == g_ElevatedGames[x] || getItemInfo()->getParentId() == g_ElevatedGames[x])
		{
			needElevation = true;
			break;
		}

		x++;
	}

	auto ei = getItemInfo()->getActiveExe();

	gcString args;
	gcString ea(ei->getExeArgs());
	gcString ua(ei->getUserArgs());

	if (ea.size() > 0)
		args += " " + ea;

	if (ua.size() > 0)
		args += " " + ua;

	m_pUserCore->getItemManager()->setRecent(getItemInfo()->getId());

	auto pUserEx = m_pUserCore->getInternal();
	HWND hMainWin;

	if (pUserEx)
		hMainWin = pUserEx->getMainWindowHandle();

	bool res = UTIL::WIN::launchExe(ei->getExe(), args.c_str(), needElevation, hMainWin);

	if (!res)
		throw gcException(ERR_LAUNCH, GetLastError(), gcString("Failed to create {0} process. [{1}: {2}].\n", getItemInfo()->getName(), GetLastError(), ei->getExe()));
}

bool ItemHandle::createDesktopShortcut()
{
	gcString workingDir = UTIL::OS::getDesktopPath();
	gcString path("{0}\\{1}.lnk", workingDir, UTIL::WIN::sanitiseFileName(getItemInfo()->getName()));
	gcString link("desura://launch/{0}/{1}", getItemInfo()->getId().getTypeString(), getItemInfo()->getShortName());

	gcString icon(getItemInfo()->getIcon());

	if (UTIL::FS::isValidFile(icon))
	{
		gcString out(icon);
		out += ".ico";

		if (UTIL::MISC::convertToIco(icon.c_str(), out.c_str()))
			icon = out;
		else
			icon = "";
	}
	else
	{
		icon = "";
	}

	if (icon == "")
		icon = UTIL::OS::getCurrentDir(L"\\desura.exe");

	UTIL::FS::delFile(path);
	UTIL::WIN::createShortCut(gcWString(path).c_str(), link.c_str(), workingDir.c_str(), "", false, (icon.size()>0)?icon.c_str():nullptr);

	return UTIL::FS::isValidFile(path);
}

bool ItemHandle::createMenuEntry()
{
	return false;
}

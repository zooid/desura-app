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
#include "InstallInfo.h"

#include "XMLMacros.h"

using namespace UserCore::Misc;


InstallInfo::InstallInfo(DesuraId id, DesuraId pid)
{
	m_bInstalled = false;
	m_iID = id;
	m_iParentID = pid;
}

InstallInfo::~InstallInfo()
{
}

void InstallInfo::loadXmlData(const XML::gcXMLElement &xmlNode, gcRefPtr<WildcardManager> &pWildCard)
{
	auto lwc = gcRefPtr<WildcardManager>::create(pWildCard);

	auto wcNode = xmlNode.FirstChildElement("wcards");
	if (wcNode.IsValid())
	{
		lwc->parseXML(wcNode);
	}

	xmlNode.GetChild("name", m_szName);

	if (!xmlNode.FirstChildElement("settings").IsValid())
		return;

	auto icsNode = xmlNode.FirstChildElement("settings").FirstChildElement("installlocations");

	if (!icsNode.IsValid())
		return;

	icsNode.for_each_child("installlocation", [&](const XML::gcXMLElement &xmlChild)
	{
		if (m_bInstalled)
			return;

		const std::string path = xmlChild.GetChild("path");
		const std::string check = xmlChild.GetChild("check");

		if (path.empty() || check.empty())
			return;

		char* CheckRes = nullptr;
		char* PathRes = nullptr;

		try
		{
			lwc->constructPath(check.c_str(), &CheckRes);
			lwc->constructPath(path.c_str(), &PathRes);

			if (CheckRes && PathRes && UTIL::FS::isValidFile(UTIL::FS::PathWithFile(CheckRes)))
			{
				m_szPath = PathRes;
				m_bInstalled = true;
			}
		}
		catch (gcException &e)
		{
			Debug(gcString("InstallInfo: Error parsing wildcards for installInfo: {0}\n", e));
		}

		safe_delete(CheckRes);
		safe_delete(PathRes);
	});
}

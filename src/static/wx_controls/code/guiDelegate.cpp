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
#include "wx_controls/guiDelegate.h"

wxDEFINE_EVENT(wxEVT_GUIDELEGATE, wxGuiDelegateEvent);
IMPLEMENT_DYNAMIC_CLASS(wxGuiDelegateEvent, wxNotifyEvent)



wxGuiDelegateEvent::wxGuiDelegateEvent()
	: wxNotifyEvent(wxEVT_GUIDELEGATE, 0)
{
}

wxGuiDelegateEvent::wxGuiDelegateEvent(std::shared_ptr<Invoker> &invoker, int winId)
	: wxNotifyEvent(wxEVT_GUIDELEGATE, winId)
	, m_pDelegate(invoker)
{
	gcAssert(m_pDelegate);
}

wxGuiDelegateEvent::wxGuiDelegateEvent(const wxGuiDelegateEvent& event)
	: wxNotifyEvent(event)
	, m_pDelegate(event.m_pDelegate)
{
}

wxGuiDelegateEvent::~wxGuiDelegateEvent()
{
}

wxEvent *wxGuiDelegateEvent::Clone() const
{
	return new wxGuiDelegateEvent(*this);
}

void wxGuiDelegateEvent::invoke()
{
	if (m_pDelegate)
		m_pDelegate->invoke();
	else
		gcAssert(false);
}






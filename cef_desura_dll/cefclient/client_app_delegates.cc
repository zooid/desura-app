// Copyright (c) 2012 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "cefclient/client_app.h"
#include "cefclient/client_renderer.h"

#if defined(OS_LINUX)
#include "cefclient/print_handler_gtk.h"
#endif


namespace client {

// static
void ClientApp::CreateBrowserDelegates(BrowserDelegateSet& delegates) {
}

void JSExtenderCreateRenderDelegates( client::ClientApp::RenderDelegateSet& delegates );


// static
void ClientApp::CreateRenderDelegates(RenderDelegateSet& delegates) {
	JSExtenderCreateRenderDelegates( delegates );
}

// static
void ClientApp::RegisterCustomSchemes(
    CefRefPtr<CefSchemeRegistrar> registrar,
    std::vector<CefString>& cookiable_schemes) {
}

}  // namespace client

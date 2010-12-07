/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: NPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is 
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or 
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the NPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the NPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

//////////////////////////////////////////////////
//
// CPlugin class implementation
//

#ifdef XP_UNIX
#include <string.h>
#include <unistd.h>
#include <cstdlib>
#endif

#define TRUE true
#define FALSE false

#include "plugin.h"
#include "config.h"


#define DECLARE_NPOBJECT_CLASS_WITH_BASE(_class, ctor)                        \
static NPClass s##_class##_NPClass = {                                        \
NP_CLASS_STRUCT_VERSION_CTOR,                                               \
ctor,                                                                       \
ScriptablePluginObjectBase::_Deallocate,                                    \
ScriptablePluginObjectBase::_Invalidate,                                    \
ScriptablePluginObjectBase::_HasMethod,                                     \
ScriptablePluginObjectBase::_Invoke,                                        \
ScriptablePluginObjectBase::_InvokeDefault,                                 \
ScriptablePluginObjectBase::_HasProperty,                                   \
ScriptablePluginObjectBase::_GetProperty,                                   \
ScriptablePluginObjectBase::_SetProperty,                                   \
ScriptablePluginObjectBase::_RemoveProperty,                                \
ScriptablePluginObjectBase::_Enumerate,                                     \
ScriptablePluginObjectBase::_Construct                                      \
}

#define GET_NPOBJECT_CLASS(_class) &s##_class##_NPClass

void
ScriptablePluginObjectBase::Invalidate()
{
}

bool
ScriptablePluginObjectBase::HasMethod(NPIdentifier name)
{
	return false;
}

bool
ScriptablePluginObjectBase::Invoke(NPIdentifier name, const NPVariant *args,
								   uint32_t argCount, NPVariant *result)
{
	return false;
}

bool
ScriptablePluginObjectBase::InvokeDefault(const NPVariant *args,
										  uint32_t argCount, NPVariant *result)
{
	return false;
}

bool
ScriptablePluginObjectBase::HasProperty(NPIdentifier name)
{
	return false;
}

bool
ScriptablePluginObjectBase::GetProperty(NPIdentifier name, NPVariant *result)
{
	return false;
}

bool
ScriptablePluginObjectBase::SetProperty(NPIdentifier name,
										const NPVariant *value)
{
	return false;
}

bool
ScriptablePluginObjectBase::RemoveProperty(NPIdentifier name)
{
	return false;
}

bool
ScriptablePluginObjectBase::Enumerate(NPIdentifier **identifier,
									  uint32_t *count)
{
	return false;
}

bool
ScriptablePluginObjectBase::Construct(const NPVariant *args, uint32_t argCount,
									  NPVariant *result)
{
	return false;
}

// static
void
ScriptablePluginObjectBase::_Deallocate(NPObject *npobj)
{
	// Call the virtual destructor.
	delete (ScriptablePluginObjectBase *)npobj;
}

// static
void
ScriptablePluginObjectBase::_Invalidate(NPObject *npobj)
{
	((ScriptablePluginObjectBase *)npobj)->Invalidate();
}

// static
bool
ScriptablePluginObjectBase::_HasMethod(NPObject *npobj, NPIdentifier name)
{
	return ((ScriptablePluginObjectBase *)npobj)->HasMethod(name);
}

// static
bool
ScriptablePluginObjectBase::_Invoke(NPObject *npobj, NPIdentifier name,
									const NPVariant *args, uint32_t argCount,
									NPVariant *result)
{
	return ((ScriptablePluginObjectBase *)npobj)->Invoke(name, args, argCount,
														 result);
}

// static
bool
ScriptablePluginObjectBase::_InvokeDefault(NPObject *npobj,
										   const NPVariant *args,
										   uint32_t argCount,
										   NPVariant *result)
{
	return ((ScriptablePluginObjectBase *)npobj)->InvokeDefault(args, argCount,
																result);
}

// static
bool
ScriptablePluginObjectBase::_HasProperty(NPObject * npobj, NPIdentifier name)
{
	return ((ScriptablePluginObjectBase *)npobj)->HasProperty(name);
}

// static
bool
ScriptablePluginObjectBase::_GetProperty(NPObject *npobj, NPIdentifier name,
										 NPVariant *result)
{
	return ((ScriptablePluginObjectBase *)npobj)->GetProperty(name, result);
}

// static
bool
ScriptablePluginObjectBase::_SetProperty(NPObject *npobj, NPIdentifier name,
										 const NPVariant *value)
{
	return ((ScriptablePluginObjectBase *)npobj)->SetProperty(name, value);
}

// static
bool
ScriptablePluginObjectBase::_RemoveProperty(NPObject *npobj, NPIdentifier name)
{
	return ((ScriptablePluginObjectBase *)npobj)->RemoveProperty(name);
}

// static
bool
ScriptablePluginObjectBase::_Enumerate(NPObject *npobj,
									   NPIdentifier **identifier,
									   uint32_t *count)
{
	return ((ScriptablePluginObjectBase *)npobj)->Enumerate(identifier, count);
}

// static
bool
ScriptablePluginObjectBase::_Construct(NPObject *npobj, const NPVariant *args,
									   uint32_t argCount, NPVariant *result)
{
	return ((ScriptablePluginObjectBase *)npobj)->Construct(args, argCount,
															result);
}


class ConstructablePluginObject : public ScriptablePluginObjectBase
{
public:
	ConstructablePluginObject(NPP npp)
	: ScriptablePluginObjectBase(npp)
	{
	}
	
	virtual bool Construct(const NPVariant *args, uint32_t argCount,
						   NPVariant *result);
};

static NPObject *
AllocateConstructablePluginObject(NPP npp, NPClass *aClass)
{
	return new ConstructablePluginObject(npp);
}

DECLARE_NPOBJECT_CLASS_WITH_BASE(ConstructablePluginObject,
								 AllocateConstructablePluginObject);

bool
ConstructablePluginObject::Construct(const NPVariant *args, uint32_t argCount,
									 NPVariant *result)
{
	printf("Creating new ConstructablePluginObject!\n");
	
	NPObject *myobj =
	NPN_CreateObject(mNpp, GET_NPOBJECT_CLASS(ConstructablePluginObject));
	if (!myobj)
		return false;
	
	OBJECT_TO_NPVARIANT(myobj, *result);
	
	return true;
}

class ScriptablePluginObject : public ScriptablePluginObjectBase
{
public:
	ScriptablePluginObject(NPP npp)
	: ScriptablePluginObjectBase(npp)
	{
	}
	
	virtual bool HasMethod(NPIdentifier name);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
						uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
							   NPVariant *result);
};

static NPObject *
AllocateScriptablePluginObject(NPP npp, NPClass *aClass)
{
	return new ScriptablePluginObject(npp);
}

DECLARE_NPOBJECT_CLASS_WITH_BASE(ScriptablePluginObject,
								 AllocateScriptablePluginObject);

bool
ScriptablePluginObject::HasMethod(NPIdentifier name)
{
	char *fname = NPN_UTF8FromIdentifier(name);
	bool rv = !strcmp(fname, "handleLink");
	NPN_MemFree(fname);
	return rv;
}

bool
ScriptablePluginObject::HasProperty(NPIdentifier name)
{
	return false;
}

bool
ScriptablePluginObject::GetProperty(NPIdentifier name, NPVariant *result)
{
	VOID_TO_NPVARIANT(*result);
	
	return false;
}

bool
ScriptablePluginObject::Invoke(NPIdentifier nname, const NPVariant *args,
							   uint32_t argCount, NPVariant *result)
{
	char *name = NPN_UTF8FromIdentifier(nname);
	
	if (name && !strcmp((const char *)name, "handleLink") && argCount == 1 && NPVARIANT_IS_STRING(args[0])) {
		const char* link;
#ifndef XUL_1_9_2
		link = args[0].value.stringValue.UTF8Characters;
#else
		link = args[0].value.stringValue.utf8characters;
#endif
		char cmdline[512] = "dbus-send --print-reply --type=method_call --dest=info.dolezel.fatrat / info.dolezel.fatrat.addTransfers string:";
		strncat(cmdline, link, 510-strlen(cmdline));
		cmdline[511] = 0;
		
		if (system(cmdline))
		{
			if (!fork()) {
				if (execlp("fatrat", "fatrat", link, 0) == -1)
					return false;
			}
		}
		return true;
	}
	
	NPN_MemFree(name);
	return false;
}

bool
ScriptablePluginObject::InvokeDefault(const NPVariant *args, uint32_t argCount,
									  NPVariant *result)
{
	//printf ("ScriptablePluginObject default method called!\n");
	
	//STRINGZ_TO_NPVARIANT(strdup("default method return val"), *result);
	
	return false;
}

CPlugin::CPlugin(NPP pNPInstance) :
m_pNPInstance(pNPInstance),
m_pNPStream(NULL),
m_bInitialized(FALSE),
m_pScriptableObject(NULL)
{
	
}

CPlugin::~CPlugin()
{
	if (m_pScriptableObject)
		NPN_ReleaseObject(m_pScriptableObject);
}

NPBool CPlugin::init(NPWindow* pNPWindow)
{
	m_bInitialized = true;
	return TRUE;
}

void CPlugin::shut()
{
	m_bInitialized = FALSE;
}

NPBool CPlugin::isInitialized()
{
	return m_bInitialized;
}

int16 CPlugin::handleEvent(void* event)
{
	return 0;
}

// this will force to draw a version string in the plugin window
void CPlugin::showVersion()
{
}

// this will clean the plugin window
void CPlugin::clear()
{
}

void CPlugin::getVersion(char* *aVersion)
{
	*aVersion = 0;
}

NPObject *
CPlugin::GetScriptableObject()
{
	if (!m_pScriptableObject) {
		m_pScriptableObject =
		NPN_CreateObject(m_pNPInstance, GET_NPOBJECT_CLASS(ScriptablePluginObject));
	}
	
	if (m_pScriptableObject) {
		NPN_RetainObject(m_pScriptableObject);
	}
	
	return m_pScriptableObject;
}

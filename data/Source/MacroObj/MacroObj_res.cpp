#include "MacroObj_pch.h"

#include "Resource.h"

static ksapi::IResourcesModulePtr resourceModule;

void * GetModule()
{
  return &resourceModule;
}

#define MENU_ID(idName)   resourceModule->AddMenuId(L"" #idName, idName);
#define STRING_ID(idName) resourceModule->AddStringId(L"" #idName, idName);

void LoadResources(ksapi::IApplication & app)
{
  resourceModule = app.CreateResourcesModule(L"MacroObj");

  MENU_ID(IDR_LIBID)
  MENU_ID(CM_MACRO_3D)
  STRING_ID(IDR_LIBID)
  STRING_ID(IDS_CAPTION)
  STRING_ID(IDS_TAB_NAME)
  STRING_ID(ID_DIAMETER)
  STRING_ID(ID_FACE1)
  STRING_ID(ID_FACE2)
  STRING_ID(ID_ORIFICE_VAR)
  STRING_ID(IDP_CHOICE1)
  STRING_ID(IDP_CHOICE2)
  STRING_ID(IDP_DEFAULTNAME)

  resourceModule->End();
}

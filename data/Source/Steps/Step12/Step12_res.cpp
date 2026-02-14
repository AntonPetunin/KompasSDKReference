#include "Step12_pch.h"
#include "Resource.h"

static ksapi::IResourcesModulePtr resourceModule;

void * GetModule()
{
  return &resourceModule;
}

#define STRING_ID(idName) resourceModule->AddStringId(L"" #idName, idName);
#define MENU_ID(idName)   resourceModule->AddMenuId(L"" #idName, idName);

void LoadResources(ksapi::IApplication & app)
{
  resourceModule = app.CreateResourcesModule(L"Step12");

  MENU_ID(IDR_LIB)
  MENU_ID(IDC_POPUP)
  MENU_ID(IDC_1_MENU)
  MENU_ID(IDC_2_MENU)

  STRING_ID(IDR_LIB)
  STRING_ID(IDS_LIB_PROP_MANAGER)
  STRING_ID(IDS_INSTRUMENTS_MANAGER)

  resourceModule->End();
}


#include <Resource.h>
#include <Step4_KsAPI_2D_pch.h>

static ksapi::IResourcesModulePtr resourceModule;

void * GetModule()
{
  return &resourceModule;
}

#define MENU_ID(idName)   resourceModule->AddMenuId(L"" #idName, idName);
#define STRING_ID(idName) resourceModule->AddStringId(L"" #idName, idName);

void LoadResources(ksapi::IApplication & app)
{
  resourceModule = app.CreateResourcesModule(L"Step4_KsAPI_2D");

  STRING_ID(IDS_DRAWING_1)
  STRING_ID(IDS_DRAWING_2)
  STRING_ID(IDS_CONFIG)
  STRING_ID(IDS_CONFIG_1)
  STRING_ID(IDS_CONFIG_2)
  STRING_ID(IDS_CONFIG_3)
  STRING_ID(IDS_LOAD_CFG_ERR)

  resourceModule->End();
}

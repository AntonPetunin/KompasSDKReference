#include <UserSymbolWithLeader_pch.h>


static ksapi::IResourcesModulePtr resourceModule;

void * GetModule()
{
  return &resourceModule;
}

#define STRING_ID(idName) resourceModule->AddStringId(L"" #idName, idName);
#define MENU_ID(idName)   resourceModule->AddMenuId(L"" #idName, idName);

void LoadResources(ksapi::IApplication & app)
{
  resourceModule = app.CreateResourcesModule(L"UserSymbolWithLeader");

  STRING_ID(IDS_EDIT);
  STRING_ID(IDS_EDIT_TIP);

  MENU_ID(IDR_LIB)
  MENU_ID(IDI_USERSYMBOLWITHLEADER)

  resourceModule->End();
}

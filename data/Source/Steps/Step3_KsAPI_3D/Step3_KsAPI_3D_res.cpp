#include "Step3_KsAPI_3D_pch.h"
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
  resourceModule = app.CreateResourcesModule(libName);

  MENU_ID(IDR_LIBID)

  STRING_ID(IDS_NODOC)
  STRING_ID(IDS_OBJ)
  STRING_ID(IDS_EDIT)
  STRING_ID(IDS_NOCREATE)
  STRING_ID(IDS_POINT)
  STRING_ID(IDS_ROUGH)
  STRING_ID(IDS_BASE)
  STRING_ID(IDS_POSOBJ)
  STRING_ID(IDS_ENDPOINT)
  STRING_ID(IDS_MARK1)
  STRING_ID(IDS_MARK2)
  STRING_ID(IDS_LEADER)
  STRING_ID(IDS_BRAND)
  STRING_ID(IDS_MARK)
  STRING_ID(IDS_TOLERANCE)
  STRING_ID(IDS_BEGINBRANCH)
  STRING_ID(IDS_BEGINSHELF)
  STRING_ID(IDS_TABLE)

  resourceModule->End();
}

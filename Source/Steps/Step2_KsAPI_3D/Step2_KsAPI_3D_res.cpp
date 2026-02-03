#include "Step2_KsAPI_3D_pch.h"
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
  STRING_ID(IDS_OBJ1)
  STRING_ID(IDS_OBJ2)
  STRING_ID(IDS_PLANE)
  STRING_ID(IDS_NOCREATE)
  STRING_ID(IDS_LINEDIM3D)
  STRING_ID(IDS_LENGTH)
  STRING_ID(IDS_DIM)
  STRING_ID(IDS_NOTDIM)
  STRING_ID(IDS_NEWPLANE)
  STRING_ID(IDS_NOTSETPLANE)
  STRING_ID(IDS_DIMVAL)
  STRING_ID(IDS_NOTCIRCLE)
  STRING_ID(IDS_EDIT)
  STRING_ID(IDS_UNIT)
  STRING_ID(IDS_DIMTEXT)
  STRING_ID(IDS_OBJECT1)
  STRING_ID(IDS_OBJECT2)

  resourceModule->End();
}

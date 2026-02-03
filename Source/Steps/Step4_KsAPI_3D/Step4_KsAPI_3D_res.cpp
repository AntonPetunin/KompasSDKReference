#include "Step4_KsAPI_3D_pch.h"
#include "Resource.h"

static ksapi::IResourcesModulePtr resourceModule;

void * GetModule()
{
  return &resourceModule;
}

#define STRING_ID(idName) resourceModule->AddStringId(L"" #idName, idName);

void LoadResources(ksapi::IApplication & app)
{
  resourceModule = app.CreateResourcesModule(L"Step4_KsAPI_3D");

  STRING_ID(IDR_LIBID)
  STRING_ID(IDS_EDIT)

  resourceModule->End();
}

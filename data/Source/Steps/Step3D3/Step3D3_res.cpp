#include <KsAPI.h>

#include "Resource.h"

static ksapi::IResourcesModulePtr resourceModule;

void * GetModule()
{
  return &resourceModule;
}

#define STRING_ID(idName) resourceModule->AddStringId(L"" #idName, idName);

void LoadResources(ksapi::IApplication & app)
{
  resourceModule = app.CreateResourcesModule(libname);

  STRING_ID(IDS_MSG_TITTLE)
  STRING_ID(IDS_YES)
  STRING_ID(IDS_NO)
  STRING_ID(IDS_CANCEL)
  STRING_ID(IDS_WRONG_DOCUMENT)
  STRING_ID(IDS_ERROR)
  STRING_ID(IDS_OFFSETPLANE)
  STRING_ID(IDS_CHANGE_OFFSETPLANE_PARAM)
  STRING_ID(IDS_CREATE_AXIS3D_BY_2PLANES)
  STRING_ID(IDS_AXIS3D_BY_2PLANES)
  STRING_ID(IDS_CHANGE_BASE_PLANE)
  STRING_ID(IDS_CREATE_PLANE3D_BY_ANGLE)
  STRING_ID(IDS_PLANE3D_BY_ANGLE)
  STRING_ID(IDS_AXIS3D_BY_CONEFACE)

  resourceModule->End();
}

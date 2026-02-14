#include "Cube_pch.h"

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
  resourceModule = app.CreateResourcesModule(libname);

  STRING_ID(IDS_TITLE)
  STRING_ID(IDS_OPERATION_NAME)

  MENU_ID(IDR_LIB)
  MENU_ID(CMP_CUBE)
  MENU_ID(CMP_MODEL_OBJS)
  MENU_ID(CMP_SUB_FEATS)
  MENU_ID(CMP_RESULT_BODY)
  MENU_ID(POPUP_TESTS)

  resourceModule->End();
}

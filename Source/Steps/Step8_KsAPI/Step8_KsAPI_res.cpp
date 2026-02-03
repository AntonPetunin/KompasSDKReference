#include "Resource.h"

#include <KsAPI.h>

using namespace ksapi;

static IResourcesModulePtr resourceModule;

void * GetModule()
{
  return &resourceModule;
}

#define STRING_ID(idName) resourceModule->AddStringId(L"" #idName, idName);

void LoadResources(IApplication & app)
{
  resourceModule = app.CreateResourcesModule(libName);

  STRING_ID(FORMAT_ATTRINFO)
  STRING_ID(FORMAT_ATTRREMAINED)
  STRING_ID(FORMAT_ATTRTYPEINFO)
  STRING_ID(FORMAT_ATTRTYPEINFO2)
  STRING_ID(FORMAT_ATTRTYPEINFO3)
  STRING_ID(FORMAT_COLUMNINFO)
  STRING_ID(FORMAT_COLUMNINFO2)
  STRING_ID(FORMAT_COLUMNINFORECORD)

  STRING_ID(CANTCREATEATTRIBUTE)
  STRING_ID(DELETEATTRIBUTE)
  STRING_ID(ENTERPASSWORD)
  STRING_ID(PICKOBJECT)

  resourceModule->End();
}

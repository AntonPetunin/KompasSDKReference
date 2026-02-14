////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief Функции управления ресурсами из XML.

  Загружаются при старте библиотеки, используются через resourceModule.
*/
//
////////////////////////////////////////////////////////////////////////////////
#include <KsAPI.h>
#include "Resource.h"

static ksapi::IResourcesModulePtr resourceModule;

//------------------------------------------------------------------------------
/**
  Получить модуль ресурсов.
*/
//---
void * GetResourceModule()
{
  return &resourceModule;
}

#define MENU_ID(idName) resourceModule->AddMenuId(L"" #idName, idName);

//------------------------------------------------------------------------------
/**
  Загрузить ресурсы из XML файла.
*/
//---
void LoadResources(ksapi::IApplication & app)
{
  resourceModule = app.CreateResourcesModule(L"Step10");

  MENU_ID(MENU_POS_LEADER);
  MENU_ID(CREATE_NEW);
  MENU_ID(CHOOSE_EXITING);

  resourceModule->End();
}

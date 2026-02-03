////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief Функции управления ресурсами из XML.

  Загружаются при старте библиотеки, используются через resourceModule.
*/
//
////////////////////////////////////////////////////////////////////////////////
#include <MyConverter_pch.h>


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

#define STRING_ID(idName) resourceModule->AddStringId(L"" #idName, idName);
#define MENU_ID(idName)   resourceModule->AddMenuId(L"" #idName, idName);

//------------------------------------------------------------------------------
/**
  Загрузить ресурсы из XML файла.
*/
//---
void LoadResources(ksapi::IApplication & app)
{
  resourceModule = app.CreateResourcesModule(kLibName);

  STRING_ID(IDR_LIBID);

  STRING_ID(IDS_ERR_DOC);
  STRING_ID(IDS_ERROR_DOC_TYPE);

  STRING_ID(IDS_DOC_M3D);
  STRING_ID(IDS_DOC_A3D);
  STRING_ID(IDS_DOC_CDW);
  STRING_ID(IDS_DOC_FRW);
  STRING_ID(IDS_DOC_SPW);
  STRING_ID(IDS_DOC_KDW);
  STRING_ID(IDS_DOC_T3D);

  STRING_ID(IDS_STYLE_DOC);
  STRING_ID(IDS_DLG_CAPTION);
  STRING_ID(IDS_DLG_BTN_YES);
  STRING_ID(IDS_DLG_BTN_NO);

  MENU_ID(IDR_LIBID);
  MENU_ID(IDI_SAVE);
  MENU_ID(IDI_OPEN);

  resourceModule->End();
}

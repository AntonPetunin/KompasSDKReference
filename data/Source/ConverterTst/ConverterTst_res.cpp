////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief Функции управления ресурсами из XML.

  Загружаются при старте библиотеки, используются через resourceModule.
*/
//
////////////////////////////////////////////////////////////////////////////////
#include <ConverterTst_pch.h>


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

  STRING_ID(IDS_NO_GET_API);
  STRING_ID(IDS_NO_GET_CONVERTER);
  STRING_ID(IDS_NO_GET_CONVERT_PARAMS);

  STRING_ID(IDS_STYLE_DOC);
  STRING_ID(IDS_DLG_BTN_YES);
  STRING_ID(IDS_DLG_BTN_NO);

  STRING_ID(IDS_CONVERT_SUCCESS);
  STRING_ID(IDS_CONVERT_ERROR);

  MENU_ID(IDR_LIBID);
  MENU_ID(IDI_SAVE);

  resourceModule->End();
}

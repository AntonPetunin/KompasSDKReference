////////////////////////////////////////////////////////////////////////////////
//
// MacroObj.cpp - Библиотека на Visual C++
//
////////////////////////////////////////////////////////////////////////////////
#include "MacroObj_pch.h"

#include "MacroObj.h"
#include "Orifice.h"


ksapi::IApplication * ksAppl = nullptr;
ksapi::IProceduresLibraryPtr myLibrary;
extern void * GetModule();

//-------------------------------------------------------------------------------
// Задать идентификатор ресурсов
// ---
std::int32_t GetMenuId()
{
  return IDR_LIBID;
}


//-------------------------------------------------------------------------------
// Головная функция библиотеки
// ---
void RunCommand(int32_t commandId, ksapi::ksRunCommandModeEnum mode)
{
  switch (commandId)
  {
    case 1:
    {
      Orifice * macroObj = new Orifice();

      if (macroObj->InitProcessParam())
        macroObj->ChoiceFaces();

      break;
    }
  }
}

extern void LoadResources(ksapi::IApplication & app);
extern ksapi::ILibraryPropertiesObject * LibPropObjectEntry(ksapi::IKompasAPIObjectPtr currentObject);

//------------------------------------------------------------------------------
/**
  Получить номер версии приложения.
*/
//---
void GetApplicationVersion(int32_t & major, int32_t & minor, int32_t & release, int32_t & build)
{
  major = BuildVersionMajor;
  minor = BuildVersionMinor;
  release = BuildVersionPatch;
  build = BuildVersionBuild;
}


//------------------------------------------------------------------------------
/**
  Инициализация библиотеки.
*/
//---
APP_EXP_FUNC(bool) LoadKompasLibrary(ksapi::IApplication & app, ksapi::IKompasLibraryActions & libraryActions)
{
  libraryActions.AddGetApplicationVersion(GetApplicationVersion);
  libraryActions.AddRunCommandHandler(RunCommand);
  libraryActions.AddGetLibraryPropertiesObjectHandler(LibPropObjectEntry);
  libraryActions.AddGetMenuIdHandler(GetMenuId);

  ksAppl = &app;

  if (ksapi::ILibraryManagerPtr libraryManager = app.GetLibraryManager())
  {
    if (ksapi::IProceduresLibrariesPtr proceduresLibraries = libraryManager->GetProceduresLibraries())
      myLibrary = proceduresLibraries->GetItemByName(L"MacroObj");
  }

  LoadResources(app);

  return true;
}


//------------------------------------------------------------------------------
/**
  Освобождения ресурсов перед выгрузкой библиотеки.
*/
//---
APP_EXP_FUNC(void) UnloadKompasLibrary()
{
}


//------------------------------------------------------------------------------
// Загрузить строку из ресурса
// ---
std::wstring LoadStr(std::int32_t strID)
{
  std::wstring resStr;

  if (ksAppl)
  {
    if (ksapi::ILibraryManagerPtr libMng = ksAppl->GetLibraryManager())
    {
      if (ksapi::IProceduresLibrariesPtr procLibs = libMng->GetProceduresLibraries())
      {
        if (ksapi::IProceduresLibraryPtr procLib = procLibs->GetItemByName(L"MacroObj"))
        {
          if (ksapi::IKompasDictionaryPtr dictionary = procLib->GetDictionary())
            resStr = dictionary->LoadAndConvertLangString(GetModule(), strID);
        }
      }
    }
  }

  return resStr;
}

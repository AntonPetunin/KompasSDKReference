////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////

#include "OneWindow_pch.h"

#include "OneWindow.h"
#include "LibObj.h"

ksapi::IApplication * application; ///< Приложение

//------------------------------------------------------------------------------
/**
  Задать идентификатор ресурсов
*/
//---
unsigned int GetMenuId()
{
  return IDR_LIBID;
}


//------------------------------------------------------------------------------
/**
  Головная функция библиотеки
*/
//---
void RunCommand(int32_t commandId, ksapi::ksRunCommandModeEnum mode)
{
  if (application)
  {
    if (ksapi::IKompasDocument3DPtr doc3D = application->GetActiveDocument())
    {
      if (ksapi::IDocumentFramePtr frame = doc3D->GetDocumentFrame())
      {
        if (LibObj * obj = FindOrCreateLibObj(frame, true))
        {
          switch (commandId)
          {
            case 1:
            {
              obj->CreateForce();
              break;
            }
            case 4:
            {
              obj->OnOfTree();
              break;
            }
            case 5:
            {
              obj->Refresh();
              break;
            }
          }
        }
      }
    }
  }
}

extern void LoadResources(ksapi::IApplication & app);

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
  libraryActions.AddGetMenuIdHandler(GetMenuId);

  application = &app;
  LoadResources(app);

  return true;
}

extern void ClearLibObjs();

//------------------------------------------------------------------------------
/**
  Освобождения ресурсов перед выгрузкой библиотеки.
*/
//---
APP_EXP_FUNC(void) UnloadKompasLibrary()
{
  ClearLibObjs();
}

extern void * GetModule();

//------------------------------------------------------------------------------
/**
  Загрузить строку из ресурса
*/
//---
std::wstring LoadStr(std::int32_t strID)
{
  std::wstring res;

  if (ksapi::ILibraryManagerPtr libraryManager = application ? application->GetLibraryManager() : nullptr)
  {
    if (ksapi::IProceduresLibrariesPtr proceduresLibraries = libraryManager->GetProceduresLibraries())
    {
      if (ksapi::IProceduresLibraryPtr lib = proceduresLibraries->GetItemByName(L"OneWindow"))
      {
        if (ksapi::IKompasDictionaryPtr dictionary = lib->GetDictionary())
          return dictionary->LoadAndConvertLangString(GetModule(), strID);
      }
    }
  }

  return {};
}

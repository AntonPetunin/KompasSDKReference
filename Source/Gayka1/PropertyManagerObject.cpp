////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////

#include "Gayka1_pch.h"

#include "PropertyManagerObject.h"
#include "Resource.h"
#include "ParameterObject.h"
#include <format>

extern ksapi::IApplication * application;
extern void * GetModule();

//------------------------------------------------------------------------------
/**
  Загрузить строку из ресурса
*/
//---
std::wstring LoadStr(int id)
{
  std::wstring resStr;

  if (application)
  {
    if (ksapi::ILibraryManagerPtr libMng = application->GetLibraryManager())
    {
      if (ksapi::IProceduresLibrariesPtr procLibs = libMng->GetProceduresLibraries())
      {
        if (ksapi::IProceduresLibraryPtr procLib = procLibs->GetItemByName(L"Конструкторский элемент на Visual C++ с панелью"))
        {
          if (ksapi::IKompasDictionaryPtr dictionary = procLib->GetDictionary())
            resStr = dictionary->LoadAndConvertLangString(GetModule(), id);
        }
      }
    }
  }

  return resStr;
}


//------------------------------------------------------------------------------
/**
  Вывод сообщения
*/
//---
int LibMessage(const std::wstring & str, ksMessageTypeEnum dlgType, ksMessageButtonSetEnum dlgButtons, bool showModal)
{
  if (!str.empty() && application)
    // текст сообщения, заголовок, параметры
    return application->ShowMessageBox(str, LoadStr(ID_LIBID), dlgType, dlgButtons, showModal);

  return 0;
}


//------------------------------------------------------------------------------
/**
  Выдача библиотекой сообщения из ресурсов 
*/
//---
int LibMessage(int strId, ksMessageTypeEnum dlgType, ksMessageButtonSetEnum dlgButtons, bool showModal)
{
  return ::LibMessage(LoadStr(strId), dlgType, dlgButtons, showModal);
}


//------------------------------------------------------------------------------
/**
  Получение файла помощи
*/
//---
void OpenHelp(int Id)
{
  if (application)
  {
    if (ksapi::ILibraryManagerPtr libMng = application->GetLibraryManager())
    {
      if (ksapi::IProceduresLibrariesPtr procLibs = libMng->GetProceduresLibraries())
      {
        if (ksapi::IProceduresLibraryPtr procLib = procLibs->GetItemByName(LoadStr(ID_LIBID)))
          procLib->OpenHelp(Id);
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief Библиотека MyConverter.

  Для регистрации MyConverter в системе требуется запустить файл MyConverter.reg (Windows), MyCinverter.sh(Linux).
  Скрипт регистрации поместит информацию о конвертере в реестр (имитацию реестра, для Linux).
*/
//
////////////////////////////////////////////////////////////////////////////////
#include <MyConverter_pch.h>

#include <MyConverter.h>
#include <LibConverter.h>

#include <format>

ksapi::IApplication * kompasApp = nullptr;

void * GetResourceModule();                    // Модуль ресурсов с загруженными ресурсами
void LoadResources(ksapi::IApplication & app); // Модуль ресурсов с загруженными ресурсами

//------------------------------------------------------------------------------
/**
  Задать идентификатор ресурсов.
*/
//---
int32_t GetMenuId()
{
  return IDR_LIBID;
}

//------------------------------------------------------------------------------
/**
  Выполнение команды
*/
//---
void RunCommand(unsigned int commandId, ksapi::ksRunCommandModeEnum mode)
{
  // Передаём команду меню в конвертер
  // commandId = 1 - "Сохранение текущего файла"
  // commandId = 2 - "Открыть файл"
  GetLibConverter().ExecuteCommand(commandId);
}

//------------------------------------------------------------------------------
/**
  Инициализация библиотеки.
*/
//---
APP_EXP_FUNC(bool) LoadKompasLibrary(ksapi::IApplication & app, ksapi::IKompasLibraryActions & libraryActions)
{
  // Обработчик команд библиотеки.
  libraryActions.AddRunCommandHandler(RunCommand);
  // Обработчик, отдающий конвертер в Компас.
  libraryActions.AddGetKompasConverterHandler([] { return GetLibConverter().GetKompasConverter(); });
  // ID меню в ресурсах.
  libraryActions.AddGetMenuIdHandler(GetMenuId);

  kompasApp = &app;
  LoadResources(app); // Функция загрузки строковых ресурсов.

  return true;
}


//------------------------------------------------------------------------------
/**
  Освобождения ресурсов перед выгрузкой библиотеки.
*/
//---
APP_EXP_FUNC(void) UnloadKompasLibrary()
{
  ConverterDestroy();
}

//------------------------------------------------------------------------------
/**
  Получить интерфейс прикладной библиотеки.
*/
//---
static ksapi::IProceduresLibraryPtr GetLibrary()
{
  static ksapi::IProceduresLibraryPtr proceduresLibrary;

  if (!proceduresLibrary && kompasApp)
    if (ksapi::ILibraryManagerPtr libMng = kompasApp->GetLibraryManager())
      if (ksapi::IProceduresLibrariesPtr procLibs = libMng->GetProceduresLibraries())
        proceduresLibrary = procLibs->GetItemByName(kLibName);

  return proceduresLibrary;
}

//------------------------------------------------------------------------------
/**
  Загрузить строку из ресурса.
*/
//---
std::wstring LoadResourceString(int32_t strId)
{
  if (ksapi::IProceduresLibraryPtr lib = GetLibrary())
    if (ksapi::IKompasDictionaryPtr dictionary = lib->GetDictionary())
      return dictionary->LoadAndConvertLangString(GetResourceModule(), strId);

  return std::wstring();
}


//------------------------------------------------------------------------------
/**
  Выдать сообщение с указанной строкой.
*/
//---
ksMessageButtonEnum LibMessage(const std::wstring & str, ksMessageTypeEnum dialogType, ksMessageButtonSetEnum dialogButtons)
{
  return kompasApp->ShowMessageBox(str, LoadResourceString(IDR_LIBID), dialogType, dialogButtons, true /*showModal*/);
}

//------------------------------------------------------------------------------
/**
  Выдать сообщение со строкой из ресурсов.
*/
//---
ksMessageButtonEnum LibMessage(int32_t strId, ksMessageTypeEnum dialogType, ksMessageButtonSetEnum dialogButtons)
{
  return LibMessage(LoadResourceString(strId), dialogType, dialogButtons);
}

//------------------------------------------------------------------------------
/**
  Найти в конце строки расширение и вернуть его.
*/
//---
std::wstring FindExt(const std::wstring & filename)
{
  size_t pos = filename.rfind(L".");
  if (pos == std::wstring::npos)
    return {};
  std::wstring ret = filename.substr(pos);
  if (ret.back() == L'|')
    ret.pop_back();
  return ret;
}

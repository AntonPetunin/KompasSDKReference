////////////////////////////////////////////////////////////////////////////////
//
// UserSymbolWithLeader - Линия выноски с пользовательским символом (KsAPI)
//
////////////////////////////////////////////////////////////////////////////////
#include <UserSymbolWithLeader_pch.h>
#include <UserSymbol.h>

#include <format>

void * GetModule();
void LoadResources(ksapi::IApplication & app);

ksapi::IApplication * kompasApp = nullptr;

//------------------------------------------------------------------------------
/**
  Получить интерфейс прикладной библиотеки.
*/
//---
static ksapi::IProceduresLibraryPtr GetLibrary()
{
  static IProceduresLibraryPtr res;

  if (!res && kompasApp)
    if (ILibraryManagerPtr libMng = kompasApp->GetLibraryManager())
      if (IProceduresLibrariesPtr procLibs = libMng->GetProceduresLibraries())
        res = procLibs->GetItemByName(klibName);

  return res;
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
      return dictionary->LoadAndConvertLangString(GetModule(), strId);

  return std::wstring();
}

//------------------------------------------------------------------------------
/**
  Задать идентификатор ресурсов.
*/
//---
int32_t GetMenuId()
{
  return IDR_LIB;
}

//------------------------------------------------------------------------------
/**
  Выполнение команды
*/
//---
void RunCommand(unsigned int commandId, ksapi::ksRunCommandModeEnum mode)
{
  switch (commandId)
  {
    case 1:
      (new UserSymbol)->Draw();
      break;
  }
}

//------------------------------------------------------------------------------
/**
  Инициализация библиотеки.
*/
//---
APP_EXP_FUNC(bool) LoadKompasLibrary(ksapi::IApplication & app, ksapi::IKompasLibraryActions & libraryActions)
{
  libraryActions.AddRunCommandHandler(RunCommand); // Обработчик команд.
  libraryActions.AddGetMenuIdHandler(GetMenuId);   // ID меню в ресурсах.

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
  // отписаться от всех событий.
  ABaseEvent::TerminateEvents();
}

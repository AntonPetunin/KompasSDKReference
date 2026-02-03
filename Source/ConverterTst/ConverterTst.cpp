////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief Библиотека ConverterTst, тестирования конвертера MyConverter.

  MyConverter должен быть зарегистрирован в реестре под именем MyLibConverter (см. комментарий в MyConverter).
*/
//
////////////////////////////////////////////////////////////////////////////////
#include <ConverterTst_pch.h>

#include <../MyConverter/LibConverterParamInterface.h>

#include <format>

ksapi::IApplication * kompasApp = nullptr;

void * GetResourceModule();                    // Модуль ресурсов с загруженными ресурсами.
void LoadResources(ksapi::IApplication & app); // Загрузить ресурсы из XML.
void TestConverter();                          // Вызвать функцию конвертирования из MyConverter.

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
  if (commandId == 1)
    TestConverter();
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
  Получить путь до конвертера.
*/
//---
std::wstring GetConverterPath()
{
  if (ksapi::ILibraryManagerPtr libManager = kompasApp->GetLibraryManager())
    return libManager->GetConverterPath(L"MyLibConverter");
  return {};
}


//------------------------------------------------------------------------------
/**
  Вызвать функцию конвертирования из MyConverter.
*/
//---
void TestConverter()
{
  int32_t err = 0;

  ksapi::IConverterPtr converter;
  ILibConverterParamPtr convertParams;
  if (!kompasApp)
    err = IDS_NO_GET_API;
  else if (!(converter = kompasApp->GetConverterByPath(GetConverterPath())))
    err = IDS_NO_GET_CONVERTER;
  else if (!(convertParams = converter->GetConverterParameters(0 /*command*/)))
    err = IDS_NO_GET_CONVERT_PARAMS;

  if (err != 0)
  {
    LibMessage(err, ksMessageError, ksButtonSetOk);
    return;
  }

  ksMessageButtonEnum btn = kompasApp->ShowMessageDialog(nullptr /*windowHandle*/, LoadResourceString(IDS_STYLE_DOC) /*text*/,
                                                         LoadResourceString(IDR_LIBID) /*caption*/, L"" /*explanation*/,
                                                         ksMessageQuestion, ksButtonSetYesNo, true /*showModal*/,
                                                         LoadResourceString(IDS_DLG_BTN_YES), LoadResourceString(IDS_DLG_BTN_NO),
                                                         L"" /*cancelButton*/, 0 /*helpId*/, L"" /*helpFileName*/);
  convertParams->SetSaveLineStyle(btn == ksButtonYes);
  err = converter->Convert(L"ConverterTst.cdw", L"ConverterTst.mycdw", libConv_Exp_cdw, false /*showParam*/);
  LibMessage(err == 1 ? IDS_CONVERT_SUCCESS : IDS_CONVERT_ERROR, ksMessageInformation, ksButtonSetOk);
}

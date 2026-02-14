#include "MessageUtils.h"
#include "Resource.h"

extern ksapi::IApplication * kompasApp;
extern void * GetModule();

//------------------------------------------------------------------------------
/**
  Загрузить строку из ресурса.
*/
//---
std::wstring LoadStr(std::int32_t strId)
{
  if (ksapi::ILibraryManagerPtr libraryManager = kompasApp->GetLibraryManager())
  {
    if (ksapi::IProceduresLibrariesPtr proceduresLibraries = libraryManager->GetProceduresLibraries())
    {
      if (ksapi::IProceduresLibraryPtr lib = proceduresLibraries->GetItemByName(libName))
      {
        if (ksapi::IKompasDictionaryPtr dictionary = lib->GetDictionary())
          return dictionary->LoadAndConvertLangString(GetModule(), strId);
      }
    }
  }

  return {};
}

//------------------------------------------------------------------------------
/**
  Сообщение от библиотеки.
*/
//---
std::int32_t LibMessage(const std::wstring & message, ksMessageTypeEnum dlgType, ksMessageButtonSetEnum dlgButtons,
                        bool showModal)
{
  std::int32_t res = 0;

  if (!message.empty() && kompasApp)
    res = kompasApp->ShowMessageBox(message, LoadStr(IDR_LIBID), dlgType, dlgButtons, showModal);

  return res;
}

//------------------------------------------------------------------------------
/**
  Диалог да/нет/отмена.
*/
//---
std::int32_t YesNoMessage(const std::wstring & message, bool question)
{
  std::int32_t res = LibMessage(message, question ? ksMessageQuestion : ksMessageWarning, ksButtonSetYesNoCancel, false);
  return std::int32_t(res == DialogResult::Positive);
}

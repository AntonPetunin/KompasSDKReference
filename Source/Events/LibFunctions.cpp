#include "LibFunctions.h"
#include "BaseEvent.h"
#include "Events_pch.h"

extern ksapi::IApplication * kompasApp; // Интерфейс Приложения КОМПАС-3D

void * GetModule();

//------------------------------------------------------------------------------
/**
  Загрузить строку из ресурса.
*/
//---
std::wstring LoadResourceString(int32_t strId)
{
  if (ksapi::ILibraryManagerPtr libraryManager = kompasApp->GetLibraryManager())
  {
    if (ksapi::IProceduresLibrariesPtr proceduresLibraries = libraryManager->GetProceduresLibraries())
    {
      if (ksapi::IProceduresLibraryPtr lib = proceduresLibraries->GetItemByName(libname))
      {
        if (ksapi::IKompasDictionaryPtr dictionary = lib->GetDictionary())
          return dictionary->LoadAndConvertLangString(GetModule(), strId);
      }
    }
  }

  return std::wstring();
}

//------------------------------------------------------------------------------
/**
  Выдать сообщение с указанной строкой.
*/
//---
ksMessageButtonEnum LibMessageNoModal(const std::wstring & str, ksMessageTypeEnum dialogType,
                                      ksMessageButtonSetEnum dialogButtons, bool showModal)
{
  return kompasApp->ShowMessageBox(str, libname, dialogType, dialogButtons, showModal);
}


//------------------------------------------------------------------------------
/**
  Выдать сообщение с указанной строкой.
*/
//---
ksMessageButtonEnum LibMessage(const std::wstring & str, ksMessageTypeEnum dialogType, ksMessageButtonSetEnum dialogButtons,
                               bool showModal)
{
  return kompasApp->ShowMessageBox(str, libname, dialogType, dialogButtons, showModal);
}


//------------------------------------------------------------------------------
/**
  Выдать сообщение с указанной строкой.
*/
//---
ksMessageButtonEnum LibMessage(int32_t ids, ksMessageTypeEnum dialogType, ksMessageButtonSetEnum dialogButtons, bool showModal)
{
  return kompasApp->ShowMessageBox(LoadResourceString(ids), libname, dialogType, dialogButtons, showModal);
}


//------------------------------------------------------------------------------
/**
  .
*/
//---
ksMessageButtonEnum YesNoDialog(const std::wstring & explanation, const std::wstring & positiveButton,
                                const std::wstring & negativeButton, const std::wstring & cancelButton)
{
  return kompasApp->ShowMessageDialog(nullptr, explanation, libname, explanation, ksMessageTypeEnum::ksMessageQuestion,
                                      ksMessageButtonSetEnum::ksButtonSetYesNoCancel, true, positiveButton, negativeButton,
                                      cancelButton, -1, L"");
}

//------------------------------------------------------------------------------
/**
  .
*/
//---
ksMessageButtonEnum YesNoCancelDialog(const std::wstring & question)
{
  return kompasApp->ShowMessageDialog(nullptr, question, libname, question, ksMessageTypeEnum::ksMessageQuestion,
                                      ksMessageButtonSetEnum::ksButtonSetYesNoCancel, true, L"", L"", L"", -1, L"");
}

//------------------------------------------------------------------------------
/**
  .
*/
//---
ksMessageButtonEnum YesNoDialog(const std::wstring & question)
{
  return kompasApp->ShowMessageDialog(nullptr, question, libname, question, ksMessageTypeEnum::ksMessageQuestion,
                                      ksMessageButtonSetEnum::ksButtonSetYesNo, true, L"", L"", L"", -1, L"");
}

//------------------------------------------------------------------------------
/**
  .
*/
//---
ksapi::IKompasDocumentPtr CreateDocument(DocumentTypeEnum type)
{
  ksapi::IKompasDocumentPtr res;

  if (ksapi::IDocumentsPtr docs = kompasApp->GetDocuments())
    res = docs->Add(type, true);

  return res;
}

//------------------------------------------------------------------------------
/**
  .
*/
//---
void PrintKompasError()
{
  ksapi::IKompasErrorPtr error = kompasApp->GetKompasError();
  if (error->GetCode() != etSuccess)
  {
    LibMessage(FormatResourceString(IDS_ERROR_CODE, error->GetCode()), ksMessageTypeEnum::ksMessageError);
    error->Clear(); // Обнулить ошибки
  }
}

//------------------------------------------------------------------------------
/**
  
*/
//---
std::wstring DocumentType(DocumentTypeEnum docType)
{
  switch (docType)
  {
    case DocumentTypeEnum::ksDocumentUnknown:
      return LoadResourceString(IDS_DOC_DOC);
    case DocumentTypeEnum::ksDocumentDrawing:
      return LoadResourceString(IDS_DOC_DRAWING);
    case DocumentTypeEnum::ksDocumentFragment:
      return LoadResourceString(IDS_DOC_FRAGMENT);
    case DocumentTypeEnum::ksDocumentPart:
      return LoadResourceString(IDS_DOC_PART);
    case DocumentTypeEnum::ksDocumentAssembly:
    case DocumentTypeEnum::ksDocumentTechnologyAssembly:
      return LoadResourceString(IDS_DOC_ASSEMBLY);
    case DocumentTypeEnum::ksDocumentSpecification:
      return LoadResourceString(IDS_DOC_SPC);
    case DocumentTypeEnum::ksDocumentTextual:
      return LoadResourceString(IDS_DOC_TXT);
    default:
      return L"";
  }

  return {};
}

//------------------------------------------------------------------------------
/**
  
*/
//---
std::wstring SeparateStringVector(const std::vector<std::wstring> & vec)
{
  std::wstring args;
  if (!vec.empty())
  {
    for (const auto & el : vec)
      args += std::filesystem::path(el).filename().wstring() + L",";

    args.erase(args.size() - 1);
  }
  return args;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
std::wstring GetTempApi()
{
  return GetTempApi(EventType::Unkown);
}


//------------------------------------------------------------------------------
/**
  
*/
//---
std::wstring GetTempApi(EventType eventType)
{
  auto tmp = std::filesystem::temp_directory_path().wstring();
  tmp += L"API\\";

  switch (eventType)
  {
    case EventType::Document:
      tmp += L"EventCom_DocEvents\\";
      break;
    default:
      break;
  }

  if (!std::filesystem::exists(tmp))
    std::filesystem::create_directories(tmp);

  return tmp;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
std::wstring GetFileName(const std::wstring path)
{
  return std::filesystem::path(path).filename().wstring();
}


//------------------------------------------------------------------------------
/**

*/
//---
ksapi::IProceduresLibraryPtr GetLibrary()
{
  static ksapi::IProceduresLibraryPtr res;

  if (!res && kompasApp)
  {
    if (ksapi::ILibraryManagerPtr libMng = kompasApp->GetLibraryManager())
    {
      if (ksapi::IProceduresLibrariesPtr procLibs = libMng->GetProceduresLibraries())
        res = procLibs->GetItemByName(libname);
    }
  }

  return res;
}

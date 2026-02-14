////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
#include "SpecificationDocumentEvent.h"
#include "LibFunctions.h"

extern ksapi::IApplication * kompasApp; // Интерфейс Приложения КОМПАС-3D
extern AppConfig theApp;

//------------------------------------------------------------------------------
/**
  
*/
//---
SpecificationDocumentEvent::SpecificationDocumentEvent(const ksapi::IKompasDocumentPtr document)
  : BaseEvent(EventType::SpecificationDocument, document)
{
}


//------------------------------------------------------------------------------
/**
  
*/
//---
SpecificationDocumentEvent::~SpecificationDocumentEvent()
{
  UnAdvise();
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool SpecificationDocumentEvent::Advise()
{
  bool res = BaseEvent::Advise() && m_doc;

  if (!res) // Если уже подписались
    return res;

  auto handlerOwner = libname;

  if (ksapi::ISpecificationDocumentPtr spcDoc = m_doc)
  {
    ksapi::ISpecificationDocumentEventsPtr events = spcDoc->GetSpecificationDocumentEvents();
    events->AddDocumentAddHandler(handlerOwner, [this](const std::wstring & sheetName) { OnDocumentAdd(sheetName); });
    events->AddDocumentRemoveHandler(handlerOwner, [this](const std::wstring & sheetName) { OnDocumentRemove(sheetName); });
    events->AddDocumentBeginRemoveHandler(handlerOwner,
                                          [this](const std::wstring & sheetName) { return OnDocumentBeginRemove(sheetName); });
    events->AddDocumentBeginAddHandler(handlerOwner, [this]() { return OnDocumentBeginAdd(); });
    events->AddSpcStyleChangeHandler(handlerOwner, [this](const std::wstring & sheetName, int32_t formNumber)
                                     { return OnSpcStyleChange(sheetName, formNumber); });
    events->AddSpcStyleBeginChangeHandler(handlerOwner, [this](const std::wstring & sheetName, int32_t formNumber)
                                          { return OnSpcStyleBeginChange(sheetName, formNumber); });

    ShowEventName(LoadResourceString(IDS_ADVISE_SPC_DOC));
  }


  return res;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationDocumentEvent::UnAdvise()
{
  if (m_advise && m_doc)
  {
    if (ksapi::ISpecificationDocumentPtr spcDoc = m_doc)
    {
      spcDoc->GetSpecificationDocumentEvents()->RemoveAllHandlers(libname);
    }

    BaseEvent::UnAdvise();

    ShowEventName(LoadResourceString(IDS_UNADVISE_SPC_DOC));
  }
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationDocumentEvent::ShowEventName(const std::wstring & eventName, const std::wstring & args)
{
  if (theApp.m_mes_SPCDOC)
    BaseEvent::ShowEventName(eventName, m_doc, args);
}


#pragma region ООбработчики событий документа спецификации
//------------------------------------------------------------------------------
/**
  
*/
//---
bool SpecificationDocumentEvent::OnDocumentBeginAdd()
{
  ShowEventName(L"SpcDocEvent::DocumentBeginAdd");
  return !theApp.m_mes_SPCDOC || YesNoDialog(LoadResourceString(IDS_SPC_DOC_ADD_DOC)) == ksMessageButtonEnum::ksButtonYes;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationDocumentEvent::OnDocumentAdd(const std::wstring & sheetName)
{
  ShowEventName(L"SpcDocEvent::DocumentAdd", std::format(L"sheetName({})", GetFileName(sheetName)));
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool SpecificationDocumentEvent::OnDocumentBeginRemove(const std::wstring & sheetName)
{
  ShowEventName(L"SpcDocEvent::DocumentBeginRemove", std::format(L"sheetName({})", GetFileName(sheetName)));
  return !theApp.m_mes_SPCDOC || YesNoDialog(LoadResourceString(IDS_SPC_DOC_REMOVE_DOC)) == ksMessageButtonEnum::ksButtonYes;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationDocumentEvent::OnDocumentRemove(const std::wstring & sheetName)
{
  ShowEventName(L"SpcDocEvent::DocumentRemove", std::format(L"sheetName({})", GetFileName(sheetName)));
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool SpecificationDocumentEvent::OnSpcStyleBeginChange(const std::wstring & sheetName, int32_t formNumber)
{
  ShowEventName(L"SpcDocEvent::SpcStyleBeginChange",
                std::format(L"sheetName({}), formNumber({})", GetFileName(sheetName), formNumber));
  return !theApp.m_mes_SPCDOC || YesNoDialog(LoadResourceString(IDS_SPC_DOC_CHANGE_STYLE)) == ksMessageButtonEnum::ksButtonYes;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool SpecificationDocumentEvent::OnSpcStyleChange(const std::wstring & sheetName, int32_t formNumber)
{
  ShowEventName(L"SpcDocEvent::SpcStyleChange",
                std::format(L"sheetName({}), formNumber({})", GetFileName(sheetName), formNumber));
  return true;
}
#pragma endregion

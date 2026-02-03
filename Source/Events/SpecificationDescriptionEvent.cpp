////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
#include "SpecificationDescriptionEvent.h"
#include "LibFunctions.h"

extern ksapi::IApplication * kompasApp; // Интерфейс Приложения КОМПАС-3D
extern AppConfig theApp;

//------------------------------------------------------------------------------
/**
  
*/
//---
SpecificationDescriptionEvent::SpecificationDescriptionEvent(const ksapi::IKompasDocumentPtr document)
  : BaseEvent(EventType::SpecificationDescription, document)
{
}


//------------------------------------------------------------------------------
/**
  
*/
//---
SpecificationDescriptionEvent::~SpecificationDescriptionEvent()
{
  UnAdvise();
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool SpecificationDescriptionEvent::Advise()
{
  bool res = BaseEvent::Advise() && m_doc;

  if (!res) // Если уже подписались
    return res;

  auto handlerOwner = libname;

  if (ksapi::ISpecificationDescriptionsPtr scpDescrs = m_doc->GetSpecificationDescriptions())
  {
    ksapi::ISpecificationDescriptionEventsPtr events = scpDescrs->Events();
    events->AddTuningSpcStyleBeginChangeHandler(handlerOwner, [this](const ksapi::ISpecificationDescriptionPtr & description)
                                                { return OnTuningSpcStyleBeginChange(description); });
    events->AddTuningSpcStyleChangeHandler(handlerOwner,
                                           [this](const ksapi::ISpecificationDescriptionPtr & description, bool wasChanged)
                                           { OnTuningSpcStyleChange(description, wasChanged); });
    events->AddChangeCurrentSpcDescriptionHandler(handlerOwner, [this](const ksapi::ISpecificationDescriptionPtr & description)
                                                  { OnChangeCurrentSpcDescription(description); });
    events->AddSpcDescriptionAddHandler(handlerOwner, [this](const ksapi::ISpecificationDescriptionPtr & description)
                                        { OnSpcDescriptionAdd(description); });
    events->AddSpcDescriptionRemoveHandler(handlerOwner, [this](const ksapi::ISpecificationDescriptionPtr & description)
                                           { OnSpcDescriptionRemove(description); });
    events->AddSpcDescriptionBeginEditHandler(handlerOwner, [this](const ksapi::ISpecificationDescriptionPtr & description)
                                              { return OnSpcDescriptionBeginEdit(description); });
    events->AddSpcDescriptionEditHandler(handlerOwner,
                                         [this](const ksapi::ISpecificationDescriptionPtr & description, bool wasChanged)
                                         { OnSpcDescriptionEdit(description, wasChanged); });
    events->AddBeginCalcPositionsHandler(handlerOwner, [this]() { return OnBeginCalcPositions(); });
    events->AddCalcPositionsHandler(handlerOwner, [this]() { OnCalcPositions(); });
    events->AddBeginCreateObjectHandler(handlerOwner, [this](int32_t objectType) { return OnBeginCreateObject(objectType); });

    ShowEventName(LoadResourceString(IDS_ADVISE_SPC_DESCR));
  }

  return res;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationDescriptionEvent::UnAdvise()
{
  if (m_advise && m_doc)
  {
    if (ksapi::ISpecificationDescriptionsPtr scpDescrs = m_doc->GetSpecificationDescriptions())
    {
      scpDescrs->Events()->RemoveAllHandlers(libname);
    }

    BaseEvent::UnAdvise();
    ShowEventName(LoadResourceString(IDS_UNADVISE_SPC_DESCR));
  }
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationDescriptionEvent::ShowEventName(const std::wstring & eventName, const std::wstring & args)
{
  if (theApp.m_mes_SPC_DESCR)
    BaseEvent::ShowEventName(eventName, m_doc, args);
}


#pragma region Обработчики событий описания спецификации
//------------------------------------------------------------------------------
/**
  
*/
//---
bool SpecificationDescriptionEvent::OnTuningSpcStyleBeginChange(const ksapi::ISpecificationDescriptionPtr & description)
{
  ShowEventName(L"SpecificationEvent::TuningSpcStyleBeginChange");
  return !theApp.m_mes_SPC_DESCR || YesNoDialog(LoadResourceString(IDS_SPC_DOC_CHANGE_STYLE)) == ksMessageButtonEnum::ksButtonYes;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationDescriptionEvent::OnTuningSpcStyleChange(const ksapi::ISpecificationDescriptionPtr & description,
                                                           bool wasChanged)
{
  ShowEventName(L"SpecificationEvent::TuningSpcStyleChange", wasChanged ? L"True" : L"False");
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationDescriptionEvent::OnChangeCurrentSpcDescription(const ksapi::ISpecificationDescriptionPtr & description)
{
  ShowEventName(L"SpecificationEvent::ChangeCurrentSpcDescription");
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationDescriptionEvent::OnSpcDescriptionAdd(const ksapi::ISpecificationDescriptionPtr & description)
{
  ShowEventName(L"SpecificationEvent::SpcDescriptionAdd");
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationDescriptionEvent::OnSpcDescriptionRemove(const ksapi::ISpecificationDescriptionPtr & description)
{
  ShowEventName(L"SpecificationEvent::SpcDescriptionRemove");
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool SpecificationDescriptionEvent::OnSpcDescriptionBeginEdit(const ksapi::ISpecificationDescriptionPtr & description)
{
  ShowEventName(L"SpecificationEvent::SpcDescriptionBeginEdit");
  return !theApp.m_mes_SPC_DESCR || YesNoDialog(LoadResourceString(IDS_SPC_DESCR_EDIT)) == ksMessageButtonEnum::ksButtonYes;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationDescriptionEvent::OnSpcDescriptionEdit(const ksapi::ISpecificationDescriptionPtr & description, bool wasChanged)
{
  ShowEventName(L"SpecificationEvent::SpcDescriptionEdit", wasChanged ? L"True" : L"False");
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool SpecificationDescriptionEvent::OnBeginCalcPositions()
{
  ShowEventName(L"SpecificationEvent::BeginCalcPositions");
  return !theApp.m_mes_SPC_DESCR || YesNoDialog(LoadResourceString(IDS_SPC_DESCR_CALC_POS)) == ksMessageButtonEnum::ksButtonYes;
}


//------------------------------------------------------------------------------
/**
  
*/
//---
void SpecificationDescriptionEvent::OnCalcPositions()
{
  ShowEventName(L"SpecificationEvent::CalcPositions");
}


//------------------------------------------------------------------------------
/**
  
*/
//---
bool SpecificationDescriptionEvent::OnBeginCreateObject(int32_t objectType)
{
  ShowEventName(L"SpecificationEvent::BeginCreateObject", std::to_wstring(objectType));
  return !theApp.m_mes_SPC_DESCR || YesNoDialog(LoadResourceString(IDS_SPC_DESCR_CREATE)) == ksMessageButtonEnum::ksButtonYes;
}
#pragma endregion

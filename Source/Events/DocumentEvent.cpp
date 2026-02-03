////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
#include "DocumentEvent.h"
#include "LibFunctions.h"

extern ksapi::IApplication * kompasApp; // Интерфейс Приложения КОМПАС-3D
extern AppConfig theApp;

////////////////////////////////////////////////////////////////////////////////
//
// DocumentEvent  - обработчик событий документа
//
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
//
// ---
DocumentEvent::DocumentEvent(const ksapi::IKompasDocumentPtr document)
  : BaseEvent(EventType::Document, document)
{
}


//-------------------------------------------------------------------------------
//
// ---
DocumentEvent::DocumentEvent(const ksapi::IKompasDocumentPtr document, bool silent)
  : BaseEvent(EventType::Document, document)
  , m_silent(silent)
{
}


//-------------------------------------------------------------------------------
//
// ---
DocumentEvent::~DocumentEvent()
{
  UnAdvise();
}


//-------------------------------------------------------------------------------
//
// ---
bool DocumentEvent::Advise()
{
  bool res = BaseEvent::Advise() && m_doc;

  if (!res) // Если уже подписались
    return res;

  auto handlerOwner = libname;

  ksapi::IKompasDocumentEventsPtr events = m_doc->Events();

  if (!events)
    return false;

  events->AddBeginCloseDocumentHandler(handlerOwner, [this]() { return this->OnBeginCloseDocument(); });
  events->AddCloseDocumentHandler(handlerOwner, [this]() { this->OnCloseDocument(); });
  events->AddBeginSaveDocumentHandler(handlerOwner,
                                      [this](const std::wstring & fileName) { return this->OnBeginSaveDocument(fileName); });
  events->AddSaveDocumentHandler(handlerOwner, [this]() { this->OnSaveDocument(); });
  events->AddActivateDocumentHandler(handlerOwner, [this]() { return this->OnActiveDocument(); });
  events->AddDeactivateDocumentHandler(handlerOwner, [this]() { this->OnDeactiveDocument(); });
  events->AddBeginSaveAsDocumentHandler(handlerOwner, [this]() { return this->OnBeginSaveAsDocument(); });
  events->AddProcessActivate(handlerOwner, [this](int32_t processId) { return this->OnProcessActivate(processId); });
  events->AddProcessDeactivateHandler(handlerOwner, [this](int32_t processId) { this->OnProcessDeactivate(processId); });
  events->AddBeginProcessHandler(handlerOwner, [this](int32_t processId) { return this->OnBeginProcess(processId); });
  events->AddEndProcessHandler(handlerOwner,
                               [this](int32_t processId, bool success) { return this->OnEndProcess(processId, success); });
  events->AddBeginAutoSaveDocumentHandler(handlerOwner,
                                          [this](const std::wstring & fileName) { this->OnBeginAutoSaveDocument(fileName); });
  events->AddAutoSaveDocumentHandler(handlerOwner, [this]() { this->OnAutoSaveDocument(); });
  events->AddActivateToolbarSetHandler(handlerOwner,
                                       [this](const std::wstring & oldToolbarSetId, const std::wstring & newToolbarSetId)
                                       { this->OnActivateToolbarSet(oldToolbarSetId, newToolbarSetId); });

  ShowEventName(LoadResourceString(IDS_ADVISE_DOC));
  return res;
}


//-------------------------------------------------------------------------------
//
// ---
void DocumentEvent::UnAdvise()
{
  if (m_advise && m_doc)
  {
    if (ksapi::IKompasDocumentEventsPtr events = m_doc->Events())
      events->RemoveAllHandlers(libname);

    BaseEvent::UnAdvise();

    ShowEventName(LoadResourceString(IDS_UNADVISE_DOC));
  }
}


//-------------------------------------------------------------------------------
//
// ---
void DocumentEvent::ShowEventName(const std::wstring & eventName, ksapi::IKompasDocumentPtr document, const std::wstring & args)
{
  if (!m_silent && theApp.m_mes_DOC)
    BaseEvent::ShowEventName(eventName, document, args);
}


#pragma region Обработчики событий документа
//-----------------------------------------------------------------------------
// Начало закрытия документа.
// ---
bool DocumentEvent::OnBeginCloseDocument()
{
  ShowEventName(L"DocumentEvent::BeginCloseDocument");

  return m_silent || !theApp.m_mes_DOC || YesNoDialog(LoadResourceString(IDS_DOC_CLOSE)) == ksMessageButtonEnum::ksButtonYes;
}


//-----------------------------------------------------------------------------
// Документ закрыт.
// ---
void DocumentEvent::OnCloseDocument()
{
  ShowEventName(L"DocumentEvent::CloseDocument");

  if (m_doc)
    EventContainer::UnAdviseByDoc(m_doc);
}


//-----------------------------------------------------------------------------
// Начало сохранения документа.
// ---
bool DocumentEvent::OnBeginSaveDocument(const std::wstring & fileName)
{
  ShowEventName(L"DocumentEvent::BeginSaveDocument", m_doc, std::format(L"docName={}", fileName));
  return m_silent || !theApp.m_mes_DOC || YesNoDialog(LoadResourceString(IDS_DOC_SAVE)) == ksMessageButtonEnum::ksButtonYes;
}


//-----------------------------------------------------------------------------
// Документ сохранен.
// ---
void DocumentEvent::OnSaveDocument()
{
  ShowEventName(L"DocumentEvent::SaveDocument", m_doc);
}


//-----------------------------------------------------------------------------
// Документ активизирован.
// ---
bool DocumentEvent::OnActiveDocument()
{
  ShowEventName(L"DocumentEvent::Activate", m_doc);
  return true;
}


//-----------------------------------------------------------------------------
// Документ деактивизирован.
// ---
void DocumentEvent::OnDeactiveDocument()
{
  ShowEventName(L"DocumentEvent::Deactivate", m_doc);
}

//-----------------------------------------------------------------------------
// Процесс сохранения файла с новым именем.
// ---
bool DocumentEvent::OnBeginSaveAsDocument()
{
  ShowEventName(L"DocumentEvent::BeginSaveAsDocument", m_doc);

  if (m_silent || !theApp.m_mes_DOC)
    return true;

  bool res = false;
  ksMessageButtonEnum button =
    YesNoDialog(FormatResourceString(IDS_DOC_BEGIN_SAVE_QUESTION, DocumentType(m_doc->GetDocumentType())));

  switch (button)
  {
    case ksMessageButtonEnum::ksButtonYes:
    {
      std::wstring name = m_doc->GetFullPath();
      DocumentTypeEnum type = m_doc->GetDocumentType();

      if (name.empty())
      {
        switch (type)
        {
          case DocumentTypeEnum::ksDocumentTextual:
            name = L"1.kdw";
            break;
          case DocumentTypeEnum::ksDocumentFragment:
            name = L"1.frw";
            break;
          case DocumentTypeEnum::ksDocumentDrawing:
            name = L"1.cdw";
            break;
          case DocumentTypeEnum::ksDocumentSpecification:
            name = L"1.spw";
            break;
          case DocumentTypeEnum::ksDocumentPart:
            name = L"1.m3d";
            break;
          case DocumentTypeEnum::ksDocumentAssembly:
          case DocumentTypeEnum::ksDocumentTechnologyAssembly:
            name = L"1.a3d";
            break;
        }
        res = !m_doc->SaveAs(GetTempApi(m_eventType) + name);
      }
      else
      {
        res = !m_doc->Save();
      }

      break;
    }

    case ksMessageButtonEnum::ksButtonNo:
    {
      res = true;
      break;
    }
    case ksMessageButtonEnum::ksButtonCancel:
    {
      res = false;
    }
  }
  return res;
}


//-----------------------------------------------------------------------------
// Активизация процесса
// ---
bool DocumentEvent::OnProcessActivate(int32_t processId)
{
  ShowEventName(L"DocumentEvent::ProcessActivate", m_doc, std::format(L"id={}", processId));
  return true;
}

//-----------------------------------------------------------------------------
// Деактивизация процесса
// ---
void DocumentEvent::OnProcessDeactivate(int32_t processId)
{
  ShowEventName(L"DocumentEvent::ProcessDeactivate", m_doc, std::format(L"id={}", processId));
}


//-----------------------------------------------------------------------------
// Начало процесса.
// ---
bool DocumentEvent::OnBeginProcess(int32_t processId)
{
  ShowEventName(L"DocumentEvent::BeginProcess", m_doc, std::format(L"id={}", processId));
  return true;
}


//-----------------------------------------------------------------------------
// Завершение процесса
// ---
bool DocumentEvent::OnEndProcess(int32_t processId, bool success)
{
  ShowEventName(L"DocumentEvent::EndProcess", m_doc, std::format(L"id={}, success={}", processId, success ? L"true" : L"false"));
  return true;
}

//-----------------------------------------------------------------------------
// Начало автосохранения документа.
// ---
void DocumentEvent::OnBeginAutoSaveDocument(const std::wstring & fileName)
{
  ShowEventName(L"DocumentEvent::BeginAutoSaveDocument", m_doc, std::format(L"docName={}", fileName));
  //TODO: return m_silent ||!theApp.m_mes_DOC || YesNoDialog(IDS_DOC_AUTOSAVE) == ksMessageButtonEnum::ksButtonYes;
}


//-----------------------------------------------------------------------------
// Документ автосохранен
// ---
void DocumentEvent::OnAutoSaveDocument()
{
  ShowEventName(L"DocumentEvent::AutoSaveDocument", m_doc);
}

//-----------------------------------------------------------------------------
//
// ---
void DocumentEvent::OnActivateToolbarSet(const std::wstring & oldToolbarSetId, const std::wstring & newToolbarSetId)
{
  ShowEventName(L"DocumentEvent::OnActivateToolbarSet", m_doc,
                std::format(L"oldToolbarSetId={}, newToolbarSetId={}", oldToolbarSetId, newToolbarSetId));
}
#pragma endregion

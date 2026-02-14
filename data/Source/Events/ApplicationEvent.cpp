#include "ApplicationEvent.h"
#include "LibFunctions.h"

extern ksapi::IApplication * kompasApp; // Интерфейс Приложения КОМПАС-3D
extern AppConfig theApp;

//-------------------------------------------------------------------------------
//
// ---
ApplicationEvent::ApplicationEvent()
  : BaseEvent(EventType::Application)
{
}


//-------------------------------------------------------------------------------
//
// ---
ApplicationEvent::~ApplicationEvent()
{
  UnAdvise();
}


//-------------------------------------------------------------------------------
//
// ---
bool ApplicationEvent::Advise()
{
  bool res = BaseEvent::Advise();

  if (!res) // Если уже подписались
    return res;

  ksapi::IApplicationEventsPtr events = kompasApp->Events();

  if (!events)
    return false;

  auto handlerOwner = libname;

  events->AddBeginCreateHandler(handlerOwner, [this](DocumentTypeEnum docType) { return this->OnBeginCreate(docType); });
  events->AddCreateDocumentHandler(handlerOwner,
                                   [this](ksapi::IKompasDocumentPtr document) { this->OnCreateDocument(document); });
  events->AddBeginOpenDocumentHandler(handlerOwner,
                                      [this](const std::wstring & fileName) { return this->OnBeginOpenDocument(fileName); });
  events->AddOpenDocumentHandler(handlerOwner, [this](ksapi::IKompasDocumentPtr document) { this->OnOpenDocument(document); });
  events->AddBeginOpenFileHandler(handlerOwner, [this]() { return this->OnBeginOpenFile(); });
  events->AddBeginDragOpenFilesHandler(handlerOwner, [this](std::vector<std::wstring> & fileNames, bool insert)
                                       { return this->OnBeginDragOpenFiles(fileNames, insert); });
  events->AddBeginRequestFilesHandler(handlerOwner,
                                      [this](ksRequestFilesTypeEnum requestId, std::vector<std::wstring> & fileNames)
                                      { return this->OnBeginRequestFiles(requestId, fileNames); });
  events->AddChangeActiveDocumentHandler(handlerOwner,
                                         [this](ksapi::IKompasDocumentPtr document) { this->OnChangeActiveDocument(document); });
  events->AddApplicationDestroyHandler(handlerOwner, [this]() { this->OnApplicationDestroy(); });
  events->AddBeginCloseAllDocumentHandler(handlerOwner, [this]() { return this->OnBeginCloseAllDocument(); });
  events->AddKeyDownHandler(handlerOwner, [this](int32_t & keyCode, int32_t keyFlags, const ksapi::IPressedKeysPtr & systemKeys)
                            { return this->OnKeyDown(keyCode, keyFlags, systemKeys); });
  events->AddKeyUpHandler(handlerOwner, [this](int32_t & keyCode, int32_t keyFlags, const ksapi::IPressedKeysPtr & systemKeys)
                          { return this->OnKeyUp(keyCode, keyFlags, systemKeys); });
  events->AddKeyPressedHandler(handlerOwner,
                               [this](int32_t & keyCode, int32_t keyFlags, const ksapi::IPressedKeysPtr & systemKeys)
                               { return this->OnKeyPressed(keyCode, keyFlags, systemKeys); });
  events->AddBeginChoiceMaterialHandler(handlerOwner,
                                        [this](int32_t materialId) { return this->OnBeginChoiceMaterial(materialId); });
  events->AddChoiceMaterialHandler(handlerOwner, [this](int32_t materialId, const std::wstring & materialName, double density)
                                   { this->OnChoiceMaterial(materialId, materialName, density); });
  events->AddIsNeedConvertToSavePreviousHandler(handlerOwner,
                                                [this](ksapi::IKompasDocumentPtr document, ksSaveDocumentVersionEnum version,
                                                       ksapi::ISaveToPreviousParamPtr saveParam, bool & needConvert)
                                                {
                                                  return this->OnIsNeedConvertToSavePrevious(document, version, saveParam,
                                                                                             needConvert);
                                                });
  events->AddBeginConvertToSavePreviousHandler(handlerOwner,
                                               [this](ksapi::IKompasDocumentPtr document, ksSaveDocumentVersionEnum version,
                                                      ksapi::ISaveToPreviousParamPtr saveParam)
                                               { return this->OnBeginConvertToSavePrevious(document, version, saveParam); });
  events->AddEndConvertToSavePreviousHandler(handlerOwner,
                                             [this](ksapi::IKompasDocumentPtr document, ksSaveDocumentVersionEnum version,
                                                    ksapi::ISaveToPreviousParamPtr saveParam)
                                             { return this->OnEndConvertToSavePrevious(document, version, saveParam); });
  events->AddChangeThemeHandler(handlerOwner, [this](ksThemeEnum theme) { return this->OnChangeTheme(theme); });
  events->AddBeginChoiceMarkingHandler(handlerOwner, [this](const std::wstring & oldMarking, std::wstring & newMarking)
                                       { return this->OnBeginChoiceMarking(oldMarking, newMarking); });

  return res;
}


//-------------------------------------------------------------------------------
//
// ---
void ApplicationEvent::UnAdvise()
{
  if (m_advise)
  {
    if (ksapi::IApplicationEventsPtr events = kompasApp->Events())
      events->RemoveAllHandlers(libname);

    BaseEvent::UnAdvise();
  }
}


//-------------------------------------------------------------------------------
//
// ---
void ApplicationEvent::ShowEventName(const std::wstring & eventName, ksapi::IKompasDocumentPtr document,
                                     const std::wstring & args)
{
  if (theApp.m_mes_APP)
    BaseEvent::ShowEventName(eventName, document, args);
}


#pragma region Обработчики событий
//-------------------------------------------------------------------------------
//
// ---
bool ApplicationEvent::OnBeginCreate(DocumentTypeEnum docType)
{
  ShowEventName(L"ApplicationEvent::BeginCreate", nullptr, std::to_wstring(docType));

  if (!theApp.m_mes_APP)
    return true;

  bool res = false;

  ksMessageButtonEnum button = ksMessageButtonEnum::ksButtonCancel;

  if (docType == DocumentTypeEnum::ksDocumentUnknown)
  { // Тип документа не известен - предлагаем создать чертёж
    button =
      YesNoCancelDialog(FormatResourceString(IDS_APP_BEGIN_CREATE_QUESTION1, DocumentType(DocumentTypeEnum::ksDocumentDrawing)));
  }
  else
  { // Тип документа известен - создём
    button = YesNoDialog(FormatResourceString(IDS_APP_BEGIN_CREATE_QUESTION2, DocumentType(docType)));
  }


  switch (button)
  {
    case ksMessageButtonEnum::ksButtonYes:
    {
      ksapi::IKompasDocumentPtr doc =
        CreateDocument(docType == DocumentTypeEnum::ksDocumentUnknown ? DocumentTypeEnum::ksDocumentDrawing : docType);
      // если документ создан стандартный диалог не нужен
      res = !doc;

      if (doc && theApp.m_auto)
        AdviseDoc(doc);

      break;
    }
    case ksMessageButtonEnum::ksButtonNo:
    {
      res = docType == DocumentTypeEnum::ksDocumentUnknown;
      break;
    }
    case ksMessageButtonEnum::ksButtonCancel:
    {
      res = false;
      break;
    }
  }

  return res;
}


//-------------------------------------------------------------------------------
//
// ---
void ApplicationEvent::OnCreateDocument(ksapi::IKompasDocumentPtr document)
{
  ShowEventName(L"ApplicationEvent::CreateDocument", document);

  if (theApp.m_mes_DOC_FRAME &&
      YesNoDialog(LoadResourceString(IDS_ADVISE_DOC_FRAME_QUESTION)) == ksMessageButtonEnum::ksButtonYes)
    EventContainer::NewEvent(EventType::DocumentFrame, document);

  if (theApp.m_auto)
    AdviseDoc(document);
}


//-------------------------------------------------------------------------------
//
// ---
bool ApplicationEvent::OnBeginOpenDocument(const std::wstring & fileName)
{
  std::wstring mess = L"ApplicationEvent::BeginOpenDocumen";
  mess += L"\n";
  mess += std::filesystem::path(fileName).filename().wstring();

  ShowEventName(mess);

  bool res = true;

  if (theApp.m_mes_APP)
    res = YesNoDialog(mess) == ksMessageButtonEnum::ksButtonYes;


  return res;
}


//-------------------------------------------------------------------------------
//
// ---
void ApplicationEvent::OnOpenDocument(ksapi::IKompasDocumentPtr document)
{
  ShowEventName(L"ApplicationEvent::OpenDocumen", document);

  if (theApp.m_mes_DOC_FRAME &&
      YesNoDialog(LoadResourceString(IDS_ADVISE_DOC_FRAME_QUESTION)) == ksMessageButtonEnum::ksButtonYes)
    EventContainer::NewEvent(EventType::DocumentFrame, document);

  if (theApp.m_auto)
    AdviseDoc(document);
}


//-------------------------------------------------------------------------------
//
// ---
bool ApplicationEvent::OnBeginOpenFile()
{
  ShowEventName(L"ApplicationEvent::BeginOpenFile");

  if (!theApp.m_mes_APP)
    return true;

  bool res = false;

  ksMessageButtonEnum button = YesNoCancelDialog(LoadResourceString(IDS_APP_BEGIN_OPEN_QUESTION));

  switch (button)
  {
    case ksMessageButtonEnum::ksButtonYes:
    {
      ksapi::IKompasDocumentPtr doc = CreateDocument(DocumentTypeEnum::ksDocumentDrawing);
      // если документ создан стандартный диалог не нужен
      res = !doc;

      if (doc && theApp.m_auto)
        AdviseDoc(doc);

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
      break;
    }
  }

  return res;
}


//-------------------------------------------------------------------------------
//
// ---
bool ApplicationEvent::OnBeginDragOpenFiles(std::vector<std::wstring> & fileNames, bool insert)
{
  std::wstring args = SeparateStringVector(fileNames);
  args += L", insert = " + std::to_wstring(insert);

  ShowEventName(L"ApplicationEvent::BeginDragOpenFiles", nullptr,
                std::format(L"files: {}, insert = {}", SeparateStringVector(fileNames), insert));
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool ApplicationEvent::OnBeginRequestFiles(ksRequestFilesTypeEnum requestId, std::vector<std::wstring> & fileNames)
{
#ifdef TESTS
  ShowEventName(L"ApplicationEvent::BeginRequestFiles", nullptr,
                std::format(L"requestId = {}, files: {}, {}", static_cast<int32_t>(requestId), SeparateStringVector(fileNames),
                            fileNames.size()));
  return true;
#else
  return !theApp.m_mes_APP || YesNoDialog(LoadResourceString(IDS_APP_BEGIN_REQUEST_FILES)) == ksMessageButtonEnum::ksButtonYes;
#endif
}


//-------------------------------------------------------------------------------
//
// ---
void ApplicationEvent::OnChangeActiveDocument(ksapi::IKompasDocumentPtr document)
{
  ShowEventName(L"ApplicationEvent::ChangeActiveDocument", document);
}


//-------------------------------------------------------------------------------
//
// ---
void ApplicationEvent::OnApplicationDestroy()
{
  ShowEventName(L"ApplicationEvent::ApplicationDestroy");
  UnAdvise();
}


//-------------------------------------------------------------------------------
//
// ---
bool ApplicationEvent::OnBeginCloseAllDocument()
{
  ShowEventName(L"ApplicationEvent::BeginCloseAllDocument");
  return !theApp.m_mes_APP || YesNoDialog(LoadResourceString(IDS_APP_CLOSE_ALL)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
//
// ---
bool ApplicationEvent::OnKeyDown(int32_t & keyCode, int32_t keyFlags, const ksapi::IPressedKeysPtr & systemKeys)
{
  ShowEventName(L"ApplicationEvent::KeyDown", nullptr,
                std::format(L"{}, {}, {}, {}, {}", keyCode, keyFlags, systemKeys->IsAltKey(), systemKeys->IsControlKey(),
                            systemKeys->IsShiftKey()));
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool ApplicationEvent::OnKeyUp(int32_t & keyCode, int32_t keyFlags, const ksapi::IPressedKeysPtr & systemKeys)
{
  ShowEventName(L"ApplicationEvent::KeyUp", nullptr,
                std::format(L"{}, {}, {}, {}, {}", keyCode, keyFlags, systemKeys->IsAltKey(), systemKeys->IsControlKey(),
                            systemKeys->IsShiftKey()));
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool ApplicationEvent::OnKeyPressed(int32_t & keyCode, int32_t keyFlags, const ksapi::IPressedKeysPtr & systemKeys)
{
  ShowEventName(L"ApplicationEvent::KeyPressed", nullptr,
                std::format(L"{}, {}, {}, {}, {}", keyCode, keyFlags, systemKeys->IsAltKey(), systemKeys->IsControlKey(),
                            systemKeys->IsShiftKey()));
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool ApplicationEvent::OnBeginChoiceMaterial(int32_t materialId)
{
  ShowEventName(L"ApplicationEvent::BeginChoiceMaterial", nullptr, std::to_wstring(materialId));
  return !theApp.m_mes_APP ||
         YesNoDialog(LoadResourceString(IDS_APP_BEGIN_CHOICEMATERIAL_QUESTION)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
//
// ---
void ApplicationEvent::OnChoiceMaterial(int32_t materialId, const std::wstring & materialName, double density)
{
  ShowEventName(L"ApplicationEvent::ChoiceMaterial", nullptr, std::format(L"{}, {}, {}", materialId, materialName, density));
}


//-------------------------------------------------------------------------------
//
// ---
bool ApplicationEvent::OnIsNeedConvertToSavePrevious(ksapi::IKompasDocumentPtr document, ksSaveDocumentVersionEnum version,
                                                     ksapi::ISaveToPreviousParamPtr saveParam, bool & needConvert)
{
  ShowEventName(L"ApplicationEvent::IsNeedConvertToSavePrevious", document,
                std::format(L"{}, {}, {}", static_cast<int32_t>(version), saveParam ? L"not null" : L"null", needConvert));
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
void ApplicationEvent::OnBeginConvertToSavePrevious(ksapi::IKompasDocumentPtr document, ksSaveDocumentVersionEnum version,
                                                    ksapi::ISaveToPreviousParamPtr saveParam)
{
  ShowEventName(L"ApplicationEvent::BeginConvertToSavePrevious", document,
                std::format(L"{}, {}", static_cast<int32_t>(version), saveParam ? L"not null" : L"null"));
}


//-------------------------------------------------------------------------------
//
// ---
void ApplicationEvent::OnEndConvertToSavePrevious(ksapi::IKompasDocumentPtr document, ksSaveDocumentVersionEnum version,
                                                  ksapi::ISaveToPreviousParamPtr saveParam)
{
  ShowEventName(L"ApplicationEvent::EndConvertToSavePrevious", document,
                std::format(L"{}, {}", static_cast<int32_t>(version), saveParam ? L"not null" : L"null"));
}


//-------------------------------------------------------------------------------
//
// ---
bool ApplicationEvent::OnChangeTheme(ksThemeEnum theme)
{
  ShowEventName(L"ApplicationEvent::ChangeTheme", nullptr, std::to_wstring(theme));
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool ApplicationEvent::OnBeginChoiceMarking(const std::wstring & oldMarking, std::wstring & newMarking)
{
  ShowEventName(L"ApplicationEvent::BeginChoiceMarking", nullptr, std::format(L"{}, {}", oldMarking, newMarking));
  return true;
}
#pragma endregion

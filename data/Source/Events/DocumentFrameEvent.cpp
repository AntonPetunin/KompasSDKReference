#include "DocumentFrameEvent.h"
#include "LibFunctions.h"

extern ksapi::IApplication * kompasApp; // Интерфейс Приложения КОМПАС-3D
extern AppConfig theApp;

////////////////////////////////////////////////////////////////////////////////
//
// DocumentFrameEvent  - обработчик событий от окна документа
//
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
//
// ---
DocumentFrameEvent::DocumentFrameEvent(const ksapi::IKompasDocumentPtr document)
  : BaseEvent(EventType::DocumentFrame, document)
{
}


//-------------------------------------------------------------------------------
//
// ---
DocumentFrameEvent::~DocumentFrameEvent()
{
  UnAdvise();
}


//-------------------------------------------------------------------------------
//
// ---
bool DocumentFrameEvent::Advise()
{
  bool res = BaseEvent::Advise() && m_doc;

  if (!res) // Если уже подписались
    return res;

  if (ksapi::IDocumentFramePtr frame = m_doc->GetDocumentFrame())
  {
    auto handlerOwner = libname;
    ksapi::IDocumentFrameEventsPtr events = frame->Events();

    if (!events)
      return false;

    events->AddBeginPaintHandler(handlerOwner,
                                 [this](const ksapi::IPaintObjectPtr & paintObject) { return OnBeginPaint(paintObject); });
    events->AddClosePaintHandler(handlerOwner, [this](const ksapi::IPaintObjectPtr & paintObject) { OnClosePaint(paintObject); });
    events->AddMouseDownHandler(handlerOwner, [this](const ksapi::IPressedKeysPtr & keys, int32_t x, int32_t y)
                                { return OnMouseDown(keys, x, y); });
    events->AddMouseUpHandler(handlerOwner, [this](const ksapi::IPressedKeysPtr & keys, int32_t x, int32_t y)
                              { return OnMouseUp(keys, x, y); });
    events->AddMouseDblClickHandler(handlerOwner, [this](const ksapi::IPressedKeysPtr & keys, int32_t x, int32_t y)
                                    { return OnMouseDblClick(keys, x, y); });
    events->AddBeginPaintGlHandler(handlerOwner, [this](uint32_t drawMode, const ksapi::IOpenGLObjectPtr & glObject)
                                   { return OnBeginPaintGl(drawMode, glObject); });
    events->AddClosePaintGlHandler(handlerOwner, [this](uint32_t drawMode, const ksapi::IOpenGLObjectPtr & glObject)
                                   { OnClosePaintGl(drawMode, glObject); });
    events->AddAddGabaritHandler(handlerOwner, [this](ksapi::IGabaritObjectPtr gabarit) { OnAddGabarit(gabarit); });
    events->AddActivateHandler(handlerOwner, [this]() { OnActivate(); });
    events->AddDeactivateHandler(handlerOwner, [this]() { OnDeactivate(); });
    events->AddCloseHandler(handlerOwner, [this]() { OnClose(); });
    events->AddMouseMoveHandler(handlerOwner, [this](const ksapi::IPressedKeysPtr & keys, int32_t x, int32_t y)
                                { return OnMouseMove(keys, x, y); });
    events->AddBeginPaintTmpObjectsHandler(handlerOwner, [this]() { return OnBeginPaintTmpObjects(); });
    events->AddClosePaintTmpObjectsHandler(handlerOwner, [this]() { OnClosePaintTmpObjects(); });
    events->AddMouseWheelHandler(handlerOwner, [this](const ksapi::IPressedKeysPtr & keys, int32_t zDelta, int32_t x, int32_t y)
                                 { return OnMouseWheelHandler(keys, zDelta, x, y); });
  }

  ShowEventName(LoadResourceString(IDS_ADVISE_DOC_FRAME));
  return res;
}


//-------------------------------------------------------------------------------
//
// ---
void DocumentFrameEvent::UnAdvise()
{
  if (m_advise && m_doc)
  {
    if (ksapi::IDocumentFramePtr frame = m_doc->GetDocumentFrame())
    {
      if (ksapi::IDocumentFrameEventsPtr events = frame->Events())
        events->RemoveAllHandlers(libname);

      BaseEvent::UnAdvise();
      ShowEventName(LoadResourceString(IDS_UNADVISE_DOC_FRAME));
    }
  }
}


//-----------------------------------------------------------------------------
// Выдать имя метода
// ---
void DocumentFrameEvent::ShowEventName(const std::wstring & eventName, const std::wstring & args /*= L""*/)
{
  if (theApp.m_mes_DOC_FRAME)
    BaseEvent::ShowEventName(eventName, args);
}


#pragma region Обработчики событий окна документа
//-------------------------------------------------------------------------------
// Начало отрисовки документа
// ---
bool DocumentFrameEvent::OnBeginPaint(const ksapi::IPaintObjectPtr & paintObject)
{
  ShowEventName(L"DocumentFrameEvent::BeginPaint", paintObject ? L"Not NULL" : L"NULL");
  return true;
}


//-------------------------------------------------------------------------------
// Конец отрисовки документа
// ---
void DocumentFrameEvent::OnClosePaint(const ksapi::IPaintObjectPtr & paintObject)
{
  ShowEventName(L"DocumentFrameEvent::ClosePaint", paintObject ? L"Not NULL" : L"NULL");
}


//-------------------------------------------------------------------------------
// Нажатие кнопки мыши
// ---
bool DocumentFrameEvent::OnMouseDown(const ksapi::IPressedKeysPtr & keys, int32_t x, int32_t y)
{
  int32_t mouseKey = 0;
  if (keys->IsMouseLeftButton())
    mouseKey = 1;
  else if (keys->IsMouseMidleButton())
    mouseKey = 2;
  else if (keys->IsMouseRightButton())
    mouseKey = 3;

  ShowEventName(L"DocumentFrameEvent::MouseDown",
                std::format(L"btn={}; keys={},{},{}; x={}; y={}", mouseKey, keys->IsAltKey(), keys->IsControlKey(),
                            keys->IsShiftKey(), std::to_wstring(x), std::to_wstring(y)));
  return true;
}


//-------------------------------------------------------------------------------
// Отпускание кнопки мыши
// ---
bool DocumentFrameEvent::OnMouseUp(const ksapi::IPressedKeysPtr & keys, int32_t x, int32_t y)
{
  int32_t mouseKey = 0;
  if (keys->IsMouseLeftButton())
    mouseKey = 1;
  else if (keys->IsMouseMidleButton())
    mouseKey = 2;
  else if (keys->IsMouseRightButton())
    mouseKey = 3;

  ShowEventName(L"DocumentFrameEvent::MouseUp",
                std::format(L"btn={}; keys={},{},{}; x={}; y={}", mouseKey, keys->IsAltKey(), keys->IsControlKey(),
                            keys->IsShiftKey(), std::to_wstring(x), std::to_wstring(y)));
  return true;
}


//-------------------------------------------------------------------------------
// Двойной клик кнопки мыши
// ---
bool DocumentFrameEvent::OnMouseDblClick(const ksapi::IPressedKeysPtr & keys, int32_t x, int32_t y)
{
  int32_t mouseKey = 0;
  if (keys->IsMouseLeftButton())
    mouseKey = 1;
  else if (keys->IsMouseMidleButton())
    mouseKey = 2;
  else if (keys->IsMouseRightButton())
    mouseKey = 3;

  ShowEventName(L"DocumentFrameEvent::MouseDblClick",
                std::format(L"btn={}; keys={},{},{}; x={}; y={}", mouseKey, keys->IsAltKey(), keys->IsControlKey(),
                            keys->IsShiftKey(), std::to_wstring(x), std::to_wstring(y)));
  return true;
}


//-------------------------------------------------------------------------------
// Начало создания листа в контексте OpenGL
// ---
bool DocumentFrameEvent::OnBeginPaintGl(uint32_t drawMode, const ksapi::IOpenGLObjectPtr & glObject)
{
  ShowEventName(L"DocumentFrameEvent::BeginPaintGl",
                std::format(L"drawMode={}; glObject={}", drawMode, glObject ? L"Not NULL" : L"NULL"));
  return true;
}


//-------------------------------------------------------------------------------
// Окончание создания листа в контексте OpenGL
// ---
void DocumentFrameEvent::OnClosePaintGl(uint32_t drawMode, const ksapi::IOpenGLObjectPtr & glObject)
{
  ShowEventName(L"DocumentFrameEvent::ClosePaintGl",
                std::format(L"drawMode={}; glObject={}", drawMode, glObject ? L"Not NULL" : L"NULL"));
}


//-------------------------------------------------------------------------------
// Определение габаритов документа
// ---
void DocumentFrameEvent::OnAddGabarit(ksapi::IGabaritObjectPtr gabarit)
{
  ShowEventName(L"DocumentFrameEvent::AddGabarit", gabarit ? L"Not NULL" : L"NULL");
}


//-------------------------------------------------------------------------------
// Окно активизировалось
// ---
void DocumentFrameEvent::OnActivate()
{
  ShowEventName(L"DocumentFrameEvent::Activate");
}


//-------------------------------------------------------------------------------
// Окно деактивизировалось
// ---
void DocumentFrameEvent::OnDeactivate()
{
  ShowEventName(L"DocumentFrameEvent::Deactivate");
}


//-------------------------------------------------------------------------------
// Закрытие окна
// ---
void DocumentFrameEvent::OnClose()
{
  ShowEventName(L"DocumentFrameEvent::Close");

  if (m_doc)
    EventContainer::UnAdviseByTypeAndDoc(m_eventType, m_doc);
}


//-------------------------------------------------------------------------------
// Перемещение мыши
// ---
bool DocumentFrameEvent::OnMouseMove(const ksapi::IPressedKeysPtr & keys, int32_t x, int32_t y)
{
#ifndef TESTS
  ShowEventName(L"DocumentFrameEvent::MouseMove",
                std::format(L"keys={},{},{}; x={}; y={}", keys->IsAltKey(), keys->IsControlKey(), keys->IsShiftKey(),
                            std::to_wstring(x), std::to_wstring(y)));
#endif
  return true;
}


//-------------------------------------------------------------------------------
// Начало отрисовки временных объектов
// ---
bool DocumentFrameEvent::OnBeginPaintTmpObjects()
{
  ShowEventName(L"DocumentFrameEvent::BeginPaintTmpObjects");
  return true;
}


//-------------------------------------------------------------------------------
// Конец отрисовки временных объектов
// ---
void DocumentFrameEvent::OnClosePaintTmpObjects()
{
  ShowEventName(L"DocumentFrameEvent::ClosePaintTmpObjects");
}


//-------------------------------------------------------------------------------
// Вращение колеса мыши
// ---
bool DocumentFrameEvent::OnMouseWheelHandler(const ksapi::IPressedKeysPtr & keys, int32_t zDelta, int32_t x, int32_t y)
{
  ShowEventName(L"DocumentFrameEvent::MouseWheelHandler",
                std::format(L"keys={},{},{}; zDelta={}; x={}; y={}", keys->IsAltKey(), keys->IsControlKey(), keys->IsShiftKey(),
                            zDelta, std::to_wstring(x), std::to_wstring(y)));
  return true;
}
#pragma endregion

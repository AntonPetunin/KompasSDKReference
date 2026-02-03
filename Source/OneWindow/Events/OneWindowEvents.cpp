////////////////////////////////////////////////////////////////////////////////
//
// OneWindow.cpp - Библиотека на Visual C++
//
////////////////////////////////////////////////////////////////////////////////
#include "OneWindow_pch.h"

#include "OneWindowEvents.h"
#include "../CPropMen.h"
#include "../LibObj.h"

extern ksapi::IApplication * application; ///< Приложение

//------------------------------------------------------------------------------
/**
  Конструктор с инициализацией hash
*/
//---
DocumentIdentifier::DocumentIdentifier()
{
  ksapi::IKompasDocument * doc = application ? application->GetActiveDocument().Get() : nullptr;
  m_documentHansh = doc ? std::hash<ksapi::IKompasDocument *>()(doc) : 0;
}


//------------------------------------------------------------------------------
/**
  Добавить событие в контейнер событий
*/
//---
size_t OWEventCollection::Add(IOneWindowEvent * additionEvent)
{
  size_t outHash = 0;

  if (additionEvent)
  {
    outHash = additionEvent->GetHash();

    if (!m_events.contains(outHash))
      m_events[outHash] = std::shared_ptr<IOneWindowEvent>(additionEvent);
    else
      m_events[outHash].reset(additionEvent);
  }

  return outHash;
}


IOneWindowEvent * OWEventCollection::GetEvent(size_t eventHash)
{
  return m_events.contains(eventHash) ? m_events[eventHash].get() : nullptr;
}

//------------------------------------------------------------------------------
/**
  Удалить событие из контейнера и отписаться от него
*/
//---
void OWEventCollection::RemoveWithUnadvise(size_t eventHash)
{
  if (m_events.contains(eventHash))
    m_events.erase(eventHash);
}


//------------------------------------------------------------------------------
/**
  Очистить контейнер событий
*/
//---
void OWEventCollection::Clear()
{
  m_events.clear();
}


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
PropertyManagerEvent::PropertyManagerEvent(const std::wstring & subName, ksapi::IPropertyManager & manager,
                                           PropertyManagerObject & obj)
  : DocumentIdentifier()
  , m_propMng(&manager)
  , m_obj(&obj)
  , m_subName(subName)
{
  Advise();
}


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
PropertyManagerEvent::PropertyManagerEvent(const std::wstring & subName, ksapi::IProcessParam & procParam,
                                           PropertyManagerObject & obj)
  : DocumentIdentifier()
  , m_procParam(&procParam)
  , m_obj(&obj)
  , m_subName(subName)
{
  Advise();
}


//------------------------------------------------------------------------------
/**
  Деструктор
*/
//---
PropertyManagerEvent::~PropertyManagerEvent()
{
  Unadvise();
}

//------------------------------------------------------------------------------
/**
  Подписаться на события
*/
//---
void PropertyManagerEvent::Advise()
{
  ksapi::IPropertiesManagerEventsPtr events;

  if (m_procParam)
    events = m_procParam->Events();

  if (m_propMng)
    events = m_propMng->Events();

  if (events)
  {
    events->AddButtonClickHandler(m_subName, [this](std::int32_t buttonId) { return ButtonClick(buttonId); });
    events->AddUserMenuCommandHandler(m_subName, [this](const ksapi::IPropertyControlPtr & control, int32_t menuId)
                                      { return ControlCommand(control, menuId); });
    events->AddButtonUpdateHandler(m_subName, [this](std::int32_t buttonId, bool & enable, int & checked)
                                   { return ButtonUpdate(buttonId, checked, enable); });
  }
}


//------------------------------------------------------------------------------
/**
  Отписаться от событий
*/
//---
void PropertyManagerEvent::Unadvise()
{
  ksapi::IPropertiesManagerEventsPtr events;

  if (m_propMng)
    events = m_propMng->Events();
  else if (m_procParam)
    events = m_procParam->Events();

  if (events)
    events->RemoveAllHandlers(m_subName);
}


//------------------------------------------------------------------------------
/**
  Получить hash события
*/
//---
size_t PropertyManagerEvent::GetHash()
{
  std::size_t h1 = std::hash<std::int32_t>{}(m_obj->GetFirstTabID());
  std::size_t h2 = std::hash<std::int32_t>{}(m_obj->GetSpecToolBarType());
  std::size_t h3 = 0;

  if (m_propMng)
    h3 = std::hash<std::wstring>{}(m_propMng->GetCaption());
  else if (m_procParam)
    h3 = std::hash<std::wstring>{}(m_procParam->GetCaption());

  return h1 ^ (h2 << 1) ^ h3 << 8 ^ GetDocumentHash() << 16;
}


//------------------------------------------------------------------------------
/**
  Получить объект события
*/
//---
ksapi::IAPIObjectPtr PropertyManagerEvent::GetEventObject()
{
  ksapi::IAPIObjectPtr res = m_propMng;
  return !res ? m_procParam : nullptr;
}

//------------------------------------------------------------------------------
/**
  Событие изменения значения контрола
*/
//---
bool PropertyManagerEvent::ButtonClick(std::int32_t buttonID)
{
  return m_obj && m_obj->OnButtonClick(buttonID);
}


//------------------------------------------------------------------------------
/**
  Нажатие кнопки контрола
*/
//---
bool PropertyManagerEvent::ControlCommand(const ksapi::IPropertyControlPtr & ctrl, std::int32_t buttonID)
{
  if (ctrl)
  {
    if (m_obj)
      m_obj->OnControlCommand(ctrl->GetId(), buttonID);
  }

  return true;
}


//------------------------------------------------------------------------------
/**
  Установка состояния кнопки спецпанели.
*/
//---
bool PropertyManagerEvent::ButtonUpdate(std::int32_t buttonID, std::int32_t & check, bool & enable)
{
  return m_obj && m_obj->OnButtonUpdate(buttonID, check, enable);
}


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
PropertyTreeEvent::PropertyTreeEvent(const std::wstring & subName, ksapi::IPropertyTree & tree, PropertyTreeEventCallbacks & obj)
  : DocumentIdentifier()
  , m_tree(&tree)
  , m_obj(&obj)
  , m_subName(subName)
{
  Advise();
}


//------------------------------------------------------------------------------
/**
  Деструктор
*/
//---
PropertyTreeEvent::~PropertyTreeEvent()
{
  Unadvise();
}


//------------------------------------------------------------------------------
/**
  Подписаться на события
*/
//---
void PropertyTreeEvent::Advise()
{
  if (m_tree)
  {
    ksapi::IPropertyTreeEventsPtr events = m_tree->Events();

    if (!events)
      return;

    events->AddBeginTextEditHandler(m_subName,
                                    [this](const ksapi::IPropertyTreeNodePtr & node)
                                    {
                                      BeginTextEdit(node);
                                      return true;
                                    });
    events->AddEndTextEditHandler(m_subName,
                                  [this](const ksapi::IPropertyTreeNodePtr & node)
                                  {
                                    EndTextEdit(node);
                                    return true;
                                  });
    events->AddSelectNodesHandler(m_subName,
                                  [this](const std::vector<ksapi::IPropertyTreeNodePtr> & nodes)
                                  {
                                    SelectNodes(nodes);
                                    return true;
                                  });
    events->AddUnselectNodesHandler(m_subName,
                                    [this](const std::vector<ksapi::IPropertyTreeNodePtr> & nodes)
                                    {
                                      UnselectNodes(nodes);
                                      return true;
                                    });
    events->AddContextMenuRequestHandler(m_subName,
                                         [this](const ksapi::IPropertyTreeNodePtr & node)
                                         {
                                           ContextMenuRequest(node);
                                           return true;
                                         });
  }
}


//------------------------------------------------------------------------------
/**
  Отписаться от событий
*/
//---
void PropertyTreeEvent::Unadvise()
{
  if (m_tree)
  {
    if (ksapi::IPropertyTreeEventsPtr events = m_tree->Events())
      events->RemoveAllHandlers(m_subName);
  }
}


//------------------------------------------------------------------------------
/**
  Получить hash события
*/
//---
size_t PropertyTreeEvent::GetHash()
{
  std::size_t h1 = std::hash<std::wstring>{}(m_tree->GetName());
  std::size_t h2 = std::hash<std::int32_t>{}(m_tree->GetControlType());
  return h1 ^ (h2 << 6) ^ GetDocumentHash() << 16;
}


//------------------------------------------------------------------------------
/**
  Получить объект события
*/
//---
ksapi::IAPIObjectPtr PropertyTreeEvent::GetEventObject()
{
  return m_tree;
}

//------------------------------------------------------------------------------
/**
  Событие "Узлы селектированы".
*/
//---
void PropertyTreeEvent::SelectNodes(const std::vector<ksapi::IPropertyTreeNodePtr> & nodes)
{
  if (m_obj)
    m_obj->OnSelectNodes(nodes);
}


//------------------------------------------------------------------------------
/**
  Событие "С узлов снято селектирование".
*/
//---
void PropertyTreeEvent::UnselectNodes(const std::vector<ksapi::IPropertyTreeNodePtr> & nodes)
{
  if (m_obj)
    m_obj->OnUnselectNodes(nodes);
}


//------------------------------------------------------------------------------
/**
  Запрос контекстного меню для узла дерева.
*/
//---
void PropertyTreeEvent::ContextMenuRequest(const ksapi::IPropertyTreeNodePtr & node)
{
  if (m_obj)
    m_obj->OnContextMenuRequest(node);
}


//------------------------------------------------------------------------------
/**
  Событие начала переименования узла дерева.
*/
//---
void PropertyTreeEvent::BeginTextEdit(const ksapi::IPropertyTreeNodePtr & node)
{
  if (m_obj)
    m_obj->OnBeginTextEdit(node);
}


//------------------------------------------------------------------------------
/**
  Событие завершения переименования узла дерева.
*/
//---
void PropertyTreeEvent::EndTextEdit(const ksapi::IPropertyTreeNodePtr & node)
{
  if (m_obj)
    m_obj->OnEndTextEdit(node);
}


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
DocumentFrameEvent::DocumentFrameEvent(const std::wstring & subName, ksapi::IDocumentFrame & frame,
                                       DocumentFrameEventCallback & obj)
  : DocumentIdentifier()
  , m_frame(&frame)
  , m_obj(&obj)
  , m_subName(subName)
{
  Advise();
}


//------------------------------------------------------------------------------
/**
  Деструктор
*/
//---
DocumentFrameEvent::~DocumentFrameEvent()
{
  Unadvise();
}


//------------------------------------------------------------------------------
/**
  Подписаться на события
*/
//---
void DocumentFrameEvent::Advise()
{
  if (m_frame)
  {
    ksapi::IDocumentFrameEventsPtr events = m_frame->Events();
    events->AddClosePaintGlHandler(m_subName, [this](unsigned int drawMode, const ksapi::IOpenGLObjectPtr & glObject)
                                   { OnBeginPaintGL(glObject, drawMode); });
    events->AddAddGabaritHandler(m_subName, [this](ksapi::IGabaritObjectPtr gabarit) { return OnAddGabarit(gabarit); });
    events->AddCloseHandler(m_subName, [this]() { return OnCloseFrame(); });
  }
}


//------------------------------------------------------------------------------
/**
  Отписаться от событий
*/
//---
void DocumentFrameEvent::Unadvise()
{
  if (m_frame)
  {
    ksapi::IDocumentFrameEventsPtr events = m_frame->Events();
    events->RemoveAllHandlers(m_subName);
  }
}


//------------------------------------------------------------------------------
/**
  Получить hash события
*/
//---
size_t DocumentFrameEvent::GetHash()
{
  std::size_t h1 = std::hash<std::wstring>{}(m_frame->GetCaption());
  std::size_t h2 = std::hash<std::int32_t>{}(m_frame->GetType());
  return h1 ^ (h2 << 8) ^ GetDocumentHash() << 16;
}


//------------------------------------------------------------------------------
/**
  Получить объект события
*/
//---
ksapi::IAPIObjectPtr DocumentFrameEvent::GetEventObject()
{
  return m_frame;
}


//------------------------------------------------------------------------------
/**
  Начало создания листа в контексте OpenGL
*/
//---
bool DocumentFrameEvent::OnBeginPaintGL(const ksapi::IOpenGLObjectPtr & glObj, std::int32_t drawMode)
{
  return m_obj && m_obj->BeginPaintGL(glObj, drawMode);
}


//------------------------------------------------------------------------------
/**
  Определение габаритов документа
*/
//---
bool DocumentFrameEvent::OnAddGabarit(ksapi::IGabaritObjectPtr & gabObj)
{
  return m_obj && m_obj->AddGabarit(gabObj);
}


//------------------------------------------------------------------------------
/**
  Закрытие окна
*/
//---
bool DocumentFrameEvent::OnCloseFrame()
{
  return m_obj && m_obj->CloseFrame();
}


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
Process3DEvent::Process3DEvent(const std::wstring & subName, PropertyManagerObject & propMng, ksapi::IProcess3D & process)
  : DocumentIdentifier()
  , m_process(&process)
  , m_propMng(&propMng)
  , m_subName(subName)
{
  Advise();
}


//------------------------------------------------------------------------------
/**
  Деструктор
*/
//---
Process3DEvent::~Process3DEvent()
{
  Unadvise();
}


//------------------------------------------------------------------------------
/**
  Подписаться на события
*/
//---
void Process3DEvent::Advise()
{
  if (m_process)
  {
    ksapi::IProcess3DEventsPtr events = m_process->Events();
    events->AddEndProcessHandler(m_subName, [this]() { return OnEndProcess(); });
    events->AddFilterObjectsHandler(m_subName,
                                    [this](const ksapi::IKompasAPIObjectPtr & object) { return OnFilterObjects(object); });
    events->AddPlacementChangedHandler(m_subName,
                                       [&](const ksapi::IKompasAPIObjectPtr & object) { return LibObj::SelectFace(object); });
  }
}


//------------------------------------------------------------------------------
/**
  Отписаться от событий
*/
//---
void Process3DEvent::Unadvise()
{
  if (m_process)
    m_process->Events()->RemoveAllHandlers(m_subName);
}


//------------------------------------------------------------------------------
/**
  Получить hash события
*/
//---
size_t Process3DEvent::GetHash()
{
  std::size_t h1 = std::hash<std::int32_t>{}(m_process->GetSelectionBandMode());
  std::size_t h2 = std::hash<std::int32_t>{}(m_propMng->GetFirstTabID());
  std::size_t h3 = std::hash<std::int32_t>{}(m_propMng->GetSpecToolBarType());
  return h1 ^ (h2 << 1) ^ h3 << 8 ^ GetDocumentHash() << 16;
}


//------------------------------------------------------------------------------
/**
  Получить объект события
*/
//---
ksapi::IAPIObjectPtr Process3DEvent::GetEventObject()
{
  return m_process;
}


//------------------------------------------------------------------------------
/**
  Обработчик события "Фильтрация объетов".
*/
//---
bool Process3DEvent::OnFilterObjects(const ksapi::IKompasAPIObjectPtr & object)
{
  // Фильтруем только грани
  return object && object->GetType() == ksObjectFace;
}


//------------------------------------------------------------------------------
/**
  Обработчик события "Окончание процесса".
*/
//---
bool Process3DEvent::OnEndProcess()
{
  if (m_propMng)
    m_propMng->EndProcess();

  return true;
}

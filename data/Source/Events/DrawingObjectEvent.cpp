#include "DrawingObjectEvent.h"
#include "LibFunctions.h"

extern ksapi::IApplication * kompasApp; // Интерфейс Приложения КОМПАС-3D
extern AppConfig theApp;


//-------------------------------------------------------------------------------
//
// ---
std::vector<ksapi::IDrawingObjectPtr> ToDrawingObjects(const std::vector<ksapi::IKompasAPIObjectPtr> & objects)
{
  std::vector<ksapi::IDrawingObjectPtr> drwObjs(objects.begin(), objects.end());
  return drwObjs;
}


//-------------------------------------------------------------------------------
//
// ---
DrawingObjectEvent::DrawingObjectEvent(const ksapi::IKompasDocumentPtr document)
  : BaseEvent(EventType::Object2D, document)
{
  InitEventHandler();
}


//-------------------------------------------------------------------------------
//
// ---
DrawingObjectEvent::DrawingObjectEvent(const ksapi::IKompasDocumentPtr document, const std::wstring & handlerName)
  : BaseEvent(EventType::Object2D, document)
  , m_handlerName(handlerName)
{
  InitEventHandler();
}


//-------------------------------------------------------------------------------
//
// ---
void DrawingObjectEvent::InitEventHandler()
{
  if (ksapi::IKompasDocument2DPtr doc2D = m_doc)
    m_events = doc2D->GetObject2DEvents(m_handlerName);
}


//-------------------------------------------------------------------------------
//
// ---
DrawingObjectEvent::~DrawingObjectEvent()
{
  m_copyObjects.clear();
  UnAdvise();
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::Advise()
{
  bool res = BaseEvent::Advise() && m_doc;

  if (!res) // Если уже подписались
    return res;

  if (m_events)
  {
    m_events->AddChangeActiveHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects)
                                     { return OnChangeActive(objects); });
    m_events->AddCreateObjectHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects)
                                     { return OnCreateObject(objects); });
    m_events->AddUpdateObjectHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects)
                                     { return OnUpdateObject(objects); });
    m_events->AddDeleteHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects) { return OnDelete(objects); });
    m_events->AddBeginDeleteHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects)
                                    { return OnBeginDelete(objects); });
    m_events->AddBeginDestroyObjectHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects)
                                           { return OnBeginDestroyObject(objects); });
    m_events->AddDestroyObjectHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects)
                                      { return OnDestroyObject(objects); });
    m_events->AddBeginMoveHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects) { return OnBeginMove(objects); });
    m_events->AddBeginRotateHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects)
                                    { return OnBeginRotate(objects); });
    m_events->AddBeginScaleHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects)
                                   { return OnBeginScale(objects); });
    m_events->AddBeginSymmetryHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects)
                                      { return OnBeginSymmetry(objects); });
    m_events->AddBeginCopyHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects) { return OnBeginCopy(objects); });
    m_events->AddBeginTransformHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects)
                                       { return OnBeginTransform(objects); });
    m_events->AddMoveHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects) { return OnMove(objects); });
    m_events->AddRotateHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects) { return OnRotate(objects); });
    m_events->AddScaleHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects) { return OnScale(objects); });
    m_events->AddSymmetryHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects) { return OnSymmetry(objects); });
    m_events->AddCopyHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects) { return OnCopy(objects); });
    m_events->AddTransformHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects) { return OnTransform(objects); });
    m_events->AddBeginPropertyChangedHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects)
                                             { return OnBeginPropertyChanged(objects); });
    m_events->AddPropertyChangedHandler([this](const std::vector<ksapi::IDrawingObjectPtr> & objects)
                                        { return OnPropertyChanged(objects); });
    m_events->AddBeginProcessHandler([this](DrawingObjectTypeEnum objectType, const ksapi::IDrawingObjectPtr & object)
                                     { return OnBeginProcess(objectType, object); });
    m_events->AddEndProcessHandler([this](DrawingObjectTypeEnum objectType) { return OnEndProcess(objectType); });
  }

  ShowEventName(LoadResourceString(IDS_ADVISE_OBJECT_2D), {}, false);


  return res;
}


//-------------------------------------------------------------------------------
//
// ---
void DrawingObjectEvent::UnAdvise()
{
  if (m_advise && m_doc)
  {
    m_copyObjects.clear();
    if (m_events)
    {
      m_events->RemoveAllObjects();
      m_events->RemoveAllObjectsTypes();
      m_events->RemoveAllHandlers();
    }

    BaseEvent::UnAdvise();
    ShowEventName(LoadResourceString(IDS_UNADVISE_OBJECT_2D), {}, false);
  }
}


//-------------------------------------------------------------------------------
// Добавить объекты на события которых подписываемся.
// ---
void DrawingObjectEvent::AddObjects(const std::vector<ksapi::IKompasAPIObjectPtr> & objects)
{
  BaseEvent::AddObjects(objects);

  if (m_events)
    m_events->AddObjects(ToDrawingObjects(objects));
}


//-------------------------------------------------------------------------------
// Удалить объекты на события которых подписываемся.
// ---
void DrawingObjectEvent::RemoveObjects(const std::vector<ksapi::IKompasAPIObjectPtr> & objects)
{
  BaseEvent::RemoveObjects(objects);

  if (m_events)
    m_events->RemoveObjects(ToDrawingObjects(objects));
}


//-------------------------------------------------------------------------------
// Удалить все объекты на события которых подписываемся.
// ---
void DrawingObjectEvent::RemoveAllObjects()
{
  BaseEvent::RemoveAllObjects();

  if (m_events)
    m_events->RemoveAllObjects();
}


//-------------------------------------------------------------------------------
// Добавить типы объектов на события которых подписываемся.
// ---
void DrawingObjectEvent::AddObjectsTypes(const std::vector<int32_t> & types)
{
  BaseEvent::AddObjectsTypes(types);

  if (m_events)
    m_events->AddObjectsTypes(types);
}


//-------------------------------------------------------------------------------
// Удалить типы объектов на события которых подписываемся.
// ---
void DrawingObjectEvent::RemoveObjectsTypes(const std::vector<int32_t> & types)
{
  BaseEvent::RemoveObjectsTypes(types);

  if (m_events)
    m_events->RemoveObjectsTypes(types);
}


//-------------------------------------------------------------------------------
//  Удалить все типы объектов на события которых подписываемся.
// ---
void DrawingObjectEvent::RemoveAllObjectsTypes()
{
  BaseEvent::RemoveAllObjectsTypes();

  if (m_events)
    m_events->RemoveAllObjectsTypes();
}


//-------------------------------------------------------------------------------
//
// ---
void DrawingObjectEvent::ShowEventName(const std::wstring & eventName, const std::vector<ksapi::IDrawingObjectPtr> & objects,
                                       bool showResult, const std::wstring & args /*= L""*/)
{
  if (!theApp.m_mes_OBJ_2DDOC)
    return;

  BaseEvent::ShowEventName(eventName, m_doc, args);
  ShowObjectsParam(objects);

  if (ksapi::IKompasDocument2DPtr doc2D = m_doc; doc2D && showResult)
  {
    ksapi::IDrawingObjectEventsResultPtr eventRes = doc2D->GetDrawingObjectEventsResult();
    int32_t notifyType = eventRes->GetNotifyType();
    if (eventRes && notifyType != 0)
    {
      double angle = eventRes->GetAngle();
      std::vector<ksapi::IDrawingObjectPtr> copyObjects = eventRes->GetCopyObjects();
      double sx = 0.0, sy = 0.0;
      eventRes->GetScale(sx, sy);
      double x = 0.0, y = 0.0, x1 = 0.0, y1 = 0.0;
      eventRes->GetSheetPoint(0, x, y);
      eventRes->GetSheetPoint(1, x1, y1);
      bool copy = eventRes->IsCopy();
      std::wstring message = std::format(
        L"Object2DNotifyResult: GetNotifyType={},\nGetAngle={}, GetCopyObject={},\nGetScale( {}, {} ),\nGetSheetPoint( 1, {}, {} ),\nGetSheetPoint( 0, {}, {} ),\nIsCopy={}",
        notifyType, angle, copyObjects.empty() ? L"Empty" : std::to_wstring(copyObjects.size()), sx, sy, x1, y1, x, y,
        copy ? L"True" : L"False");

      ksapi::ISelectionManagerPtr selectionMngr = doc2D->GetSelectionManager();
#ifndef TESTS
      SelectObjects(copyObjects, true);
#endif
      ShowEventName(message, {}, false);
#ifndef TESTS
      SelectObjects(copyObjects, false);
#endif
    }
  }
}


#pragma region Обработчики событий объектов 2D документа
//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnChangeActive(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ShowEventName(L"Obj2DEvent::ChangeActive", objects, false);
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnBeginDelete(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ShowEventName(L"Obj2DEvent::BeginDelete", objects, false);
  return !theApp.m_mes_OBJ_2DDOC || YesNoDialog(LoadResourceString(IDS_OBJECT_DELETE)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnDelete(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ShowEventName(L"Obj2DEvent::Delete", objects, false);

  std::vector<ksapi::IKompasAPIObjectPtr> apiObjs(objects.begin(), objects.end());
  RemoveObjects(apiObjs);

  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnBeginMove(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  return !theApp.m_mes_OBJ_2DDOC || YesNoDialog(LoadResourceString(IDS_OBJECT_MOVE)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnMove(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ShowEventName(L"Obj2DEvent::Move", objects, true);
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnBeginRotate(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ShowEventName(L"Obj2DEvent::BeginRotate", objects, false);
  return !theApp.m_mes_OBJ_2DDOC || YesNoDialog(LoadResourceString(IDS_OBJECT_ROTATE)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnRotate(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ShowEventName(L"Obj2DEvent::Rotate", objects, true);
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnBeginScale(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ShowEventName(L"Obj2DEvent::BeginScale", objects, false);
  return !theApp.m_mes_OBJ_2DDOC || YesNoDialog(LoadResourceString(IDS_OBJECT_SCALE)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnScale(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ShowEventName(L"Obj2DEvent::Scale", objects, true);
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnBeginTransform(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ShowEventName(L"Obj2DEvent::BeginTransform", objects, false);
  return !theApp.m_mes_OBJ_2DDOC || YesNoDialog(LoadResourceString(IDS_OBJECT_TRANSFORM)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnTransform(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ShowEventName(L"Obj2DEvent::Transform", objects, true);
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnBeginCopy(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ShowEventName(L"Obj2DEvent::BeginCopy", objects, false);
  m_copyObjects = objects;
#ifndef TESTS
  SelectObjects(m_copyObjects, true);
#endif
  bool res = !theApp.m_mes_OBJ_2DDOC || YesNoDialog(LoadResourceString(IDS_OBJECT_COPY)) == ksMessageButtonEnum::ksButtonYes;

#ifndef TESTS
  if (!res)
    SelectObjects(m_copyObjects, false);
#endif

  return res;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnCopy(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
#ifndef TESTS
  SelectObjects(objects, true);
#endif
  ShowEventName(L"Obj2DEvent::Copy", objects, true);

#ifndef TESTS
  SelectObjects(m_copyObjects, false);
  SelectObjects(objects, false);
#endif
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnBeginSymmetry(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ShowEventName(L"Obj2DEvent::BeginSymmetry", objects, false);
  return !theApp.m_mes_OBJ_2DDOC || YesNoDialog(LoadResourceString(IDS_OBJECT_SYMMETRY)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnSymmetry(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ShowEventName(L"Obj2DEvent::Symmetry", objects, true);
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnBeginProcess(DrawingObjectTypeEnum objectType, const ksapi::IDrawingObjectPtr & object)
{
  ShowEventName(L"Obj2DEvent::BeginProcess", {}, false, std::format(L"objectType = {}; ", std::to_wstring(objectType)));
  return !theApp.m_mes_OBJ_2DDOC || YesNoDialog(LoadResourceString(IDS_BEGIN_PROCESS)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnEndProcess(DrawingObjectTypeEnum objectType)
{
  ShowEventName(L"Obj2DEvent::EndProcess", {}, false, std::format(L"objectType {};", std::to_wstring(objectType)));
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnCreateObject(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ShowEventName(L"Obj2DEvent::CreateObject", objects, false);
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnUpdateObject(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ShowEventName(L"Obj2DEvent::UpdateObject", objects, false);
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnBeginDestroyObject(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ShowEventName(L"Obj2DEvent::BeginDestroyObject", objects, false);
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnDestroyObject(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ShowEventName(L"Obj2DEvent::DestroyObject", objects, false);
  return true;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnBeginPropertyChanged(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ShowEventName(L"Obj2DEvent::BeginPropertyChanged", {}, false);
  return !theApp.m_mes_OBJ_2DDOC || YesNoDialog(LoadResourceString(IDS_OBJECT_CHANGED)) == ksMessageButtonEnum::ksButtonYes;
}


//-------------------------------------------------------------------------------
//
// ---
bool DrawingObjectEvent::OnPropertyChanged(const std::vector<ksapi::IDrawingObjectPtr> & objects)
{
  ShowEventName(L"Obj2DEvent::PropertyChanged", {}, false);
  return true;
}
#pragma endregion

//-------------------------------------------------------------------------------
//
// ---
void DrawingObjectEvent::SelectObjects(const std::vector<ksapi::IDrawingObjectPtr> & objects, bool select)
{
  ksapi::IKompasDocument2DPtr doc2D = m_doc;
  std::vector<ksapi::IKompasAPIObjectPtr> apiObjs;
  apiObjs.reserve(objects.size());

  for (auto & obj : objects)
    apiObjs.push_back(obj);

  if (ksapi::ISelectionManagerPtr selectionMngr = doc2D ? doc2D->GetSelectionManager() : nullptr)
  {
    if (select)
      selectionMngr->Select(apiObjs);
    else
      selectionMngr->Unselect(apiObjs);
  }

  if (!select && objects == m_copyObjects)
    m_copyObjects.clear();
}

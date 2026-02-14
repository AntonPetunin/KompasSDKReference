#pragma once
#include "Events_pch.h"
#include <set>

#ifdef TESTS
#include <Snapshot.h>
#endif

class BaseEvent;

/////////////////////////////////////////////////////////////////////////////
//
// Типы обработчиков событий
//
/////////////////////////////////////////////////////////////////////////////
enum class EventType
{
  Unkown = 0,
  Application = CMP_APP_ADVISE,
  DocumentFrame = 15,
  Document = CMP_DOC_ADVISE,
  Document2D = CMP_DOC_2D_ADVISE,
  Object2D = CMP_OBJECT_2D_ADVISE,
  SelectionManager = CMP_SELECTION_MNG_ADVISE,
  Stamp = CMP_STAMP_ADVISE,
  Document3D = CMP_DOC_3D_ADVISE,
  Object3D = CMP_ADVISE_O3D_UNKNOWN,
  SpecificationObject = CMP_SPC_ADVISE_ALL,
  SpecificationDocument = CMP_SPC_DOC_ADVISE,
  SpecificationDescription = CMP_SPC_ADVISE
};

using EventsMap = std::multimap<EventType, std::unique_ptr<BaseEvent>>;


/////////////////////////////////////////////////////////////////////////////
//
// Контейнер для обработчиков событий
//
/////////////////////////////////////////////////////////////////////////////
class EventContainer final
{
  static EventsMap m_events;

public:
  EventContainer() = delete;
  ~EventContainer() = default;

  /// Создать новый обработчик указанного типа
  static BaseEvent * NewEvent(EventType eventType);
  static BaseEvent * NewEvent(EventType eventType, ksapi::IKompasDocumentPtr document);
  static BaseEvent * NewEvent(EventType eventType, ksapi::IKompasDocumentPtr document, int32_t objectType,
                              const ksapi::IKompasAPIObjectPtr & object);
  /// Отписать все события
  static void UnAdviseAll();
  /// Отписать все события по указанному типу
  static void UnAdviseByType(EventType eventType);
  /// Отписать все события по указанному документу
  static void UnAdviseByDoc(ksapi::IKompasDocumentPtr document);
  /// Отписать все события указанного типа и документа
  static void UnAdviseByTypeAndDoc(EventType eventType, ksapi::IKompasDocumentPtr document);
  /// Отписать все события указанного объекта
  static void UnAdviseFromObject(EventType eventType, ksapi::IKompasDocumentPtr document, int32_t objectType,
                                 const ksapi::IKompasAPIObjectPtr & object);
  /// Проверить наличие подписки в контейнере по типу события
  static BaseEvent * FindEvents(EventType eventType);
  /// Проверить наличие подписки в контейнере по типу и документу
  static BaseEvent * FindEvents(EventType eventType, const ksapi::IKompasDocumentPtr & document);
  /// Проверить наличие подписки в контейнере по типу, документу и типу объекта или по самому объекту
  static BaseEvent * FindEvents(EventType eventType, const ksapi::IKompasDocumentPtr & document, int32_t objectType,
                                const ksapi::IKompasAPIObjectPtr & object);
  /// Получить список активных подписок
  static void ListEvents();

private:
  static BaseEvent * NewEvent(EventType eventType, ksapi::IKompasDocumentPtr document, int32_t objectType, bool silent,
                              const ksapi::IKompasAPIObjectPtr & object);
};

/////////////////////////////////////////////////////////////////////////////
//
// Базовый класс для обработчиков событий
//
/////////////////////////////////////////////////////////////////////////////
class BaseEvent
{
public:
  BaseEvent() = delete;
  BaseEvent(EventType eventType);
  BaseEvent(EventType eventType, const ksapi::IKompasDocumentPtr & document);
  virtual ~BaseEvent();

  /// Подписаться на получение событий
  virtual bool Advise();
  /// Отписаться от получения событий
  virtual void UnAdvise();

  virtual std::wstring EventCaption() = 0;
  std::wstring GetListAdvisedObjects();
  EventType GetType() { return m_eventType; }
  ksapi::IKompasDocumentPtr GetDocument() { return m_doc; }
  void ShowEventName(const std::wstring & eventName, ksapi::IKompasDocumentPtr document, const std::wstring & args = L"");
  void ShowEventName(const std::wstring & eventName, const std::wstring & args = L"");
  std::wstring GetDocumentParam(ksapi::IKompasDocumentPtr doc);
  /// Вывести параметры 2D объекта
  void ShowObjParam(ksapi::IDrawingObjectPtr obj);
  void ShowObjectsParam(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  /// Вывести параметры 3D объекта
  std::wstring GetObjectParam(const ksapi::IKompasAPIObjectPtr & obj);

  bool IsAdvised() { return m_advise; }
  /// Проверить подписку на объект по типу
  bool IsObjectAdvised(int32_t objectType);
  /// Проверить подписку на объект
  bool IsObjectAdvised(const ksapi::IKompasAPIObjectPtr & object);

  /// Добавить типы объектов на события которых подписываемся.
  virtual void AddObjectsTypes(const std::vector<int32_t> & types);
  /// Удалить типы объектов на события которых подписываемся.
  virtual void RemoveObjectsTypes(const std::vector<int32_t> & types);
  /// Удалить все типы объектов на события которых подписываемся.
  virtual void RemoveAllObjectsTypes();

  /// Добавить объекты на события которых подписываемся.
  virtual void AddObjects(const std::vector<ksapi::IKompasAPIObjectPtr> & objects);
  /// Удалить объекты на события которых подписываемся.
  virtual void RemoveObjects(const std::vector<ksapi::IKompasAPIObjectPtr> & objects);
  /// Удалить все объекты на события которых подписываемся.
  virtual void RemoveAllObjects();

#ifdef TESTS
  static EventMessages m_eventMessages;
#endif

protected:
  EventType m_eventType = EventType::Unkown;
  bool m_advise = false;           ///< true - сделан Advise
  ksapi::IKompasDocumentPtr m_doc; ///< документ для отписки

  std::set<int32_t> m_advisedTypes;                      ///< Типы подписанных объектов
  std::set<ksapi::IKompasAPIObjectPtr> m_advisedObjects; ///< Подписанные объекты
};

/// Подписка на события документа
void AdviseDoc(ksapi::IKompasDocumentPtr document);

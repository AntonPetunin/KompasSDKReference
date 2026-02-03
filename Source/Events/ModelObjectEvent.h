#pragma once

#include "BaseEvent.h"

////////////////////////////////////////////////////////////////////////////////
//
// ModelObjectEvent  - обработчик событий объектов 3D документа
//
////////////////////////////////////////////////////////////////////////////////
class ModelObjectEvent final : public BaseEvent
{
  ksapi::IModelObjectEventsPtr m_events;
  std::wstring m_handlerName = L"ModelObjectEvent";

public:
  ModelObjectEvent(const ksapi::IKompasDocumentPtr document);
  ModelObjectEvent(const ksapi::IKompasDocumentPtr document, const std::wstring & handlerName);
  virtual ~ModelObjectEvent();

  virtual bool Advise() override;   // подписаться на получение событий
  virtual void UnAdvise() override; // отписаться от получения событий

  virtual std::wstring EventCaption() override { return L"Obj3DEvent: " + GetDocumentParam(m_doc); };

  /// Вывести сообщение от обработчика с проверкой
  void ShowEventName(const std::wstring & eventName, const std::vector<ksapi::IModelObjectPtr> & objects, bool showResult,
                     const std::wstring & args = L"");

  /// Добавить типы объектов на события которых подписываемся.
  virtual void AddObjectsTypes(const std::vector<int32_t> & types) override;
  /// Удалить типы объектов на события которых подписываемся.
  virtual void RemoveObjectsTypes(const std::vector<int32_t> & types) override;
  /// Удалить все типы объектов на события которых подписываемся.
  virtual void RemoveAllObjectsTypes() override;

  /// Добавить объекты на события которых подписываемся.
  virtual void AddObjects(const std::vector<ksapi::IKompasAPIObjectPtr> & objects) override;
  /// Удалить объекты на события которых подписываемся.
  virtual void RemoveObjects(const std::vector<ksapi::IKompasAPIObjectPtr> & objects) override;
  /// Удалить все объекты на события которых подписываемся.
  virtual void RemoveAllObjects() override;

#pragma region Обработчики событий объектов 3D документа
  /// Обработчик события "Начало удаления объектов".
  bool OnBeginDeleteObject(const std::vector<ksapi::IModelObjectPtr> & objects);
  /// Обработчик события "Удаления объектов".
  void OnDeleteObject(const std::vector<ksapi::IModelObjectPtr> & objects);
  /// Обработчик события "Объект исключен/включен в расчет".
  void OnExcludedObject(const std::vector<ksapi::IModelObjectPtr> & objects, bool excluded);
  /// Обработчик события "Объект скрыт/показан".
  void OnHiddenObject(const std::vector<ksapi::IModelObjectPtr> & objects, bool hidden);
  /// Обработчик события "Начало изменения свойств объекта".
  bool OnBeginPropertyChanged(const std::vector<ksapi::IModelObjectPtr> & objects);
  /// Обработчик события "Изменены свойства объекта".
  void OnPropertyChanged(const std::vector<ksapi::IModelObjectPtr> & objects);
  /// Обработчик события "Начало изменения положения объекта".
  bool OnBeginPlacementChanged(const std::vector<ksapi::IModelObjectPtr> & objects);
  /// Обработчик события "Изменено положения объекта".
  void OnPlacementChanged(const std::vector<ksapi::IModelObjectPtr> & objects);
  /// Обработчик события "Начало редактирования\создания объекта" (false - запрещает процесс).
  bool OnBeginProcess(ksObj3dTypeEnum objectType, const ksapi::IModelObjectPtr & object);
  /// Обработчик события "Завершение редактирования/создания объекта".
  void OnEndProcess(ksObj3dTypeEnum objectType);
  /// Обработчик события "Создание объекта".
  void OnCreateObject(const std::vector<ksapi::IModelObjectPtr> & objects);
  /// Обработчик события "Редактирование объекта".
  void OnUpdateObject(const ksapi::IModelObjectPtr & object);
  /// Обработчик события "Начало изменения типа загрузки".
  bool OnBeginLoadStateChange(const std::vector<ksapi::IModelObjectPtr> & objects, ksLoadStateEnum loadState);
  /// Обработчик события "Завершение изменения типа загрузки".
  void OnLoadStateChange(const std::vector<ksapi::IModelObjectPtr> & objects, ksLoadStateEnum loadState);
#pragma endregion

private:
  void InitEventHandler();
  void ChooseObjects(const std::vector<ksapi::IModelObjectPtr> & objects, bool choose);
};

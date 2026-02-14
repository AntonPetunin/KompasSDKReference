#pragma once

#include "BaseEvent.h"

////////////////////////////////////////////////////////////////////////////////
//
// DrawingObjectEvent  - обработчик событий объектов 2D документа
//
////////////////////////////////////////////////////////////////////////////////
class DrawingObjectEvent final : public BaseEvent
{
  ksapi::IDrawingObjectEventsPtr m_events;
  std::wstring m_handlerName = L"DrawingObjectEvent";
  std::vector<ksapi::IDrawingObjectPtr> m_copyObjects;

public:
  DrawingObjectEvent(const ksapi::IKompasDocumentPtr document);
  DrawingObjectEvent(const ksapi::IKompasDocumentPtr document, const std::wstring & handlerName);
  virtual ~DrawingObjectEvent();

  virtual bool Advise() override;   // подписаться на получение событий
  virtual void UnAdvise() override; // отписаться от получения событий

  virtual std::wstring EventCaption() override { return L"Obj2DEvent: " + GetDocumentParam(m_doc); };

  /// Вывести сообщение от обработчика с проверкой
  void ShowEventName(const std::wstring & eventName, const std::vector<ksapi::IDrawingObjectPtr> & objects, bool showResult,
                     const std::wstring & args = L"");

  /// Добавить объекты на события которых подписываемся.
  virtual void AddObjects(const std::vector<ksapi::IKompasAPIObjectPtr> & objects) override;
  /// Удалить объекты на события которых подписываемся.
  virtual void RemoveObjects(const std::vector<ksapi::IKompasAPIObjectPtr> & objects) override;
  /// Удалить все объекты на события которых подписываемся.
  virtual void RemoveAllObjects() override;

  /// Добавить типы объектов на события которых подписываемся.
  virtual void AddObjectsTypes(const std::vector<int32_t> & types) override;
  /// Удалить типы объектов на события которых подписываемся.
  virtual void RemoveObjectsTypes(const std::vector<int32_t> & types) override;
  /// Удалить все типы объектов на события которых подписываемся.
  virtual void RemoveAllObjectsTypes() override;

#pragma region Обработчики событий объектов 2D документа
  // Переключение активности объекта (вид, слой)
  bool OnChangeActive(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  // Создание объекта.
  bool OnCreateObject(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  // Редактирование объекта.
  bool OnUpdateObject(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  // Удаление объекта
  bool OnDelete(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  // Начало удаления сдвига, false - запрещает удаление
  bool OnBeginDelete(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  // Создание объекта.
  bool OnBeginDestroyObject(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  // Редактирование объекта.
  bool OnDestroyObject(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  // Начало сдвига объекта, false - запрещает сдвиг
  bool OnBeginMove(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  //Начало поворота объекта, false - запрещает поворот
  bool OnBeginRotate(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  // Начало масштабирования объекта, false - запрещает поворот
  bool OnBeginScale(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  // Начало симметрии  объекта, false - запрещает симметрию
  bool OnBeginSymmetry(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  // Начало копирования объекта, false - запрещает копирование
  bool OnBeginCopy(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  // Начало трансформации объекта, false - запрещает трансформацию
  bool OnBeginTransform(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  //Сдвиг объекта
  bool OnMove(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  // Поворот объекта
  bool OnRotate(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  // Масштабирование объекта
  bool OnScale(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  // Симметрия  объекта
  bool OnSymmetry(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  // Копирование объекта
  bool OnCopy(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  // Трансформация объекта
  bool OnTransform(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  // Начало изменения свойств объекта
  bool OnBeginPropertyChanged(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  // Изменены свойства объекта
  bool OnPropertyChanged(const std::vector<ksapi::IDrawingObjectPtr> & objects);

  // Начало редактирования\создания объекта.false - запрещает процесс.
  bool OnBeginProcess(DrawingObjectTypeEnum objectType, const ksapi::IDrawingObjectPtr & object);

  // Конец редактирования\создания объекта
  bool OnEndProcess(DrawingObjectTypeEnum objectType);
#pragma endregion

private:
  void InitEventHandler();
  void SelectObjects(const std::vector<ksapi::IDrawingObjectPtr> & objects, bool select);
};

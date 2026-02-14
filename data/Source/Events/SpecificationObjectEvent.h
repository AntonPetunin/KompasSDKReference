#pragma once

#include "BaseEvent.h"

////////////////////////////////////////////////////////////////////////////////
//
// SpecificationObjectEvent  - обработчик событий объектов спецификации
//
////////////////////////////////////////////////////////////////////////////////
class SpecificationObjectEvent final : public BaseEvent
{
  ksapi::ISpecificationObjectEventsPtr m_events;
  std::wstring m_handlerName = L"SpecificationObjectEvent";

public:
  SpecificationObjectEvent(const ksapi::IKompasDocumentPtr document);
  SpecificationObjectEvent(const ksapi::IKompasDocumentPtr document, const std::wstring & handlerName);
  virtual ~SpecificationObjectEvent();

  virtual bool Advise() override;   // подписаться на получение событий
  virtual void UnAdvise() override; // отписаться от получения событий

  virtual std::wstring EventCaption() override { return L"SpcObjectEvent: " + GetDocumentParam(m_doc); };

  /// Вывести сообщение от обработчика с проверкой
  void ShowEventName(const std::wstring & eventName, const ksapi::ISpecificationObjectPtr & obj, const std::wstring & args = L"");

  /// Добавить типы объектов на события которых подписываемся.
  virtual void AddObjectsTypes(const std::vector<int32_t> & types) override;
  /// Удалить типы объектов на события которых подписываемся.
  virtual void RemoveObjectsTypes(const std::vector<int32_t> & types) override;


#pragma region Обработчики событий объектов спецификации
  /// Описание обработчика события "Начало удаления объекта".
  bool OnBeginDelete(const ksapi::ISpecificationObjectPtr & obj);

  /// Описание обработчика события "Удаления объекта".
  void OnDelete(const ksapi::ISpecificationObjectPtr & obj);

  /// Описание обработчика события "Двойной клик в ячейке".
  bool OnCellDblClick(const ksapi::ISpecificationObjectPtr & obj, int32_t columnNum);

  /// Описание обработчика события "Начало редактирования в ячейке".
  bool OnCellBeginEdit(const ksapi::ISpecificationObjectPtr & obj, int32_t columnType);

  /// Описание обработчика события "Изменился текущий объект".
  void OnChangeCurrent(const ksapi::ISpecificationObjectPtr & obj);

  /// Описание обработчика события "Начало добавления документа".
  bool OnDocumentBeginAdd(const ksapi::ISpecificationObjectPtr & obj);

  /// Описание обработчика события "Добавление документа в объекте СП".
  void OnDocumentAdd(const ksapi::ISpecificationObjectPtr & obj, const std::wstring & nameDoc);

  /// Описание обработчика события "Удаление документа из объекта СП".
  void OnDocumentRemove(const ksapi::ISpecificationObjectPtr & obj, const std::wstring & nameDoc);

  /// Описание обработчика события "Начало измения геометрии объекта СП".
  bool OnBeginGeomChange(const ksapi::ISpecificationObjectPtr & obj);

  /// Описание обработчика события "Геометрия объекта СП изменилась".
  void OnGeomChange(const ksapi::ISpecificationObjectPtr & obj);

  /// Описание обработчика события "Начало редактирования\создания объекта (false - запрещает процесс)".
  bool OnBeginProcess(ksSpecificationObjectTypeEnum specificationObjectType, const ksapi::ISpecificationObjectPtr & obj);

  /// Описание обработчика события "Конец редактирования\создания объекта".
  void OnEndProcess(ksSpecificationObjectTypeEnum specificationObjectType);

  /// Описание обработчика события "Создание объекта".
  void OnCreate(const ksapi::ISpecificationObjectPtr & obj);

  /// Описание обработчика события "Редактирование объекта".
  void OnUpdate(const ksapi::ISpecificationObjectPtr & obj);

  /// Описание обработчика события "Начало копирования объекта".
  bool OnBeginCopy(const ksapi::ISpecificationObjectPtr & obj);

  /// Описание обработчика события "Копирование объекта".
  void OnCopy(const ksapi::ISpecificationObjectPtr & obj);
#pragma endregion

private:
  void InitEventHandler();
};

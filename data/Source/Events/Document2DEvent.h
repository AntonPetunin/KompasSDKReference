#pragma once

#include "BaseEvent.h"

////////////////////////////////////////////////////////////////////////////////
//
// Document2DEvent  - обработчик событий 2D документа
//
////////////////////////////////////////////////////////////////////////////////
class Document2DEvent final : public BaseEvent
{
public:
  Document2DEvent(const ksapi::IKompasDocumentPtr document);
  virtual ~Document2DEvent();

  virtual bool Advise() override;   // подписаться на получение событий
  virtual void UnAdvise() override; // отписаться от получения событий

  virtual std::wstring EventCaption() override { return L"Document2DEvent: " + GetDocumentParam(m_doc); };

  /// Вывести сообщение от обработчика с проверкой
  void ShowEventName(const std::wstring & eventName, const std::wstring & args = L"");

#pragma region Обработчики событий 2D документа
  // Начало перестроения модели
  bool OnBeginRebuild();
  // Модель перестроена
  bool OnRebuild();
  // Начало выбора материала
  bool OnBeginChoiceMaterial();
  // Начало вставки фрагмента (до диалога выбора имени)
  bool OnBeginInsertFragment();
  // Начало выбора свойства
  bool OnBeginChoiceProperty(const ksapi::IKompasAPIObjectPtr & object, ksapi::NumericId propertyTypeId);
  // Закончен выбор свойства
  bool OnChoiceProperty(const ksapi::IKompasAPIObjectPtr & object, ksapi::NumericId propertyTypeId);
  // Начало удаления значения свойства
  bool OnBeginDeleteProperty(const ksapi::IKompasAPIObjectPtr & object, ksapi::NumericId propertyTypeId);
  // Завершение удаления свойства
  bool OnDeleteProperty(const ksapi::IKompasAPIObjectPtr & object, ksapi::NumericId propertyTypeId);
#pragma endregion
};

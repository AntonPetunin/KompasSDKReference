#pragma once

#include "BaseEvent.h"

////////////////////////////////////////////////////////////////////////////////
//
// SpecificationDocumentEvent  - обработчик событий документа спецификации.
//
////////////////////////////////////////////////////////////////////////////////
class SpecificationDocumentEvent final : public BaseEvent
{
public:
  SpecificationDocumentEvent(const ksapi::IKompasDocumentPtr document);
  virtual ~SpecificationDocumentEvent();

  /// Подписаться на получение событий
  virtual bool Advise() override;
  /// Отписаться от получения событий
  virtual void UnAdvise() override;

  virtual std::wstring EventCaption() override { return L"SpcDocEvent: " + GetDocumentParam(m_doc); };
  /// Вывести сообщение от обработчика с проверкой
  void ShowEventName(const std::wstring & eventName, const std::wstring & args = L"");

#pragma region Обработчики событий документа спецификации
  /// Обработчик события "Начало добавления документа сборочного чертежа".
  bool OnDocumentBeginAdd();
  /// Обработчик события "Добавление документа сборочного чертежа".
  void OnDocumentAdd(const std::wstring & sheetName);
  /// Обработчик события "Начало удаления документа сборочного чертежа".
  bool OnDocumentBeginRemove(const std::wstring & sheetName);
  /// Обработчик события "Удаление документа сборочного чертежа".
  void OnDocumentRemove(const std::wstring & sheetName);
  /// Обработчик события "Начало изменения стиля спецификации".
  bool OnSpcStyleBeginChange(const std::wstring & sheetName, int32_t formNumber);
  /// Обработчик события "Стиль спецификации изменился".
  bool OnSpcStyleChange(const std::wstring & sheetName, int32_t formNumber);
#pragma endregion
};

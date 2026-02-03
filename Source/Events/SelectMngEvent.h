#pragma once

#include "BaseEvent.h"

////////////////////////////////////////////////////////////////////////////////
//
// SelectMngEvent  - обработчик событий менеджера селектирования документа
//
////////////////////////////////////////////////////////////////////////////////
class SelectMngEvent final : public BaseEvent
{
  DocumentTypeEnum docType = DocumentTypeEnum::ksDocumentUnknown;

public:
  SelectMngEvent(const ksapi::IKompasDocumentPtr document);
  virtual ~SelectMngEvent();

  /// Подписаться на получение событий
  virtual bool Advise() override;
  /// Отписаться от получения событий
  virtual void UnAdvise() override;

  virtual std::wstring EventCaption() override { return L"SelectMngEvent: " + GetDocumentParam(m_doc); };
  /// Вывести сообщение от обработчика с проверкой
  void ShowEventName(const std::wstring & eventName, const std::vector<ksapi::IKompasAPIObjectPtr> & objects);

#pragma region Обработчики событий менеджера селектирования документа
  /// Обработчик события "Объект селектирован".
  void OnSelect(const std::vector<ksapi::IKompasAPIObjectPtr> & objects);

  /// Обработчик события "Объект расселектирован".
  void OnUnselect(const std::vector<ksapi::IKompasAPIObjectPtr> & objects);

  /// Обработчик события "Все объекты расселектированы".
  void OnUnselectAll();
#pragma endregion
};

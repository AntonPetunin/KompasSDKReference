#pragma once

#include "BaseEvent.h"

////////////////////////////////////////////////////////////////////////////////
//
// SpecificationDescriptionEvent  - обработчик событий описания спецификации.
//
////////////////////////////////////////////////////////////////////////////////
class SpecificationDescriptionEvent final : public BaseEvent
{
public:
  SpecificationDescriptionEvent(const ksapi::IKompasDocumentPtr document);
  virtual ~SpecificationDescriptionEvent();

  /// Подписаться на получение событий
  virtual bool Advise() override;
  /// Отписаться от получения событий
  virtual void UnAdvise() override;

  virtual std::wstring EventCaption() override { return L"SpecificationDescriptionEvent: " + GetDocumentParam(m_doc); };
  /// Вывести сообщение от обработчика с проверкой
  void ShowEventName(const std::wstring & eventName, const std::wstring & args = L"");

#pragma region Обработчики событий описания спецификации
  /// Обработчик события "Начало изменения настроек спецификации".
  bool OnTuningSpcStyleBeginChange(const ksapi::ISpecificationDescriptionPtr & description);

  /// Обработчик события "Настройки спецификации изменились".
  void OnTuningSpcStyleChange(const ksapi::ISpecificationDescriptionPtr & description, bool wasChanged);
  /// Обработчик события "Изменилось текущее описание спецификации".
  void OnChangeCurrentSpcDescription(const ksapi::ISpecificationDescriptionPtr & description);
  /// Обработчик события "Добавилось описание спецификации".
  void OnSpcDescriptionAdd(const ksapi::ISpecificationDescriptionPtr & description);
  /// Обработчик события "Удалилось описание спецификации".
  void OnSpcDescriptionRemove(const ksapi::ISpecificationDescriptionPtr & description);
  /// Обработчик события "Начало редактирования описания спецификации".
  bool OnSpcDescriptionBeginEdit(const ksapi::ISpecificationDescriptionPtr & description);
  /// Обработчик события "Отредактировали описание спецификации".
  void OnSpcDescriptionEdit(const ksapi::ISpecificationDescriptionPtr & description, bool wasChanged);
  /// Обработчик события "Начало расчета позиций".
  bool OnBeginCalcPositions();
  /// Обработчик события "Проведен расчет позиций".
  void OnCalcPositions();
  /// Обработчик события "Начало создания объекта СП (до диалога выбора раздела)".
  bool OnBeginCreateObject(int32_t objectType);
#pragma endregion
};

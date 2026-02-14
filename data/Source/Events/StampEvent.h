#pragma once

#include "BaseEvent.h"

////////////////////////////////////////////////////////////////////////////////
//
// StampEvent  - обработчик событий штампа
//
////////////////////////////////////////////////////////////////////////////////
class StampEvent final : public BaseEvent
{
public:
  StampEvent(const ksapi::IKompasDocumentPtr document);
  virtual ~StampEvent();

  /// Подписаться на получение событий
  virtual bool Advise() override;
  /// Отписаться от получения событий
  virtual void UnAdvise() override;

  virtual std::wstring EventCaption() override { return L"StampEvent: " + GetDocumentParam(m_doc); };
  /// Вывести сообщение от обработчика с проверкой
  void ShowEventName(const std::wstring & eventName, const std::wstring & args = L"");

#pragma region Обработчики событий штампа
  /// Обработчик события "Начало работы со штампом".
  bool OnBeginEditStamp();
  /// Обработчик события "Завершение работы со штампом".
  void OnEndEditStamp(bool wasChanged);
  /// Обработчик события "Двойной клик в ячейке штампа".
  bool OnStampCellDblClick(int32_t cellId);
  /// Обработчик события "Начало редактирования в ячейке штампа".
  bool OnStampCellBeginEdit(int32_t cellId);
  /// Обработчик события "Начало очистки ячеек штампа".
  bool OnStampBeginClearCells(const std::vector<int32_t> & currentCellId);
#pragma endregion

private:
  /// Получить штамп первой страницы.
  ksapi::IStampPtr GetStamp();
};

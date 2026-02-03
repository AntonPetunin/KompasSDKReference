#pragma once

#include "BaseEvent.h"

////////////////////////////////////////////////////////////////////////////////
//
// DocumentEvent  - обработчик событий документа
//
////////////////////////////////////////////////////////////////////////////////
class DocumentEvent final : public BaseEvent
{
  bool m_silent = false; ///< Флаг для отмены вывода сообщений (случай когда подписываемся ради контроля закрытия документа)

public:
  DocumentEvent(const ksapi::IKompasDocumentPtr document);
  DocumentEvent(const ksapi::IKompasDocumentPtr document, bool silent);
  virtual ~DocumentEvent();

  /// Подписаться на получение событий
  virtual bool Advise() override;
  /// Отписаться от получения событий
  virtual void UnAdvise() override;

  virtual std::wstring EventCaption() override { return L"DocumentEvent: " + GetDocumentParam(m_doc); };
  /// Вывести сообщение от обработчика с проверкой
  void ShowEventName(const std::wstring & eventName, ksapi::IKompasDocumentPtr document = nullptr,
                     const std::wstring & args = L"");

#pragma region Обработчики событий документа
  bool OnBeginCloseDocument();
  void OnCloseDocument();
  bool OnBeginSaveDocument(const std::wstring & fileName);
  void OnSaveDocument();
  bool OnActiveDocument();
  void OnDeactiveDocument();
  bool OnBeginSaveAsDocument();
  bool OnProcessActivate(int32_t processId);
  void OnProcessDeactivate(int32_t processId);
  bool OnBeginProcess(int32_t processId);
  bool OnEndProcess(int32_t processId, bool success);
  void OnBeginAutoSaveDocument(const std::wstring & fileName);
  void OnAutoSaveDocument();
  void OnActivateToolbarSet(const std::wstring & oldToolbarSetId, const std::wstring & newToolbarSetId);
#pragma endregion
};

#pragma once
#include "BaseEvent.h"


////////////////////////////////////////////////////////////////////////////////
//
// Обработчик событий окна документа
//
////////////////////////////////////////////////////////////////////////////////
class DocumentFrameEvent final : public BaseEvent
{
public:
  DocumentFrameEvent(const ksapi::IKompasDocumentPtr document);
  virtual ~DocumentFrameEvent();

  /// Подписаться на получение событий
  virtual bool Advise() override;
  /// Отписаться от получения событий
  virtual void UnAdvise() override;

  void ShowEventName(const std::wstring & eventName, const std::wstring & args = L"");
  virtual std::wstring EventCaption() override { return L"DocumentFrameEvent: " + GetDocumentParam(m_doc); };

#pragma region Обработчики событий окна документа
  bool OnBeginPaint(const ksapi::IPaintObjectPtr & paintObject);
  void OnClosePaint(const ksapi::IPaintObjectPtr & paintObject);
  bool OnMouseDown(const ksapi::IPressedKeysPtr & keys, int32_t x, int32_t y);
  bool OnMouseUp(const ksapi::IPressedKeysPtr & keys, int32_t x, int32_t y);
  bool OnMouseDblClick(const ksapi::IPressedKeysPtr & keys, int32_t x, int32_t y);
  bool OnBeginPaintGl(uint32_t drawMode, const ksapi::IOpenGLObjectPtr & glObject);
  void OnClosePaintGl(uint32_t drawMode, const ksapi::IOpenGLObjectPtr & glObject);
  void OnAddGabarit(ksapi::IGabaritObjectPtr gabarit);
  void OnActivate();
  void OnDeactivate();
  void OnClose();
  bool OnMouseMove(const ksapi::IPressedKeysPtr & keys, int32_t x, int32_t y);
  bool OnBeginPaintTmpObjects();
  void OnClosePaintTmpObjects();
  bool OnMouseWheelHandler(const ksapi::IPressedKeysPtr & keys, int32_t zDelta, int32_t x, int32_t y);
#pragma endregion
};

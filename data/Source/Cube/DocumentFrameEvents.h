#pragma once
#include "Cube_pch.h"

class DocumentFrameEvents final
{
  ksapi::IApplication * m_kompasApp = nullptr;
  ksapi::IDocumentFrameEventsPtr m_events;
  bool m_is_advised = false;

public:
  DocumentFrameEvents(ksapi::IApplication * kompasApp, ksapi::IDocumentFramePtr & documentFrame, int redrawType = 0);
  ~DocumentFrameEvents();

  static int redrawType;

#pragma region Обработчики событий окна документа
  bool OnBeginPaintGl(unsigned int drawMode, const ksapi::IOpenGLObjectPtr & glObject);
  void OnClose();
#pragma endregion Обработчики событий окна документа

private:
  /// Подписка на события окна документа
  void AdviseFrameEvent();

  /// Отписка от событий окна документа
  void UnAdviseFrameEvent();
};

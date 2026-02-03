#pragma once

#include <KsAPI.h>
#include <TransferService.h>
#include <KompasLibraryActions.h>

#include "Resource.h"

#include <memory>
#include <map>
#include <filesystem>

/////////////////////////////////////////////////////////////////////////////
//
// Настройки приложения
//
/////////////////////////////////////////////////////////////////////////////
struct AppConfig
{
  bool m_auto = false;         ///< Признак автоматической подписки при запуске
  bool m_mes_2DDOC = true;     ///< Признак вывода сообщений о событиях 2D документов
  bool m_mes_3DDOC = true;     ///< Признак вывода сообщений о событиях 3D документов
  bool m_mes_APP = true;       ///< Признак вывода сообщений о событиях Компаса
  bool m_mes_DOC = true;       ///< Признак вывода сообщений о событиях документов
  bool m_mes_OBJ_2DDOC = true; ///< Признак вывода сообщений о объектах 2D документов
  bool m_mes_OBJ_3DDOC = true; ///< Признак вывода сообщений о объектах 3D документов
  bool m_mes_OBJ_SPC = true;   ///< Признак вывода сообщений о объектах спецификации
  bool m_mes_SELECT = true;    ///< Признак вывода сообщений о событиях менеджера селектирования
  bool m_mes_SPC_DESCR = true; ///< Признак вывода сообщений о событиях настройки спецификации
  bool m_mes_SPCDOC = true;    ///< Признак вывода сообщений о событиях документов спецификации
  bool m_mes_STAMP = true;     ///< Признак вывода сообщений о событиях штампа
  bool m_mes_DOC_FRAME = true; ///< Признак вывода сообщений о событиях окна документа
};

constexpr auto libname = L"Events";

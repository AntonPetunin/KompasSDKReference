////////////////////////////////////////////////////////////////////////////////
///
/**
  \file 
  \brief Содержит класс, пример бибилотечного конвертера документов.
*/
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <MyConverter_pch.h>
#include <LibConverterParamInterface.h>

////////////////////////////////////////////////////////////////////////////////
//
/// Класс - реализация библиотечного конвертера.
//
////////////////////////////////////////////////////////////////////////////////
class LibConverter
{
protected:
  bool m_saveLineStyle = true;              ///< Признак сохранения стиля линий.
  ksapi::ILibraryConverterPtr m_pConverter; ///< Указатель на класс конвертера Компаса.
  ILibConverterParamPtr m_pConverterParam;  ///< Указатель класс параметров конвертера.

public:
  /// Получить указатель на компас конвертер.
  ksapi::ILibraryConverterPtr GetKompasConverter();

  /// Запустить визуальное редактирование параметров.
  bool VisualEditConvertParam(ksapi::WindowHandle parentHwnd, int32_t command);
  /// Конвертировать
  int32_t Convert(const std::wstring & inputFile, const std::wstring & outFile, LibConverterCommands command, bool showParam);
  /// Проверить, можно ли выгрузить библиотеку из памяти.
  bool GetCanUnLoadLibrary();
  /// Выполнить команду меню.
  void ExecuteCommand(int32_t command);
  /// Получить указатель на класс параметров конвертера.
  ILibConverterParamPtr GetConvertParam(int32_t command);
  // Получить фильтр и команду для конвертера по типу документа.
  std::wstring GetFilter(int32_t docType, bool saveAs, int32_t & command);
  /// Получить фильтр и команду для конвертера по расширению документа.
  std::wstring GetFilter(const std::wstring & ext, bool saveAs, int32_t & command);

  /// Установить признак сохранения стиля линий (вызывается через интерфейс параметров).
  void SetSaveLineStyle(bool val);
  /// Получить признак сохранения стиля линий (вызывается через интерфейс параметров).
  bool GetSaveLineStyle();
};

/// Получить библиотечный конвертер. Создаётся при первом вызове.
LibConverter & GetLibConverter();
/// Удаление конвертера (вызывается при выгрузке библиотеки).
void ConverterDestroy();

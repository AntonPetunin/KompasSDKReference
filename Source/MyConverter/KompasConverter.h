////////////////////////////////////////////////////////////////////////////////
///
/**
  \file 
  \brief Содержит класс, реализующий интерфейс конвертора KsAPI.
*/
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <LibConverterParam.h>

////////////////////////////////////////////////////////////////////////////////
//
/// Класс - реализация интерфейса конвертора KsAPI.
/**
  Перевызывает методы библиотечного конвертора.
*/
////////////////////////////////////////////////////////////////////////////////
class KompasConverter : public ksapi::ILibraryConverter
{
  int32_t m_count = 0; ///< Счётчик копий указателей на класс.

public:
  /// Увеличить счётчик ссылок на объект.
  virtual void AddRef() override;
  /// Уменьшить счётчик ссылок на объект и разрушить его, если счётчик станет равным нулю.
  virtual void Release() override;
  /// Вернуть указатель на базовый класс KsAPI для this, если указанный параметр соответствует типу объекта.
  virtual ksapi::IAPIObject * QueryInterface(ksapi::IAPIObjectIID iid) override;
  /// Вернуть параметры конвертора по номеру команды.
  virtual ksapi::IAPIObjectPtr GetConverterParameters(int32_t command) override;
  /// Получить признак, можно ли выгружать библиотеку конвертора.
  virtual bool CanUnloadLibrary() override;
  /// Вернуть фильтр расширений и команду конвертера для его обработки по типу документа.
  virtual std::wstring GetFilter(int32_t docType, bool saveAs, int32_t & command) override;
  /// Конвертировать файл.
  virtual int32_t Convert(const std::wstring & inputFile, const std::wstring & outfile, int32_t command, bool showParam) override;
  /// Отобразить диалог редактирования параметров конвертора.
  virtual bool VisualEditConvertParam(ksapi::WindowHandle parentHwnd, int32_t command) override;
  /// Вернуть параметры конвертора (для обратной совместимости со старыми конверторами).
  virtual IUnknown * GetCOMConverterParameters(int32_t command) override;
};

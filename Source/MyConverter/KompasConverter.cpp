////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief Класс, реализующий интерфейс конвертора KsAPI.

  Своего функционала не имеет, перевызывает методы библиотечного конвертора.
*/
//
////////////////////////////////////////////////////////////////////////////////
#include <KompasConverter.h>

#include <LibConverter.h>

//------------------------------------------------------------------------------
/**
  Увеличить счётчик ссылок на объект.
*/
//---
void KompasConverter::AddRef()
{
  ++m_count;
}

//------------------------------------------------------------------------------
/**
  Уменьшить счётчик ссылок на объект и разрушить его, если счётчик станет равным нулю.
*/
//---
void KompasConverter::Release()
{
  if (--m_count == 0)
    delete this;
}

//------------------------------------------------------------------------------
/**
  Вернуть указатель на базовый класс KsAPI для this, если указанный параметр соответствует типу объекта.
  \param iid - идентификатор типа для сравнения.
  \return Указатель на базовый класс, если типы совместимы. Иначе nullptr.
*/
//---
ksapi::IAPIObject * KompasConverter::QueryInterface(ksapi::IAPIObjectIID iid)
{
  if (iid == ksapi::IID_IAPIObject || iid == ksapi::IID_ILibraryObject || iid == ksapi::IID_ILibraryConverter)
    return this;
  return nullptr;
}

//------------------------------------------------------------------------------
/**
  Вернуть параметры конвертора по номеру команды.
*/
//---
ksapi::IAPIObjectPtr KompasConverter::GetConverterParameters(int32_t command)
{
  return GetLibConverter().GetConvertParam(command);
}

//------------------------------------------------------------------------------
/**
   Получить признак, можно ли выгружать библиотеку конвертора.
*/
//---
bool KompasConverter::CanUnloadLibrary()
{
  return GetLibConverter().GetCanUnLoadLibrary();
}

//------------------------------------------------------------------------------
/**
  Вернуть фильтр расширений и команду конвертера для его обработки по типу документа.
  \details Описание параметров актуально для текущей реализации в MyConverter.
  \param[in] docType - тип документа из перечисления DocumentTypeEnum. -1 - вернуть фильтр для всех документов.
  \param[in] saveAs - флаг, необходимый для определения типа команды (true - сохранение(экспорт), false - загрузка(импорт))
  \param[out] command - команда из перечисления LibConverterCommands, либо -1 - если не определена.
  \return Строка фильтра расширений документов для использования в диалогах.
*/
//---
std::wstring KompasConverter::GetFilter(int32_t docType, bool saveAs, int32_t & command)
{
  command = 2;
  return GetLibConverter().GetFilter(docType, saveAs, command);
}

//------------------------------------------------------------------------------
/**
  Конвертировать файл.
  \details Описание параметров актуально для текущей реализации в MyConverter.
  \param[in] inputFile - исходный файл для конвертации (для команд экспорта возможна пустая строка - будет конвертирован текущий файл).
  \param[in] outfile - целевой файл (для команд импорта возможна пустая строка - будет создан новый документ).
  \param[in] command - команда из перечисления LibConverterCommands.
  \param[in] showParam - показывать диалог параметров.
  \return Успешность выполнения (0 - неудача, 1 - успех).
*/
//---
int32_t KompasConverter::Convert(const std::wstring & inputFile, const std::wstring & outfile, int32_t command, bool showParam)
{
  return GetLibConverter().Convert(inputFile, outfile, static_cast<LibConverterCommands>(command), showParam);
}

//------------------------------------------------------------------------------
/**
  Показать диалог редактирования параметров конвертации.
  \param[in] parentHwnd - хендл окна диалога.
  \param[in] command - команда конвертирования.
  \return Успешность выполнения.
*/
//---
bool KompasConverter::VisualEditConvertParam(ksapi::WindowHandle parentHwnd, int32_t command)
{
  return GetLibConverter().VisualEditConvertParam(parentHwnd, command);
}

//------------------------------------------------------------------------------
/**
  Вернуть параметры конвертора (для обратной совместимости со старыми конверторами).
*/
//---
IUnknown * KompasConverter::GetCOMConverterParameters(int32_t command)
{
  return nullptr;
}

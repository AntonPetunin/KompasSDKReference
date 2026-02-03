////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief Реализация класса библиотечнго конвертера.
*/
//
////////////////////////////////////////////////////////////////////////////////
#include <LibConverter.h>

#include <MyConverter.h>
#include <KompasConverter.h>

#include <format>
#include <memory>

std::unique_ptr<LibConverter> g_converter; ///< Объект библиотечного конвертера.
extern ksapi::IApplication * kompasApp;    ///< Интерфейс приложения KsAPI.
extern bool ShowPropDlg();                 ///< Вызвать диалог настройки параметров конвертации.


//------------------------------------------------------------------------------
/**
  Получить библиотечный конвертер. Создаётся при первом вызове.
*/
//---
LibConverter & GetLibConverter()
{
  if (!g_converter)
    g_converter = std::make_unique<LibConverter>();
  return *g_converter;
}


//------------------------------------------------------------------------------
/**
  Удаление конвертера (вызывается при выгрузке библиотеки).
*/
//---
void ConverterDestroy()
{
  g_converter.release();
}


//------------------------------------------------------------------------------
/**
  Получить KsAPI интерфейс.
*/
//---
ksapi::ILibraryConverterPtr LibConverter::GetKompasConverter()
{
  if (!m_pConverter)
    m_pConverter = new KompasConverter();
  else
    m_pConverter->AddRef();

  return m_pConverter;
}


//------------------------------------------------------------------------------
/**
  Запустить визуальное редактирование параметров.
*/
//---
bool LibConverter::VisualEditConvertParam(ksapi::WindowHandle parentHwnd, int32_t command)
{
  m_saveLineStyle = ShowPropDlg();
  return true;
}


//------------------------------------------------------------------------------
/**
  Конвертировать файл.
  \param[in] inputFile - исходный файл для конвертации (для команд экспорта возможна пустая строка - будет конвертирован текущий файл).
  \param[in] outfile - целевой файл (для команд импорта возможна пустая строка - будет создан новый документ).
  \param[in] command - команда из перечисления LibConverterCommands.
  \param[in] showParam - показывать диалог параметров.
  \return Успешность выполнения (0 - неудача, 1 - успех).
*/
//---
int32_t LibConverter::Convert(const std::wstring & inputFile, const std::wstring & outFile, LibConverterCommands command,
                              bool showParam)
{
  // Визуальное указание параметров конвертации пользователем.
  if (showParam)
    VisualEditConvertParam(nullptr, command);

  int32_t res = 0;  // Результат выполнения функции.
  std::wstring msg; // Сообщение на экран

  // Для команд не производится действий, только вывод текста на экран в качестве демонстрации работы конвертера.
  switch (command)
  {
    case libConv_Exp_m3d:
      msg = std::format(L"Команда libConv_Exp_m3d\nКонвертация {}файла модели Компаса {}\nв формат (*.mym3d) {}",
                        !inputFile.empty() ? L"" : L"текущего ", !inputFile.empty() ? inputFile : L"", outFile);
      res = 1;
      break;

    case libConv_Exp_a3d:
      msg = std::format(L"Команда libConv_Exp_a3d\nКонвертация {}файла сборки Компаса {}\nв формат (*.mya3d) {}",
                        !inputFile.empty() ? L"" : L"текущего ", !inputFile.empty() ? inputFile : L"", outFile);
      res = 1;
      break;

    case libConv_Exp_t3d:
      msg = std::format(L"Команда libConv_Exp_t3d\nКонвертация {}файла технологической сборки Компаса {}\nв формат (*.myt3d) {}",
                        !inputFile.empty() ? L"" : L"текущего ", !inputFile.empty() ? inputFile : L"", outFile);
      res = 1;
      break;

    case libConv_Exp_frw:
      msg = std::format(L"Команда libConv_Exp_frw\nКонвертация {}файла фрагмента Компаса {}\nв формат (*.myfrw) {}",
                        !inputFile.empty() ? L"" : L"текущего ", !inputFile.empty() ? inputFile : L"", outFile);
      res = 1;
      break;

    case libConv_Exp_cdw:
      msg = std::format(L"Команда libConv_Exp_cdw\nКонвертация {}файла чертежа Компаса {}\nв формат (*.mycdw) {}",
                        !inputFile.empty() ? L"" : L"текущего ", !inputFile.empty() ? inputFile : L"", outFile);
      res = 1;
      break;

    case libConv_Exp_spw:
      msg = std::format(L"Команда libConv_Exp_spw\nКонвертация {}файла спецификации Компаса {}\nв формат (*.myspw) {}",
                        !inputFile.empty() ? L"" : L"текущего ", !inputFile.empty() ? inputFile : L"", outFile);
      res = 1;
      break;

    case libConv_Exp_kdw:
      msg = std::format(L"Команда libConv_Exp_kdw\nКонвертация {}текстового файла Компаса {}\nв формат (*.mykdw) {}",
                        !inputFile.empty() ? L"" : L"текущего ", !inputFile.empty() ? inputFile : L"", outFile);
      res = 1;
      break;

    case libConv_Imp_m3d:
      msg = std::format(L"Команда libConv_Imp_m3d\nКонвертация файла (*.mym3d)\n {} ->\n{}  ", inputFile,
                        !outFile.empty() ? outFile : L"в новый документ.");
      res = 1;
      break;

    case libConv_Imp_a3d:
      msg = std::format(L"Команда libConv_Imp_a3d\nКонвертация файла (*.mya3d)\n {} ->\n{}  ", inputFile,
                        !outFile.empty() ? outFile : L"в новый документ.");
      res = 1;
      break;

    case libConv_Imp_t3d:
      msg = std::format(L"Команда libConv_Imp_t3d\nКонвертация файла (*.myt3d)\n {} ->\n{}  ", inputFile,
                        !outFile.empty() ? outFile : L"в новый документ.");
      res = 1;
      break;

    case libConv_Imp_frw:
      msg = std::format(L"Команда libConv_Imp_frw\nКонвертация файла (*.myfrw)\n {} ->\n{}  ", inputFile,
                        !outFile.empty() ? outFile : L"в новый документ.");
      res = 1;
      break;

    case libConv_Imp_cdw:
      msg = std::format(L"Команда libConv_Imp_cdw\nКонвертация файла (*.mycdw)\n {} ->\n{}  ", inputFile,
                        !outFile.empty() ? outFile : L"в новый документ.");
      res = 1;
      break;

    case libConv_Imp_spw:
      msg = std::format(L"Команда libConv_Imp_spw\nКонвертация файла (*.myspw)\n {} ->\n{}  ", inputFile,
                        !outFile.empty() ? outFile : L"в новый документ.");
      res = 1;
      break;

    case libConv_Imp_kdw:
      msg = std::format(L"Команда libConv_Imp_kdw\nКонвертация файла (*.mykdw)\n {} ->\n{}  ", inputFile,
                        !outFile.empty() ? outFile : L"в новый документ.");
      res = 1;
      break;

    default:
      break;
  }

  msg += std::format(L"\nСвойтсво \"Стиль линий\" установлено как - {}", m_saveLineStyle ? L"сохранять." : L"не сохранять.");
  LibMessage(msg, ksMessageInformation, ksButtonSetOk);

  return res;
}


//------------------------------------------------------------------------------
/**
  Проверить, можно ли выгрузить библиотеку из памяти.
*/
//---
bool LibConverter::GetCanUnLoadLibrary()
{
  return m_pConverterParam ? false : true;
}


//------------------------------------------------------------------------------
/**
  Выполнить команду из меню.
*/
//---
void LibConverter::ExecuteCommand(int32_t libMenuCommand)
{
  if (libMenuCommand == 0 || !kompasApp)
    return;

  int32_t converterCommand = -1;

  // Команда "сохранить файл"
  if (libMenuCommand == 1)
  {
    std::wstring filter;
    ksapi::IKompasDocumentPtr currentDoc;
    if (currentDoc = kompasApp->GetActiveDocument())
      filter = GetFilter(currentDoc->GetDocumentType(), true /*saveAs*/, converterCommand);

    if (filter.empty())
    {
      LibMessage(!currentDoc ? IDS_ERR_DOC : IDS_ERROR_DOC_TYPE, ksMessageError, ksButtonSetOk);
      return;
    }

    std::wstring outFile = kompasApp->GetApplicationDialogs()->SaveFile(nullptr /*windowHandler*/, L"" /*defaultExtention*/,
                                                                        filter, L"" /*oldFileName*/, false /*prevew*/);
    if (!outFile.empty())
      Convert(L"" /*inputFile*/, outFile, static_cast<LibConverterCommands>(converterCommand), true /*showParam*/);

    return;
  }

  // Команда "открыть файл"

  // Получаем фильтр (все поддерживаемые)
  std::wstring filter = GetFilter(-1 /*docType*/, false /*saveAs*/, converterCommand);
  std::wstring inFile = kompasApp->GetApplicationDialogs()->ChoiceFile(nullptr /*windowsHandle*/, L"" /*defaultExt*/, filter,
                                                                       L"" /*startDirectory*/, ksCurrentProfiles,
                                                                       false /*preview*/);
  if (!inFile.empty())
  {
    // Получаем команду уже для выбранного файла и конвертируем его.
    GetFilter(FindExt(inFile), false /*saveAs*/, converterCommand);
    Convert(inFile, L"" /*outFile*/, static_cast<LibConverterCommands>(converterCommand), true /*showParam*/);
  }
}


//------------------------------------------------------------------------------
/**
  Получить параметры библиотечного конвертера. Создаются при первом вызове.
  \details Т.к. интерфейс параметров один для всех команд - номер команды не обрабатывается.
*/
//---
ILibConverterParamPtr LibConverter::GetConvertParam(int32_t command)
{
  if (!m_pConverterParam)
    m_pConverterParam = new LibConverterParam();

  return m_pConverterParam;
}

//------------------------------------------------------------------------------
/**
  Вернуть фильтр расширений и команду конвертера для его обработки по типу документа.
  \param[in] docType - тип документа из перечисления DocumentTypeEnum. -1 - вернуть фильтр для всех документов.
  \param[in] saveAs - флаг, необходимый для определения типа команды (true - сохранение(экспорт), false - загрузка(импорт))
  \param[out] command - команда из перечисления LibConverterCommands, либо -1 - если не определена.
  \return Строка фильтра расширений документов для использования в диалогах.
*/
//---
std::wstring LibConverter::GetFilter(int32_t docType, bool saveAs, int32_t & command)
{
  if (command == 0)
    return {};

  std::wstring filter;

  switch (docType)
  {
    case ksDocumentPart:
      filter = LoadResourceString(IDS_DOC_M3D); // "Файлы моделей(*.mym3d)|*.mym3d|"
      command = saveAs ? libConv_Exp_m3d : libConv_Imp_m3d;
      break;

    case ksDocumentAssembly:
      filter = LoadResourceString(IDS_DOC_A3D); // "Файлы сборок(*.mya3d)|*.mya3d|"
      command = saveAs ? libConv_Exp_a3d : libConv_Imp_a3d;
      break;

    case ksDocumentDrawing:
      filter = LoadResourceString(IDS_DOC_CDW); // "Файлы чертежей(*.mycdw)|*.mycdw|"
      command = saveAs ? libConv_Exp_cdw : libConv_Imp_cdw;
      break;

    case ksDocumentFragment:
      filter = LoadResourceString(IDS_DOC_FRW); // "Файлы фрагментов(*.myfrw)|*.myfrw|";
      command = saveAs ? libConv_Exp_frw : libConv_Imp_frw;
      break;

    case ksDocumentSpecification:
      filter = LoadResourceString(IDS_DOC_SPW); // "Файлы спецификаций(*.myspw)|*.myspw|";
      command = saveAs ? libConv_Exp_spw : libConv_Imp_spw;
      break;

    case ksDocumentTextual:
      filter = LoadResourceString(IDS_DOC_KDW); // "Файлы текстовых документов(*.mykdw)|*.mykdw|";
      command = saveAs ? libConv_Exp_kdw : libConv_Imp_kdw;
      break;

    case ksDocumentTechnologyAssembly:
      filter = LoadResourceString(IDS_DOC_T3D); // "Файлы моделей(*.myt3d)|*.myt3d|"
      command = saveAs ? libConv_Exp_t3d : libConv_Imp_t3d;
      break;

    default:
      filter = LoadResourceString(IDS_DOC_M3D) + LoadResourceString(IDS_DOC_A3D) + LoadResourceString(IDS_DOC_CDW) +
               LoadResourceString(IDS_DOC_FRW) + LoadResourceString(IDS_DOC_SPW) + LoadResourceString(IDS_DOC_KDW) +
               LoadResourceString(IDS_DOC_T3D);
      command = -1;
      break;
  }

  return filter;
}


//------------------------------------------------------------------------------
/**
  Получить фильтр и номер команды по расширению документа.
  \details В KsAPI нет аналогичной перегрузки метода получения фильтра. Метод используется только при обработке команд из меню.
           Фактически, метод определяет тип документа по расширению и перевызывает перегрузку по типу документа.
  \param[in] ext - расширение документа.
  \param[in] saveAs - флаг, необходимый для определения типа команды (true - сохранение(экспорт), false - загрузка(импорт))
  \param[out] command - команда из перечисления LibConverterCommands, либо -1 - если не определена.
  \return Строка фильтра расширений документов для использования в диалогах.
*/
//---
std::wstring LibConverter::GetFilter(const std::wstring & ext, bool saveAs, int32_t & command)
{
  int32_t docType = -1;
  if (ext == FindExt(LoadResourceString(IDS_DOC_M3D)))
    docType = ksDocumentPart;
  else if (ext == FindExt(LoadResourceString(IDS_DOC_A3D)))
    docType = ksDocumentAssembly;
  else if (ext == FindExt(LoadResourceString(IDS_DOC_CDW)))
    docType = ksDocumentDrawing;
  else if (ext == FindExt(LoadResourceString(IDS_DOC_FRW)))
    docType = ksDocumentFragment;
  else if (ext == FindExt(LoadResourceString(IDS_DOC_SPW)))
    docType = ksDocumentSpecification;
  else if (ext == FindExt(LoadResourceString(IDS_DOC_KDW)))
    docType = ksDocumentTextual;
  else if (ext == FindExt(LoadResourceString(IDS_DOC_T3D)))
    docType = ksDocumentTechnologyAssembly;

  return GetFilter(docType, saveAs, command);
}


//------------------------------------------------------------------------------
/**
  Установить признак сохранения стиля линий (вызывается через интерфейс параметров).
*/
//---
void LibConverter::SetSaveLineStyle(bool val)
{
  m_saveLineStyle = val;
}


//------------------------------------------------------------------------------
/**
  Получить признак сохранения стиля линий (вызывается через интерфейс параметров).
*/
//---
bool LibConverter::GetSaveLineStyle()
{
  return m_saveLineStyle;
}

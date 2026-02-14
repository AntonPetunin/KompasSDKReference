////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////

#include "Step4_KsAPI_2D.h"
#include "Resource.h"

#include <filesystem>
#include <algorithm>
#include <filesystem>
#include <math.h>
#include <ranges>

#ifdef _MSC_VER
#include <winspool.h>
#else
//#include <caps/caps.h>
#endif

namespace fs = std::filesystem;
ksapi::IApplication * appl = nullptr; ///< Приложение

//------------------------------------------------------------------------------
/**
  Задать идентификатор ресурсов
*/
//---
unsigned int GetMenuId()
{
  return IDR_LIBID;
}


/// Пример работы с интерфейсом задания на печать
void PrintSample();
/// Пример работы с интерфейсом настроек принтера
void PrintSettingsSample();
/// Создание и сохранение конфигурации
void CreateAndSaveConfig();
/// Загрузка конфигурации
void LoadConfig();


//------------------------------------------------------------------------------
/**
  Головная функция библиотеки
*/
//---
void RunCommand(int32_t commandId, ksapi::ksRunCommandModeEnum mode)
{
  switch (commandId)
  {
    case 1:
      PrintSample();
      break;
    case 2:
      PrintSettingsSample();
      break;
    case 3:
      CreateAndSaveConfig();
      break;
    case 4:
      LoadConfig();
      break;
  }
}

extern void LoadResources(ksapi::IApplication & app);

//------------------------------------------------------------------------------
/**
  Получить номер версии приложения.
*/
//---
void GetApplicationVersion(int32_t & major, int32_t & minor, int32_t & release, int32_t & build)
{
  major = BuildVersionMajor;
  minor = BuildVersionMinor;
  release = BuildVersionPatch;
  build = BuildVersionBuild;
}


//-------------------------------------------------------------------------------
/**
  Инициализация библиотеки.
*/
//---
APP_EXP_FUNC(bool) LoadKompasLibrary(ksapi::IApplication & app, ksapi::IKompasLibraryActions & libaryActions)
{
  libaryActions.AddGetApplicationVersion(GetApplicationVersion);
  libaryActions.AddRunCommandHandler(RunCommand);
  libaryActions.AddGetMenuIdHandler(GetMenuId);

  appl = &app;
  LoadResources(app);

  return true;
}


//------------------------------------------------------------------------------
/**
  Освобождения ресурсов перед выгрузкой библиотеки.
*/
//---
APP_EXP_FUNC(void) UnloadKompasLibrary()
{
}


//------------------------------------------------------------------------------
/**
  Рассчитать масштаб для вывода
*/
//---
void CalcScale(ksapi::IPrintJobPtr & print)
{
  if (print)
  {
    double pageWidth, pageHeight;
    // Определяем габариты страницы устройства печати
    print->GetPageGabarites(pageWidth, pageHeight);

    for (std::int32_t i = 0, count = print->GetSheetsCount(); i < count; i++)
    {
      // Получаем лист документа
      if (ksapi::IPrintJobSheetPtr sheet{print->GetSheet(i)})
      {
        double w, h;
        // Определяем габариты листа
        sheet->GetGabarites(w, h);

        // Вычисляем масштаб вывода
        if (pageWidth > w)
          sheet->SetScale(1);
        else
          sheet->SetScale(pageWidth / w);

        // Размещаем лист в начале страницы
        sheet->SetX(pageWidth * i);
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Полное имя файла
*/
//---
bool GetFullName(const std::wstring & inName, std::wstring & outName)
{
  bool res = false;

  outName = L"";

  if (ksapi::ILibraryManagerPtr libMng = appl ? appl->GetLibraryManager() : nullptr)
  {
    if (ksapi::IProceduresLibrariesPtr procLibs = libMng->GetProceduresLibraries())
    {
      if (ksapi::IProceduresLibraryPtr lib = procLibs->GetItemByName(L"Step4_KsAPI_2D"))
        outName = lib->GetFullPath();
    }
  }

  if (!outName.empty())
  {
    auto fullName = std::filesystem::path(outName).parent_path();
    res = std::filesystem::exists(fullName.append(inName));

    if (!res)
      fullName.append(L"Step4_KsAPI_2D").append(inName);

    if (res = std::filesystem::exists(fullName))
      outName = fullName.wstring();
  }

  return res;
}


//------------------------------------------------------------------------------
/**
  Пример работы с интерфейсом задания на печать
*/
//---
void PrintSample()
{
  // Получаем интерфейс задания на печать
  if (ksapi::IPrintJobPtr print{appl ? appl->GetPrintJob() : nullptr})
  {
    std::wstring fileName;

    if (GetFullName(LoadStr(IDS_DRAWING_1), fileName))
      // Добавляем документ дважды (все листы)
      print->AddSheetsByRange(fileName, L"", ksAllSheets);

    // Индексы удаляемых листов
    std::vector<std::int32_t> indices(4);
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 4;
    indices[3] = 5;

    // Удаляем листы
    print->RemoveSheets(indices);
    // Рассчитываем масштаб
    CalcScale(print);
    // Выводим окно предварительного просмотра
    print->ShowPreviewWindow();
  }
}


//------------------------------------------------------------------------------
/**
  Пример работы с интерфейсом настроек принтера
*/
//---
void PrintSettingsSample()
{
  // Получаем интерфейс задания на печать
  if (ksapi::IPrintJobPtr print{appl ? appl->GetPrintJob() : nullptr})
  {
    std::wstring sheetPath;

    // Добавляем документ
    if (GetFullName(LoadStr(IDS_DRAWING_1), sheetPath))
      print->AddSheetsByRange(sheetPath, L"", ksAllSheets);

    // Индексы удаляемых листов
    std::vector<std::int32_t> indices(3);
    indices[0] = 1;
    indices[1] = 2;
    indices[2] = 3;

    // Удаляем листы
    print->RemoveSheets(indices);

    // Получаем интерфейс настроек принтера
    if (ksapi::IPrintJobPrinterSettingsPtr printSettings{print})
    {
      std::wstring configuration;
      bool res = GetFullName(LoadStr(IDS_CONFIG), configuration);

      // Загружаем конфигурацию принтера
      if (res && printSettings->LoadPrinterConfig(configuration))
      {
        // Получаем интерфейс листа документа
        if (ksapi::IPrintJobSheetPtr sheet{print->GetSheet(0)})
        {
          double w, h, scale;
          // Получаем габариты листа
          sheet->GetGabarites(w, h);
          // Получаем масштаб листа
          scale = sheet->GetScale();
          // Подгоняем размер бумаги под размер листа
          printSettings->SetPaperWidth(static_cast<std::int32_t>(floor(10 * w * scale)));
          printSettings->SetPaperLength(static_cast<std::int32_t>(floor(10 * h * scale)));
          // Выводим окно предварительного просмотра
          print->ShowPreviewWindow();
        }
      }
      else
      {
        appl->ShowMessageBox(LoadStr(IDS_LOAD_CFG_ERR), LoadStr(IDR_LIBID), ksMessageError, ksButtonSetOk, false);
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Сравнить строки
*/
//---
bool EqualsStrings(const std::wstring & name1, const std::wstring & name2)
{
  std::wstring lowerName1;
  std::wstring lowerName2;

  std::ranges::copy(name1 | std::views::transform(towlower), std::back_inserter(lowerName1));
  std::ranges::copy(name2 | std::views::transform(towlower), std::back_inserter(lowerName2));

  return !lowerName1.compare(lowerName2);
}


//------------------------------------------------------------------------------
/**
  Создание и сохранение конфигурации
*/
//---
void CreateAndSaveConfig()
{
  // Получаем интерфейс задания на печать
  ksapi::IPrintJobPtr print(appl ? appl->GetPrintJob() : nullptr);

  if (print)
  {
    std::wstring sheetPath;

    if (GetFullName(LoadStr(IDS_DRAWING_2), sheetPath))
      // Добавляем документ
      print->AddSheetsByRange(sheetPath, L"", ksAllSheets);

    // Получаем интерфейс настроек принтера
    if (ksapi::IPrintJobPrinterSettingsPtr printSettings{print})
    {
      unsigned long needBufLen = 0;
      unsigned long foundCount = 0;
      bool hasPrinter = false;
      std::wstring printerName;

#ifdef _MSC_VER
      // Определяем размер буфера
      ::EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL /*all printers*/, 2, NULL, 0 /*buffer length*/,
                     &needBufLen, &foundCount);

      if (needBufLen)
      {
        LPPRINTER_INFO_2 pBuff = (LPPRINTER_INFO_2)::malloc(needBufLen);

        // Получаем структуру информации о принтерах
        if (::EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL /*all printers*/, 2, (LPBYTE)pBuff, needBufLen,
                           &needBufLen, &foundCount) &&
            foundCount)
        {
          // Получаем первый принтер из списка
          printerName = pBuff->pPrinterName;

          // Этот принтер не позволяет задавать нестандартный размер бумаги, берем следующий
          if (EqualsStrings(printerName, L"Microsoft XPS Document Writer"))
          {
            LPPRINTER_INFO_2 pPrintInfo = pBuff;
            ++pPrintInfo;
            printerName = pPrintInfo->pPrinterName;
          }
          // Задаем параметры печати
          printSettings->InitPrinterSettings(printerName, false, 0 /*пользовательский*/, 1500, 1000, 0);

          std::wstring conf1Path;

          if (GetFullName(LoadStr(IDS_CONFIG_1), conf1Path))
            // Сохраняем конфигурацию
            printSettings->SavePrinterConfig(conf1Path);

          // Меняем размер бумаги
          printSettings->SetPaperLength(1600);
          printSettings->SetPaperWidth(1900);

          std::wstring conf2Path;

          if (GetFullName(LoadStr(IDS_CONFIG_2), conf2Path))
            // Сохраняем конфигурацию
            printSettings->SavePrinterConfig(conf2Path);

          // Меняем ориентацию бумаги
          bool isPortrait = printSettings->IsPortraitPage();
          printSettings->SetPortraitPage(!isPortrait);
          // Выводим окно предварительного просмотра
          print->ShowPreviewWindow();
        }

        ::free(pBuff);
      }
#endif
    }
  }
}


//------------------------------------------------------------------------------
/**
  Загрузка конфигурации
*/
//---
void LoadConfig()
{
  // Получаем интерфейс задания на печать
  if (ksapi::IPrintJobPtr print{appl ? appl->GetPrintJob() : nullptr})
  {
    std::wstring sheetPath;

    // Добавляем документ
    if (GetFullName(LoadStr(IDS_DRAWING_2), sheetPath))
      print->AddSheetsByRange(sheetPath, L"", ksAllSheets);

    // Получаем интерфейс настроек принтера

    if (ksapi::IPrintJobPrinterSettingsPtr printSettings{print})
    {
      std::wstring conf1Path;
      bool res = GetFullName(LoadStr(IDS_CONFIG_1), conf1Path);

      // Загружаем сохраненную конфигурацию
      if (res && printSettings->LoadPrinterConfig(conf1Path))
      {
        // Меняем размер бумаги
        printSettings->SetPaperLength(1800);
        printSettings->SetPaperWidth(1200);
        // Меняем ориентацию бумаги
        bool isPortrait = printSettings->IsPortraitPage();
        printSettings->SetPortraitPage(!isPortrait);

        std::wstring conf3Path;
        // Сохраняем конфигурацию
        if (GetFullName(LoadStr(IDS_CONFIG_3), conf3Path))
          printSettings->SavePrinterConfig(conf3Path);

        // Выводим окно предварительного просмотра
        print->ShowPreviewWindow();
      }
    }
  }
}

extern void * GetModule();

//------------------------------------------------------------------------------
/**
  Загрузить строку из ресурса
*/
//---
std::wstring LoadStr(int strID)
{
  std::wstring resStr;

  if (appl)
  {
    if (ksapi::ILibraryManagerPtr libMng = appl->GetLibraryManager())
    {
      if (ksapi::IProceduresLibrariesPtr procLibs = libMng->GetProceduresLibraries())
      {
        if (ksapi::IProceduresLibraryPtr procLib = procLibs->GetItemByName(L"Step4_KsAPI_2D"))
        {
          if (ksapi::IKompasDictionaryPtr dictionary = procLib->GetDictionary())
            return dictionary->LoadAndConvertLangString(GetModule(), strID);
        }
      }
    }
  }

  return resStr;
}

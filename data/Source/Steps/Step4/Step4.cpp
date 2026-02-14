////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////

#include "Step4_pch.h"

#include <format>
#include "Resource.h"

#include <filesystem>
#include <ranges>

#include "Step4.h"
#include "CPropMen.h"
#include "APIDraw.h"
#include "DataBase.h"
#include "Doc2DDispatcher.h"


ksapi::IApplication * ksAppl = nullptr;
extern void * GetModule();

//------------------------------------------------------------------------------
/**
  Задать идентификатор ресурсов
*/
//---
unsigned int GetMenuId()
{
  return IDR_LIBID;
}


//------------------------------------------------------------------------------
/**
  Загрузить строку из ресурса
*/
//---
std::wstring LoadStr(std::int32_t strID)
{
  std::wstring res;

  if (ksapi::ILibraryManagerPtr libraryManager = ksAppl->GetLibraryManager())
  {
    if (ksapi::IProceduresLibrariesPtr proceduresLibraries = libraryManager->GetProceduresLibraries())
    {
      if (ksapi::IProceduresLibraryPtr lib = proceduresLibraries->GetItemByName(L"Step4"))
      {
        if (ksapi::IKompasDictionaryPtr dictionary = lib->GetDictionary())
          return dictionary->LoadAndConvertLangString(GetModule(), strID);
      }
    }
  }

  return std::wstring();
}

namespace fs = std::filesystem;

//------------------------------------------------------------------------------
/**
  Получить полный путь по источнику и относительному пути
*/
//---
fs::path GetAbsolutePathFromRelative(const std::wstring & sourcePath, const std::wstring & relativePath)
{
  auto parts = std::wstring(relativePath) | std::views::split('\\') |
               std::views::transform([](auto r) { return std::wstring(r.data(), r.size()); });

  auto iter = parts.begin();
  std::size_t lastIndex = sourcePath.size() - 1;

  while (iter != parts.end())
  {
    if (*iter != L"..")
      break;

    lastIndex = sourcePath.find_last_of(L"\\", lastIndex - 1);
    iter++;
  }

  fs::path fullPath;

  if (iter == parts.begin())
  {
    fullPath = fs::path(sourcePath).parent_path();
    fullPath.append(relativePath);
  }
  else
  {
    fullPath.append(sourcePath.begin(), sourcePath.begin() + lastIndex);
    fullPath.append(relativePath.begin() + relativePath.find(*iter), relativePath.end());
  }

  return fullPath;
}


/// Работа с текстовой базой данных
void DrawTxtDB();
/// Выбор имени файла
void SelectFileName();
/// Пример с обработкой очереди сообщений
void TaskAccess();
/// Отрисовать слайд
void WorkRelativePath();
/// Работа с относительными путями файлов
void ShowSlide();


//------------------------------------------------------------------------------
/**
  Головная функция библиотеки
*/
//---
void RunCommand(unsigned int commandId, ksapi::ksRunCommandModeEnum mode)
{
  switch (commandId)
  {
    // Работа с БД
    case IDC_1_MENU:
    {
      DrawTxtDB();
      break;
    }
    // Выбор имени файла
    case IDC_3_MENU:
    {
      SelectFileName();
      break;
    }
    // Ввод длинного целого
    case IDC_4_MENU:
    {
      std::int32_t index;

      // Ввод длинного целого
      if (ksAppl && ksAppl->GetApplicationDialogs()->ReadInt(nullptr, LoadStr(ENTER_INDEX), 10000, -100000l, 100000l, index))
        LibMessage(std::format(L"{} {}", LoadStr(INDEX_OUT), index));
      else
        LibMessage(LoadStr(REJECTION));

      break;
    }
    // Работа с относительными путями файлов
    case IDC_7_MENU:
    {
      WorkRelativePath();
      break;
    }

    default:
    {
      if (ksapi::IKompasDocument2DPtr doc = ksAppl->GetActiveDocument())
      {
        switch (commandId)
        {
          case IDC_2_MENU:
          {
            APIDraw * Api7 = new APIDraw();

            if (Api7->InitProcessParam())
              Api7->ChoicePlacement();

            break;
          }
          // Показ слайда
          case IDC_5_MENU:
          {
            ShowSlide();
            break;
          }
          // Обработка сообщений
          case IDC_6_MENU:
          {
            TaskAccess();
            break;
          }
        }
      }
      else
        LibMessage(LoadStr(DOCUMENT_ERROR));

      break;
    }
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


//------------------------------------------------------------------------------
/**
  Инициализация библиотеки.
*/
//---
APP_EXP_FUNC(bool) LoadKompasLibrary(ksapi::IApplication & app, ksapi::IKompasLibraryActions & libraryActions)
{
  libraryActions.AddGetApplicationVersion(GetApplicationVersion);
  libraryActions.AddRunCommandHandler(RunCommand);
  libraryActions.AddGetMenuIdHandler(GetMenuId);

  ksAppl = &app;
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
  Работа с текстовой базой данных
  Данные сформированы в виде таблицы обнородных строк, колонки
  разделены  пробелами или запятыми
*/
//---
void DrawTxtDB()
{
  std::wstring fileName =
    ksAppl->GetApplicationDialogs()->ChoiceFile(nullptr, L"*.db", LoadStr(SQLITE_FILES_FILTER), L"", ksWorkFiles, 0);

  // Интерактивный выбор файла, возвращает реальную длину имени файла, либо 0 вслучае отказа
  if (!fileName.empty())
  {
    // Cвязать объект базы с определенной базой данных, для текстового файла - имя файла
    if (DataBase * db = DataBase::GetDBConnection(fileName))
    {
      // Пример для считывания всех записей из базы в структуру record
      // Структура для считывания информации из базы
      struct Record
      {
        short int index;
        double a, b;
      };

      // Cоздать отношение - буфер для считывания информации из таблицы
      // В нашем случае это два double и один std::int32_t, при этом не важно сколько колонок
      // В базе, важно чтобы их было >= описанных в Relation
      auto scanRows = [](void * result, int count, char ** rowData, char **)
      {
        if (std::vector<Record> * records = reinterpret_cast<std::vector<Record> *>(result))
        {
          Record resRecord;
          resRecord.index = std::stoi(rowData[0]);
          resRecord.a = std::stod(rowData[1]);
          resRecord.b = std::stod(rowData[2]);
          records->emplace_back(resRecord);
        }

        return 0;
      };

      std::vector<Record> records;
      db->ExecuteQuery("SELECT id, A, B FROM TEST_DB", scanRows, &records);

      if (!records.empty())
      {
        // Считывание данных из базы
        for (const auto & record : records)
          LibMessage(std::format(L"a = {:.1f}, b = {:.1f}, index = {}", record.a, record.b, record.index));

        LibMessage(LoadStr(SAMPLE_END));
      }

      std::vector<std::wstring> values = db->GetWStringColumnValues("SELECT TEST_DB.C FROM TEST_DB", 0);

      if (!values.empty())
      {
        for (std::wstring value : values)
          // Считать очередную порцию информации и положть в buf
          LibMessage(value);

        LibMessage(LoadStr(SAMPLE_END));
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Выбор имени файла
*/
//---
void SelectFileName()
{
  std::wstring selectedFile;

  if (ksAppl)
  {
    ksapi::IApplicationDialogsPtr dialogs = ksAppl->GetApplicationDialogs();

    if (!dialogs)
      return;

    std::wstring filter = LoadStr(DOCUMENT2D_FILTER);
    selectedFile = dialogs->ChoiceFile(nullptr, L"*.cdw", filter, L"", ksWorkFiles, true);
  }

  if (!selectedFile.empty())
    LibMessage(std::format(L"Путь к файлу: {}\nРеальный размер строки {} символов", selectedFile, selectedFile.length()));
  else
    LibMessage(LoadStr(REJECTION));
}


//------------------------------------------------------------------------------
/**
  Пример с обработкой очереди сообщений
*/
//---
void TaskAccess()
{
  ksAppl->SetEnableTaskAccess(false); // Запретили доступ к задаче
  Doc2DDispatcher dispatcher;

  for (std::int32_t i = 0; i < 5000; i++)
  {
    dispatcher.CreateLineSegment(10, 10 + i, 20, 10 + i, 1);

    if (!(i % 10))
    {
      // Через каждые 100 отрезков обрабатываем очередь сообщений
      // При этом ОС получает возможность выполнить свои
      // действия, например переключиться на другую задачу
      ksAppl->PumpWaitingMessages();
    }
  }
  ksAppl->SetEnableTaskAccess(true); // Разрешили доступ к задаче
}


//------------------------------------------------------------------------------
/**
  Записать слайд
*/
//---
void WriteSlideStep()
{
  std::wstring name;

  if (ksAppl)
  {
    ksapi::IApplicationDialogsPtr dialogs = ksAppl->GetApplicationDialogs();

    if (!dialogs)
      return;

    name = dialogs->SaveFile(nullptr, L"*.rc", L"", L"", false);

    // Выберем файл для записи
    if (!name.empty())
    {
      // Точка привязки слайда - верхний левый угол габаритного прямоугольника слайда
      std::int32_t slideId;
      // Ввод целого числа
      if (dialogs->ReadInt(nullptr, LoadStr(ENTER_SLIDE_ID), 100, 0, 32000, slideId))
      {
        WindowHandle frameHandle = nullptr;

        if (ksapi::IKompasDocument2DPtr doc2D = ksAppl->GetActiveDocument())
        {
          ksapi::IDocumentFramePtr docFrame = doc2D->GetDocumentFrame();
          frameHandle = docFrame ? docFrame->GetWindowHandle() : nullptr;
        }

        // Записать слайд
        // TODO
        //if (WriteSlide)
        //  ErrorMessage(LoadStr(SELECTION_GROUP_IS_EMPTY));
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Отрисовать слайд
*/
//---
void ShowSlide()
{
  if (ksAppl)
  {
    ksapi::IKompasDocument2DPtr doc = ksAppl->GetActiveDocument();
    ksapi::IProcessPtr proc = doc ? doc->GetLibProcess(ksProcess2DPlacement) : nullptr;
    ksapi::IProcessParamPtr params = ksAppl ? ksAppl->CreateProcessParam() : nullptr;
    ksapi::IPropertyTabsPtr tabs = params ? params->GetPropertyTabs() : nullptr;
    ksapi::IPropertyTabPtr tab = tabs ? tabs->Add(L"SlideBox") : nullptr;
    ksapi::IPropertyControlsPtr collection = tab ? tab->GetPropertyControls() : nullptr;
    ksapi::IPropertySlideBoxPtr slideBox = collection ? collection->Add(ksControlSlideBox) : nullptr;

    if (slideBox)
    {
      slideBox->SetSlide(IDC_SLIDE, GetModule());
      params->SetSpecToolbar(pnEscHelp);
      proc->SetProcessParam(params);
      proc->Run(false, false);
    }
  }
}


//------------------------------------------------------------------------------
/**
  Работа с относительными путями файлов
*/
//---
void WorkRelativePath()
{
  if (!ksAppl)
    return;

  auto choiceFileWithoutFilters = []()
  {
    return ksAppl->GetApplicationDialogs()->ChoiceFile(nullptr, L"*.*", LoadStr(ALL_FILES), L"", ksWorkFiles, 0);
  };

  std::wstring mainName = choiceFileWithoutFilters();

  // Имя задающего файла
  if (!mainName.empty())
  {
    fs::path fileName = choiceFileWithoutFilters();

    // Требуемый файл
    if (!fileName.empty())
    {
      // Относительный путь. Если указан один и тот же файл - вывести его имя.
      std::wstring resName = fileName.compare(mainName) != 0 ? fs::relative(fileName, mainName).wstring()
                                                             : fileName.filename().wstring();

      std::wstring outMessage(LoadStr(FULL_PATH_TO_SETUP_FILE));
      outMessage.append(mainName)
        .append(L"\n")
        .append(LoadStr(FULL_PATH_TO_REQUIRED_FILE))
        .append(fileName.wstring())
        .append(L"\n")
        .append(LoadStr(RELATIVE_PATH_TO_REQUIRED_FILE))
        .append(resName);

      LibMessage(outMessage);

      fs::path fullName = GetAbsolutePathFromRelative(mainName, resName);

      outMessage = LoadStr(FULL_PATH_TO_SETUP_FILE);
      outMessage.append(mainName)
        .append(L"\n")
        .append(LoadStr(RELATIVE_PATH_TO_REQUIRED_FILE))
        .append(resName)
        .append(L"\n")
        .append(LoadStr(FULL_PATH_TO_REQUIRED_FILE))
        .append(fullName.wstring());

      LibMessage(outMessage);
    }
  }
}

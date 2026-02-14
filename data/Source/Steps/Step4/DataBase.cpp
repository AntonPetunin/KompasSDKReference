#include "Step4_pch.h"

#include <filesystem>
#include <memory>

#include "Doc2DDispatcher.h"
#include "DataBase.h"
#include <sqlite3.h>
#include "Step4.h"
#include "CPropMen.h"
#include "Resource.h"

#define MB_ICONWARNING 0x00000030L

const std::wstring dbName = L"Step4.db";
extern ksapi::IApplication * ksAppl;

namespace
{
//------------------------------------------------------------------------------
/**
  
*/
//---
std::wstring GetModuleFilePath()
{
  if (ksapi::ILibraryManagerPtr libraryManager = ksAppl ? ksAppl->GetLibraryManager() : nullptr)
  {
    if (ksapi::IProceduresLibrariesPtr proceduresLibraries = libraryManager->GetProceduresLibraries())
    {
      if (ksapi::IProceduresLibraryPtr library = proceduresLibraries->GetItemByName(LoadStr(IDR_LIBID)))
        return library->GetFullPath();
    }
  }

  return {};
}


//------------------------------------------------------------------------------
/**
  Преобразовать строку UTF16 в строку широких символов (разный размер для Linux и Windows, например, на Linux sizeof(wchar_t) = 4)
*/
//---
std::wstring UTF16ToWCHAR(const uint16_t * src)
{
  std::wstring res;

  while (*src)
    res += static_cast<wchar_t>(*src++);

  return res;
}


//------------------------------------------------------------------------------
/**
  возвращает полное имя файла
*/
//---
bool GetFullName(const std::wstring & inName, std::wstring & outName)
{
  bool res = false;
  outName = GetModuleFilePath();

  if (!outName.empty())
  {
    auto fullName = std::filesystem::path(outName).parent_path();
    res = std::filesystem::exists(fullName.append(inName));

    if (!res && ksAppl)
    {
      std::wstring outPath;

      if (ksapi::ISystemSettingsPtr settings = ksAppl->GetSystemSettings())
      {
        fullName = settings->GetSystemPath(ksSystemFiles);
        fullName.append(inName);
      }
    }

    if (res = std::filesystem::exists(fullName))
      outName = fullName.wstring();
  }

  return res;
}
} // namespace


//------------------------------------------------------------------------------
/**
  Получить соединение с базой данных
*/
//---
DataBase * DataBase::GetDBConnection(const std::wstring & pathToDB)
{
  static std::unique_ptr<DataBase> db;
  static std::wstring dbPath;

  if (!db || dbPath != pathToDB)
  {
    std::wstring outPath;
    dbPath = pathToDB.empty() && GetFullName(dbName.c_str(), outPath) ? outPath : pathToDB;

    if (!dbPath.empty())
      db = std::make_unique<DataBase>(GetStringFromWString(dbPath));
    else
      LibMessage(LoadStr(ID_DB_NOT_FOUND_ERROR), ksMessageInformation);
  }

  return db.get();
}


//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
DataBase::DataBase(const std::string & dbPath)
  : m_dbPath(dbPath)
{
}


//------------------------------------------------------------------------------
/**
  Окрыть соединение
*/
//---
bool DataBase::Open()
{
  return sqlite3_open_v2(m_dbPath.c_str(), &m_connection, SQLITE_OPEN_READONLY, NULL) == SQLITE_OK;
}


//------------------------------------------------------------------------------
/**
  Выполнить запрос к базе
*/
//---
bool DataBase::ExecuteQuery(std::string sqlQuery, CallBackFunction callback, void * result)
{
  if (Open() && m_connection)
  {
    char * messageError;
    std::int32_t status = sqlite3_exec(m_connection, sqlQuery.c_str(), callback, result, &messageError);

    if (status != SQLITE_OK)
    {
      LibMessage(LoadStr(ID_DB_ERROR).append(GetWStringFromString(messageError)), ksMessageInformation);
      sqlite3_free(messageError);
      return false;
    }

    Close();
    return true;
  }

  return false;
}


std::vector<std::wstring> DataBase::GetWStringColumnValues(std::string sqlQuery, std::int32_t colNum)
{
  std::vector<std::wstring> values;

  if (Open() && m_connection)
  {
    sqlite3_stmt * stmt;
    std::int32_t status = sqlite3_prepare_v2(m_connection, sqlQuery.c_str(), -1, &stmt, nullptr);

    if (status != SQLITE_OK)
      LibMessage(LoadStr(ID_DB_ERROR).append(GetWStringFromString(sqlite3_errmsg(m_connection))), ksMessageInformation);
    else
    {
      while (sqlite3_step(stmt) == SQLITE_ROW)
        values.push_back(UTF16ToWCHAR(static_cast<const uint16_t *>(sqlite3_column_text16(stmt, colNum))));
    }

    Close();
  }

  return values;
}

//------------------------------------------------------------------------------
/**
  Закрыть соединение
*/
//---
bool DataBase::Close()
{
  return sqlite3_close(m_connection) == SQLITE_OK;
}

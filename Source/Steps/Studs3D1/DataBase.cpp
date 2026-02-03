#include <codecvt>
#include <filesystem>
#include <memory>
#include <locale>

#include <KsAPI.h>

#include "DataBase.h"
#include <sqlite3.h>
#include "Resource.h"

using namespace ksapi;

const std::wstring dbName = L"Studs3D1.db";

extern IApplication * kompasApp;
extern void * GetModule();
extern int32_t LibMessage(int32_t strId, const std::wstring & appendMessage, ksMessageTypeEnum dlgType = ksMessageWarning,
                          ksMessageButtonSetEnum dlgButtons = ksButtonSetOk, bool showModal = false);
extern std::wstring GetModuleFilePath();


//------------------------------------------------------------------------------
/**
  Возвращает полное имя файла.
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

    if (!res && kompasApp)
    {
      std::wstring outPath;

      if (ISystemSettingsPtr settings = kompasApp->GetSystemSettings())
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


//------------------------------------------------------------------------------
/**
  Конвертировать строку из std::string в std::wstring
*/
//---
std::wstring GetWStringFromString(const std::string & str)
{
#pragma warning(push)
#pragma warning(disable : 4996)
  std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
  return myconv.from_bytes(str);
#pragma warning(pop)
}


//------------------------------------------------------------------------------
/**
  Конвертировать строку из std::wstring в std::string
*/
//---
std::string GetStringFromWString(const std::wstring & wstr)
{
#pragma warning(push)
#pragma warning(disable : 4996)
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  return converter.to_bytes(wstr.c_str());
#pragma warning(pop)
}


//------------------------------------------------------------------------------
/**
  Получить соединение с базой данных
*/
//---
DataBase * DataBase::GetDBConnection()
{
  static std::unique_ptr<DataBase> db;

  if (!db)
  {
    std::wstring outPath;

    if (GetFullName(dbName, outPath))
    {
      std::wstring dbPath(outPath);
      db = std::make_unique<DataBase>(GetStringFromWString(dbPath));
    }
    else
    {
      // Выдать сообщение о ошибке
      LibMessage(ID_DB_NOT_FOUND_ERROR, L"");
    }
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
      // Выдать сообщение о ошибке
      LibMessage(ID_DB_ERROR, GetWStringFromString(messageError));
      sqlite3_free(messageError);
      return false;
    }

    Close();
    return true;
  }

  return false;
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

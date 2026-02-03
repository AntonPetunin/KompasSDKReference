#include "Gayka1_pch.h"

#include <filesystem>
#include <memory>

#include "Doc2DDispatcher.h"
#include "DataBase.h"
#include "ParameterObject.h"
#include <sqlite3.h>
#include "PropertyManagerObject.h"

const std::wstring dbName = L"ElementsData.db";

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

    if (GetFullName(dbName.c_str(), outPath))
    {
      std::wstring dbPath(outPath);
      db = std::make_unique<DataBase>(GetStringFromWString(dbPath));
    }
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


//------------------------------------------------------------------------------
/**
  Закрыть соединение
*/
//---
bool DataBase::Close()
{
  return sqlite3_close(m_connection) == SQLITE_OK;
}

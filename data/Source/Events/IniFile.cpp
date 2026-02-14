////////////////////////////////////////////////////////////////////////////////
//
// Настройки приложения.
//
////////////////////////////////////////////////////////////////////////////////
#include "Events_pch.h"
#include "IniFile.h"
#include "LibFunctions.h"
#include <stdexcept>
#include <locale>
#include <codecvt>
#include <array>

// Настройки приложения
extern AppConfig theApp;
std::unique_ptr<IniFile> ini;
//------------------------------------------------------------------------------
/**
  Выполнение команды
*/
//---
void ReadIni()
{
  ksapi::IProceduresLibraryPtr thisLib = GetLibrary();

  if (!thisLib)
    return;

  std::filesystem::path settingsPath = std::filesystem::path(thisLib->GetFullPath()).remove_filename() / L"Events.ini";

  ini.reset(new IniFile{settingsPath.wstring()});

  if (std::filesystem::exists(settingsPath))
  {
    std::wstring section = LoadResourceString(IDS_INI_SECTION_CFG);

    theApp.m_auto = ini->GetBoolValue(section, LoadResourceString(IDS_INI_KEY_AUTOADVISE), false);
    theApp.m_mes_2DDOC = ini->GetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_DOC2D), true);
    theApp.m_mes_3DDOC = ini->GetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_DOC3D), true);
    theApp.m_mes_APP = ini->GetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_APP), true);
    theApp.m_mes_DOC = ini->GetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_DOCS), true);
    theApp.m_mes_OBJ_2DDOC = ini->GetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_OBJDOC2D), true);
    theApp.m_mes_OBJ_3DDOC = ini->GetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_OBJDOC3D), true);
    theApp.m_mes_OBJ_SPC = ini->GetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_OBJSPC), true);
    theApp.m_mes_SELECT = ini->GetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_SELECT), true);
    theApp.m_mes_SPC_DESCR = ini->GetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_SPCDESCR), true);
    theApp.m_mes_SPCDOC = ini->GetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_SPCDOC), true);
    theApp.m_mes_STAMP = ini->GetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_STAMP), true);
    theApp.m_mes_DOC_FRAME = ini->GetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_DOCFRAME), true);
  }
  else
  {
    LibMessage(IDS_INI_NOT_EXISTS);
  }
}

//------------------------------------------------------------------------------
/**
  Выполнение команды
*/
//---
void WriteIni()
{
  if (!ini)
    return;

  std::wstring section = LoadResourceString(IDS_INI_SECTION_CFG);

  ini->SetBoolValue(section, LoadResourceString(IDS_INI_KEY_AUTOADVISE), theApp.m_auto);
  ini->SetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_DOC2D), theApp.m_mes_2DDOC);
  ini->SetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_DOC3D), theApp.m_mes_3DDOC);
  ini->SetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_APP), theApp.m_mes_APP);
  ini->SetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_DOCS), theApp.m_mes_DOC);
  ini->SetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_OBJDOC2D), theApp.m_mes_OBJ_2DDOC);
  ini->SetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_OBJDOC3D), theApp.m_mes_OBJ_3DDOC);
  ini->SetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_OBJSPC), theApp.m_mes_OBJ_SPC);
  ini->SetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_SELECT), theApp.m_mes_SELECT);
  ini->SetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_SPCDESCR), theApp.m_mes_SPC_DESCR);
  ini->SetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_SPCDOC), theApp.m_mes_SPCDOC);
  ini->SetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_STAMP), theApp.m_mes_STAMP);
  ini->SetBoolValue(section, LoadResourceString(IDS_INI_KEY_MESS_DOCFRAME), theApp.m_mes_DOC_FRAME);

  ini->Save();
}

//------------------------------------------------------------------------------
/**
  Конструктор.
*/
//---
void Trim(std::wstring & str)
{
  str.erase(0, str.find_first_not_of(L" \t\n\r\f\v"));
  str.erase(str.find_last_not_of(L" \t\n\r\f\v") + 1);
}


//------------------------------------------------------------------------------
/**
   
*/
//---
IniFile::IniFile(const std::wstring & filename)
  : m_filename(filename)
{
  Load();
}


//------------------------------------------------------------------------------
/**
   Прочитать файл.
*/
//---
void IniFile::Load()
{
  std::wifstream file(m_filename);
#pragma warning(push)
#pragma warning(disable : 4996)
  file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));
#pragma warning(pop)

  if (!file.is_open())
  {
    LibMessage(IDS_INI_READ_FAIL);
    return;
  }

  std::wstring line;
  Section * currentSection = nullptr;
  while (std::getline(file, line))
  {
    Trim(line);
    if (line.empty() || line[0] == L';' || line[0] == L'#')
    {
      // Пропускаем комментарии и пустые строки
      continue;
    }

    size_t sectionOpen = line.find(L'[');
    size_t sectionClose = line.find(L']');
    if (sectionOpen != std::wstring::npos && sectionClose != std::wstring::npos)
    {
      std::wstring sectionName = line.substr(sectionOpen + 1, sectionClose - sectionOpen - 1);
      Trim(sectionName);

      // Проверяем, существует ли секция
      auto it = std::find_if(m_sections.begin(), m_sections.end(), [&](const Section & s) { return s.name == sectionName; });

      if (it == m_sections.end())
      {
        // Создаем новую секцию
        m_sections.push_back({sectionName, {}, {}});
        currentSection = &m_sections.back();
      }
      else
      {
        currentSection = &(*it);
      }
    }
    else if (currentSection)
    {
      size_t equalsPos = line.find(L'=');
      if (equalsPos != std::wstring::npos)
      {
        std::wstring key = line.substr(0, equalsPos);
        std::wstring value = line.substr(equalsPos + 1);
        Trim(key);
        Trim(value);

        // Добавляем ключ, если его еще нет
        if (currentSection->keyValuePairs.find(key) == currentSection->keyValuePairs.end())
        {
          currentSection->keys.push_back(key);
        }
        currentSection->keyValuePairs[key] = value;
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Записать файл.
*/
//---
void IniFile::Save()
{
  std::wofstream file(m_filename, std::ios::out | std::ios::binary);
#pragma warning(push)
#pragma warning(disable : 4996)
  file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));
#pragma warning(pop)

  if (!file.is_open())
  {
    LibMessage(IDS_INI_WRITE_FAIL);
    return;
  }

  file.put(0xFEFF);

  for (const auto & section : m_sections)
  {
    file << L"[" << section.name << L"]" << std::endl;
    for (const auto & key : section.keys)
    {
      file << key << L"=" << section.keyValuePairs.at(key) << std::endl;
    }
    file << std::endl;
  }
}


//------------------------------------------------------------------------------
/**
   Получить значение в виде строки.
*/
//---
std::wstring IniFile::GetValue(const std::wstring & sectionName, const std::wstring & key, const std::wstring & defaultValue)
{
  auto sectionIt = std::find_if(m_sections.begin(), m_sections.end(), [&](const Section & s) { return s.name == sectionName; });

  if (sectionIt != m_sections.end())
  {
    const auto & keyValuePairs = sectionIt->keyValuePairs;
    auto keyIt = keyValuePairs.find(key);
    if (keyIt != keyValuePairs.end())
    {
      return keyIt->second;
    }
  }

  return defaultValue;
}


//------------------------------------------------------------------------------
/**
   Получить значение в виде булевого типа.
*/
//---
bool IniFile::GetBoolValue(const std::wstring & sectionName, const std::wstring & key, bool defaultValue)
{
  auto sectionIt = std::find_if(m_sections.begin(), m_sections.end(), [&](const Section & s) { return s.name == sectionName; });

  if (sectionIt != m_sections.end())
  {
    const auto & keyValuePairs = sectionIt->keyValuePairs;
    auto keyIt = keyValuePairs.find(key);
    if (keyIt != keyValuePairs.end())
    {
      try
      {
        return std::stoi(keyIt->second);
      }
      catch (const std::invalid_argument &)
      {
        return defaultValue;
      }
      catch (const std::out_of_range &)
      {
        return defaultValue;
      }
    }
  }

  return defaultValue;
}


//------------------------------------------------------------------------------
/**
   Записать значение в виде строки.
*/
//---
void IniFile::SetValue(const std::wstring & sectionName, const std::wstring & key, const std::wstring & value)
{
  auto sectionIt = std::find_if(m_sections.begin(), m_sections.end(), [&](const Section & s) { return s.name == sectionName; });

  if (sectionIt == m_sections.end())
  {
    // Создаем новую секцию, если она не существует
    m_sections.push_back({sectionName, {key}, {{key, value}}});
  }
  else
  {
    // Добавляем ключ, если его еще нет
    if (sectionIt->keyValuePairs.find(key) == sectionIt->keyValuePairs.end())
    {
      sectionIt->keys.push_back(key);
    }
    sectionIt->keyValuePairs[key] = value;
  }
}


//------------------------------------------------------------------------------
/**
   Записать значение в виде булевого типа.
*/
//---
void IniFile::SetBoolValue(const std::wstring & sectionName, const std::wstring & key, bool boolValue)
{
  std::wstring value = boolValue ? L"1" : L"0";

  auto sectionIt = std::find_if(m_sections.begin(), m_sections.end(), [&](const Section & s) { return s.name == sectionName; });

  if (sectionIt == m_sections.end())
  {
    // Создаем новую секцию, если она не существует
    m_sections.push_back({sectionName, {key}, {{key, value}}});
  }
  else
  {
    // Добавляем ключ, если его еще нет
    if (sectionIt->keyValuePairs.find(key) == sectionIt->keyValuePairs.end())
    {
      sectionIt->keys.push_back(key);
    }
    sectionIt->keyValuePairs[key] = value;
  }
}

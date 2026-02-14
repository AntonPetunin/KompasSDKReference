////////////////////////////////////////////////////////////////////////////////
//
// Настройки приложения.
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

void ReadIni();
void WriteIni();

class IniFile
{
private:
  struct Section
  {
    std::wstring name;
    std::vector<std::wstring> keys;                               // Порядок ключей
    std::unordered_map<std::wstring, std::wstring> keyValuePairs; // Ключи и их значения
  };

  // Порядок секций
  std::vector<Section> m_sections;
  std::wstring m_filename;

public:
  /// Конструктор.
  IniFile(const std::wstring & filename);
  /// Прочитать файл.
  void Load();
  /// Записать файл.
  void Save();
  /// Получить значение в виде строки.
  std::wstring GetValue(const std::wstring & sectionName, const std::wstring & key, const std::wstring & defaultValue = L"");
  /// Получить значение в виде булевого типа.
  bool GetBoolValue(const std::wstring & sectionName, const std::wstring & key, bool defaultValue = false);
  /// Записать значение в виде строки.
  void SetValue(const std::wstring & sectionName, const std::wstring & key, const std::wstring & value);
  /// Записать значение в виде булевого типа.
  void SetBoolValue(const std::wstring & sectionName, const std::wstring & key, bool value);
};

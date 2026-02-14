#pragma once
#include "Events_pch.h"

enum class EventType;

/// Загрузить строку из ресурсов по указанному идентификатору.
std::wstring LoadResourceString(int32_t strId);

/// Выдать сообщение по указанному идентификатору строки из ресурсов.
ksMessageButtonEnum LibMessage(int32_t ids, ksMessageTypeEnum dialogType = ksMessageWarning,
                               ksMessageButtonSetEnum dialogButtons = ksButtonSetOk, bool showModal = true);

/// Выдать сообщение с указанной строкой.
ksMessageButtonEnum LibMessage(const std::wstring & str, ksMessageTypeEnum dialogType = ksMessageWarning,
                               ksMessageButtonSetEnum dialogButtons = ksButtonSetOk, bool showModal = true);

ksMessageButtonEnum YesNoDialog(const std::wstring & explanation, const std::wstring & positiveButton,
                                const std::wstring & negativeButton, const std::wstring & cancelButton);

ksMessageButtonEnum YesNoCancelDialog(const std::wstring & question);

ksMessageButtonEnum YesNoDialog(const std::wstring & question);

ksapi::IKompasDocumentPtr CreateDocument(DocumentTypeEnum type);

void PrintKompasError();

std::wstring DocumentType(DocumentTypeEnum docType);

std::wstring SeparateStringVector(const std::vector<std::wstring> & vec);

/// Форматирование строки из ресурсов.
template<typename... Args>
std::wstring FormatResourceString(int32_t strId, Args... formatArgs)
{
  return std::vformat(LoadResourceString(strId), std::make_wformat_args(formatArgs...));
};

/// Временный каталог для файлов
std::wstring GetTempApi();
std::wstring GetTempApi(EventType eventType);

/// Форматирование путей
std::wstring GetFileName(const std::wstring path);

ksapi::IProceduresLibraryPtr GetLibrary();

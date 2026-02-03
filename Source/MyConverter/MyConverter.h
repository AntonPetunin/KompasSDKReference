#pragma once

#include <MyConverter_pch.h>

/// Выдать сообщение со строкой из ресурсов.
ksMessageButtonEnum LibMessage(int32_t strId, ksMessageTypeEnum dialogType, ksMessageButtonSetEnum dialogButtons);
/// Выдать сообщение с заданной строкой.
ksMessageButtonEnum LibMessage(const std::wstring & str, ksMessageTypeEnum dialogType, ksMessageButtonSetEnum dialogButtons);
// Загрузить строку из ресурсов.
std::wstring LoadResourceString(int32_t strId);
/// Найти в строке расширение файла и вернуть его.
std::wstring FindExt(const std::wstring & filename);

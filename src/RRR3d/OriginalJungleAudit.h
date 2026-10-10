#pragma once
#include "resource/ResourceFileSystem.h"
#include <optional>
namespace rrr3d::diagnostics
{
bool auditJungleStarts(const r3d::resource::ResourceFileSystem& resources);
bool auditAllMapAi(
    const r3d::resource::ResourceFileSystem& resources,
    std::optional<std::size_t> selectedTrack = std::nullopt);
}

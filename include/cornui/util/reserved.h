#pragma once

#include <string>

/**
 * @param tag Name of the tag.
 * @return Whether the tag is reserved and cannot be redefined.
 */
bool tagIsReserved(const std::string& tag) noexcept;

/**
 * @param name Name of the attribute.
 * @return Whether the attribute is reserved and cannot be set directly.
 */
bool attrIsReserved(const std::string& name) noexcept;

/**
 * @param name Name of the attribute.
 * @return Whether the attribute's value is a JS script.
 */
bool attrIsScript(const std::string& name) noexcept;

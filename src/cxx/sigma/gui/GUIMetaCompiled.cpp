#include "sigma/gui/GUIMetaCompiled.hpp"

namespace sigma
{
namespace gui
{
namespace meta_comp
{

const chaos::str::UTF8String logging(
    "{"
    "    \"profile\": \"Sigma-GUI\","
    "    \"outputs\":"
    "    {"
    "        \"StdOutput\":"
    "        {"
    "            \"enabled\": true,"
    "            \"verbosity_level\": \"debug\""
    "        },"
    "        \"FileOutput\":"
    "        {"
    "            \"enabled\": true,"
    "            \"verbosity_level\": \"debug\","
    "            \"base_path\": [\"logs\"]"
    "        }"
    "    }"
    "}"
);

} // namespace meta_comp
} // namespace gui
} // namespace sigma
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

const chaos::str::UTF8String resource_locations(
    "{"
    "    \"resource_path\": [\"res\"],"
    "    \"gui_resource_path\": [\"${resource_path}\", \"gui\"],"
    "    \"fonts_resource_path\": [\"${gui_resource_path}\", \"fonts\"]"
    "}"
);
const chaos::str::UTF8String fonts(
    "{"
    "    \"supported_formats\": [\"ttf\", \"otf\"]"
    "}"
);

} // namespace meta_comp
} // namespace gui
} // namespace sigma

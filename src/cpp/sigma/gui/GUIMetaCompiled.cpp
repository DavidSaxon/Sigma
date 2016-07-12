#include "sigma/gui/GUIMetaCompiled.hpp"

namespace sigma
{
namespace gui
{
namespace meta_comp
{

const arc::str::UTF8String logging(
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
    "            \"enabled\": false,"
    "            \"verbosity_level\": \"debug\","
    "            \"base_path\": [\"logs\"],"
    "            \"path_date_syntax\": \"%Y_%m_%d\","
    "            \"path_time_syntax\": \"%H_%M_%S\","
    "            \"file_extension\": \"log\""
    "        }"
    "    }"
    "}"
);

const arc::str::UTF8String resource_locations(
    "{"
    "    \"resource_path\": [\"res\"],"
    "    \"gui_resource_path\": [\"@{resource_path}\", \"gui\"],"
    "    \"fonts_resource_path\": [\"@{gui_resource_path}\", \"fonts\"]"
    "}"
);
const arc::str::UTF8String fonts(
    "{"
    "    \"supported_formats\": [\"ttf\", \"otf\"]"
    "}"
);


const arc::str::UTF8String widgets_startup(
    "{"
    "    \"splash_screen\":"
    "    {"
    "        \"window_flags\": [\"SplashScreen\"],"
    "        \"size\":"
    "        {"
    "            \"height\": \"50#\","
    "            \"width\": \"1:2\""
    "        }"
    "    }"
    "}"

);

} // namespace meta_comp
} // namespace gui
} // namespace sigma

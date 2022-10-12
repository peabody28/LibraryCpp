#include "JsonHelper.h"

std::vector<std::map<std::string, std::string>> JsonHelper::Parse(std::string json)
{
    std::vector<std::map<std::string, std::string>> objs;
    std::map<std::string, std::string> obj;

    bool isString = false;
    bool isKey = false;
    bool isValueWaited = false;
    bool isValue = false;

    std::string key = "";
    std::string value = "";

    for (char c : json)
    {
        if (c == '}')
        {
            objs.push_back(obj);
            obj.clear();
        }

        if (c == '\"')
        {
            if (isString && isValue)
            {
                obj.insert(std::pair<std::string, std::string>(key, value));
                isKey = isValue = isValueWaited = false;
                key = value = "";
            }
            else if (isKey)
            {
                isKey = false;
            }
            else if (!isKey && !isValueWaited)
            {
                isKey = true;
            }
            else if (isValueWaited)
            {
                isValue = true;
            }

            isString = isString ^ true;

            continue;
        }

        if (isString)
        {
            if (isKey)
                key = key + c;
            else
                value = value + c;
        }

        if (c == ':')
            isValueWaited = true;
    }

    return objs;
}
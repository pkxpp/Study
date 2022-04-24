---
--- Date         : 2020-07-10 18:01:23
--- Author       : HUAFEI2, LIAOGAOCAN, LICHAO7
--- LastEditors  : Please set LastEditors
--- LastEditTime : 2021-03-30 19:43:29
--- Description  : lua string patch function
---

---
--- Remove string space of string start and end.
---
--- @param str string
--- @return string
---
string.trim = function(str)
    if type(str) ~= "string" then
        print(debug.traceback())
        return "";
    end
    return (string.gsub(str, "^%s*(.-)%s*$", "%1"));
end

---
--- Split string with sep
--- Be careful!!!
--- Normally, if SEP is not found, a TABLE containing the incoming string is returned.
--- But under certain circumstances, if `sep` is never found in the string, a `table` of length 1 is returned,
--- the only element being a string filled with `\0` and `size`=1.
---
--- If `sep` is a string of multiple characters, use string.splitbyword please, otherwise the following example will occur:
--- string.split("aabbcbc", "bb") -> {aa, c, c}
---
--- @param str string
--- @param sep string
--- @return table
--- @return boolean
---
string.split = function(str, sep)
    local rt     = {};
    local bEmpty = false;
    if type(str) ~= "string" or type(sep) ~= "string" then
        return rt, bEmpty;
    end
    string.gsub(str, '[^' .. sep .. ']+', function(w)
        table.insert(rt, w)
    end);
    bEmpty = (rt and #rt == 1 and rt[1] == "\0");
    return rt, bEmpty;
end

---
--- Split string by word
---
--- @param str string
--- @param word string
--- @return table
---
string.splitbyword = function(str, word)
    local rt     = {};
    if type(str) ~= "string" or type(word) ~= "string" then
        return rt;
    end
    local start, len = 1, #str;
    while start < len do
        local sepStart, sepStop = string.find(str, word, start);
        local substr = "";
        if sepStart and sepStop then
            substr = string.sub(str, start, sepStart - 1);
            start = sepStop + 1;
        else
            substr = string.sub(str, start);
            start = len;
        end
        table.insert(rt, substr);
    end
    for index, value in ipairs(rt) do
        print(index, value);
    end
    return rt;
end

---
--- Replace searchValue in source with replaceValue.
---
--- @param source string
--- @param searchValue string
--- @param replaceValue string
--- @return string
---
string.replace = function(source, searchValue, replaceValue)
    if type(source) ~= "string" or type(searchValue) ~= "string" or type(replaceValue) ~= "string" then
        print(debug.traceback())
        return "";
    end
    local fnRep = function(_)
        return replaceValue;
    end
    local result = string.gsub(source, "("..searchValue..")", fnRep);
    return result;
end

string.parseNumbers = function(szValue, splitter)
    splitter = splitter or ",";
    if type(szValue) ~= "string" or type(splitter) ~= "string" then
        print(debug.traceback())
        print(szValue, type(szValue))
        return "";
    end
    szValue = string.trim(szValue);
    local tbNumberStr = string.split(szValue, splitter);
    local tbNumber = {};
    for _, v in ipairs(tbNumberStr) do
        local nNumber = tonumber(v);
        table.insert(tbNumber, nNumber);
    end
    return tbNumber;
end

---
--- Test the string is empty or not.
---
--- @param str string
--- @return boolean
---
string.isempty = function(str)
    return not (str and #str > 0 and str ~= "\0");
end

local tbTest = string.split("123,")
for k, v in pairs(tbTest) do
    print(k, v)
end
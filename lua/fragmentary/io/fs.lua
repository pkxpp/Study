---
--- Date         : 2020-07-21 11:46:41
--- Author       : LICHAO7
--- LastEditors  : Please set LastEditors
--- LastEditTime : 2021-11-03 17:24:21
--- Description  : lfs(lua file system) wrapper and implementations.
--- Documentation: @link{https://keplerproject.github.io/luafilesystem/manual.html#installation}
---
FS      = FS or {};
FS.Path = FS.Path or {};

---
--- helper function
---
--- @protected
--- @type table
---
local Helper = {
    IsStrSplitEmpty = function(parts)
        return parts and #parts == 1 and parts[1] == "\0";
    end
};

---
--- Local file IO.
---
--- Call this method will open file directly.
---
--- **"r"**: read mode (the default);
--- **"w"**: write mode;
--- **"a"**: append mode;
--- **"r+"**: update mode, all previous data is preserved;
--- **"w+"**: update mode, all previous data is erased;
--- **"a+"**: append update mode, previous data is preserved, writing is only.
---
--- @param path string
--- @param mode string | '"r"' | '"w"' | '"a"' | '"r+"' | '"w+"' | '"a+"' | '"rb"' | '"wb"' | '"ab"' | '"rb+"' | '"wb+"' | '"ab+"'
--- @return File
---
local File = function(path, mode)
-- protected:
    local this = {};

-- public:
    this._Path = path;
    this._hFile = io.open(path, mode);

    ---
    --- Read file.
    ---
    --- @param length number read length, default is all.
    --- @return string|nil
    ---
    function this:Read(length)
        if not this:Valid() then
            return nil;
        end
        length = length or "*a";
        return this._hFile:read(length);
    end

    ---
    --- Read file next line.
    ---
    --- @return string|nil
    ---
    function this:ReadNextLine()
        if not this:Valid() then
            return nil;
        end
        return this._hFile:read("*l");
    end

    ---
    --- Read a number from file (current position).
    ---
    --- @return number|nil
    ---
    function this:ReadNumber()
        if not this:Valid() then
            return nil;
        end
        return this._hFile:read("*n");
    end

    ---
    --- Write content to file.
    ---
    --- @param content string
    --- @return boolean
    ---
    function this:Write(content)
        if not this:Valid() then
            return false;
        end
        if type(content) ~= "string" then
            content = tostring(content);
        end
        return this._hFile:write(content);
    end

    ---
    --- Seek to current position with offset and return current position.
    ---
    --- @param offset number
    --- @return number|nil
    ---
    function this:Seek(offset)
        if not this:Valid() then
            return nil;
        end
        return this._hFile:seek("cur", offset);
    end

    ---
    --- Seek to file start with offset.
    ---
    --- @param offset number
    --- @return number|nil
    ---
    function this:SeekStart(offset)
        if not this:Valid() then
            return nil;
        end
        return this._hFile:seek("set", offset);
    end

    ---
    --- Seek to file end with offset.
    ---
    --- @param offset number
    --- @return number|nil
    ---
    function this:SeekEnd(offset)
        if not this:Valid() then
            return nil;
        end
        return this._hFile:seek("end", offset);
    end

    ---
    --- Close file handle.
    ---
    --- @return boolean
    ---
    function this:Close()
        if this:Valid() then
            this._hFile:close();
            this._hFile = nil;
        end
        return true;
    end

    ---
    --- Test the file handle is valid.
    ---
    --- @return boolean
    ---
    function this:Valid()
        return io.type(this._hFile);
    end

    ---
    --- Get the file size.
    ---
    --- @return number
    ---
    function this:Size()
        return FS.GetSize(this._Path);
    end

    return this;
end

---
--- Local file IO Wrapper.
---
--- Call this method will open file directly.
---
--- **"r"**: read mode (the default);
--- **"w"**: write mode;
--- **"a"**: append mode;
--- **"r+"**: update mode, all previous data is preserved;
--- **"w+"**: update mode, all previous data is erased;
--- **"a+"**: append update mode, previous data is preserved, writing is only.
---
--- @param path string
--- @param mode string | '"r"' | '"w"' | '"a"' | '"r+"' | '"w+"' | '"a+"' | '"rb"' | '"wb"' | '"ab"' | '"rb+"' | '"wb+"' | '"ab+"'
--- @return File
---
FS.File = File;

---
--- Changes the current working directory to the given path.
--- Returns true in case of success or nil plus an error string.
---
--- @param path string
--- @overload fun(path:string):string
--- @return boolean
---
FS.Chdir = function(path)
    if not FS.Path.IsDir(path) then
        return false;
    end
    return lfs.chdir(path);
end

---
--- Creates a lockfile (called lockfile.lfs) in path if it does not exist and returns the lock.
---
--- If the lock already exists checks if it's stale, using the second parameter (default for the
--- second parameter is `INT_MAX`, which in practice means the lock will never be stale.
--- To free the the lock call `lock:free()`.
--- In case of any errors it returns nil and the error message.
--- In particular, if the lock exists and is not stale it returns the "File exists" message.
---
--- @param path string
--- @param seconds_stale number
--- @overload fun(path:string, seconds_stale:number):number|nil
--- @overload fun(path:string, seconds_stale:number):string|nil
--- @return number|nil
---
FS.LockDir = function(path, ...)
    if not FS.Path.IsDir(path) then
        return nil;
    end
    return lfs.lock_dir(path, ...);
end

---
--- Locks a file or a part of it.
---
--- This function works on open files; the file handle should be specified as the first argument.
---
--- The string mode could be either `r` (for a read/shared lock) or `w` (for a write/exclusive lock).
---
--- The optional arguments start and length can be used to specify a starting point and its length; both should be numbers.
---
--- Returns true if the operation was successful; in case of error, it returns nil plus an error string.
--- @param filehandle file
--- @param mode string r or w
--- @overload fun(filehandle:file, mode:string, start:number, length:number):boolean
--- @return boolean
---
FS.Lock = function(filehandle, mode, ...)
    return (filehandle and filehandle.close and mode) and lfs.lock(filehandle, mode, ...);
end

---
--- Unlocks a file or a part of it. 
---
--- This function works on open files; the file handle should be specified as the first argument. 
--- The optional arguments start and length can be used to specify a starting point and its length; both should be numbers.
--- Returns true if the operation was successful; in case of error, it returns nil plus an error string.
---
--- @param filehandle FILE io.file
--- @param start number
--- @param length number
--- @return boolean
--- @overload fun(filehandle:FILE, start:string, length:string):string
---
FS.UnLock = function(filehandle, start, length)
    return (filehandle and filehandle.close) and lfs.unlock(filehandle, start, length);
end

---
--- Returns a string with the current working directory or nil plus an error string.
---
--- @overload fun():nil
--- @return string
---
FS.GetCwd = function()
    return lfs.currentdir();
end

---
--- List the entries of a given directory.
---
--- Raises an error if path is not a directory.
--- @param dirname string
--- @return table|nil
---
FS.ListDir = function(dirname)
    if not FS.Path.Exists(dirname) then
        return nil;
    end

    local iter, dir = lfs.dir(dirname);
    local dirList   = {};
    while true do
        local path = dir:next();
        if not path then
            break;
        end
        if path ~= "." and path ~= ".." then
            table.insert(dirList, path);
        end
    end
    dir:close();
    iter = nil;
    dir  = nil;
    return dirList;
end

---
--- Walk the dir.
---
--- @param dirname string
--- @param callback function
--- @return void
---
--- callback(current:string, dirs:table, files:table)
---
FS.Walk = function(dirname, callback)
    assert(FS.Path.Exists(dirname),      "Path is not exists." .. dirname);
    assert(type(callback) == "function", "Callback function is required.");

    local iter, dir = lfs.dir(dirname);
    local files   = {};
    local dirs    = {};
    local current = dirname;
    while true do
        local dirOrFile = dir:next();
        if dirOrFile and dirOrFile ~= "." and dirOrFile ~= ".." then
            local path = FS.Path.Join(current, dirOrFile);
            if FS.Path.IsFile(path) then
                table.insert(files, dirOrFile);
            else
                table.insert(dirs, dirOrFile);
            end
        end
        if dirOrFile == nil then
            callback(current, dirs, files);
            local i = 1;
            while i <= #dirs do
                local nextDir = FS.Path.Join(current, dirs[i]);
                FS.Walk(nextDir, callback);
                i = i + 1;
            end break
        end
    end
    dir:close();
    iter = nil;
    dir  = nil;
end

---
--- Creates a link.
---
--- The first argument is the object to link to and the second is the name of the link.
---
--- If the optional third argument is true, the link will by a symbolic link (by default, a hard link is created).
--- @param old string The object to link.
--- @param new string The name of the link.
--- @param symbolic boolean default is false(a hard link will be created.)
---
FS.MakeLink = function (old, new, symbolic)
    return (old and new) and lfs.link(old, new, not not symbolic);
end

---
--- Creates a new directory.
---
--- The argument is the name of the new directory.
--- Returns true in case of success or nil, an error message and a system-dependent error code in case of error.
---
--- @param dirname string
--- @return boolean
---
FS.MakeDir = function(dirname)
    return dirname and lfs.mkdir(dirname);
end

---
--- Creates a new directory and children.
---
--- The argument is the name of the new directory.
--- Returns true in case of success or nil, an error message and a system-dependent error code in case of error.
---
--- @param dirname string
--- @return boolean
---
FS.MakeDirs = function(dirname)
    local dirs = FS.Path.Split(dirname);
    local root = nil;
    while true do
        local next = table.remove(dirs, 1);
        if next then
            if root then
                root = FS.Path.Join(root, next);
            else
                root = next;
            end
            if not FS.Path.Exists(root) then
                lfs.mkdir(root);
            end
        else break end;
    end
    return true;
end

---
--- Removes an existing directory.
---
--- The argument is the name of the directory.
--- Returns true in case of success or nil, an error message and a system-dependent error code in case of error.
---
--- @param dirname string
--- @return boolean
---
FS.RemoveDir = function(dirname)
    if not FS.Path.Exists(dirname) then
        return false;
    end
    return lfs.rmdir(dirname);
end

---
--- Removes an existing directory and children recursive.
---
--- The argument is the name of the directory.
--- Returns true in case of success or nil, an error message and a system-dependent error code in case of error.
---
--- @param dirname string
--- @return boolean
---
FS.RemoveAll = function(dirname)
    if not FS.Path.Exists(dirname) then
        return false;
    end
    local iter, dir = lfs.dir(dirname);
    while true do
        local path = dir:next();
        if not path then
            lfs.rmdir(dirname);
            break;
        end
        if path ~= "." and path ~= ".." then
            path = FS.Path.Join(dirname, path);
            if FS.Path.IsDir(path) then
                FS.RemoveAll(path);
                lfs.rmdir(path);
            else
                os.remove(path);
            end
        end
    end
    dir:close();
    iter = nil;
    dir  = nil;
    return true;
end

---
--- Sets the writing mode for a file.
---
--- The mode string can be either "binary" or "text".
--- Returns true followed the previous mode string for the file, or nil followed by an error string in case of errors. 
--- On non-Windows platforms, where the two modes are identical, setting the mode has no effect, and the mode is always returned as binary.
---
--- @param file file
--- @param mode string
--- @return boolean
---
FS.SetMode = function(file, mode)
    return (file and mode) and lfs.setmode(file, mode);
end

---
--- Returns a table with the file attributes corresponding to filepath (or nil followed
--- by an error message and a system-dependent error code in case of error).
---
--- @param filepath string
--- @param request_name string
--- @param result_table table
--- @overload fun(path:string):number
--- @overload fun(path:string):nil
---
--- If the second optional argument is given and is a string, then only the value of the named
--- attribute is returned (this use is equivalent to lfs.attributes(filepath)[request_name],
--- but the table is not created and only one attribute is retrieved from the O.S.).
--- @overload fun(path:string, request_name:string):string
---
--- If a table is passed as the second argument, it (result_table) is filled with attributes
--- and returned instead of a new table. The attributes are described as follows; attribute mode
--- is a string, all the others are numbers, and the time related attributes use the same time reference of os.time:
---
--- result_table
---
--- result_table.dev A number on Unix systems, this represents the device that the inode resides on. On Windows systems, represents the drive number of the disk containing the file.
---
--- result_table.ino A number on Unix systems, this represents the inode number. On Windows systems this has no meaning.
---
--- result_table.mode A string string representing the associated protection mode (the values could be file, directory, link, socket, named pipe, char device, block device or other).
---
--- result_table.nlink A number number of hard links to the file.
---
--- result_table.uid A number user-id of owner (Unix only, always 0 on Windows).
---
--- result_table.gid A number group-id of owner (Unix only, always 0 on Windows).
---
--- result_table.rdev A number on Unix systems, represents the device type, for special file inodes. On Windows systems represents the same as dev.
---
--- result_table.access A number time of last access.
---
--- result_table.modification A number time of last data modification.
---
--- result_table.change A number time of last file status change.
---
--- result_table.size A number file size, in bytes.
---
--- result_table.permissions A string file permissions string.
---
--- result_table.blocks A number block allocated for file; (Unix only).
---
--- result_table.blksize A number optimal file system I/O blocksize; (Unix only).
---
--- @overload fun(path:string, result_table:table<string, nil>):result_table
---
--- This function uses stat internally thus if the given filepath is a symbolic link, it is
--- followed (if it points to another link the chain is followed recursively) and the information
--- is about the file it refers to. To obtain information about the link itself,
--- see function lfs.symlinkattributes.
---
FS.GetAttributes = function(filepath, ...)
    if not FS.Path.Exists(filepath) then
        return nil;
    end
    return lfs.attributes(filepath, ...);
end

---
--- Return file modified date.
---
--- @param filepath string
--- @return number|nil
---
FS.GetModifiedDate = function(filepath)
    if not FS.Path.Exists(filepath) then
        return nil;
    end
    return lfs.attributes(filepath, "modification");
end

---
--- Return file access date.
---
--- @param filepath string
--- @return number|nil
---
FS.GetAccessDate = function(filepath)
    if not FS.Path.Exists(filepath) then
        return nil;
    end
    return lfs.attributes(filepath, "access");
end

---
--- Return file permissions.
---
--- @param filepath string
--- @return string|nil
---
FS.GetPermissions = function(filepath)
    if not FS.Path.Exists(filepath) then
        return nil;
    end
    return lfs.attributes(filepath, "permissions");
end

---
--- Return file size.
---
--- @param filepath string
--- @return number
---
FS.GetSize = function(filepath)
    if not FS.Path.Exists(filepath) then
        return nil;
    end
    return lfs.attributes(filepath, "size");
end

---
--- Identical to lfs.attributes except that it obtains information about the link itself (not the file it refers to).
---
--- It also adds a target field, containing the file name that the symlink points to.
--- On Windows this function does not yet support links, and is identical to lfs.attributes.
---
--- @param filename string
--- @param request_name string
--- @return string|nil
---
FS.GetSymlinkAttributes = function(filename, request_name)
    if not FS.Path.Exists(filename) or not request_name then
        return nil;
    end
    return lfs.symlinkattributes(filename, request_name);
end

---
--- Set access and modification times of a file.
---
--- This function is a bind to utime function.
--- The first argument is the filename, the second argument (atime) is the access time, and the third argument (mtime) is the modification time.
--- Both times are provided in seconds (which should be generated with Lua standard function os.time).
--- If the modification time is omitted, the access time provided is used; if both times are omitted, the current time is used.
--- Returns true in case of success or nil, an error message and a system-dependent error code in case of error.
---
--- @param filepath string
--- @param atime number
--- @param etime number
--- @return boolean
--- @overload fun(filepath:string, atime:string, mtime:string):string
---
FS.Touch = function(filepath, atime, mtime)
    return FS.Path.Exists(filepath) and lfs.touch(filepath, atime, mtime);
end

---
--- Open a local file.
---
--- **"r"**: read mode (the default);
--- **"w"**: write mode;
--- **"a"**: append mode;
--- **"r+"**: update mode, all previous data is preserved;
--- **"w+"**: update mode, all previous data is erased;
--- **"a+"**: append update mode, previous data is preserved, writing is only.
---
--- @param path string
--- @param mode string | '"r"' | '"w"' | '"a"' | '"r+"' | '"w+"' | '"a+"' | '"rb"' | '"wb"' | '"ab"' | '"rb+"' | '"wb+"' | '"ab+"'
--- @return File
---
FS.Open = function(path, mode)
    local file = File(path, mode or "r");
    if not file.Valid() then
        file.Close();
        file = nil;
    end
    return file;
end

---
--- Close a file object.
---
--- @param file File
--- @return boolean
---
FS.CloseFile = function(file)
    local retVal = false;
    if file and file.Valid and file.Valid() then
        file.Close();
        file   = nil;
        retVal = true;
    end
    if file and file.close then
        file:close();
        file   = nil;
        retVal = true;
    end
    return retVal;
end

---
--- Returns an iterator for each line of the file.
---
--- @param path string
--- @return function iterator
---
FS.FileLines = function(path)
    assert(FS.Path.Exists(path), "File is not exists.");
    return io.lines(path);
end

---
--- Copy file or segmented copy file if file size > 32000000B(256Mb).
---
--- @param opath string
--- @param tpath string
--- @return boolean
---
FS.Copy = function(opath, tpath)
    assert(FS.Path.Exists(opath), "File is not exists.");
    local fsize  = FS.GetSize(opath);
    local fragment_size = 32000000; -- 256Mb
    local ofile = FS.Open(opath, "rb");
    local tfile = FS.Open(tpath, "wb+");
    local start = fragment_size;
    if fsize <= fragment_size then
        tfile:Write(ofile:Read());
    else
        while start < fsize do
            tfile:Write(ofile:Read(start));
            ofile:Seek(start);
            start = start + fragment_size;
        end
    end
    ofile:Close();
    tfile:Close();
end

---
--- Remove file.
---
--- @param path string
--- @return boolean
---
FS.RemoveFile = function(path)
    local result = false;
    if FS.Path.Exists(path) then
        result = not not os.remove(path);
    end
    return result;
end

---
--- Move file or rename file.
---
--- On Windows, the actual API for `os.rename` is `CreateFileA`, so you need to be careful with the filename encoding;
--- Under `*nix` OS. Rename the underlying implementation is Rename, which cannot move across devices. 
--- Files moving across devices should be copied first and deleted later.
---
--- @param opath string
--- @param npath string
--- @return boolean
---
FS.Move = function (opath, npath)
    local result = false;
    if FS.Path.Exists(opath) and not FS.Path.Exists(npath) then
        result = not not os.rename(opath, npath);
    end
    return result;
end

---
--- Max path for Windows style
---
--- @type number
---
FS.Path.MAX_PATH_WIN = 260;

---
--- Join path with os sep.
---
--- @param path string
--- @return string
---
FS.Path.Join = function(path, ...)
    assert(type(path) == "string", "Incorrect path type.");
    local paths = {...};
    local sep   = FS.Path.SEP;
    path = string.gsub(path, "[/\\\"\\?\'\\*><|]*$", "");
    while true do
        local part = table.remove(paths, 1);
        if part then
            part = string.gsub(part, "^[/\\\"\\?\'\\*><|]*", "");
            part = string.gsub(part, "[/\\\"\\?\'\\*><|]*$", "");
            part = string.gsub(part, "[/\\]", sep);
            path = path .. sep .. part;
        else break end
    end
    paths = nil;
    return path;
end

---
--- Split path to list with os sep.
---
--- @param path string
--- @return table
---
FS.Path.Split = function(path)
    local parts = string.split(path or "", FS.Path.SEP);
    if parts and #parts == 1 and parts[1] == "\0" then
        parts = nil;
        return {};
    end
    return parts;
end

---
--- Split path suffix.
---
--- @param path string
--- @return string
---
FS.Path.SplitExt = function(path)
    local filename = FS.Path.SplitFile(path or "");
    local fext = "";
    if filename and filename ~= "" then
        local extParts = string.split(filename, ".");
        if not Helper.IsStrSplitEmpty(extParts) then
            fext = string.format(".%s", extParts[#extParts]);
        end
    end
    return fext;
end

---
--- Split the folder in which the file resides.
---
--- @param path string
--- @return string
---
FS.Path.SplitDir = function(path)
    local pathParts = FS.Path.Split(path);
    local fdir = "";
    if #pathParts > 0 then
        table.remove(pathParts, #pathParts);
        fdir = table.concat(pathParts, FS.Path.SEP);
    end
    return fdir;
end

---
--- Split the filename.
---
--- @param path string
--- @return string
---
FS.Path.SplitFile = function(path)
    local pathParts = FS.Path.Split(path);
    local filename = "";
    if #pathParts > 0 then
        filename = table.remove(pathParts, #pathParts);
    end
    return filename;
end

---
--- Get the system path sep.
---
--- !This is an immediate and anonymous method, as a property.
--- @type string
---
FS.Path.SEP = (function()
    -- if os.ENV_CONST.IS_WINDOWS then
        return "\\";
    -- else
        -- return "/";
    -- end
end)();

---
--- Test the path is exists or not.
---
--- !!! All nonexistent paths always return false.
--- @param path string
--- @return boolean
---
FS.Path.Exists = function(path)
    return path and (lfs.attributes(path) ~= nil);
end

---
--- Test the path is a dir or not.
---
--- !!! All nonexistent paths always return false.
--- @param path string
--- @return boolean
---
FS.Path.IsDir = function(path)
    return FS.Path.Exists(path) and lfs.attributes(path, "mode") == "directory";
end

---
--- Test the path is a file or not.
---
--- !!! All nonexistent paths always return false.
--- @param path string
--- @return boolean
---
FS.Path.IsFile = function(path)
    return FS.Path.Exists(path) and lfs.attributes(path, "mode") == "file";
end

---
--- Test the path is a link or not.
---
--- !!! All nonexistent paths always return false.
--- @param path string
--- @return boolean
---
FS.Path.IsLink = function(path)
    return FS.Path.Exists(path) and lfs.attributes(path, "mode") == "link";
end

---
--- Return number of hard links to the file.
---
--- !!! All nonexistent paths always return nil.
--- @param path string
--- @return number
---
FS.Path.LinkNums = function(path)
    return lfs.attributes(path or "", "nlink");
end

---
--- return  relative path
---
--- @param absolute path string
--- @return string
---
FS.Path.GetRelativePath = function(path, sep)
    sep = sep or FS.Path.SEP;
    path = string.lower(path);
    path = string.replace(path, "\\", sep);
    path = string.replace(path, "/", sep);
    local current_dir = FS.GetCwd();
    current_dir = string.lower(current_dir);
    current_dir = string.replace(current_dir, "\\", sep);
    current_dir = string.replace(current_dir, "/", sep);
    local nBegin, nEnd = string.find(path, current_dir, 1, true);
    if nBegin and nEnd then
        path = string.sub(path, nEnd + 2, #path)
    end
    return path;
end

---
--- return  abs path
---
--- @param relative path string
--- @return string
---
FS.Path.GetAbsolutePath = function(path, sep)
    sep = sep or FS.Path.SEP;
    path = string.replace(path, "\\", sep);
    path = string.replace(path, "/", sep);

    local current_dir = FS.GetCwd();
    current_dir = string.replace(current_dir, "\\", sep);
    current_dir = string.replace(current_dir, "/", sep);
    local nBegin, nEnd = string.find(path, current_dir, 1, true);
    if not nBegin and not nEnd then
        path = current_dir .. sep .. path;
    end

    return path;
end

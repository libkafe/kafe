# libkafe scripting API
## API compatibility level 1

This documentation assumes virtual module `kafe` is included as variable `k`, as demonstrated bellow:

```lua
local k = require('kafe')
```

For future compatibility, all method calls are prefixed with this variable because more modules might be
introduced at some point.

### Before using the scripting API

Kafe scripting API is designed to be as low level or "native" as possible. Thus it is up to you to ensure
any shell commands you pass to the scripting API are safe to execute in their target hosts.

The scripting API does not interact with remote privilege escalation tools (`su`, `sudo` etc.) - it is up to you to
escalate privileges as needed by using the native tools provided by the remote system. Remote prompts are not
supported at this time. One way to deal with privilege escalation is to use `sudo` command with
preconfigured sudo lines in `sudoers` configuration. You should only add the commands you intend to execute
to `sudoers` and never allow wildcard sudo commands to be executed without password prompt
(e.g. `ALL=(ALL:ALL) NOPASSWD: ALL`). Instead, try to design your remote automation as lean as root-less as possible.

All command invocation is strictly sequential - there is no parallelization and parallel invocation of the
scripting API is not supported - even though you can achieve this in Lua. This is a conscious design decision.
Parallelization of remote management tasks might seem a good idea at first, but it rarely is - parallelization makes
remote error handling much more difficult, might leave remote systems in an inconsistent state and requires a
much larger investment in writing the automation tasks. In remote application deployment, parallelization is often used
to ensure all remote hosts receive deployments nearly at the same time. Parallelization is rarely a good solution
to this - you should split your tasks in groups of smaller tasks instead. For example, you can handle near same time deployment
by splitting up the tasks like this - upload all artifacts to all servers, unpack all artifacts on all servers,
symlink all the deployment on all servers, reload services on all servers.

SSH connections to remote servers are reused - one remote server with unique combination of host, user and port will
only have one connection created regardless of environment and role.

## Defining tasks

Kafe tasks are defined in a file called `kafe.lua`. This file is usually placed in a directory that might or might not
contain additional content related to the script itself. For example when automating software project deployments
you could place `kafe.lua` at the root directory of your software project.

Kafe CLI will always look for a file named `kafe.lua` in the current working directory.

Tasks are defined at the root level of the file by invoking `k.task(...)` method.

```lua
local k = require('kafe')

k.task('hello_world_task', function()
    -- Your code here.
end)
```

### Passing arguments to tasks from command line

It is possible to pass arbitrary arguments to the tasks when invoking them from command line.
Arguments are forwarded verbatim without modification, and are always of type string.

Consider a task defined like in this example:

```lua
local k = require('kafe')

k.task('hello_world_task', function(arg1, arg2)
    print(arg1, arg2)
    -- Your code here.
end)
```

When invoked by command line `kafe do staging hello_world_task hello world` this task will receive
arguments `arg1` with value `hello` and `arg2` with value `world`.

**IMPORTANT:** task arguments are never mandatory. You have to validate arguments received by the task function
before using them.

**IMPORTANT:** take care when invoking multiple tasks at the same time. All tasks requested will receive
all arguments as forwarded from the CLI.

## Global values

#### array[string] arg

Program arguments where index `0` is full path to the current
script (`<path>/kafe.lua`) with subsequent values (if any) containing extra arguments provided to the `do` command.

## API values

#### string k.version

Printable version of libkafe.

#### int k.version_major

Major version of libkafe.

#### int k.version_minor

Minor version of libkafe.

#### int k.version_release

Release version of libkafe.

#### int k.api_level

Current API level.

#### int k.environment

Current environment name.

## API methods

### void k.require_api(int version)

Require executing environment to be compatible to certain API compatibility level.

Results in hard failure if:

- API level of the environment is less than the one requested.

##### An example of usage

```lua
local k = require('kafe')
k.require_api(1) -- Fails if API compatibility level is not 1
```

### void k.task(string name, function callable)

Define a task that can be invoked when the script is executed.

Results in hard failure if:

- Task with given name already exists.

##### An example of usage

```lua
local k = require('kafe')
k.task('example_name', function()
    -- Your code here
end)
```

### void k.add_inventory(string user, string host, int port, string env, string role)

Add a server to inventory of given environment with given role.

Results in hard failure if:

- Invalid port is provided (outside range 1-65535); or
- The same server is added to inventory again (duplicate).

##### An example of usage

```lua
local k = require('kafe')
local username = k.getenv('USER') -- get username from env? :)
k.add_inventory(username, 'one.example.org', 22, 'staging', 'example')
k.add_inventory(username, 'two.example.org', 22, 'production', 'example')
k.add_inventory(username, 'three.example.org', 22, 'production', 'example')
```

### bool k.on(string role, function callable [,bool skip_empty_inv = true])

Execute given function on each remote server with given role, in current environment.

The third argument indicates if no remote servers available in current environment
and role combination should be considered an error. By default, libkafe will silently
skip over missing inventories.

The return value indicates whether or not the the invocation succeeded.

**IMPORTANT:** it is not possible to nest `.on(...)` invocations, e.g. you can not call `.on(...)`
when doing so results in calling another `.on(...)`. This behavior is not allowed and will result in hard failure.

Results in hard failure if:

- Called in a way that results in nested context; or
- If there are no servers in given inventory and `skip_empty_env` is not true.

##### An example of usage

```lua
local k = require('kafe')

k.task('example_task', function()
    local my_todo = function()
        -- ... your code here
    end

    -- Execute function my_todo on all servers with role example_role
    -- Fail the script if execution failed for any reason
    if not k.on('example_role', my_todo)
        then error('Could not execute my_todo for some reason') end
end)
```

### void k.within(string directory_path)

Execute all subsequent *remote* commands in given context within given directory.

Effectively prepends `cd <dir> &&` to all subsequent shell commands in the same `k.on(...)` context.

**IMPORTANT:** this command does not verify if remote directory exists or not. Invoking
subsequent remote shell commands in directory that does not exist will result in their
hard failure.

##### An example of usage

```lua
local k = require('kafe')

k.task('example_task', function()
    local my_todo = function()
        k.within('/tmp')
        k.shell('ls') -- will print contents of remote /tmp in stdout
    end

    k.on('example_role', my_todo)
end)
```

### (string stdout, string stderr, int exit_code) k.exec(string command [, bool print_output = true])

Execute a remote shell command and return it's outputs along with exit code.

Second optional argument indicates if the remote output should also be
logged in output of the tool. This option is enabled by default.

##### An example of usage

```lua
local k = require('kafe')

k.task('example_task', function()
    local my_todo = function()
        local out, err, code = k.exec('whoami')
        -- out: remote stdout
        -- err: remote stderr
        -- code: remote exit code
    end

    k.on('example_role', my_todo)
end)
```

### bool k.shell(string command)

Execute a remote shell command, log output and return exit status as boolean. Will
return true if exit status of the remote command is `0`, false otherwise.

##### An example of usage

```lua
local k = require('kafe')

k.task('example_task', function()
    local my_todo = function()
        local result = k.shell('whoami')
        -- result: boolean true if whoami exit code == 0, false otherwise
    end

    k.on('example_role', my_todo)
end)
```

### string k.archive_dir_tmp(string directory)

Create a `.tar.gz` archive from given *local* directory and get the full path to the archive once created.
The resulting archive will be created in the temporary directory of the *local* machine.

**IMPORTANT:** resulting archive will be deleted automatically once execution of the project is complete.

Results in hard failure if:

- Archive directory does not exist.

##### An example of usage

```lua
local k = require('kafe')

k.task('example_task', function()
    local archive = k.archive_dir_tmp('/home/example/some_folder')
    -- archive: string path to .tar.gz file
end)
```

### void k.archive_dir(string archive_file, string directory)

Create a `.tar.gz` archive from given *local* directory and get the full path to the archive once created.
The resulting archive will be created in the path given as second argument on the *local* machine.

Results in hard failure if:

- Archive directory does not exist; or
- File or directory exists at the path provided in `archive_file`.

##### An example of usage

```lua
local k = require('kafe')

k.task('example_task', function()
    k.archive_dir('/home/example/some_archive.tar.gz', '/home/example/some_folder')
end)
```

### bool k.upload_file(string local_file, string remote_file)

Upload local file to remote server in given path. `remote_file` can be
a file or directory.

This command returns true if upload succeeded, and false on failure.

**IMPORTANT:** remote directory to upload to must exist prior to upload.

**IMPORTANT:** any existing remote files will be silently overwritten.

##### An example of usage

```lua
local k = require('kafe')

k.task('example_task', function()
    local archive = k.archive_dir_tmp('/home/example/some_folder')

    local my_todo = function()
        if not k.shell('mkdir -p /tmp/example')
            then error('Failed to ensure remote directory') end

        if not k.upload_file(archive, '/tmp/example/')
            then error('Failed to upload archive to remote directory') end
    end

    k.on('example_role', my_todo)
end)
```

### bool k.download_file(string local_file, string remote_file)

Download remote file from remote server to given local path.

This command returns true if download succeeded, and false on failure.

**IMPORTANT:** any existing local files will be silently overwritten.

##### An example of usage

```lua
local k = require('kafe')

k.task('example_task', function()
    local my_todo = function()
        if not k.download_file('/local/path/example.txt', '/remote/path/example.txt')
            then error('Failed to download remote file') end
    end

    k.on('example_role', my_todo)
end)
```

### bool k.upload_str(string content, string remote_file)

Upload text as file to remote server in given path. `remote_file` must be valid file.

This command returns true if upload succeeded, and false on failure.

**IMPORTANT:** remote directory to upload to must exist prior to upload.

**IMPORTANT:** any existing remote files will be silently overwritten.

##### An example of usage

```lua
local k = require('kafe')

k.task('example_task', function()
    local my_todo = function()
        if not k.upload_str('Hello world!', '/remote/path/example.txt')
            then error('Failed to upload text to remote file') end
    end

    k.on('example_role', my_todo)
end)
```

### string|bool k.download_str(string remote_file)

Download remote file from remote server as string.

This command returns content of the file as string if download succeeded, and false on failure.

##### An example of usage

```lua
local k = require('kafe')

k.task('example_task', function()
    local my_todo = function()
        local content = k.download_str('/remote/path/example.txt')
        if not content then error('Failed to download remote file as text') end
        print(content) -- Prints contents of the remote file to stdout
    end

    k.on('example_role', my_todo)
end)
```

## void k.define(string key, any value)

Define a runtime variable in context of the executing script. These
values can be used for string templating in all commands that receive
string values, including `define` itself.

Accepts any value that can be cast to string as second argument.

**IMPORTANT:** keys are case sensitive.

**IMPORTANT:** existing values using the same key will be silently overwritten.

##### An example of usage

```lua
local k = require('kafe')

k.task('example_task', function()
    k.define('remote_path', '/tmp/example')

    local my_todo = function()
        -- Try to create /tmp/example/ using placeholder
        if not k.shell('mkdir -p {{remote_path}}')
            -- ... or fail with meaningful error
            then error('Could not create remote path {{remote_path}}') end
    end

    k.on('example_role', my_todo)
end)
```

## string k.strfvars(string text)

Replace any placeholder values of format `{{key}}` to their values as defined
using `k.define(...)`.

**IMPORTANT:** keys are case sensitive.

**IMPORTANT:** non-existent values will be replaced with empty string.

##### An example of usage

```lua
local k = require('kafe')

k.define('remote_path', '/tmp/example')
k.define('remote_file', 'hello.txt')
local path = k.strfvars('{{remote_path}}/{{remote_file}}')
print(path) -- prints /tmp/example/hello.txt
```

## string k.strfenv(string text)

Replace any placeholder values of format `{{key}}` to their values from
executing environment variables.

**IMPORTANT:** keys are case sensitive.

**IMPORTANT:** non-existent values will be replaced with empty string.

**IMPORTANT:** script environment and Lua standard `os.getenv(...)` MIGHT return different results when executing
environment is not CLI (future compatibility). You should always use `k.strfenv` and `k.getenv` to access
environment variables from within Kafe scripts.

##### An example of usage

```lua
local k = require('kafe')

local hello = k.strfvars('Hello, my name is {{USER}}!')
print(hello) -- prints Hello, my name is <username>!
```

## string|nil k.getenv(string key)

Get an environment value. Returns nil if environment value is not defined.

**IMPORTANT:** keys are case sensitive.

**IMPORTANT:** script environment and Lua standard `os.getenv(...)` MIGHT return different results when executing
environment is not CLI (future compatibility). You should always use `k.strfenv` and `k.getenv` to access
environment variables from within Kafe scripts.

##### An example of usage

```lua
local k = require('kafe')

local user = k.getenv('USER')
print(user) -- prints <username>
```

## (string stdout, int code) k.local_exec(string command [, bool print_output = true])

Execute local shell command and return it's stdout and exit code.

Second optional argument indicates if the remote output should also be
logged in output of the tool. This option is enabled by default.
```lua
local k = require('kafe')

k.task('example_task', function()
        local out, err, code = k.local_exec('whoami')
        -- out: local stdout
        -- err: local stderr
        -- code: local exit code
end)
```

## bool k.local_shell(string command)

Execute a local shell command, log output and return exit status as boolean. Will
return true if exit status of the local command is `0`, false otherwise.

##### An example of usage

```lua
local k = require('kafe')

k.task('example_task', function()
    local result = k.local_shell('whoami')
    -- result: boolean true if whoami exit code == 0, false otherwise
end)
```

## void k.local_within(string directory_path)

Execute all subsequent *local* commands within given directory, regardless of the remote context.

Results in hard failure if:

- Given directory does not exist.

##### An example of usage

```lua
local k = require('kafe')

k.task('example_task', function()
    k.local_within('/tmp')
    k.local_shell('ls') -- will print contents of local /tmp in stdout
end)
```

## `.kafeignore` support for file archiving
#### New in version 1.1.2

It is possible to create an exclusion list of files to be excluded from archival operations. This is done by
creating a `.kafeignore` file at the root of the directory containing pattern list of all files and directories to
be ignored by the archival operations.

The pattern matching is implemented using GNU fnmatch() with flags `FNM_PATHNAME | FNM_EXTMATCH | FNM_LEADING_DIR`.
See https://www.gnu.org/software/libc/manual/html_node/Wildcard-Matching.html
and https://www.man7.org/linux/man-pages/man3/fnmatch.3.html for more information.

**IMPORTANT:** `.kafeignore` patterns are NOT exactly compatible with other ignore formats, such as `.gitignore`,
    nested `.kafeignore` files are not supported - only the topmost ignore file will be parsed,
    and `.kafeignore` file itself is NOT automatically ignored, since you might want to preserve it in some cases.

#### Basic pattern examples

```ignorelang
# Ignore the directory, all subdirectories and files.
some_dir

# Ignore all subdirectories and files, retain the directory itself.
some_dir/*

# Ignore anything in subdirectories, but retain directory and immediate descendants.
some_dir/**/*

# Ignore everything in the directory, except for file or directory named `some_file`
some_dir/!(some_file)
```

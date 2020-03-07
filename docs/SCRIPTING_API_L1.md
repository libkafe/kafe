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

## Available API values

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

## Available API methods

### void k.require_api(int version)

Require executing environment to be compatible to certain API compatibility level.

Results in hard failure if:

- API level of the environment is less than the one requested.

### void k.task(string name, function callable)

Define a task that can be invoked when the script is executed.

Results in hard failure if:

- Task with given name already exists.

### void k.add_inventory(string user, string host, int port, string env, string role)

Add a server to inventory of given environment with given role.

Results in hard failure if:

- Invalid port is provided (outside range 1-65535); or
- The same server is added to inventory again (duplicate).

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

### void k.within(string directory_path)

Execute all subsequent *remote* commands in given context within given directory.

Effectively prepends `cd <dir> &&` to all subsequent shell commands in the same `k.on(...)` context.

**IMPORTANT:** this command does not verify if remote directory exists or not. Invoking
subsequent remote shell commands in directory that does not exist will result in their
hard failure.

### (string stdout, string stderr, int exit_code) k.exec(string command [, bool print_output = true])

Execute a remote shell command and return it's outputs along with exit code. 

Second optional argument indicates if the remote output should also be 
logged in output of the tool. This option is enabled by default.

### bool k.shell(string command)

Execute a remote shell command, log output and return exit status as boolean. Will
return true if exit status of the remote command is `0`, false otherwise.

### string k.archive_dir_tmp(string directory)

Create a `.tar.gz` archive from given *local* directory and get the full path to the archive once created.
The resulting archive will be created in the temporary directory of the *local* machine.

**IMPORTANT:** resulting archive will be deleted automatically once execution of the project is complete.

Results in hard failure if:

- Archive directory does not exist.

### void k.archive_dir(string directory, string archive_file)

Create a `.tar.gz` archive from given *local* directory and get the full path to the archive oncre created.
The resulting archive will be created in the path given as second argument on the *local* machine.

Results in hard failure if:

- Archive directory does not exist; or
- File or directory exists at the path provided in `archive_file`.

### bool k.upload_file(string local_file, string remote_file)

Upload local file to remote server in given path. `remote_file` can be
a file or directory.

This command returns true if upload succeeded, and false on failure.

**IMPORTANT:** remote directory to upload to must exist prior to upload.

**IMPORTANT:** any existing remote files will be silently overwritten.

### bool k.download_file(string local_file, string remote_file)

Download remote file from remote server to given local path.

This command returns true if download succeeded, and false on failure.

**IMPORTANT:** any existing local files will be silently overwritten.

### bool k.upload_str(string content, string remote_file)

Upload text as file to remote server in given path. `remote_file` must be valid file. 

This command returns true if upload succeeded, and false on failure.

**IMPORTANT:** remote directory to upload to must exist prior to upload.

**IMPORTANT:** any existing remote files will be silently overwritten.

### string|bool k.download_str(string remote_file)

Download remote file from remote server as string.

This command returns content of the file as string if download succeeded, and false on failure.

## void k.define(string key, any value)

Define a runtime variable in context of the executing script. These
values can be used for string templating in all commands that receive
string values, including `define` itself.

Accepts any value that can be cast to string as second argument.

**IMPORTANT:** keys are case sensitive.

**IMPORTANT:** existing values using the same key will be silently overwritten.

## string k.strfvars(string text)

Replace any placeholder values of format `{{key}}` to their values as defined
using `k.define(...)`.

**IMPORTANT:** keys are case sensitive.

**IMPORTANT:** non-existent values will be replaced with empty string.

## string k.strfenv(string text)

Replace any placeholder values of format `{{key}}` to their values from
executing environment variables.

**IMPORTANT:** keys are case sensitive.

**IMPORTANT:** non-existent values will be replaced with empty string.

## (string stdout, int code) k.local_exec(string command [, bool print_output = true])

Execute local shell command and return it's stdout and exit code.
 
Second optional argument indicates if the remote output should also be 
logged in output of the tool. This option is enabled by default.

## bool k.local_shell(string command)

Execute a local shell command, log output and return exit status as boolean. Will
return true if exit status of the local command is `0`, false otherwise.

## void k.local_within(string directory_path)

Execute all subsequent *local* commands within given directory, regardless of the remote context.

Results in hard failure if:

- Given directory does not exist.



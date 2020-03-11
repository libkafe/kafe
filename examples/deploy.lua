--[[
    WARNING: DO NOT copy paste this code and execute verbatim. This script is an example and is not meant to be copied
    and used without modification.
--]]

-- Place the contents of this file in a file named kafe.lua in the root of your project.
-- Edit as needed.

local k = require('kafe')
-- Ensure the kafe runtime has support for the API level this script requires.
k.require_api(1)

-- Add remote servers to inventory.
-- Arguments: username, hostname/ip, port, environment, role.
k.add_inventory('john', 'one.example.org', 22, 'production', 'example_app')
k.add_inventory('john', 'two.example.org', 22, 'production', 'example_app')
k.add_inventory('john', 'stage.example.org', 22, 'staging', 'example_app')

-- Define a single isolated task.
k.task('deploy', function()
    -- Change local working directory.
    k.local_within("~/software/")
    -- Create an archive from folder as a temporary file.
    -- You can provide absolute path or relative path to current working directory
    --   of the process or one defined with `local_within`.
    local archive = k.archive_dir_tmp('./example')

    local deploy = function()
        -- Define some runtime properties.
        k.define('deploy_to', '/opt/example_app/')
        k.define('version', os.time(os.date('!*t')))

        -- Create remote directory to deploy to.
        -- Names inside double curly brackets will be replaced with values previously set with `define`.
        -- Same applies to all calls against Kafe API with text arguments.
        if not k.shell('mkdir -p {{deploy_to}}')
            then error('Could not create deployment directory target') end

        -- Change remote directory to deployment target directory.
        k.within('{{deploy_to}}')

        -- Create release path if it does not exist.
        if not k.shell('mkdir -p releases/{{version}}')
            then error('Could not create release root directory') end

        -- Upload our example application as an archive.
        k.upload_file(archive, 'releases/{{version}}/upload.tar.gz')

        -- Unpack our example application from the archive.
        if not k.shell('tar -xvf releases/{{version}}/upload.tar.gz -C releases/{{version}}')
            then error('Could not unpack uploaded archive') end

        -- Remove the archive file (we don't need it any longer).
        if not k.shell('rm releases/{{version}}/upload.tar.gz')
            then error('Failed to remove uploaded archive') end

        -- Useful: allow group users to deal with our freshly uploaded content
        if not k.shell('chmod g+rw releases && chmod g+rw -Rf releases/')
            then error('Failed to fix chmod') end
    end

    local symlink = function()
        -- Change remote directory to deployment target directory.
        k.within('{{deploy_to}}')

        -- Symlink the new version of the application to {{deploy_to}}/current.
        if not k.shell('ln -nsfv releases/{{version}}/ current')
            then error('Failed to update the symlink to the new version') end
    end

    local reload_service = function()
        -- Reload the service
        if not k.shell('sudo systemctl reload example')
            then error('Failed to reload the service') end
    end

    -- Upload and unpack the new version
    if not k.on('example_app', deploy)
        then error('Failed to deploy') end

    -- Symlink the new version
    if not k.on('example_app', symlink)
        then error('Failed to symlink') end

    -- Reload the example service to apply the new version
    if not k.on('example_app', reload_service)
        then error('Failed to reload the service') end

end)

# This is a comment
stage_one:
    echo 'Hello World' > hello
    # Also a comment
    cat hello

stage_two:
    whoami # Another comment
    :cwd {{KAFE_STAGE_DIR}} # This is an internal command
    pwd
    ls -lah

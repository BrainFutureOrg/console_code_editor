a="$(git log -1 --pretty=format:%h)"
echo "#define GIT_LAST_COMMIT_HASH \"$a\"">git_variables.h

gitdescription_new=$(shell git describe | sed 's/^release-//')
gitdescription_old=$(shell sed 's/.*"\(.*\)".*/\1/' version_git.h)

ifneq ($(gitdescription_new), $(gitdescription_old))
version_git.h:
	echo $(gitdescription_new)
	echo $(gitdescription_old)
	echo '#define GIT_DESCRIPTION "$(gitdescription_new)"' >$@
endif

# doxygen helper

set(ENV{skyscrappers_ROOT} ${PDIR})
message(${PDIR})
execute_process(
	COMMAND doxygen "${PDIR}/doc/doxygen")

deps_config := \
	deprecated/Kconfig \
	public/Kconfig \
	kernel/Kconfig \
	protocol/Kconfig \
	midp/Kconfig \
	graph/Kconfig \
	shell/Kconfig \
	mediaplayer/Kconfig \
	jsext/Kconfig \
	guitool/Kconfig \
	dtvmx/Kconfig \
	Kconfig

include/config/auto.conf: \
	$(deps_config)

$(deps_config): ;
